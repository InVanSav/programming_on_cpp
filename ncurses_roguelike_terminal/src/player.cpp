#include "include/rogue.hpp"

namespace roguelike {

Player *Player::PlayerSetUp() {
  Player *new_player;
  new_player = new Player;
  new_player->position_ = new Position;

  new_player->health_ = 40;
  new_player->attack_ = 1;
  new_player->gold_ = 0;
  new_player->exp_ = 0;
  new_player->max_health_ = 40;

  return new_player;
}

void Player::PlacePlayer(Room **rooms, Player *user) {
  user->position_->x_ = rooms[3]->position_.x_ + 1;
  user->position_->y_ = rooms[3]->position_.y_ + 1;
}

Position *Player::HandleInput(int input, Player *user) {
  Position *new_position;
  new_position = new Position;
  switch (input) {
    /* move up */
    case 'w':
    case 'W':
      new_position->y_ = user->position_->y_ - 1;
      new_position->x_ = user->position_->x_;
      break;

      /* move down */
    case 's':
    case 'S':
      new_position->y_ = user->position_->y_ + 1;
      new_position->x_ = user->position_->x_;
      break;

      /* move left */
    case 'a':
    case 'A':
      new_position->y_ = user->position_->y_;
      new_position->x_ = user->position_->x_ - 1;
      break;

      /* move right */
    case 'd':
    case 'D':
      new_position->y_ = user->position_->y_;
      new_position->x_ = user->position_->x_ + 1;
      break;

    default:
      break;

  }

  return new_position;

}

/* check what is at next position */
void Level::CheckPosition(Position *new_position, Level *level) {
  Player *user;
  user = level->user_;
  switch (mvinch(new_position->y_, new_position->x_)) {
    case '.':
    case '#':
    case '+':
      Player::PlayerMove(new_position, user, level->tiles_);
      break;
    case 'X':
    case 'G':
    case 'T':
      Level::Combat(user, Monster::GetMonsterAt(new_position, level->monsters_), 1);

    default:
      break;
  }

}

void Player::PlayerMove(Position *new_position, Player *user, char **level) {
  user->position_->y_ = new_position->y_;
  user->position_->x_ = new_position->x_;
}

void Player::DrawPlayer(Player *player) {
  mvprintw(player->position_->y_, player->position_->x_, "@");
  move(player->position_->y_, player->position_->x_);
}

}
