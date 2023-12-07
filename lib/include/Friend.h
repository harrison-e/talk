#ifndef TUITALK_FRIEND_H
#define TUITALK_FRIEND_H
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
using std::string, std::vector, std::shared_ptr;

#include "Message.h"
#include "Utility.h"

class Friend {
protected:
  string name;
  string status;
  uint32_t number;
  uint8_t pubkey[TOX_PUBLIC_KEY_SIZE];
  TOX_CONNECTION connection;
  vector<shared_ptr<Message>> unread_messages;
  vector<shared_ptr<Message>> chat_history;

public:
  explicit Friend(uint32_t number)
  : number {number}, connection {TOX_CONNECTION_NONE} {}
  ~Friend() = default;

  string get_name() const { return name; }
  string get_status() const { return status; }
  uint32_t get_number() const { return number; }
  uint8_t* get_pub_key() { return pubkey; }
  TOX_CONNECTION get_connection() const { return connection; }
  vector<shared_ptr<Message>>& get_unread_messages() { return unread_messages; }
  vector<shared_ptr<Message>>& get_history() { return chat_history; }

  void set_name(string name) { this->name = name; }
  void set_status(string status) { this->status = status; }
  void set_connection(TOX_CONNECTION conn) { connection = conn; }
  void add_to_unread(shared_ptr<Message> message) { unread_messages.push_back(message); }
  void add_to_history(shared_ptr<Message> message) { chat_history.push_back(message); }
};

#endif //TUITALK_FRIEND_H