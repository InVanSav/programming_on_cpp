#include "include/rogue.hpp"
#include "include/utils.hpp"

namespace roguelike {

Level *Level::CreateLevel(int level) {
  Level *new_level;
  new_level = reinterpret_cast<Level *>(new Level *);
  new_level->rooms_ = reinterpret_cast<Room **>(new Room);

  new_level->level_ = level;
  new_level->number_of_rooms_ = 6;
  new_level->rooms_ = Room::RoomsSetUp();
  ConnectDoors(new_level);
  new_level->tiles_ = SaveLevelPositions();

  new_level->user_ = Player::PlayerSetUp();
  Player::PlacePlayer(new_level->rooms_, new_level->user_);

  AddMonsters(new_level);

  return new_level;
}

void Level::DrawLevel(Level *level) {
  int max_height = 25, max_width = 100;
  int x, y, i;

  // printing tiles
  for (y = 0; y < max_height; y++) {
    for (x = 0; x < max_width; x++) {
      mvaddch(y, x, level->tiles_[y][x]);
    }
  }

  // print monsters
  for (i = 0; i < level->number_of_monsters_; i++) {
    Monster::DrawMonster(level->monsters_[i]);
  }

  Player::DrawPlayer(level->user_);
}

Room **Room::RoomsSetUp() {
  int x;
  Room **rooms;
  rooms = reinterpret_cast<Room **>(new Room[6]);

  for (x = 0; x < 6; x++) {
    rooms[x] = Room::CreateRoom(x, 4);
    Room::DrawRoom(rooms[x]);
  }

  return rooms;
}

void Level::ConnectDoors(Level *level) {
  int i, j;
  int random_room, random_door;
  int count;
  srand(time(nullptr));

  for (i = 0; i < level->number_of_rooms_; i++) {
    for (j = 0; j < level->rooms_[i]->number_of_doors_; j++) {
      if (level->rooms_[i]->doors_[j]->connected_ == 1) {
        continue;
      }

      count = 0;

      while (count < 2) {
        random_room = rand() % level->number_of_rooms_;
        random_door = rand() % level->rooms_[random_room]->number_of_doors_;

        if (level->rooms_[random_room]->doors_[random_door]->connected_ == 1 || random_room == i) {
          count++;
          continue;
        }

        Utils::PathFind(&level->rooms_[random_room]->doors_[random_door]->position_,
                 &level->rooms_[i]->doors_[j]->position_);

        level->rooms_[random_room]->doors_[random_door]->connected_ = 1;
        level->rooms_[i]->doors_[j]->connected_ = 1;
        break;
      }

    }
  }

}

char **Level::SaveLevelPositions() {
  int max_height = 30, max_width = 200;
  int x, y;
  char **positions;
  positions = new char*[max_height];

  for (y = 0; y < max_height; y++) {
    positions[y] = new char[max_width];
    for (x = 0; x < max_width; x++) {
      positions[y][x] = mvinch(y, x);
    }
  }

  return positions;
}

}