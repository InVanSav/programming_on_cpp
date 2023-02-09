#include "include/rogue.hpp"

namespace roguelike {

void Level::Combat(Player *player, Monster *monster, int order) {

  /* player attacking */
  if (order == 1) {
    monster->health_ -= player->attack_;
    monster->is_attacked_ = true;

    if (monster->health_ > 0) {
      player->health_ -= monster->attack_;
      player->is_attacked_ = true;
    } else {
      Monster::KillMonster(monster);
      player->exp_++;
      player->gold_++;
    }
  }
    /* monster attacking */
  else {
    player->health_ -= monster->attack_;
    player->is_attacked_ = true;
    if (player->health_ > 0) {
      monster->health_ -= player->attack_;
      monster->is_attacked_ = true;
    }
  }
}

}
