#ifndef TALK_UTILITY_H
#define TALK_UTILITY_H
#include <string>
#include <cstdio>
#include <iostream>
using std::cout, std::string, std::sscanf;

extern "C" {
#include "tox/tox.h"
#include "sodium.h"
}

////// debugging utilities
static const bool DEBUGGING = true;
static void debug_print(string to_print) {
  if (DEBUGGING)
    cout << to_print << '\n';
}

////// conversion
// uses new, make sure to delete rv
static uint8_t* hex2bin(string hex) {
  size_t len = hex.length() / 2;
  auto *bin = new uint8_t[len];

  const char *c_hex = hex.c_str();
  for (size_t i = 0; i < len; ++i, c_hex += 2)
    sscanf(c_hex, "%2hhx", &bin[i]);

  return bin;
}

// uses new, make sure to delete rv
static char* bin2hex(const uint8_t *bin, size_t length) {
  char *hex = new char[2*length + 1];
  char *h_ptr = hex;

  for (int i = 0; i < length; i++, h_ptr += 2)
    sprintf(h_ptr, "%02X", bin[i]);

  return hex;
}

////// formatted printing
static const string FMT_FR = "\033[1;37;40m";
static const string FMT_ME = "\033[1;37;44m";
static const string FMT_OF = "\033[32m";
static const string FMT_ON = "\033[33m";
static const string FMT_ND = "\033[0m";

static string friend_name_fmt(string name) {
  return FMT_FR + name + FMT_ND;
}

static string self_name_fmt(string name) {
  return FMT_ME + name + FMT_ND;
}

static string connection2str(TOX_CONNECTION conn) {
  switch (conn) {
    case TOX_CONNECTION_NONE:
      return FMT_OF + "Offline" + FMT_ND;
    case TOX_CONNECTION_TCP:
      return FMT_ON + "Online(TCP)" + FMT_ND;
    case TOX_CONNECTION_UDP:
      return FMT_ON + "Online(UDP)" + FMT_ND;
    default:
      return "UNKNOWN";
  }
}

static void print_pubkey(uint8_t* address) {
  char* hex = bin2hex(address, TOX_ADDRESS_SIZE);
  std::cout << string(hex);
  delete[] hex;
}

#endif //TALK_UTILITY_H
