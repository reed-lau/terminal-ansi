#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <string>

enum class Color { kRed, kBlue, kYellow, kDefault };

void set_color(Color color) {
  const char *c = "\033[0m";
  if (color == Color::kRed) {
    c = "\033[31m";
  } else if (color == Color::kBlue) {
    c = "\033[34m";
  } else if (color == Color::kYellow) {
    c = "\033[33m";
  }

  printf("%s", c);
}

void move_to_row(int irow) { printf("\033[%d;1H", irow); }

void clear_screen() { printf("\033[2J"); }

void clear_line() { printf("\033[K"); }

void set_scroll_row(int start, int end) { printf("\033[%d;%dr", start, end); }

void reset_scroll_row() { printf("\033[r"); }

void hidden_cursor() { printf("\033[?25l"); }

void show_cursor() { printf("\033[?25h"); }

void get_term_size(int &rows, int &cols) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  rows = w.ws_row;
  cols = w.ws_col;
}

bool g_stop = false;

void sigint(int sig_no) { g_stop = true; }

int main() {
  setbuf(stdout, NULL);

  int num_row;
  int num_col;

  get_term_size(num_row, num_col);

  hidden_cursor();
  clear_screen();

  signal(SIGINT, sigint);

  set_scroll_row(2, num_row - 1);

  std::string text;
  int count = 0;
  while (!g_stop) {
    text += std::to_string(count) + "\n";
    ++count;

    clear_screen();
    // content
    {
      move_to_row(2);
      printf("%s", text.c_str());
    }

    // header
    {
      move_to_row(1);
      clear_line();

      set_color(Color::kRed);
      printf("head-(%d)", count);
      set_color(Color::kDefault);
    }

    {
      move_to_row(num_row);
      clear_line();

      set_color(Color::kBlue);
      printf("footer-(%d)", count);
      set_color(Color::kDefault);
    }

    usleep(50000);
  }

  show_cursor();
  reset_scroll_row();

  return 0;
}
