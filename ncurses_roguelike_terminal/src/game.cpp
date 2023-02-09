#include "include/rogue.hpp"

namespace roguelike {

int Game::GameLoop(Game *game) {
  char ch = '\0';
  Position *new_position;
  Level *level;

  if (game->current_level_ == 0) {
    game->levels_[game->current_level_] = Level::CreateLevel(1);
    game->current_level_++;
  }

  level = game->levels_[game->current_level_ - 1];

  /* main game loop */
  while (ch != 'q') {
    new_position = Player::HandleInput(ch, level->user_);
    Level::CheckPosition(new_position, level);
    Level::MoveMonsters(level);

    clear();
    ScreenParameters::PrintGameHub(game->levels_[game->current_level_ - 1]);
    Level::DrawLevel(game->levels_[game->current_level_ - 1]);

    if (level->number_of_monsters_ < 0) {
      game->levels_[game->current_level_] = Level::CreateLevel(game->current_level_);
      game->current_level_++;
    }

    level = game->levels_[game->current_level_ - 1];

    if (level->user_->health_ <= 0) {
      game->current_level_ = 0;
      return -1;
    }
    ch = getch();
  }

  return 0;
}

}