#ifndef TALK_UTILITY_H
#define TALK_UTILITY_H
#include <string>
#include <cstdio>
using std::string, std::sscanf;

extern "C" {
#include "tox/tox.h"
#include "sodium.h"
}

////// conversion
// uses new, make sure to delete[] rv
uint8_t* hex2bin(string hex) {
  size_t len = hex.length() / 2;
  auto *bin = new uint8_t[len];

  const char *c_hex = hex.c_str();
  for (size_t i = 0; i < len; ++i, c_hex += 2)
    sscanf(c_hex, "%2hhx", &bin[i]);

  return bin;
}

// uses new, make sure to delete[] rv
char* bin2hex(const uint8_t *bin, size_t length) {
  char *hex = new char[2*length + 1];
  char *h_ptr = hex;

  for (int i = 0; i < length; i++, h_ptr += 2)
    sprintf(h_ptr, "%02X", bin[i]);

  return hex;
}

// sets num to result
bool str2uint(char *str, uint32_t *num) {
  char *str_end;
  long l = strtol(str,&str_end,10);
  if (str_end == str || l < 0 )
    return false;
  *num = (uint32_t)l;
  return true;
}

string connection2str(TOX_CONNECTION conn) {
  switch (conn) {
    case TOX_CONNECTION_NONE:
      return "offline";
    case TOX_CONNECTION_TCP:
      return "online (TCP)";
    case TOX_CONNECTION_UDP:
      return "online (UDP)";
    default:
      return "unknown";
  }
}


////// formatted printing
/**
static mutex print_mutex;
const string FMT_END = "\033[0m";

const bool DEBUGGING = false;
const string FMT_DEBUG = "\033[1;31m[DEBUG] ";
void debug_print(string message) {
  if (DEBUGGING) {
    lock_guard<mutex> lock(print_mutex);
    cout << FMT_DEBUG + message + FMT_END << endl;
  }
}

const string FMT_INFO = "\033[1;36m[INFO] ";
void info(string message) {
  lock_guard<mutex> lock(print_mutex);
  cout << FMT_INFO + message + FMT_END << endl;
}

const string FMT_ERROR = "\033[1;31m[ERROR] ";
void error_print(string error) {
  lock_guard<mutex> lock(print_mutex);
  cout << FMT_ERROR + error + FMT_END << endl;
}

const string FMT_FRIEND_NAME = "\033[1;37;40m";
string friend_name_fmt(string name) {
  return FMT_FRIEND_NAME + ' ' + name + ' ' + FMT_END;
}

const string FMT_MY_NAME = "\033[1;37;44m";
string self_name_fmt(string name) {
  return FMT_MY_NAME + ' ' + name + ' ' + FMT_END;
}

void message_fmt(string name, string message) {
  lock_guard<mutex> lock(print_mutex);
  cout << name + "  " + message << endl;
}

const string FMT_OFFLINE = "\033[32m";
const string FMT_ONLINE = "\033[33m";
string connection2str(TOX_CONNECTION conn) {
  lock_guard<mutex> lock(print_mutex);
  switch (conn) {
    case TOX_CONNECTION_NONE:
      return FMT_OFFLINE + "offline" + FMT_END;
    case TOX_CONNECTION_TCP:
      return FMT_ONLINE + "online (TCP)" + FMT_END;
    case TOX_CONNECTION_UDP:
      return FMT_ONLINE + "online (UDP)" + FMT_END;
    default:
      return "unknown";
  }
}

const string FMT_MY_KEY = "\033[1;33m";
void print_my_pubkey(uint8_t* address) {
  lock_guard<mutex> lock(print_mutex);
  auto hex = bin2hex(address, TOX_ADDRESS_SIZE);
  cout << FMT_MY_KEY + "your public key:" + FMT_END << endl;
  cout << FMT_MY_KEY + string(hex) + FMT_END << endl;
  delete[] hex;
}

const string FMT_LIST_NAME = "\033[1;35m";
const string FMT_LIST_DESC = "\033[35m";
void list_fmt(string name, string description, bool cmd) {
  cout << FMT_LIST_NAME + (cmd ? "`" : "")  + name + (cmd ? "`" : "") + FMT_END
       << FMT_LIST_DESC + " - " + description + FMT_END
       << endl;
}
*/


#endif //TALK_UTILITY_H