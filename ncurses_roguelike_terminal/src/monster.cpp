#include "include/rogue.hpp"

namespace roguelike {

void Level::AddMonsters(Level *level) {
  int x;
  level->monsters_ = new Monster*[6];
  level->number_of_monsters_ = 0;
  srand(time(nullptr));

  for (x = 0; x < level->number_of_rooms_; x++) {
      level->monsters_[level->number_of_monsters_] = Monster::SelectMonster(level->level_);
      Monster::SetStartingPosition(level->monsters_[level->number_of_monsters_], level->rooms_[x]);
      level->number_of_monsters_++;
  }
}

Monster *Monster::SelectMonster(int level) {
  int monster;
  srand(time(nullptr));

  switch (level) {
    case 1:
    case 2:
    case 3:
      monster = 2;
      break;
    case 4:
    case 5:
      monster = (rand() % 2) + 2;
      break;
    case 6:
      monster = 3;
      break;
  }

  switch (monster) {
    case 1: /* spider */
      return Monster::CreateMonster('X', 2, 1, 1, 1, 1);
    case 2: /* goblin */
      return Monster::CreateMonster('G', 5, 3, 1, 3, 2);
    case 3: /* troll */
      return Monster::CreateMonster('T', 15, 5, 1, 5, 1);
  }
  return nullptr;
}

Monster *Monster::CreateMonster(char symbol, int health, int attack, int speed, int defence, int pathfinding) {
  Monster *new_monster;
  new_monster = new Monster;

  new_monster->symbol_ = symbol;
  new_monster->health_ = health;
  new_monster->attack_ = attack;
  new_monster->speed_ = speed;
  new_monster->defence_ = defence;
  new_monster->pathfinding_ = pathfinding;
  new_monster->alive_ = 1;

  sprintf(new_monster->string_, "%c", symbol);

  new_monster->position_ = new Position;

  return new_monster;
}

int Monster::KillMonster(Monster *monster) {
  monster->alive_ = 0;
  return 1;
}

void Monster::SetStartingPosition(Monster *monster, Room *room) {
  srand(time(nullptr));
  monster->position_->x_ = (rand() % (room->width_ - 2)) + room->position_.x_ + 1;
  monster->position_->y_ = (rand() % (room->height_ - 2)) + room->position_.y_ + 1;
}

int Level::MoveMonsters(Level *level) {
  int x;
  for (x = 0; x < level->number_of_monsters_; x++) {
    if (level->monsters_[x]->alive_ == 0)
      continue;

    if (level->monsters_[x]->pathfinding_ == 1) {
      Monster::PathfindingRandom(level->monsters_[x]->position_);
    } else {
      Monster::PathfindingSeek(level->monsters_[x]->position_, level->user_->position_);
    }
  }

  return 0;

}

int Monster::PathfindingRandom(Position *position) {
  int random;
  srand(time(nullptr));
  random = rand() % 5;

  switch (random) {
    /* step up */
    case 0:
      if (mvinch(position->y_ - 1, position->x_) == '.') {
        position->y_ = position->y_ - 1;
      }
      break;

      /* step down */
    case 1:
      if (mvinch(position->y_ + 1, position->x_) == '.') {
        position->y_ = position->y_ + 1;
      }
      break;

      /* step left */
    case 2:
      if (mvinch(position->y_, position->x_ - 1) == '.') {
        position->x_ = position->x_ - 1;
      }
      break;

      /* step right */
    case 3:
      if (mvinch(position->y_, position->x_ + 1) == '.') {
        position->x_ = position->x_ + 1;
      }
      break;

      /* do nothing */
    case 4:
      break;

  }

  return 1;
}

int Monster::PathfindingSeek(Position *start, Position *destination) {
  /* step left */
  if ((abs((start->x_ - 1) - destination->x_) < abs(start->x_ - destination->x_))
      && (mvinch(start->y_, start->x_ - 1) == '.')) {
    start->x_ = start->x_ - 1;

    /* step right */
  } else if ((abs((start->x_ + 1) - destination->x_) < abs(start->x_ - destination->x_))
      && (mvinch(start->y_, start->x_ + 1) == '.')) {
    start->x_ = start->x_ + 1;

    /* step down */
  } else if ((abs((start->y_ + 1) - destination->y_) < abs(start->y_ - destination->y_))
      && (mvinch(start->y_ + 1, start->x_) == '.')) {
    start->y_ = start->y_ + 1;

    /* step up */
  } else if ((abs((start->y_ - 1) - destination->y_) < abs(start->y_ - destination->y_))
      && (mvinch(start->y_ - 1, start->x_) == '.')) {
    start->y_ = start->y_ - 1;
  } else {
    /* do nothing */
  }

  return 1;
}

Monster *Monster::GetMonsterAt(Position *position, Monster **monsters) {
  int x;
  for (x = 0; x < 6; x++) {
    if ((position->y_ == monsters[x]->position_->y_) && (position->x_ == monsters[x]->position_->x_))
      return monsters[x];
  }
  return nullptr;
}

void Monster::DrawMonster(Monster *monster) {
  if (monster->alive_) {
    mvprintw(monster->position_->y_, monster->position_->x_, monster->string_);
  }
}

}