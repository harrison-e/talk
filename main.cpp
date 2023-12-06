#include "lib/include/CursesIO.h"
#include "lib/include/Utility.h"
#include "lib/include/Friend.h"
#include "lib/include/Request.h"
#include "lib/include/Command.h"

#include <unistd.h>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <iostream>
#include <memory>
#include <thread>
#include <future>
#include <sstream>

using std::shared_ptr, std::make_shared, std::stringstream,
std::to_string, std::stoul, std::thread, std::istream_iterator;

/*************************
 * CONSTS, STRUCTS, ETC. *
 *************************/

static const uint16_t PORT_RNG_ST = 33445;
static const uint16_t PORT_RNG_ND = 34445;

struct DHT_node {
  const char *ip;
  uint16_t port;
  const char key_hex[TOX_PUBLIC_KEY_SIZE*2 + 1];
};

struct DHT_node bootstrap_nodes[] = {
        {"node.tox.biribiri.org",      33445,
                "F404ABAA1C99A9D37D61AB54898F56793E1DEF8BD46B1038B9D822E8460FAB67"},
        {"128.199.199.197",            33445,
                "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09"},
        {"2400:6180:0:d0::17a:a001",   33445,
                "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09"},
        {"tox2.abilinski.com",        33445,
                "7A6098B590BDC73F9723FC59F82B3F9085A64D1B213AAF8E610FD351930D052D"},
};

enum CONVO_TYPE {
  CONVO_NULL,
  CONVO_FRIEND,
};



/*************************
 *    GLOBAL VARIABLES   *
 *************************/

bool RUNNING = true;
CursesIO io;
Tox *tox;
Friend me (UINT32_MAX);
vector<shared_ptr<Friend>> friends;
vector<shared_ptr<Request>> requests;
CONVO_TYPE conversation = CONVO_NULL;
uint32_t talkingTo = UINT32_MAX;

shared_ptr<Friend> friend_by_num(uint32_t friend_num) {
  for (auto f : friends) {
    if (f->get_number() == friend_num)
      return f;
  }
  return nullptr;
}

bool talking_to_friend(uint32_t friend_num) { return conversation == CONVO_FRIEND && talkingTo == friend_num; }

void add_friend(uint32_t friend_num) {
  friends.emplace_back(make_shared<Friend>(friend_num));
  TOX_ERR_FRIEND_GET_PUBLIC_KEY err_pubkey;
  tox_friend_get_public_key(tox, friend_num, friends.back()->get_pubkey(), &err_pubkey);
  if (err_pubkey != TOX_ERR_FRIEND_GET_PUBLIC_KEY_OK)
    io.error_print("friend get public key failed with code " + to_string(err_pubkey));
}

shared_ptr<Request> request_by_id(uint32_t id) {
  for (auto r : requests) {
    if (r->get_id() == id)
      return r;
  }
  return nullptr;
}

void request_remove_id(uint32_t id) {
  vector<shared_ptr<Request>>::iterator i;
  for (i = requests.begin(); i < requests.end(); ++i) {
    if ((*i)->get_id() == id)
      requests.erase(i);
  }
}

////// for ncurses



/*************************
 *       CALLBACKS       *
 *************************/

void self_conn_status_cb(Tox* tox, TOX_CONNECTION conn_status, void* user_data)
{
  me.set_connection(conn_status);
  io.info("you are " + connection2str(conn_status));
}

void friend_req_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message,
                   size_t length, void *user_data)
{
  uint32_t id = requests.size();
  requests.push_back(make_shared<Request>());
  auto req = requests.back();
  req->set_pubkey(public_key);
  req->set_id(id);
  req->set_message((char *) message);
}

void friend_msg_cb(Tox* tox, uint32_t friend_number, TOX_MESSAGE_TYPE type,
                   const uint8_t *message, size_t length,
                   void *user_data)
{
  auto f = friend_by_num(friend_number);
  if (!f) return;
  if (type == TOX_MESSAGE_TYPE_NORMAL) {
    f->add_to_history((char *) message);
    if (talking_to_friend(friend_number))
      io.message_fmt(f->get_name(), (char *) message);
    else
      io.info("new message from " + f->get_name());
  }
}

void friend_name_cb(Tox* tox, uint32_t friend_number, const uint8_t* name,
                    size_t length, void* user_data)
{
  auto f = friend_by_num(friend_number);
  if (!f) return;
  string old = f->get_name();
  f->set_name((char *) name);
  if (talking_to_friend(friend_number))
    io.info(old + " has changed their name to " + f->get_name());
}

void friend_status_msg_cb(Tox* tox, uint32_t friend_number, const uint8_t* message,
                          size_t length, void* user_data)
{
  auto f = friend_by_num(friend_number);
  if (!f) return;
  f->set_status((char *) message);
}

void friend_conn_status_cb(Tox* tox, uint32_t friend_number,
                           TOX_CONNECTION conn_status, void* user_data)
{
  auto f = friend_by_num(friend_number);
  if (!f) return;
  f->set_connection(conn_status);
  io.info(f->get_name() + " is " + connection2str(conn_status));
}



/*************************
 *       COMMANDS        *
 *************************/

vector<Command> commands;

Command* find_command(string name) {
  for (auto &command: commands) {
    if (command.get_name() == name)
      return &command;
  }
  return nullptr;
}

void cant_find_command(string name) {
  io.error_print("command `" + name + "` not found");
  io.error_print("use `help` to see all available commands");
}

void run_command(string name, vector<string>& args) {
  if (auto cmd = find_command(name))
    cmd->run(args);
  else
    cant_find_command(name);
}

void command_help(vector<string>& args) {
  // steal lock from `Utility.h` for this scope
  if (args.empty()) {
    lock_guard<mutex> lock(out_mutex);
    for (auto command: commands)
      io.list_fmt(command.get_name(), command.get_desc(), true);
  }
  else if (auto command = find_command(args[0])) {
    lock_guard<mutex> lock(out_mutex);
    io.list_fmt(command->get_name(), command->get_desc(), true);
  }
  else
    cant_find_command(args[0]);
}

void command_add_friend(vector<string>& args) {
  if (args.empty()) {
    io.error_print("usage: add <tox_id> <message>");
    return;
  }
  TOX_ERR_FRIEND_ADD err_add;
  string hex_id = args[0];
  string message = args.size() > 1 ? args[1] : "";
  uint8_t* bin_id = hex2bin(hex_id);
  uint32_t friend_num = tox_friend_add(tox, bin_id, (uint8_t *) message.c_str(), message.length(), &err_add);
  delete[] bin_id;
  if (err_add != TOX_ERR_FRIEND_ADD_OK) {
    io.error_print("add friend failed with code " + to_string(err_add));
    return;
  }
  add_friend(friend_num);
}

void command_set_name(vector<string>& args) {
  if (args.empty()) {
    io.error_print("usage: setname <name>");
    return;
  }
  TOX_ERR_SET_INFO err;
  tox_self_set_name(tox, (uint8_t *) args[0].c_str(), args[0].length(), &err);
  if (err != TOX_ERR_SET_INFO_OK) {
    io.error_print("set name failed with code " + to_string(err));
    return;
  }
  me.set_name(args[0]);
  io.info("your name was changed to " + me.get_name());
}

void command_set_status_msg(vector<string>& args) {
  if (args.empty()) {
    io.error_print("usage: setstatus <name>");
    return;
  }
  TOX_ERR_SET_INFO err;
  string message = std::accumulate(
          std::next(args.begin()),
          args.end(),
          args[0],
          [](std::string a, std::string b) {
            return a + ' ' + b;
          });
  tox_self_set_status_message(tox, (uint8_t *) message.c_str(), message.length(), &err);
  if (err != TOX_ERR_SET_INFO_OK) {
    io.error_print("set status failed with code " + to_string(err));
    return;
  }
  me.set_status(message);
  io.info("your status was changed to: " + me.get_status());
}

void command_phonebook(vector<string>& args) {
  io.list_fmt("name", "number, status", false);
  for (auto f : friends)
    io.list_fmt(f->get_name(), to_string(f->get_number()) + ", " + f->get_status(), false);
}

void command_requests(vector<string>& args) {
  io.list_fmt("request #", "message", false);
  for (auto r : requests)
    io.list_fmt(to_string(r->get_id()), r->get_message(), false);
}

void command_accept(vector<string>& args) {
  if (args.empty()) {
    io.error_print("usage: accept <request #>");
    return;
  }

  uint32_t id = stoul(args[0]);
  if (auto r = request_by_id(id)) {
    TOX_ERR_FRIEND_ADD err;
    uint32_t friend_num = tox_friend_add_norequest(tox, r->get_pubkey(), &err);
    if (err != TOX_ERR_FRIEND_ADD_OK)
      io.error_print("accept request failed with code " + to_string(err));
    else
      add_friend(friend_num);
    request_remove_id(id);
  } else {
    io.error_print("request " + args[0] + " does not exist");
  }
}

void command_deny(vector<string>& args) {
  if (args.empty()) {
    io.error_print("usage: deny <request #>");
    return;
  }

  uint32_t id = stoul(args[0]);
  if (auto r = request_by_id(id))
    request_remove_id(id);
  else
    io.error_print("request " + args[0] + " does not exist");
}

// todo: command talk

void command_exit(vector<string>& args) {
  RUNNING = false;
}

// todo: command loop
void command_loop() {
  string line;
  while (RUNNING) {
    io.reset_input();
    io.get_command(line);
    if (!line.empty()) {
      io.debug_print(line);
      stringstream ss(line);
      istream_iterator<string> begin(ss);
      istream_iterator<string> end;
      string command = *(begin++);
      vector<string> args(begin, end);
      run_command(command, args);
      line = "";
    }
  }
}


/*************************
 *         SETUP         *
 *************************/

void init_tox() {
  io.debug_print("tox initialization");
  TOX_ERR_NEW err_n;
  struct Tox_Options *opt = tox_options_new(nullptr);
  tox_options_set_start_port(opt, PORT_RNG_ST);
  tox_options_set_end_port(opt, PORT_RNG_ND);

  tox = tox_new(opt, &err_n);
  tox_options_free(opt);
  if (err_n != TOX_ERR_NEW_OK) {
    io.error_print("tox new failed with code " + to_string(err_n));
    exit(1);
  }
}

void bootstrap() {
  io.debug_print("bootstrapping");
  for (auto node : bootstrap_nodes) {
    uint8_t *bin = hex2bin(node.key_hex);
    TOX_ERR_BOOTSTRAP err;
    tox_bootstrap(tox, node.ip, node.port, bin, &err);
    if (err != TOX_ERR_BOOTSTRAP_OK) {
      switch (err) {
        case TOX_ERR_BOOTSTRAP_NULL:
          io.debug_print("null err");
          break;
        case TOX_ERR_BOOTSTRAP_BAD_HOST:
          io.debug_print("bad host err");
          break;
        case TOX_ERR_BOOTSTRAP_BAD_PORT:
          io.debug_print("bad port err");
          break;
      }
    } else {
      io.debug_print("bootstrap OK!");
    }
    io.debug_print(node.ip);
    delete[] bin;
  }
}

void init_me() {
  size_t len = tox_self_get_name_size(tox) + 1;
  uint8_t* buf = new uint8_t[len];
  tox_self_get_name(tox, buf);
  me.set_name((char *) buf);
  delete[] buf;
  len = tox_self_get_status_message_size(tox) + 1;
  buf = new uint8_t[len];
  tox_self_get_status_message(tox, buf);
  me.set_status((char *) buf);
  delete[] buf;
  tox_self_get_public_key(tox, me.get_pubkey());
}

void init_commands() {
  commands.emplace_back("help", "print this message, use with arg for singular", command_help);
  commands.emplace_back("add", "add a friend by their tox id, with optional msg", command_add_friend);
  commands.emplace_back("setname", "change your name", command_set_name);
  commands.emplace_back("setstatus", "change your status message", command_set_status_msg);
  commands.emplace_back("phonebook", "list all current friends", command_phonebook);
  commands.emplace_back("requests", "list all pending friend requests", command_requests);
  commands.emplace_back("accept", "accept a request, given its id", command_accept);
  commands.emplace_back("deny", "deny a friend request, given its id", command_deny);

  commands.emplace_back("exit", "exit the program", command_exit);
}

void setup() {
  io.debug_print("setup");
  init_tox();
  init_me();
  init_commands();
  bootstrap();

  // setup callbacks for tox
  tox_callback_self_connection_status(tox, self_conn_status_cb);

  tox_callback_friend_request(tox, friend_req_cb);
  tox_callback_friend_message(tox, friend_msg_cb);
  tox_callback_friend_name(tox, friend_name_cb);
  tox_callback_friend_status_message(tox, friend_status_msg_cb);
  tox_callback_friend_connection_status(tox, friend_conn_status_cb);
}


/*************************
 *         MAIN          *
 *************************/

int main() {
  io.info("welcome to talk");
  io.info("use `help` for usage info");
  setup();
  io.print_my_pubkey(me.get_pubkey());
  io.info("waiting to be online . . .");

  // testing cmds
//  vector<string> args { };
//  run_command("help", args);

  assert(tox != nullptr);

  thread command_thread(command_loop);

  while (RUNNING) {
    usleep(1000 * tox_iteration_interval(tox));
    tox_iterate(tox, nullptr);
  }

  command_thread.join();

  return 0;
}