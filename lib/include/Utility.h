#ifndef TALK_UTILITY_H
#define TALK_UTILITY_H
#include <string>
#include <cstdio>
#include <time.h>
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

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const string currentDateTime() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%b-%d %R", &tstruct);

  return buf;
}

#endif //TALK_UTILITY_H