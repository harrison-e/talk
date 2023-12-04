#ifndef TUITALK_SESSION_H
#define TUITALK_SESSION_H
#include "Friend.h"
#include "TUI.h"
#include <unistd.h>

const uint16_t PORT_RNG_ST = 33445;
const uint16_t PORT_RNG_ND = 34445;

/*
 * A Session encompasses the client's functionality
 */
class Session {
protected:  // data
  Tox* tox; // Tox instance
  vector<Friend> friend_list;

protected: // methods
  ////// setup
  void init_tox();


  //////


  ////// callbacks
  // me

  // friend


public:
  ////// general
  Session();
  ~Session() = default;

  void run();
};

#endif //TUITALK_SESSION_H