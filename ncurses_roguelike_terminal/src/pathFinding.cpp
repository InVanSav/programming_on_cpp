#include "include/rogue.hpp"
#include "include/utils.hpp"

namespace roguelike {

void Utils::AddPositionYX(int **frontier, int frontier_count, int y, int x) {
  frontier[frontier_count][0] = y;
  frontier[frontier_count][1] = x;
}

int Utils::CheckPosition(int y, int x) {
  char temp = mvinch(y, x);

  if (temp == '.' || temp == '|' || temp == '-')
    return 0;
  else
    return 1;
}

int Utils::AddNeighbors(int **frontier, int frontier_count, int ***came_from, int y, int x) {
  int max_height = 30, max_width = 200;

  // north
  if (y > 0 && came_from[y - 1][x][0] < 0 && CheckPosition(y - 1, x)) {
    AddPositionYX(frontier, frontier_count, y - 1, x);
    frontier_count++;
    came_from[y - 1][x][0] = y;
    came_from[y - 1][x][1] = x;
  }

  // south
  if (y < (max_height - 1) && came_from[y + 1][x][0] < 0 && CheckPosition(y + 1, x)) {
    AddPositionYX(frontier, frontier_count, y + 1, x);
    frontier_count++;
    came_from[y + 1][x][0] = y;
    came_from[y + 1][x][1] = x;
  }

  // east
  if (x < (max_width - 1) && came_from[y][x + 1][0] < 0 && CheckPosition(y, x + 1)) {
    AddPositionYX(frontier, frontier_count, y, x + 1);
    frontier_count++;
    came_from[y][x + 1][0] = y;
    came_from[y][x + 1][1] = x;
  }

  // west
  if (x > 0 && came_from[y][x - 1][0] < 0 && CheckPosition(y, x - 1)) {
    AddPositionYX(frontier, frontier_count, y, x - 1);
    frontier_count++;
    came_from[y][x - 1][0] = y;
    came_from[y][x - 1][1] = x;
  }

  return frontier_count;

}

void Utils::PathFind(Position *start, Position *end) {
  int max_height = 30, max_width = 200;
  int i, j;
  int x, y;
  int temp_y;
  int **frontier = new int*[max_width * max_height];
  int ***came_from = new int**[max_height];

  int frontier_index = 0;
  int frontier_count = 0;

  for (i = 0; i < max_height * max_width; i++) {
    frontier[i] = new int[2];
  }

  for (i = 0; i < max_height; i++) {
    came_from[i] = new int*[max_width];
    for (j = 0; j < max_width; j++) {
      came_from[i][j] = new int[2];
      came_from[i][j][0] = -1;
      came_from[i][j][1] = -1;
    }
  }

  // add start to came_from
  came_from[start->y_][start->x_][0] = -9;
  came_from[start->y_][start->x_][1] = -9;

  // add start position to frontier
  AddPositionYX(frontier, frontier_count, start->y_, start->x_);
  frontier_count++;

  while (frontier_index < frontier_count) {
    y = frontier[frontier_index][0];
    x = frontier[frontier_index][1];

    frontier_index++;

    if (y == end->y_ && x == end->x_) {
      break;
    }

    frontier_count = AddNeighbors(frontier, frontier_count, came_from, y, x);
  }

  y = end->y_;
  x = end->x_;

  while (y != start->y_ || x != start->x_) {
    temp_y = y;
    y = came_from[temp_y][x][0];
    x = came_from[temp_y][x][1];
    mvprintw(y, x, "+");
  }
}

}
