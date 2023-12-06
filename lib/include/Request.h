#ifndef TALK_REQUEST_H
#define TALK_REQUEST_H

#include <string>
#include "Utility.h"

using std::string;

class Request {
protected:
  string message;
  uint32_t id;
  uint8_t pubkey[TOX_PUBLIC_KEY_SIZE];

public:
  Request() = default;

  void set_message(const string message) { this->message = message; }
  void set_id(uint32_t id) { this->id = id; }
  void set_pubkey(const uint8_t* pubkey) { for (int i=0;i<TOX_PUBLIC_KEY_SIZE;++i) this->pubkey[i]=pubkey[i];}

  string get_message() const { return message; }
  uint32_t get_id() const { return id; }
  const uint8_t* get_pubkey() const { return pubkey; }
};

#endif //TALK_REQUEST_H
