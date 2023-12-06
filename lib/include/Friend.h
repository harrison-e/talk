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
  uint32_t number;
  uint8_t address[TOX_ADDRESS_SIZE];
  TOX_CONNECTION connection;
  vector<string> chat_history;

public:
  explicit Friend(uint32_t number)
  : number {number}, connection {TOX_CONNECTION_NONE} {}
  ~Friend() = default;

  string get_name() const { return name; }
  string get_status() const { return status; }
  uint32_t get_number() const { return number; }
  uint8_t* get_address() { return address; }
  TOX_CONNECTION get_connection() const { return connection; }
  vector<string>& get_history() { return chat_history; }

  void set_name(string name) { this->name = name; }
  void set_status(string status) { this->status = status; }
  void set_connection(TOX_CONNECTION conn) { connection = conn; }
  void set_number(uint32_t number) { this->number = number; }
  void add_to_history(string message) { chat_history.push_back(message); }
};

#endif //TUITALK_FRIEND_H