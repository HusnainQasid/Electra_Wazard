#include"grid.h"
Grid::Grid(int r, int c){
    rows=r; cols=c;
    initializegrid();
}
void Grid::clearcell(int r,int c){
    grid[r][c]=0;}
void Grid::initializegrid(){ grid.resize(rows);
    for (int r = 0; r < rows; r++) {
        grid[r].resize(cols);
        for (int c = 0; c < cols; c++) {
            grid[r][c] = 0; // Initialize as empty space

            // 1. External Border
            if (r == 1 || r == rows - 2 || c == 1 || c == cols - 2) {
                grid[r][c] = 5;
            }

            // 2. Vertical Main Dividers (Columns)
            // These are the long vertical lines that separate the rooms
            if (c == 15 || c == 30 || c == 40 || c == 53) {
                grid[r][c] = 5;
            }

            // 3. Horizontal Room Dividers (Rows)
            // Top section dividers
            if (r == 12 && (c < 15 || (c > 30 && c < 40))) {
                grid[r][c] = 5;
            }
            // Bottom section dividers
            if (r == 38 && (c < 15 || (c > 30 && c < 40))) {
                grid[r][c] = 5;
            }

            // 4. Center Cross/Hallway Structures
            // Left side "T" shape
            if (c < 15 && (r == 20 || r == 30)) {
                grid[r][c] = 5;
            }
            // Middle horizontal connectors
            if (r == 22 && (c > 30 && c < 40)) {
                grid[r][c] = 5;
            }
            if (r == 28 && (c > 30 && c < 40)) {
                grid[r][c] = 5;
            }

            // 5. Large Right-Side Room (Rectangle)
            if ((c >= 50 && c <= 68) && (r == 18 || r == 32)) {
                grid[r][c] = 5;
            }

            // 6. Small Room Nooks (Far Right)
            if (c > 62 && (r == 12 || r == 38)) {
                grid[r][c] = 5;
            }
        }
    }
}void Grid::setsocket(int r, int c){
    if(isvalidcell(r,c))
    {grid[r][c]=2;
    }
}
void Grid::setwall(int r, int c)
{
    if(isvalidcell(r,c)){
        grid[r][c]=5;
        }
}
void Grid::setmainboard(int r, int c)
{
    if(isvalidcell(r,c)){
        grid[r][c]=3;}
}
void Grid::setrestrictedd(int r, int c)
{
    if(isvalidcell(r,c))
    {
        grid[r][c]=7;}
}
bool Grid::isvalidcell(int r, int c){
     return(r>=0&&r<rows&&c>=0&&c<cols);
}
int Grid::getcell(int r, int c){
    if(isvalidcell(r,c))
    {
        return(grid[r][c]);
    }
    return -1;
}
void Grid::setCell(int r, int c, int value)
{
    if (isvalidcell(r, c))
    {
        grid[r][c] = value;
    }
}

int Grid::getrows()const{
    return rows;
}
int Grid::getcols()const{
    return cols;
}
void Grid::setfan(int r, int c){
    if(isvalidcell(r,c))
        grid[r][c] = 6;
}

void Grid::setbulb(int r, int c){
    if(isvalidcell(r,c))
        grid[r][c] = 9;
}
bool Grid::isWalkable(int r, int c)
{
    if (!isvalidcell(r, c))
        return false;

    int val = grid[r][c];

    // Wiring allowed ONLY inside walls and devices
    if (val == 5 ||   // Wall
        val == 2 ||   // Socket
        val == 3 ||   // Main Board
        val == 6 ||   // Fan
        val == 9||   // Main Board
        val == 8)     // already occupied
    {
        return true;
    }

    return false; // Empty or Restricted
}
bool Grid::isWireCell(int r, int c)
{
    if (!isvalidcell(r, c))
        return false;

    int v = grid[r][c];

    return (v == 8 || v == 5 || v == 3 || v == 2);
}
bool Grid::isWall(int r, int c)
{
    return isvalidcell(r,c) && grid[r][c] == 5;
}

bool Grid::isRestricted(int r, int c)
{
    return isvalidcell(r,c) && grid[r][c] == 7;
}

bool Grid::isEmpty(int r, int c)
{
    return isvalidcell(r,c) && grid[r][c] == 0;
}

bool Grid::canPlaceFan(int r, int c)
{
    if (!isvalidcell(r,c)) return false;
    if (!isEmpty(r,c)) return false;
    if (isWall(r,c) || isRestricted(r,c)) return false;

    return true;
}
int Grid::countFansInRoom(const Room &room)
{
    int count = 0;
    for (int i = room.top; i <= room.bottom; i++)
        for (int j = room.left; j <= room.right; j++)
            if (grid[i][j] == 6)
                count++;
    return count;
}
bool Grid::placeFan(int r, int c)
{
    if (!canPlaceFan(r,c)) return false;

    Room room = findRoom(r,c);
    int fanCount = countFansInRoom(room);

    int centerRow = (room.top + room.bottom) / 2;
    int centerCol = (room.left + room.right) / 2;

    // RULE 1: First fan must be center
    if (fanCount == 0)
    {
        if (r == centerRow && c == centerCol)
        {
            grid[r][c] = 6;
            return true;
        }
        return false;
    }

    // RULE 2: Second fan symmetric
    if (fanCount == 1)
    {
        int dist = (room.bottom - room.top) / 4;

        if ((r == centerRow - dist || r == centerRow + dist) &&
            c == centerCol)
        {
            grid[r][c] = 6;
            return true;
        }
        return false;
    }

    // Max 2 fans per room
    return false;
}

Grid::Room Grid::findRoom(int r, int c)
{
    Room room;

    room.top = r;
    while (room.top > 0 && grid[room.top-1][c] != 5)
        room.top--;

    room.bottom = r;
    while (room.bottom < rows-1 && grid[room.bottom+1][c] != 5)
        room.bottom++;

    room.left = c;
    while (room.left > 0 && grid[r][room.left-1] != 5)
        room.left--;

    room.right = c;
    while (room.right < cols-1 && grid[r][room.right+1] != 5)
        room.right++;

    return room;
}
std::vector<std::pair<int,int>> Grid::suggestFanPositions(int r, int c)
{
    std::vector<std::pair<int,int>> suggestions;

    if (!isvalidcell(r,c)) return suggestions;

    Room room = findRoom(r,c);
    int fanCount = countFansInRoom(room);

    int centerRow = (room.top + room.bottom) / 2;
    int centerCol = (room.left + room.right) / 2;

    // Case 1: No fan → suggest center
    if (fanCount == 0)
    {
        if (canPlaceFan(centerRow, centerCol))
            suggestions.push_back({centerRow, centerCol});
    }

    // Case 2: One fan → suggest symmetric positions
    else if (fanCount == 1)
    {
        int offset = (room.bottom - room.top) / 4;

        int r1 = centerRow - offset;
        int r2 = centerRow + offset;

        if (canPlaceFan(r1, centerCol))
            suggestions.push_back({r1, centerCol});

        if (canPlaceFan(r2, centerCol))
            suggestions.push_back({r2, centerCol});
    }

    return suggestions;
}
bool Grid::isFanWireCell(int r, int c) const
{
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    return (grid[r][c] == 10);
}





