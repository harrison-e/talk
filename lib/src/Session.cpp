#include "../include/Session.h"
#include <cassert>

////// SETUP
void Session::setup() {
  debug_print("setup");
  init_tox();
  bootstrap();

  // setup callbacks
  tox_callback_self_connection_status(tox, self_conn_status_cb);

  tox_callback_friend_request(tox, friend_req_cb);
  tox_callback_friend_message(tox, friend_msg_cb);
  tox_callback_friend_name(tox, friend_name_cb);
  tox_callback_friend_status_message(tox, friend_status_msg_cb);
  tox_callback_friend_connection_status(tox, friend_conn_status_cb);
}

void Session::init_tox() {
  debug_print("tox initialization");
  TOX_ERR_NEW err_n;
  struct Tox_Options *opt = tox_options_new(NULL);
  tox_options_set_start_port(opt, PORT_RNG_ST);
  tox_options_set_end_port(opt, PORT_RNG_ND);

  // todo: save data?

  tox = tox_new(opt, &err_n);
  tox_options_free(opt);
  if (err_n != TOX_ERR_NEW_OK) {
    fprintf(stderr, "tox new failed with code %d\n", err_n);
    exit(1);
  }
}

void Session::bootstrap() {
  debug_print("bootstrapping");
  for (size_t i = 0; i < sizeof(bootstrap_nodes)/sizeof(struct DHT_node); i ++) {
    uint8_t *bin = hex2bin(bootstrap_nodes[i].key_hex);
    tox_bootstrap(tox, bootstrap_nodes[i].ip, bootstrap_nodes[i].port, bin, NULL);
    delete[] bin;
  }
}

Session::Session()
{
  cout << "welcome to talk\n";
  setup();
  tox_self_get_address(tox, me.getPubKey());
  cout << "your public key:\n";
  print_pubkey(me.getPubKey());

  // extra initialization
}

////// CALLBACKS
void Session::self_conn_status_cb(Tox* tox, TOX_CONNECTION conn_status, void* user_data)
{
  me.setConnection(conn_status);
  cout << "You are " << connection2str(conn_status);
}

void Session::friend_req_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message,
                            size_t length, void *user_data)
{
  requests.push_back(make_shared<Request>()); // todo
  auto req = requests.back();
  req->setPubKey(public_key);
  req->setMessage((char *) message);
}

void Session::friend_msg_cb(Tox* tox, uint32_t friend_number, TOX_MESSAGE_TYPE type,
                            const uint8_t *message, size_t length,
                            void *user_data)
{
  auto f = friend_at(friend_number);
  if (!f) return;
  if (type == TOX_MESSAGE_TYPE_NORMAL) {
    f->addToHistory((char *) message);
    if (talking_to_friend(friend_number))
      cout << friend_name_fmt(f->getName()) << ' ' << (char *) message << '\n';
    else
      cout << "new message from " << f->getName() << '\n';
  }
}

void Session::friend_name_cb(Tox* tox, uint32_t friend_number, const uint8_t* name,
                             size_t length, void* user_data)
{
  auto f = friend_at(friend_number);
  if (!f) return;
  string old = f->getName();
  f->setName((char *) name);
  if (talking_to_friend(friend_number))
    cout << friend_name_fmt(old) << " has changed their name to " << friend_name_fmt(f->getName()) << '\n';
}

void Session::friend_status_msg_cb(Tox* tox, uint32_t friend_number, const uint8_t* message,
                                   size_t length, void* user_data)
{
  auto f = friend_at(friend_number);
  if (!f) return;
  f->setStatus((char *) message);
}

void Session::friend_conn_status_cb(Tox* tox, uint32_t friend_number,
                                    TOX_CONNECTION conn_status, void* user_data)
{
  auto f = friend_at(friend_number);
  if (!f) return;
  f->setConnection(conn_status);
  cout << friend_name_fmt(f->getName()) << " is " << connection2str(conn_status) << '\n';
}

////// RUN
void Session::run() {
  assert(tox != nullptr);
  /* todo: take async commands using <future>
   * todo: see final/testing/async.cpp
   */
  while (true) {
    usleep(1000 * tox_iteration_interval(tox));
    tox_iterate(tox, NULL);
  }
}