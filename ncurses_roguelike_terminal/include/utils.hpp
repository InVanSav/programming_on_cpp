#pragma once

namespace roguelike {

class Utils {
  static void AddPositionYX(int **frontier, int frontier_count, int y, int x);
  static int CheckPosition(int y, int x);
  static int AddNeighbors(int **frontier, int frontier_count, int ***came_from, int y, int x);
 public:
  static void PathFind(Position *start, Position *end);
};

}
