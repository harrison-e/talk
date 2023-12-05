#ifndef TUITALK_SESSION_H
#define TUITALK_SESSION_H

#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <thread>
#include <future>

#include "Friend.h"
#include "Request.h"
#include "Utility.h"

using std::shared_ptr, std::make_shared;

/**************************
 * CONSTS, STRUCTS, ETC.
 **************************/
const uint16_t PORT_RNG_ST = 33445;
const uint16_t PORT_RNG_ND = 34445;

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
};

enum CONVO_TYPE {
  CONVO_FRIEND,
  CONVO_COUNT,
  CONVO_NULL = UINT32_MAX,
};

const string FMT_FR = "\033[1;37;40m";
const string FMT_ME = "\033[1;37;44m";
const string FMT_ND = "\033[0m";

/*
 * A Session encompasses the client's functionality
 */
class Session {
protected:  // data
  Tox* tox; // Tox instance
  // friend list (friend num is idx)
  // todo del_friend -> .at(n) = nullptr
  static vector<shared_ptr<Friend>> friends;
  // request list
  static vector<shared_ptr<Request>> requests;

  Friend me; // todo
  CONVO_TYPE conversation;

protected: // methods
  ////// setup
  void setup();
  void init_tox();
  void bootstrap();

  ////// utility
  void print_pubkey();

  ////// commands
  void add_friend();

  ////// callbacks for tox
  // me
  static void self_conn_status_cb(Tox* tox, TOX_CONNECTION conn_status, void* user_data);

  // friend
  static void friend_req_cb(Tox* tox, const uint8_t* public_key, const uint8_t* message,
                            size_t length, void* user_data);
  static void friend_msg_cb(Tox* tox, uint32_t friend_number, TOX_MESSAGE_TYPE type,
                            const uint8_t* message, size_t length, void* user_data);
  static void friend_name_cb(Tox* tox, uint32_t friend_number, const uint8_t* name,
                             size_t length, void* user_data);
  static void friend_status_msg_cb(Tox* tox, uint32_t friend_number, const uint8_t* message,
                                   size_t length, void* user_data);
  static void friend_conn_status_cb(Tox* tox, uint32_t friend_number,
                                    TOX_CONNECTION conn_status, void* user_data);

public:
  ////// general
  Session();
  ~Session() = default;

  void run(); // event loop
};

#endif //TUITALK_SESSION_H