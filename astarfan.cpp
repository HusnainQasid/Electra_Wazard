#include "astarfan.h"
#include "custom_queue.h"
#include <cmath>
#include <limits>
#include <QDebug>
using namespace std;

struct FanNode
{
    int r, c;
    int g, h, f;
    int dir; // Direction from parent: 0=up, 1=down, 2=left, 3=right, -1=start
    
    FanNode(int row, int col, int gCost, int hCost, int direction)
    {
        r = row;
        c = col;
        g = gCost;
        h = hCost;
        f = g + h;
        dir = direction;
    }
};

struct CompareFanNode
{
    bool operator()(FanNode* a, FanNode* b)
    {
        return a->f > b->f;
    }
};

AStarFan::AStarFan(Grid* g)
{
    grid = g;
}

int AStarFan::heuristic(int r1, int c1, int r2, int c2)
{
    return abs(r1 - r2) + abs(c1 - c2);
}

vector<pair<int,int>> AStarFan::findPath(
    int sr, int sc,
    int er, int ec)
{
    qDebug() << "AStarFan findPath: start=(" << sr << "," << sc
             << ") end=(" << er << "," << ec << ")";

    int rows = grid->getrows();
    int cols = grid->getcols();
    // However, for simplicity and standard A*, standard closed set + gCost check is usually fine.
    // We'll stick to 2D gCost, but update if we find a cheaper path (including turn cost).
    
    vector<vector<int>> gCost(rows, vector<int>(cols, INT_MAX));
    vector<vector<pair<int,int>>> parent(
        rows, vector<pair<int,int>>(cols, {-1,-1})
        );

    CustomPriorityQueue<FanNode*, vector<FanNode*>, CompareFanNode> open;

    gCost[sr][sc] = 0;
    int h = heuristic(sr, sc, er, ec);
    open.push(new FanNode(sr, sc, 0, h, -1)); // -1 = start direction

    int dr[4] = {-1, 1, 0, 0}; // Up, Down, Left, Right
    int dc[4] = {0, 0, -1, 1}; // Up, Down, Left, Right

    int iterations = 0;

    while (!open.empty())
    {
        FanNode* current = open.top();
        open.pop();
        int r = current->r;
        int c = current->c;
        int currentDir = current->dir;
        int currentG = current->g;

        iterations++;

        // Lazy deletion: if we found a better path to this node already, skip
        if (currentG > gCost[r][c]) {
            delete current;
            continue;
        }

        // Destination reached
        if (r == er && c == ec)
        {
            qDebug() << "Fan path found after" << iterations << "iterations";
            vector<pair<int,int>> path;
            while (!(r == sr && c == sc))
            {
                path.push_back({r, c});
                auto p = parent[r][c];
                r = p.first;
                c = p.second;
            }
            path.push_back({sr, sc});
            reverse(path.begin(), path.end());

            // Cleanup
            while(!open.empty()) { delete open.top(); open.pop(); }
            delete current;
            return path;
        }

        // Explore neighbors
        for (int i = 0; i < 4; i++)
        {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (!grid->isvalidcell(nr, nc))
                continue;

            int cellValue = grid->getcell(nr, nc);

            // === UPDATED COSTS ===
            
            bool isWalkable = false;
            int baseCost = 0;

            if (cellValue == 7)
            {
                // BLOCKED: restricted areas
                isWalkable = false;
            }
            else if (cellValue == 9)
            {
                 // BLOCKED unless it's the target bulb
                 if (nr == er && nc == ec) {
                     isWalkable = true;
                     baseCost = 1;
                 } else {
                     isWalkable = false;
                 }
            }
            else if (cellValue == 5) // Wall
            {
                // CRITICAL: Wires CANNOT go through walls, except to reach the target socket/board!
                if (nr == er && nc == ec) {
                    isWalkable = true; // Destination is on a wall (e.g., socket)
                    baseCost = 1;
                } else {
                    isWalkable = false; // Block walls otherwise
                }
            }
            else if (cellValue == 8) // main Wire
            {
                // CRITICAL: Wires CANNOT go through walls, except to reach the target socket/board!
                if (nr == er && nc == ec) {
                    isWalkable = true; // Destination is on a wall (e.g., socket)
                    baseCost = 1;
                } else {
                    isWalkable = false; // Block walls otherwise
                }
            }
            else if (cellValue == 3) // main Board
            {
                // CRITICAL: Wires CANNOT go through walls, except to reach the target socket/board!
                if (nr == er && nc == ec) {
                    isWalkable = true; // Destination is on a wall (e.g., socket)
                    baseCost = 1;
                } else {
                    isWalkable = false; // Block walls otherwise
                }
            }
            else if (cellValue == 0 || cellValue == 11 || cellValue == 10) // Space, Bulb wire, Fan wire
            {
                // LOW COST: Prefer going through empty space or re-using wire paths
                isWalkable = true;
                baseCost = 1;
            }
            else if (cellValue == 2|| cellValue == 6 ) // Socket, Board, Fan
            {
                // ACCEPTABLE: Connect to these
                isWalkable = true;
                baseCost = 5;
            }
            else 
            {
                isWalkable = false;
            }

            if (!isWalkable) continue;

            // === TURN PENALTY ===
            int turnPenalty = 0;
            if (currentDir != -1 && currentDir != i) {
                turnPenalty = 15;
            }

            int newG = currentG + baseCost + turnPenalty;

            if (newG < gCost[nr][nc])
            {
                gCost[nr][nc] = newG;
                parent[nr][nc] = {r, c};
                int newH = heuristic(nr, nc, er, ec);
                open.push(new FanNode(nr, nc, newG, newH, i)); // Pass new direction 'i'
            }
        }
        delete current;
    }
    
    qDebug() << "No fan path found";
    return {};
}
