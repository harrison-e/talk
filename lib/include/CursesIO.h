#ifndef TALK_CURSESIO_H
#define TALK_CURSESIO_H
#include <curses.h>
#include <mutex>
#include <memory>
#include "Utility.h"
#include "Message.h"

using std::string, std::shared_ptr, std::mutex, std::lock_guard;

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
  void starter_message();
  void debug_print(string message);
  void info(string message);
  void error_print(string error);
  void my_message_fmt(shared_ptr<Message> m);
  void friend_message_fmt(shared_ptr<Message> m);
  void print_my_tox_id(uint8_t* address);
  void print_friend_pubkey(uint8_t* address);
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
  int prev;
  // note that this will blow up if `line` goes over one line
  int row, col;
  while ((ch = wgetch(in)) != '\n') {
    if (ch == erasechar() && line.length() > 0) {
      getyx(in, row, col);
      wmove(in, row, col - (prev < 32 ? 2 : 1));
      wdelch(in);
      line = line.substr(0, line.length() - (prev < 32 ? 2 : 1));
    } else if (ch >= 32 && ch < 127) {
      wprintw(in, "%c", ch);
      line += (char) ch;
    }
    prev = ch;
    wrefresh(in);
  }
}

// output
void CursesIO::complete_print() {
  wprintw(out, "\n\n");
  wrefresh(out);
}

void CursesIO::starter_message() {
  lock_guard<mutex> lock(out_mutex);
  wattron(out, COLOR_PAIR(KEY_COLOR));
  wprintw(out, "talk is a peer-to-peer messaging client, powered by Tox\n");
  wprintw(out, "to connect with others, either share your Tox id, or add theirs\n\n");
  wprintw(out, "try setting your name with `setname` <name>,\n");
  wprintw(out, "and setting your status with `setstatus` <status>");
  complete_print();
  wattroff(out, COLOR_PAIR(KEY_COLOR));
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

void CursesIO::friend_message_fmt(shared_ptr<Message> m) {
  lock_guard<mutex> lock(out_mutex);
  wprintw(out, (m->get_timestamp() + "  ").c_str());
  wattron(out, COLOR_PAIR(FRIEND_NAME_COLOR));
  wprintw(out, (" " + m->get_sender() + " ").c_str());
  wrefresh(out);
  wattroff(out, COLOR_PAIR(FRIEND_NAME_COLOR));
  wprintw(out, (" " + m->get_content()).c_str());
  complete_print();
}

void CursesIO::my_message_fmt(shared_ptr<Message> m) {
  lock_guard<mutex> lock(out_mutex);
  wprintw(out, (m->get_timestamp() + "  ").c_str());
  wattron(out, COLOR_PAIR(MY_NAME_COLOR));
  wprintw(out, (" " + m->get_sender() + " ").c_str());
  wrefresh(out);
  wattroff(out, COLOR_PAIR(MY_NAME_COLOR));
  wprintw(out, (" " + m->get_content()).c_str());
  complete_print();
}

void CursesIO::print_my_tox_id(uint8_t* address) {
  lock_guard<mutex> lock(out_mutex);
  auto hex = bin2hex(address, TOX_ADDRESS_SIZE);
  wattron(out, COLOR_PAIR(4));
  wprintw(out, "your Tox id: ");
  wprintw(out, (string(hex)).c_str());
  complete_print();
  wattroff(out, COLOR_PAIR(4));
  delete[] hex;
}

void CursesIO::print_friend_pubkey(uint8_t* address) {
  lock_guard<mutex> lock(out_mutex);
  auto hex = bin2hex(address, TOX_ADDRESS_SIZE);
  wattron(out, COLOR_PAIR(4));
  wprintw(out, "their public key: ");
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
