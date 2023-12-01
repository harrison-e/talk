#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>

extern "C" {
#include "sodium.h"
#include "tox/tox.h"
}

static void handle_friend_request(
        Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length,
        void *user_data) {
  // Accept the friend request:
  TOX_ERR_FRIEND_ADD err_friend_add;
  tox_friend_add_norequest(tox, public_key, &err_friend_add);
  if (err_friend_add != TOX_ERR_FRIEND_ADD_OK) {
    fprintf(stderr, "unable to add friend: %d\n", err_friend_add);
  }
}

static void handle_friend_message(
        Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type,
        const uint8_t *message, size_t length,
        void *user_data) {
  TOX_ERR_FRIEND_SEND_MESSAGE err_send;
  tox_friend_send_message(tox, friend_number, type, message, length,
                          &err_send);
  if (err_send != TOX_ERR_FRIEND_SEND_MESSAGE_OK) {
    fprintf(stderr, "unable to send message back to friend %d: %d\n",
            friend_number, err_send);
  }
}

char nibble2hex(uint8_t byte) {
  switch (byte) {
    case 0b0000:
      return '0';
    case 0b0001:
      return '1';
    case 0b0010:
      return '2';
    case 0b0011:
      return '3';
    case 0b0100:
      return '4';
    case 0b0101:
      return '5';
    case 0b0110:
      return '6';
    case 0b0111:
      return '7';
    case 0b1000:
      return '8';
    case 0b1001:
      return '9';
    case 0b1010:
      return 'a';
    case 0b1011:
      return 'b';
    case 0b1100:
      return 'c';
    case 0b1101:
      return 'd';
    case 0b1110:
      return 'e';
    case 0b1111:
      return 'f';
    default:
      return 'x';
  }
}

std::string bin2hex(uint8_t *bytes, uint8_t length) {
  std::string out {};
  std::string dout {};
  for (int n = length - 1; n >= 0; n--) {
    uint8_t byte = bytes[n];
    uint8_t lo = byte % 16;
    uint8_t hi = byte / 16;
    dout += "b" + std::to_string(byte) + '\n';
    dout += "l" + std::to_string(lo) + '=' + nibble2hex(lo) + '\n';
    dout += "h" + std::to_string(hi) + '=' + nibble2hex(hi) + '\n';
    out += nibble2hex(lo);
    out += nibble2hex(hi);
  }
  std::reverse(out.begin(), out.end());
  return out + '\n';
}

int main() {
  TOX_ERR_NEW err_new;
  Tox *tox = tox_new(NULL, &err_new);
  if (err_new != TOX_ERR_NEW_OK) {
    fprintf(stderr, "tox_new failed with error code %d\n", err_new);
    exit(1);
  }

  tox_callback_friend_request(tox, handle_friend_request);
  tox_callback_friend_message(tox, handle_friend_message);

  // output our key for msging
  uint8_t address[80];
  tox_self_get_address(tox, address);
  //std::cout << std::to_string(address[4]);
  std::cout << bin2hex(address, 80);

  while (true) {
    usleep(10000 * tox_iteration_interval(tox));
    tox_iterate(tox, NULL);
  }
}