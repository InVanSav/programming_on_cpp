#include "include/rogue.hpp"
#include "include/mainMenu.hpp"

namespace roguelike {

void MenuLoop() {
  int choice;
  Game game;
  game.current_level_ = 0;

  while (true) {
    choice = MainMenu();
    switch (choice) {
      case START_GAME:
        Game::GameLoop(&game);
        clear();
        break;
      case QUIT_GAME:
        return;
    }
  }
}

}

int main() {
  roguelike::ScreenParameters::ScreenSetUp();
  roguelike::MenuLoop();
  endwin();

  return 0;
}