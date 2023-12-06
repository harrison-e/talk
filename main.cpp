#include "lib/include/Utility.h"
#include "lib/include/Friend.h"
#include "lib/include/Request.h"
#include "lib/include/Command.h"

#include <unistd.h>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <memory>
#include <thread>
#include <future>

using std::shared_ptr, std::make_shared;

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

Tox *tox;
Friend me;
vector<shared_ptr<Friend>> friends;
vector<shared_ptr<Request>> requests;
CONVO_TYPE conversation = CONVO_NULL;
uint32_t talkingTo;

shared_ptr<Friend> friend_by_num(uint32_t num) {
  for (auto f : friends) {
    if (f->get_number() == num)
      return f;
  }
  return nullptr;
}

bool talking_to_friend(uint32_t friend_num) { return conversation == CONVO_FRIEND && talkingTo == friend_num; }



/*************************
 *       CALLBACKS       *
 *************************/

void self_conn_status_cb(Tox* tox, TOX_CONNECTION conn_status, void* user_data)
{
  me.set_connection(conn_status);
  info("you are " + connection2str(conn_status));
}

void friend_req_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message,
                   size_t length, void *user_data)
{
  requests.push_back(make_shared<Request>()); // todo
  auto req = requests.back();
  req->set_pubkey(public_key);
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
      message_fmt(friend_name_fmt(f->get_name()), (char *) message);
    else
      info("new message from " + f->get_name());
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
    info(friend_name_fmt(old) + " has changed their name to " + friend_name_fmt(f->get_name()));
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
  info(friend_name_fmt(f->get_name()) + " is " + connection2str(conn_status));
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

void command_not_found(string name) {
  error_print("command `" + name + "` not found");
  error_print("use `help` to see all available commands");
}

void run_command(string name, vector<string>& args) {
  if (auto cmd = find_command(name))
    cmd->run(args);
  else
    command_not_found(name);
}

void command_help(vector<string>& args) {
  // steal lock from `Utility.h` for this scope
  if (args.empty()) {
    lock_guard<mutex> lock(print_mutex);
    for (auto command: commands)
      command_fmt(command.get_name(), command.get_desc());
  }
  else if (auto command = find_command(args[0])) {
    lock_guard<mutex> lock(print_mutex);
    command_fmt(command->get_name(), command->get_desc());
  }
  else
    command_not_found(args[0]);
}

void command_add_friend(vector<string>& args) {
  if (args.empty()) {
    error_print("usage: add <TOX_ID> <MSG>");
    return;
  }
  string hex_id = args[0];
  string message = args.size() > 1 ? args[1] : "";
  uint8_t* bin_id = hex2bin(hex_id);


  delete[] bin_id;
}



/*************************
 *         SETUP         *
 *************************/

void init_tox() {
  debug_print("tox initialization");
  TOX_ERR_NEW err_n;
  struct Tox_Options *opt = tox_options_new(nullptr);
  tox_options_set_start_port(opt, PORT_RNG_ST);
  tox_options_set_end_port(opt, PORT_RNG_ND);

  tox = tox_new(opt, &err_n);
  tox_options_free(opt);
  if (err_n != TOX_ERR_NEW_OK) {
    fprintf(stderr, "tox new failed with code %d\n", err_n);
    exit(1);
  }
}

void bootstrap() {
  debug_print("bootstrapping");
  for (auto node : bootstrap_nodes) {
    uint8_t *bin = hex2bin(node.key_hex);
    TOX_ERR_BOOTSTRAP err;
    tox_bootstrap(tox, node.ip, node.port, bin, &err);
    if (err != TOX_ERR_BOOTSTRAP_OK) {
      switch (err) {
        case TOX_ERR_BOOTSTRAP_NULL:
          debug_print("null err");
          break;
        case TOX_ERR_BOOTSTRAP_BAD_HOST:
          debug_print("bad host err");
          break;
        case TOX_ERR_BOOTSTRAP_BAD_PORT:
          debug_print("bad port err");
          break;
      }
    } else {
      debug_print("bootstrap OK!");
    }
    debug_print(node.ip);
    delete[] bin;
  }
}

void init_me() {
  me.set_number(UINT32_MAX);
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
  commands.emplace_back("help", "print this message", command_help);
  commands.emplace_back("add", "add a friend", command_add_friend);
}

void setup() {
  debug_print("setup");
  init_tox();
  init_me();
  init_commands();
  bootstrap();

  // setup callbacks
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
  info("welcome to talk");
  info("use `help` for usage info");
  setup();
  print_my_pubkey(me.get_pubkey());
  info("waiting to be online . . .");

  // testing
  vector<string> fake_args {};
  run_command("help", fake_args);

  assert(tox != nullptr);
  while (true) {
    usleep(1000 * tox_iteration_interval(tox));
    tox_iterate(tox, nullptr);
  }
}