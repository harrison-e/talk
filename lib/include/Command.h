#ifndef TALK_COMMAND_H
#define TALK_COMMAND_H
#include <iostream>
#include <string>
#include <vector>
using std::cout, std::endl, std::string, std::vector;

class Command {
protected:
  string name;
  string description;
  void (*fn) (vector<string>&);

public:
  Command(string name, string description, void (*fn) (vector<string>&))
  : name {name}, description {description}, fn {fn} {}
  ~Command() = default;
  string get_name() { return name; }
  string get_desc() { return description; }
  void run(vector<string> args) { fn(args); }
};

#endif //TALK_COMMAND_H
