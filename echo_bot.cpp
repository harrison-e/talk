#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>

extern "C" {
#include "sodium.h"
#include "tox/tox.h"
}

#include "Utility.h"

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
  uint8_t address[TOX_ADDRESS_SIZE];
  tox_self_get_address(tox, address);
  char* hex = bin2hex(address, TOX_ADDRESS_SIZE);
  std::cout << string(hex)  << '\n';
  delete[] hex;

  while (true) {
    usleep(10000 * tox_iteration_interval(tox));
    tox_iterate(tox, NULL);
  }
}