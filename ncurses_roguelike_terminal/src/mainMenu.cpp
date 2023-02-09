#include "ncurses.h"
#include "include/rogue.hpp"
#include "include/mainMenu.hpp"

namespace roguelike {

int MainMenu() {
  ScreenParameters *screen_parameters;
  screen_parameters = ScreenParameters::GetScreenParameters();

  WINDOW *menu_win = newwin(screen_parameters->y_max_ / 2,
                            screen_parameters->x_max_ / 2,
                            screen_parameters->y_max_ / 4,
                            screen_parameters->x_max_ / 4);
  box(menu_win, 0, 0);

  mvwprintw(menu_win, 0, screen_parameters->x_max_ / 16, "Start Game");

  mvwprintw(menu_win, 0, screen_parameters->x_max_ / 8 + 2, "End Game");

  wattron(menu_win, A_BOLD);
  mvwprintw(menu_win,
            screen_parameters->y_max_ / 4 - 5,
            screen_parameters->x_max_ / 12,
            "This Game is my project for Technology of Development Software");
  wattroff(menu_win, A_BOLD);

  wattron(menu_win, A_BLINK);
  mvwprintw(menu_win,
            screen_parameters->y_max_ / 3 + 5,
            screen_parameters->x_max_ / 8 + 2,
            "Press S to Start Game, Press E to End Game");
  wattroff(menu_win, A_BLINK);

  char ch;
  while (true) {
    ch = wgetch(menu_win);
    switch (ch) {
      case 's':
      case 'S':return START_GAME;
      case 'e':
      case 'E':return QUIT_GAME;
    }
  }
}

}
