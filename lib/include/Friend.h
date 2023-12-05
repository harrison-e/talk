#ifndef TUITALK_FRIEND_H
#define TUITALK_FRIEND_H
#include <cstdint>
#include <string>
#include <vector>
using std::string, std::vector;

#include "Utility.h"

class Friend {
protected:
  string name;
  string status;
  uint8_t public_key[TOX_PUBLIC_KEY_SIZE];
  TOX_CONNECTION connection;
  vector<string> chat_history;

public:
  Friend() = default;
  ~Friend() = default;

  string getName() const { return name; }
  string getStatus() const { return status; }
  uint8_t* getPubKey() { return (uint8_t *) &public_key; }
  TOX_CONNECTION getConnection() const { return connection; }
  const vector<string>* getHistory() const { return &chat_history; }

  void setName(string name) { this->name = name; }
  void setStatus(string status) { this->status = status; }
  void setConnection(TOX_CONNECTION conn) { connection = conn; }
  void addToHistory(string message) { chat_history.push_back(message); }
};

#endif //TUITALK_FRIEND_H