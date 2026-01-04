#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include<algorithm>
#include <utility>
#include "grid.h"

class AStar
{
public:
    AStar(Grid* grid);

    // Returns path as list of (row, col)
    std::vector<std::pair<int,int>> findPath(
        int startRow, int startCol,
        int endRow, int endCol
        );

private:
    Grid* grid;

    int heuristic(int r1, int c1, int r2, int c2);
};

#endif // ASTAR_H
