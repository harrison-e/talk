#ifndef TUITALK_SESSION_H
#define TUITALK_SESSION_H
#include "Friend.h"
#include <unistd.h>

const uint16_t PORT_RNG_ST = 33445;
const uint16_t PORT_RNG_ND = 34445;

/*
 * A Session encompasses the client's functionality, representing a Tox instance
 */
class Session {
protected: // data
  Tox* tox;

protected: // methods
  // setup
  void init_tox();

public:
  ////// general
  Session();
  ~Session() = default;

  void run();


  ////// callbacks
  // me

  // friend

};

#endif //TUITALK_SESSION_H