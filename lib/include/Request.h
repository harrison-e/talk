#ifndef TALK_REQUEST_H
#define TALK_REQUEST_H

extern "C" {
#include "tox/tox.h"
#include "sodium.h"
}

class Request {
protected:
  string message;
  uint32_t id;
  uint8_t pubkey[TOX_PUBLIC_KEY_SIZE];

public:
  Request() = default;

  void setMessage(const string message) { this->message = message; }
  void setID(uint32_t id) { this->id = id; }
  void setPubKey(const uint8_t* pubkey) { for (int i=0;i<TOX_PUBLIC_KEY_SIZE;++i) this->pubkey[i]=pubkey[i];}

  string getMessage() const { return message; }
  uint32_t getID() const { return id; }
  const uint8_t* getPubKey() const { return pubkey; }
};

#endif //TALK_REQUEST_H
