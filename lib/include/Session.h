#ifndef TUITALK_SESSION_H
#define TUITALK_SESSION_H
#include "Friend.h"
#include "Utility.h"
#include <unistd.h>
#include <algorithm>
#include <iostream>

////// connection data
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


/*
 * A Session encompasses the client's functionality
 */
class Session {
protected:  // data
  Tox* tox; // Tox instance
  vector<Friend> friend_list;

protected: // methods
  ////// setup
  void setup();

  void init_tox();
  void bootstrap();

  ////// utility
  void print_pubkey();

  ////// commands
  void add_friend();

  ////// callbacks
  // me

  // friend

  ////// handlers
  // me

  // friend
  static void handle_friend_req(const uint8_t *public_key, const uint8_t *message,
                                size_t length, void *user_data);
  static void handle_friend_msg(uint32_t friend_number, TOX_MESSAGE_TYPE type,
                                const uint8_t *message, size_t length,
                                void *user_data);

public:
  ////// general
  Session();
  ~Session() = default;

  void run(); // event loop
};

#endif //TUITALK_SESSION_H