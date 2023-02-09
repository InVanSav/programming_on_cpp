#pragma once

#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <ctime>

namespace roguelike {

class Position {
 public:
  Position() = default;
  ~Position() = default;

  int x_;
  int y_;
};

class Door {
 public:
  Door() = default;
  ~Door() = default;

  Position position_;
  int connected_;
};

class Room {
 public:
  Room() = default;
  static Room *CreateRoom(int grid, int number_of_doors);
  static int DrawRoom(Room *room);
  static Room **RoomsSetUp();
  ~Room() = default;

  Position position_;
  int height_;
  int width_;
  Door **doors_;
  int number_of_doors_;
};

class Player {
 public:
  Player() = default;
  static Player *PlayerSetUp();
  static Position *HandleInput(int input, Player *user);
  static void PlayerMove(Position *new_position, Player *user, char **level);
  static void PlacePlayer(Room **rooms, Player *user);
  static void DrawPlayer(Player *player);
  ~Player() = default;

  int health_;
  int attack_;
  bool is_attacked_;
  Position *position_;
  int gold_;
  int max_health_;
  int exp_;
};

class Monster {
 public:
  Monster() = default;
  static Monster *SelectMonster(int level);
  static Monster *CreateMonster(char symbol, int health, int attack, int speed, int defence, int pathfinding);
  static void SetStartingPosition(Monster *monster, Room *room);
  static int PathfindingSeek(Position *start, Position *destination);
  static int PathfindingRandom(Position *position);
  static Monster *GetMonsterAt(Position *position, Monster **monsters);
  static int KillMonster(Monster *monster);
  static void DrawMonster(Monster *monster);
  ~Monster() = default;

  int health_;
  char string_[2];
  char symbol_;
  int attack_;
  int speed_;
  int defence_;
  bool is_attacked_;
  int pathfinding_;
  int alive_;
  Position *position_;
};

class Level {
 public:
  Level() = default;
  static Level *CreateLevel(int level);
  static char **SaveLevelPositions();
  static void ConnectDoors(Level *level);
  static void DrawLevel(Level *level);
  static void AddMonsters(Level *level);
  static int MoveMonsters(Level *level);
  static void CheckPosition(Position *new_position, Level *level);
  static void Combat(Player *player, Monster *monster, int order);
  ~Level() = default;

  Player *user_;
  int number_of_monsters_;
  char **tiles_;
  int level_;
  int number_of_rooms_;
  Room **rooms_;
  Monster **monsters_;
};

class ScreenParameters {
 public:
  ScreenParameters() = default;
  static ScreenParameters *GetScreenParameters();
  static void ScreenSetUp();
  static void PrintGameHub(Level *level);
  ~ScreenParameters() = default;

  int y_max_;
  int x_max_;
};

class Game {
 public:
  Game() = default;
  static int GameLoop(Game *game);
  ~Game() = default;

  int current_level_;
  Level *levels_[10];
};

}
