#ifndef TALK_MESSAGE_H
#define TALK_MESSAGE_H
#include "Utility.h"

class Message {
protected:
  string timestamp;
  string sender;
  string content;

public:
  Message(string sender, string content)
  : timestamp { currentDateTime() }, sender { sender }, content { content } {}
  string get_timestamp() { return timestamp; }
  string get_sender() { return sender; }
  string get_content() { return content; }
};

#endif //TALK_MESSAGE_H
