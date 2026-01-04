#ifndef ASTARFAN_H
#define ASTARFAN_H

#include <vector>
#include <utility>
#include "grid.h"

class AStarFan
{
public:
    AStarFan(Grid* g);

    std::vector<std::pair<int,int>> findPath(
        int sr, int sc,
        int er, int ec
        );

private:
    Grid* grid;
    int heuristic(int r1, int c1, int r2, int c2);
};

#endif
