#include "include/rogue.hpp"

namespace roguelike {

Room *Room::CreateRoom(int grid, int number_of_doors) {
  int i;
  Room *new_room;
  new_room = new Room;
  new_room->number_of_doors_ = number_of_doors;
  srand(time(nullptr));

  switch (grid) {
    case 0:
      new_room->position_.x_ = 0;
      new_room->position_.y_ = 0;
      break;
    case 1:
      new_room->position_.x_ = 33;
      new_room->position_.y_ = 0;
      break;

    case 2:
      new_room->position_.x_ = 66;
      new_room->position_.y_ = 0;
      break;
    case 3:
      new_room->position_.x_ = 0;
      new_room->position_.y_ = 14;
      break;

    case 4:
      new_room->position_.x_ = 33;
      new_room->position_.y_ = 14;
      break;
    case 5:
      new_room->position_.x_ = 66;
      new_room->position_.y_ = 14;
      break;

  }

  new_room->height_ = rand() % 6 + 4; /* max size 9 */
  new_room->width_ = rand() % 14 + 4; /* max size 17 */

  /* offset */
  new_room->position_.x_ += rand() % (30 - new_room->width_) + 1;
  new_room->position_.y_ += rand() % (10 - new_room->height_) + 1;

  new_room->doors_ = new Door*[number_of_doors];

  for (i = 0; i < number_of_doors; i++) {
    new_room->doors_[i] = new Door;
    new_room->doors_[i]->connected_ = 0;
  }

  /* top door */
  new_room->doors_[0]->position_.x_ = rand() % (new_room->width_ - 2) + new_room->position_.x_ + 1;
  new_room->doors_[0]->position_.y_ = new_room->position_.y_;

  /* left door */
  new_room->doors_[1]->position_.y_ = rand() % (new_room->height_ - 2) + new_room->position_.y_ + 1;
  new_room->doors_[1]->position_.x_ = new_room->position_.x_;

  /* bottom door */
  new_room->doors_[2]->position_.x_ = rand() % (new_room->width_ - 2) + new_room->position_.x_ + 1;
  new_room->doors_[2]->position_.y_ = new_room->position_.y_ + new_room->height_ - 1;

  /* right door */
  new_room->doors_[3]->position_.y_ = rand() % (new_room->height_ - 2) + new_room->position_.y_ + 1;
  new_room->doors_[3]->position_.x_ = new_room->position_.x_ + new_room->width_ - 1;

  return new_room;
}

int Room::DrawRoom(Room *room) {
  int x;
  int y;

  /* draw top and bottom */
  for (x = room->position_.x_; x < room->position_.x_ + room->width_; x++) {
    mvprintw(room->position_.y_, x, "-"); /* top */
    mvprintw(room->position_.y_ + room->height_ - 1, x, "-"); /* bottom */
  }

  /* draw floors and side walls */
  for (y = room->position_.y_ + 1; y < room->position_.y_ + room->height_ - 1; y++) {
    /* draw side walls */
    mvprintw(y, room->position_.x_, "|");
    mvprintw(y, room->position_.x_ + room->width_ - 1, "|");

    /* draw floors */
    for (x = room->position_.x_ + 1; x < room->position_.x_ + room->width_ - 1; x++) {
      mvprintw(y, x, ".");
    }
  }

  /* draw doors */
  mvprintw(room->doors_[0]->position_.y_, room->doors_[0]->position_.x_, "+");
  mvprintw(room->doors_[1]->position_.y_, room->doors_[1]->position_.x_, "+");
  mvprintw(room->doors_[2]->position_.y_, room->doors_[2]->position_.x_, "+");
  mvprintw(room->doors_[3]->position_.y_, room->doors_[3]->position_.x_, "+");

  return 1;
}

}