#include "../include/Session.h"
#include <assert.h>

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

Session::Session() {
  init_tox();
  // init rest...
}

void Session::run() {
  assert(tox != nullptr);
  while (true) {
    usleep(1000 * tox_iteration_interval(tox));
    tox_iterate(tox, NULL);
  }
}