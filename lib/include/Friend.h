#ifndef TUITALK_FRIEND_H
#define TUITALK_FRIEND_H
#include <cstdint>
#include <string>
#include <vector>
extern "C" {
  #include "tox/tox.h"
  #include "sodium.h"
}
using std::string, std::vector;

class Friend {
protected:
  string name;
  string status;
  uint8_t public_key[TOX_PUBLIC_KEY_SIZE];
  TOX_CONNECTION connection;
  vector<string> chat_history;

public:
  string getName() const { return name; }
  string getStatus() const { return status; }
  void getPubKey(uint8_t* buf) const;
  TOX_CONNECTION getConnection() const { return connection; }
  const vector<string>* getHistory() const { return &chat_history; }

};

#endif //TUITALK_FRIEND_H