#include "astar.h"
#include "custom_queue.h"
#include <cmath>
#include <limits>

using namespace std;

struct Node
{
    int r, c;
    int g, h, f;

    Node(int row, int col, int gCost, int hCost)
    {
        r = row;
        c = col;
        g = gCost;
        h = hCost;
        f = g + h;
    }
};

// For priority queue (min f-cost)
struct CompareNode
{
    bool operator()(Node* a, Node* b)
    {
        return a->f > b->f;
    }
};

AStar::AStar(Grid* g)
{
    grid = g;
}

int AStar::heuristic(int r1, int c1, int r2, int c2)
{
    // Manhattan distance
    return abs(r1 - r2) + abs(c1 - c2);
}

vector<pair<int,int>> AStar::findPath(
    int sr, int sc,
    int er, int ec)
{
    int rows = grid->getrows();
    int cols = grid->getcols();

    vector<vector<bool>> closed(rows, vector<bool>(cols, false));
    vector<vector<int>> gCost(rows, vector<int>(cols, INT_MAX));
    vector<vector<pair<int,int>>> parent(
        rows, vector<pair<int,int>>(cols, {-1,-1})
        );

    CustomPriorityQueue<Node*, vector<Node*>, CompareNode> open;

    gCost[sr][sc] = 0;
    int h = heuristic(sr, sc, er, ec);
    open.push(new Node(sr, sc, 0, h));


    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    while (!open.empty())
    {
        Node* current = open.top();
        open.pop();

        int r = current->r;
        int c = current->c;

        if (closed[r][c])
            continue;

        closed[r][c] = true;

        // Destination reached
        if (r == er && c == ec)
        {
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
            return path;
        }

        // Explore neighbors
        for (int i = 0; i < 4; i++)
        {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (!grid->isvalidcell(nr, nc))
                continue;

            // ONLY allow walls (5), main board (3), and socket (2)
            int cellValue = grid->getcell(nr, nc);
            if (cellValue != 5 && cellValue != 3 && cellValue != 2 &&cellValue !=8) {
                continue;  // Block everything else including empty cells (0)
            }

            if (closed[nr][nc])
                continue;

            int newG = gCost[r][c] + 1;

            if (newG < gCost[nr][nc])
            {
                gCost[nr][nc] = newG;
                parent[nr][nc] = {r, c};

                int newH = heuristic(nr, nc, er, ec);
                open.push(new Node(nr, nc, newG, newH));
            }
        }
    }

    // No path found
    return {};
}
