#include "../include/Session.h"
#include <cassert>

////// SETUP
void Session::setup() {
  std::cout << "setting up...\n";
  init_tox();
  bootstrap();

  // setup callbacks
  // todo
}

void Session::init_tox() {
  TOX_ERR_NEW err_n;
  struct Tox_Options *opt = tox_options_new(NULL);
  tox_options_set_start_port(opt, PORT_RNG_ST);
  tox_options_set_end_port(opt, PORT_RNG_ND);

  // todo: save data?

  tox = tox_new(opt, &err_n);
  tox_options_free(opt);
  if (err_n != TOX_ERR_NEW_OK) {
    fprintf(stderr, "tox new failed with code %d\n", err_n);
    exit(1);
  }
}

void Session::bootstrap() {
  for (size_t i = 0; i < sizeof(bootstrap_nodes)/sizeof(struct DHT_node); i ++) {
    uint8_t *bin = hex2bin(bootstrap_nodes[i].key_hex);
    tox_bootstrap(tox, bootstrap_nodes[i].ip, bootstrap_nodes[i].port, bin, NULL);
    delete[] bin;
  }
}


Session::Session() {
  std::cout << "welcome to talk\n";
  setup();
  std::cout << "your public key:\n";
  print_pubkey();
  // init rest...
}

////// UTILITY
void Session::print_pubkey() {
  uint8_t address[TOX_ADDRESS_SIZE];
  tox_self_get_address(tox, address);
  char* hex = bin2hex(address, TOX_ADDRESS_SIZE);
  std::cout << string(hex);
  delete[] hex;
}


void Session::run() {
  assert(tox != nullptr);
  while (true) {
    usleep(1000 * tox_iteration_interval(tox));
    tox_iterate(tox, NULL);
  }
}