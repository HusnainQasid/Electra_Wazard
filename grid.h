#ifndef GRID_H
#define GRID_H

#include <vector>

class Grid
{
public:
    // ---- Room definition (MUST come first) ----
    struct Room
    {
        int top;
        int bottom;
        int left;
        int right;
    };

    // ---- Constructor ----
    Grid(int r, int c);

    // ---- Grid setup ----
    void initializegrid();

    // ---- Cell setters ----
    void setwall(int r, int c);
    void setsocket(int r, int c);
    void setmainboard(int r, int c);
    void setrestrictedd(int r, int c);
    void setfan(int r, int c);
    void setbulb(int r, int c);
    void clearcell(int r, int c);
    void setCell(int r, int c, int value);

    // ---- Getters ----
    int getcell(int r, int c);
    int getrows() const;
    int getcols() const;

    // ---- Validation ----
    bool isvalidcell(int r, int c);
    bool isWalkable(int r, int c);
    bool isWireCell(int r, int c);
    bool isWall(int r, int c);
    bool isRestricted(int r, int c);
    bool isEmpty(int r, int c);
    bool isFanWireCell(int r, int c) const;

    // ---- Fan logic ----
    bool canPlaceFan(int r, int c);
    bool placeFan(int r, int c);
    int countFansInRoom(const Room &room);
    Room findRoom(int r, int c);
    std::vector<std::pair<int,int>> suggestFanPositions(int r, int c);


private:
    int rows;
    int cols;
    std::vector<std::vector<int>> grid;
};

#endif // GRID_H
