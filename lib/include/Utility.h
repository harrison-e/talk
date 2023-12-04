#ifndef TALK_UTILITY_H
#define TALK_UTILITY_H
#include <string>
#include <cstdio>
using std::string, std::sscanf;

// allocates
uint8_t* hex2bin(string hex) {
  size_t len = hex.length() / 2;
  auto *bin = new uint8_t[len];

  const char *c_hex = hex.c_str();
  for (size_t i = 0; i < len; ++i, c_hex += 2)
    sscanf(c_hex, "%2hhx", &bin[i]);

  return bin;
}

// allocates
char* bin2hex(const uint8_t *bin, size_t length) {
  char *hex = new char[2*length + 1];
  char *h_ptr = hex;

  for (int i = 0; i < length; i++, h_ptr += 2)
    sprintf(h_ptr, "%02X", bin[i]);

  return hex;
}

#endif //TALK_UTILITY_H
