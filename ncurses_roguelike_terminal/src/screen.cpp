#include <string>
#include "include/rogue.hpp"

namespace roguelike {

ScreenParameters *ScreenParameters::GetScreenParameters() {

  ScreenParameters *screen_parameters;
  screen_parameters = new ScreenParameters;

  getmaxyx(stdscr, screen_parameters->y_max_, screen_parameters->x_max_);

  return screen_parameters;
}

void ScreenParameters::ScreenSetUp() {

  initscr();
  noecho();
  keypad(stdscr, TRUE);
  refresh();

}

void ScreenParameters::PrintGameHub(Level *level) {
  ScreenParameters *screen_parameters;
  screen_parameters = ScreenParameters::GetScreenParameters();

  wattron(stdscr, A_BOLD);
  mvprintw(screen_parameters->y_max_ / 2, screen_parameters->x_max_ / 10, "    Level: %d", level->level_);
  printw("    Gold: %d", level->user_->gold_);
  printw("    Hp: %d(%d)", level->user_->health_, level->user_->max_health_);
  printw("    Attack: %d", level->user_->attack_);
  printw("    Exp: %d", level->user_->exp_);
  printw("     ");
  wattroff(stdscr, A_BOLD);

  wattron(stdscr, A_UNDERLINE);
  mvprintw(screen_parameters->y_max_ / 2 + 1,
           screen_parameters->x_max_ / 10 - 6,
           "                                                                                 ");
  wattroff(stdscr, A_UNDERLINE);

  wattron(stdscr, A_BOLD);
  mvprintw(screen_parameters->y_max_ / 2 + 3,
           screen_parameters->x_max_ / 10 - 6,
           "    X: Spider    Levels: 1-3    Health: 2    Attack: 1    Pathfinding: Random    ");
  wattroff(stdscr, A_BOLD);

  wattron(stdscr, A_BOLD);
  mvprintw(screen_parameters->y_max_ / 2 + 5,
           screen_parameters->x_max_ / 10 - 6,
           "    G: Goblin    Levels: 1-5    Health: 5    Attack: 3    Pathfinding: Seeking    ");
  wattroff(stdscr, A_BOLD);

  wattron(stdscr, A_BOLD);
  mvprintw(screen_parameters->y_max_ / 2 + 7,
           screen_parameters->x_max_ / 10 - 6,
           "    T: Troll    Levels: 4-6    Health: 15    Attack: 5    Pathfinding: Random    ");
  wattroff(stdscr, A_BOLD);

  wattron(stdscr, A_UNDERLINE);
  mvprintw(screen_parameters->y_max_ / 2 + 8,
           screen_parameters->x_max_ / 10 - 6,
           "                                                                                 ");
  wattroff(stdscr, A_UNDERLINE);

  wattron(stdscr, A_BOLD);
  wattron(stdscr, A_UNDERLINE);
  mvprintw(screen_parameters->y_max_ / 2 + 10,
           screen_parameters->x_max_ / 4 + 2,
           "Notifications:");
  wattroff(stdscr, A_UNDERLINE);
  wattroff(stdscr, A_BOLD);

  if (level->user_->is_attacked_) {
    wattron(stdscr, A_BOLD);
    mvprintw(screen_parameters->y_max_ / 2 + 12,
             screen_parameters->x_max_ / 10 - 6,
             "    Player attack: monster get -1Hp");
    wattroff(stdscr, A_BOLD);

    wattron(stdscr, A_BOLD);
    mvprintw(screen_parameters->y_max_ / 2 + 14,
             screen_parameters->x_max_ / 10 - 6,
             "    Monster attack: player @ get punch");
    wattroff(stdscr, A_BOLD);
  }

  wattron(stdscr, A_UNDERLINE);
  mvprintw(screen_parameters->y_max_ / 2 + 16,
           screen_parameters->x_max_ / 10 - 6,
           "                                                                                 ");
  wattroff(stdscr, A_UNDERLINE);

  wattron(stdscr, A_BLINK);
  mvprintw(screen_parameters->y_max_ / 2 + 17,
           screen_parameters->x_max_ / 4,
           "Press Q to Open Menu");
  wattroff(stdscr, A_BLINK);

}

}
