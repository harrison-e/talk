#ifndef TALK_CURSESIO_H
#define TALK_CURSESIO_H
#include <curses.h>
#include <mutex>
#include "Utility.h"

using std::string, std::mutex, std::lock_guard;

const bool DEBUGGING = false;

static mutex out_mutex;

const int INFO_COLOR = 1;
const int ERROR_COLOR = 2;
const int DEBUG_COLOR = 3;
const int KEY_COLOR = 4;
const int LIST_COLOR = 5;
const int MY_NAME_COLOR = 6;
const int FRIEND_NAME_COLOR = 7;

class CursesIO {
protected:
  WINDOW* out;
  WINDOW* in;

public:
  CursesIO();
  ~CursesIO();

  // input
  void reset_input();
  void get_line(string& line);

  // output
  void complete_print();
  void debug_print(string message);
  void info(string message);
  void error_print(string error);
  void my_message_fmt(string name, string message);
  void friend_message_fmt(string name, string message);
  void print_my_pubkey(uint8_t* address);
  void list_fmt(string name, string description, bool cmd);
};


// constructor
CursesIO::CursesIO() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);

  start_color();
  init_pair(INFO_COLOR, COLOR_BLUE, COLOR_BLACK);
  init_pair(ERROR_COLOR, COLOR_RED, COLOR_BLACK);
  init_pair(DEBUG_COLOR, COLOR_GREEN, COLOR_BLACK);
  init_pair(KEY_COLOR, COLOR_YELLOW, COLOR_BLACK);
  init_pair(LIST_COLOR, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(MY_NAME_COLOR, COLOR_WHITE, COLOR_BLUE);
  init_pair(FRIEND_NAME_COLOR, COLOR_WHITE, COLOR_MAGENTA);

  int height = LINES - 2;
  int width = COLS;
  int start_x = 0;
  int start_y = 0;

  out = newwin(height, width, start_y, start_x);
  in = newwin(height, width, start_y + height, start_x);

  scrollok(out, true);
  scrollok(in, true);
}

// destructor (cleanup)
CursesIO::~CursesIO() {
  delwin(in);
  delwin(out);
  endwin();
}

// input
void CursesIO::reset_input() {
  wmove(in, 1, 0);
  wprintw(in, "> ");
  wclrtoeol(in);
  wrefresh(in);
}

void CursesIO::get_line(string& line) {
  int ch;
  // note that this will blow up if `line` goes over one line
  while ((ch = wgetch(in)) != '\n') {
    if (ch == erasechar() && line.length() > 0) {
      int row, col;
      getyx(in, row, col);
      wmove(in, row, col - 1);
      wdelch(in);
      line = line.substr(0, line.length() - 1);
    } else {
      wprintw(in, "%c", ch);
      line += (char) ch;
    }
    wrefresh(in);
  }
}

// output
void CursesIO::complete_print() {
  wprintw(out, "\n");
  wrefresh(out);
}

void CursesIO::debug_print(string message) {
  if (DEBUGGING) {
    lock_guard<mutex> lock(out_mutex);
    wattron(out, COLOR_PAIR(DEBUG_COLOR));
    wprintw(out, ("[DEBUG] " + message).c_str());
    complete_print();
    wattroff(out, COLOR_PAIR(DEBUG_COLOR));
  }
}


void CursesIO::info(string message) {
  lock_guard<mutex> lock(out_mutex);
  wattron(out, COLOR_PAIR(INFO_COLOR));
  wprintw(out, ("[INFO] " + message).c_str());
  complete_print();
  wattroff(out, COLOR_PAIR(INFO_COLOR));
}


void CursesIO::error_print(string error) {
  lock_guard<mutex> lock(out_mutex);
  wattron(out, COLOR_PAIR(ERROR_COLOR));
  wprintw(out, ("[ERROR] " + error).c_str());
  complete_print();
  wattroff(out, COLOR_PAIR(ERROR_COLOR));
}

void CursesIO::friend_message_fmt(string name, string message) {
  lock_guard<mutex> lock(out_mutex);
  wattron(out, COLOR_PAIR(FRIEND_NAME_COLOR));
  wprintw(out, (" " + name + " ").c_str());
  wrefresh(out);
  wattroff(out, COLOR_PAIR(FRIEND_NAME_COLOR));
  wprintw(out, (" " + message).c_str());
  complete_print();
}

void CursesIO::my_message_fmt(string name, string message) {
  lock_guard<mutex> lock(out_mutex);
  wattron(out, COLOR_PAIR(MY_NAME_COLOR));
  wprintw(out, (" " + name + " ").c_str());
  wrefresh(out);
  wattroff(out, COLOR_PAIR(MY_NAME_COLOR));
  wprintw(out, (" " + message).c_str());
  complete_print();
}

void CursesIO::print_my_pubkey(uint8_t* address) {
  lock_guard<mutex> lock(out_mutex);
  auto hex = bin2hex(address, TOX_ADDRESS_SIZE);
  wattron(out, COLOR_PAIR(4));
  wprintw(out, "your public key: ");
  wprintw(out, (string(hex)).c_str());
  complete_print();
  wattroff(out, COLOR_PAIR(4));
  delete[] hex;
}

void CursesIO::list_fmt(string name, string rest, bool cmd) {
  wattron(out, COLOR_PAIR(5));
  wprintw(out, ((cmd ? "`" : "") + name + (cmd ? "`" : "")).c_str());
  wprintw(out, (" - " + rest).c_str());
  complete_print();
  wattroff(out, COLOR_PAIR(5));
}

#endif //TALK_CURSESIO_H
