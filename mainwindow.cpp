#include "gridwidget.h"
#include "grid.h"
#include "mainwindow.h"
#include "astar.h"
#include"astarfan.h"
#include "ui_mainwindow.h"
#include <vector>
#include <map>
#include <QInputDialog>
#include <cmath>
#include <limits>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpacerItem>

using namespace std;

Grid grid(45, 60);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // --- UI RESTRUCTURING (Programmatic Layout) ---
    
    // 1. Create Central Widget & Main Layout (Split View)
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setCentralWidget(central);

    // 2. Create Sidebar (Left)
    QWidget *sidebar = new QWidget(central);
    sidebar->setFixedWidth(280); // Fixed width sidebar
    sidebar->setObjectName("sidebar"); // For QSS
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(15);
    sidebarLayout->setContentsMargins(20, 30, 20, 30);
    
    // 3. Add Title to Sidebar
    QLabel *titleLabel = new QLabel("⚡ ELECTRO\n   WIZARD", sidebar);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: 900; color: #f38ba8; letter-spacing: 2px;");
    sidebarLayout->addWidget(titleLabel);
    sidebarLayout->addSpacing(20);

    // 4. Move Controls to Sidebar
    
    // Tool Selection
    QLabel *toolsLabel = new QLabel("🛠️ TOOLS", sidebar);
    toolsLabel->setStyleSheet("color: #bac2de; font-weight: bold; font-size: 12px;");
    sidebarLayout->addWidget(toolsLabel);
    
    ui->devicecomboBox->setParent(sidebar); // Reparent
    ui->devicecomboBox->setMinimumHeight(40);
    sidebarLayout->addWidget(ui->devicecomboBox);
    
    sidebarLayout->addSpacing(10);
    
    // Actions
    QLabel *actionsLabel = new QLabel("🚀 ACTIONS", sidebar);
    actionsLabel->setStyleSheet("color: #bac2de; font-weight: bold; font-size: 12px;");
    sidebarLayout->addWidget(actionsLabel);

    // Auto-Place Fans
    ui->autoPlaceFansButton->setParent(sidebar);
    ui->autoPlaceFansButton->setText(" 🔄 Auto-Place Fans");
    ui->autoPlaceFansButton->setMinimumHeight(45);
    ui->autoPlaceFansButton->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(ui->autoPlaceFansButton);

    // Connect Fans
    ui->connectFansButton->setParent(sidebar);
    ui->connectFansButton->setText(" 🔌 Connect Fans");
    ui->connectFansButton->setMinimumHeight(45);
    ui->connectFansButton->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(ui->connectFansButton);
    
    // Light Up
    ui->lightUpButton->setParent(sidebar);
    ui->lightUpButton->setText(" 💡 Light Up Bulbs");
    ui->lightUpButton->setMinimumHeight(45);
    ui->lightUpButton->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(ui->lightUpButton);

    // Preview
    ui->previewPathButton_->setParent(sidebar);
    ui->previewPathButton_->setText(" 🛤️ Show Paths");
    ui->previewPathButton_->setMinimumHeight(45);
    ui->previewPathButton_->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(ui->previewPathButton_);

    // Spacer to push everything up
    sidebarLayout->addStretch();
    
    // Credits/Footer
    QLabel *footer = new QLabel("v2.0 • Pro Edition", sidebar);
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("color: #6c7086; font-size: 10px;");
    sidebarLayout->addWidget(footer);

    // 5. Add Sidebar & Map to Main Layout
    mainLayout->addWidget(sidebar);
    
    // Re-parent GridWidget (Map)
    ui->widget->setParent(central);
    ui->widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(ui->widget);

    // --- STYLING (Modern Dark Theme) ---
    this->setStyleSheet(
        "QMainWindow { background-color: #1e1e2e; }"
        "QWidget#sidebar { background-color: #11111b; border-right: 1px solid #313244; }"
        
        "QLabel { color: #cdd6f4; font-family: 'Segoe UI', sans-serif; }"
        
        "QPushButton {"
        "   background-color: #1e1e2e;"
        "   color: #cdd6f4;"
        "   border: 1px solid #45475a;"
        "   border-radius: 8px;"
        "   padding-left: 15px;"
        "   text-align: left;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover { background-color: #313244; border-color: #89b4fa; color: #89b4fa; }"
        "QPushButton:pressed { background-color: #45475a; }"
        
        "QComboBox {"
        "   background-color: #181825;"
        "   color: #cdd6f4;"
        "   border: 1px solid #45475a;"
        "   border-radius: 8px;"
        "   padding: 5px 10px;"
        "}"
        "QComboBox::drop-down { border: none; }"
        "QComboBox QAbstractItemView { background-color: #181825; color: #cdd6f4; selection-background-color: #313244; }"
    );
    fanPlacementMode = false;

    ui->devicecomboBox->clear();
    ui->devicecomboBox->addItem("Wall", 5);
    ui->devicecomboBox->addItem("Socket", 2);
    ui->devicecomboBox->addItem("Main Board", 3);
    ui->devicecomboBox->addItem("Fan", 6);
    ui->devicecomboBox->addItem("Bulb", 9);
    ui->devicecomboBox->addItem("Restricted", 7);

    connect(ui->devicecomboBox, &QComboBox::currentIndexChanged,
            this, [=](int index){
                int deviceCode = ui->devicecomboBox->itemData(index).toInt();
                ui->widget->setMode(deviceCode);
            });

    connect(ui->previewPathButton_, &QPushButton::clicked,
            this, &MainWindow::on_previewPathButton_clicked);

    connect(ui->autoPlaceFansButton, &QPushButton::clicked,
            this, &MainWindow::on_autoPlaceFansButton_clicked);

    connect(ui->connectFansButton, &QPushButton::clicked,
            this, &MainWindow::on_connectFansButton_clicked);

    connect(ui->lightUpButton, &QPushButton::clicked,
            this, &MainWindow::on_lightUpBulbsButton_clicked);

    grid.initializegrid();
    ui->widget->setGrid(&grid);
}

// Helper function to collect all sockets (current + remembered)
vector<pair<int,int>> MainWindow::getAllSockets()
{
    vector<pair<int,int>> allSockets;

    // Find current sockets
    for (int r = 0; r < grid.getrows(); r++)
    {
        for (int c = 0; c < grid.getcols(); c++)
        {
            if (grid.getcell(r,c) == 2) {
                allSockets.push_back({r, c});
            }
        }
    }

    // Merge with remembered sockets (avoid duplicates)
    for (auto &oldSocket : rememberedSockets)
    {
        bool alreadyExists = false;
        for (auto &newSocket : allSockets)
        {
            if (oldSocket.first == newSocket.first &&
                oldSocket.second == newSocket.second)
            {
                alreadyExists = true;
                break;
            }
        }
        if (!alreadyExists) {
            allSockets.push_back(oldSocket);
        }
    }

    rememberedSockets = allSockets;
    return allSockets;
}

// Helper function to restore paths to original values
void MainWindow::restorePaths()
{
    for (int r = 0; r < grid.getrows(); r++)
    {
        for (int c = 0; c < grid.getcols(); c++)
        {
            int cellVal = grid.getcell(r, c);
            // Restore both main paths (8) and fan paths (10)
            if (cellVal == 8 || cellVal == 10) {
                auto key = make_pair(r, c);
                if (originalCellValues.find(key) != originalCellValues.end()) {
                    grid.setCell(r, c, originalCellValues[key]);
                } else {
                    grid.clearcell(r, c);
                }
            }
        }
    }
}

// Helper function to draw a path on the grid
int MainWindow::drawPath(const vector<pair<int,int>>& path, int pathType)
{
    int cellsDrawn = 0;
    for (auto &p : path)
    {
        int r = p.first;
        int c = p.second;
        int val = grid.getcell(r, c);

        // Different rules for main paths (8) vs fan paths (10)
        if (pathType == 8)
        {
            // MAIN PATHS: Only draw on walls (5) or existing main paths (8)
            if (val == 5 || val == 8)
            {
                auto key = make_pair(r, c);
                if (originalCellValues.find(key) == originalCellValues.end())
                {
                    originalCellValues[key] = val;
                }
                grid.setCell(r, c, pathType);
                cellsDrawn++;
            }
        }
        else if (pathType == 10)
        {
            // FAN PATHS: Draw on walls (5), empty space (0), and existing paths (8, 10, 11)
            // Don't overwrite fans (6), sockets (2), main board (3), restricted (7), or bulbs (9)
            if (val == 5 || val == 0 || val == 8 || val == 10 || val == 11)
            {
                auto key = make_pair(r, c);
                if (originalCellValues.find(key) == originalCellValues.end())
                {
                    originalCellValues[key] = val;
                }
                grid.setCell(r, c, pathType);
                cellsDrawn++;
            }
        }
        else if (pathType == 11)
        {
            // BULB PATHS: Similar to fans, can traverse most things except devices
            if (val == 5 || val == 0 || val == 8 || val == 10 || val == 11)
            {
                auto key = make_pair(r, c);
                if (originalCellValues.find(key) == originalCellValues.end())
                {
                    originalCellValues[key] = val;
                }
                grid.setCell(r, c, pathType);
                cellsDrawn++;
            }
        }
    }
    return cellsDrawn;
}

void MainWindow::on_previewPathButton_clicked()
{
    // 1. Find main board
    int sr = -1, sc = -1;
    for (int r = 0; r < grid.getrows(); r++)
    {
        for (int c = 0; c < grid.getcols(); c++)
        {
            if (grid.getcell(r,c) == 3) {
                sr = r; sc = c;
                break;
            }
        }
        if (sr != -1) break;
    }

    if (sr == -1) {
        qDebug() << "ERROR: Main board not found!";
        return;
    }

    qDebug() << "Main board found at:" << sr << sc;

    // 2. Get all sockets
    vector<pair<int,int>> currentSockets = getAllSockets();

    if (currentSockets.empty()) {
        qDebug() << "ERROR: No sockets found!";
        return;
    }

    qDebug() << "Total sockets:" << currentSockets.size();

    // 3. Restore old paths to their original values
    restorePaths();
    originalCellValues.clear();

    // 4. Restore socket markers
    for (auto &socket : currentSockets)
    {
        int val = grid.getcell(socket.first, socket.second);
        if (val == 0 || val == 8 || val == 10) {
            grid.setsocket(socket.first, socket.second);
        }
    }

    // 5. Find path to each socket
    AStar astar(&grid);
    int successfulPaths = 0;

    for (size_t i = 0; i < currentSockets.size(); i++)
    {
        int er = currentSockets[i].first;
        int ec = currentSockets[i].second;

        qDebug() << "Finding path to socket" << (i+1) << "at:" << er << ec;

        auto path = astar.findPath(sr, sc, er, ec);  // No boolean parameter

        if (path.empty()) {
            qDebug() << "  No path found to socket" << (i+1);
            continue;
        }

        qDebug() << "  Path found with" << path.size() << "cells";

        // Draw this path with type 8 (main board to socket)
        drawPath(path, 8);
        successfulPaths++;
    }

    qDebug() << "Successfully routed" << successfulPaths << "out of" << currentSockets.size() << "sockets";
    ui->widget->update();
}

void MainWindow::floodFill(int r, int c, vector<vector<bool>>& visited, Room& room)
{
    int rows = grid.getrows();
    int cols = grid.getcols();

    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (visited[r][c]) return;

    int val = grid.getcell(r, c);

    // Walls, paths, sockets, and main board define boundaries
    if (val == 5 || val == 8 || val == 10 || val == 2 || val == 3) {
        return;
    }

    visited[r][c] = true;
    room.cells.push_back({r, c});

    // Update bounding box
    room.minRow = min(room.minRow, r);
    room.maxRow = max(room.maxRow, r);
    room.minCol = min(room.minCol, c);
    room.maxCol = max(room.maxCol, c);

    // Recursively fill in 4 directions
    floodFill(r-1, c, visited, room);
    floodFill(r+1, c, visited, room);
    floodFill(r, c-1, visited, room);
    floodFill(r, c+1, visited, room);
}

MainWindow::Room MainWindow::detectRoomAt(int startR, int startC)
{
    int rows = grid.getrows();
    int cols = grid.getcols();

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    Room room;
    room.minRow = startR;
    room.maxRow = startR;
    room.minCol = startC;
    room.maxCol = startC;

    if (!grid.isvalidcell(startR, startC))
    {
        qDebug() << "Invalid starting position";
        return room;
    }

    int val = grid.getcell(startR, startC);
    if (val == 5 || val == 8 || val == 10 || val == 2)
    {
        qDebug() << "Cannot select a wall/path as room";
        return room;
    }

    floodFill(startR, startC, visited, room);

    qDebug() << "Room detected: size =" << room.cells.size()
             << ", bounds: [" << room.minRow << "-" << room.maxRow
             << ", " << room.minCol << "-" << room.maxCol << "]";

    return room;
}

void MainWindow::placeFansInRoom(const Room& room, int fanCount)
{
    if (fanCount <= 0) return;

    int roomWidth = room.maxCol - room.minCol + 1;
    int roomHeight = room.maxRow - room.minRow + 1;

    qDebug() << "Placing" << fanCount << "fan(s) in room of size" << roomWidth << "x" << roomHeight;

    vector<pair<int,int>> fanPositions;

    if (fanCount == 1)
    {
        // Single fan: center
        int centerRow = (room.minRow + room.maxRow) / 2;
        int centerCol = (room.minCol + room.maxCol) / 2;
        fanPositions.push_back({centerRow, centerCol});
    }
    else if (fanCount == 2)
    {
        // Two fans: between center and walls
        int centerRow = (room.minRow + room.maxRow) / 2;
        int centerCol = (room.minCol + room.maxCol) / 2;

        if (roomWidth >= roomHeight)
        {
            int offset = roomWidth / 4;
            fanPositions.push_back({centerRow, room.minCol + offset});
            fanPositions.push_back({centerRow, room.maxCol - offset});
        }
        else
        {
            int offset = roomHeight / 4;
            fanPositions.push_back({room.minRow + offset, centerCol});
            fanPositions.push_back({room.maxRow - offset, centerCol});
        }
    }
    else
    {
        // 3+ fans: grid layout
        int gridCols = (int)ceil(sqrt(fanCount));
        int gridRows = (int)ceil((double)fanCount / gridCols);

        qDebug() << "Creating" << gridRows << "x" << gridCols << "grid for" << fanCount << "fans";

        int rowSpacing = roomHeight / (gridRows + 1);
        int colSpacing = roomWidth / (gridCols + 1);

        int fanIndex = 0;
        for (int gr = 0; gr < gridRows && fanIndex < fanCount; gr++)
        {
            for (int gc = 0; gc < gridCols && fanIndex < fanCount; gc++)
            {
                int fanRow = room.minRow + (gr + 1) * rowSpacing;
                int fanCol = room.minCol + (gc + 1) * colSpacing;
                fanPositions.push_back({fanRow, fanCol});
                fanIndex++;
            }
        }
    }

    // Place fans
    for (auto& pos : fanPositions)
    {
        int r = pos.first;
        int c = pos.second;

        if (grid.isvalidcell(r, c))
        {
            int val = grid.getcell(r, c);
            if (val == 0 || val == 8 || val == 10 || val == 5)
            {
                grid.setfan(r, c);
                qDebug() << "  Placed fan at (" << r << "," << c << ")";
            }
            else
            {
                qDebug() << "  Cannot place fan at (" << r << "," << c << ") - occupied by" << val;
            }
        }
    }
}

void MainWindow::on_autoPlaceFansButton_clicked()
{
    fanPlacementMode = true;

    qDebug() << "Fan placement mode ENABLED - Click on a room to place fans";
    ui->autoPlaceFansButton->setText("Cancel Fan Placement");
    ui->autoPlaceFansButton->setStyleSheet("background-color: #ffcccc;");
    ui->widget->setCursor(Qt::CrossCursor);
}

void MainWindow::exitFanPlacementMode()
{
    fanPlacementMode = false;
    ui->autoPlaceFansButton->setText("Auto Place Fans");
    ui->autoPlaceFansButton->setStyleSheet("");
    ui->widget->setCursor(Qt::ArrowCursor);
}

// Calculate Euclidean distance between two points
double MainWindow::calculateDistance(int r1, int c1, int r2, int c2)
{
    int dr = r1 - r2;
    int dc = c1 - c2;
    return sqrt(dr * dr + dc * dc);
}

// Find the nearest socket to a given fan position
pair<int,int> MainWindow::findNearestSocket(int fanR, int fanC)
{
    pair<int,int> nearestSocket = {-1, -1};
    double minDistance = numeric_limits<double>::max();

    // Get all sockets (current + remembered)
    vector<pair<int,int>> allSockets = getAllSockets();

    for (auto &socket : allSockets)
    {
        double dist = calculateDistance(fanR, fanC, socket.first, socket.second);
        if (dist < minDistance)
        {
            minDistance = dist;
            nearestSocket = socket;
        }
    }

    qDebug() << "Nearest socket to fan at (" << fanR << "," << fanC
             << ") is at (" << nearestSocket.first << "," << nearestSocket.second
             << ") with distance:" << minDistance;

    return nearestSocket;
}

// Connect all fans to their nearest sockets
void MainWindow::on_connectFansButton_clicked()
{
    qDebug() << "=== CONNECTING FANS TO SOCKETS ===";

    // Find all fans (value 6) in the grid
    vector<pair<int,int>> fans;
    for (int r = 0; r < grid.getrows(); r++)
    {
        for (int c = 0; c < grid.getcols(); c++)
        {
            if (grid.getcell(r, c) == 6)  // Found a fan
            {
                fans.push_back({r, c});
            }
        }
    }

    if (fans.empty())
    {
        QMessageBox::information(this, "Connect Fans",
                                 "No fans found on the grid!\nPlease place fans first.");
        qDebug() << "No fans found!";
        return;
    }

    qDebug() << "Found" << fans.size() << "fan(s)";

    // Get all sockets
    vector<pair<int,int>> allSockets = getAllSockets();

    if (allSockets.empty())
    {
        QMessageBox::warning(this, "Connect Fans",
                             "No sockets found!\nPlease place sockets first and run 'Preview Path'.");
        qDebug() << "No sockets found!";
        return;
    }

    qDebug() << "Found" << allSockets.size() << "socket(s)";

    int successfulConnections = 0;

    // Connect each fan to its nearest socket
    for (size_t i = 0; i < fans.size(); i++)
    {
        int fanR = fans[i].first;
        int fanC = fans[i].second;

        qDebug() << "\n=== Processing fan" << (i+1) << "at (" << fanR << "," << fanC << ") ===";

        // Find nearest socket to this fan
        pair<int,int> nearestSocket = findNearestSocket(fanR, fanC);

        if (nearestSocket.first == -1)
        {
            qDebug() << "  ERROR: Could not find a socket for this fan";
            continue;
        }

        int socketR = nearestSocket.first;
        int socketC = nearestSocket.second;

        qDebug() << "Nearest socket at (" << socketR << "," << socketC << ")";
        qDebug() << "Starting AStarFan pathfinding...";

        // Create AStarFan instance for this path
        AStarFan astarFan(&grid);

        // Find path from socket to fan using AStarFan
        auto path = astarFan.findPath(socketR, socketC, fanR, fanC);

        qDebug() << "AStarFan returned path size:" << path.size();

        if (path.empty())
        {
            qDebug() << "ERROR: No path found from socket to fan";

            QMessageBox::warning(this, "Connection Failed",
                                 QString("Could not connect fan at (%1,%2) to socket at (%3,%4).\n\n"
                                         "Check the console for detailed debug information.")
                                     .arg(fanR).arg(fanC)
                                     .arg(socketR).arg(socketC));
            continue;
        }

        qDebug() << "SUCCESS: Path found with" << path.size() << "cells";

        // Draw the path with type 10 (fan-to-socket path - visually different)
        int cellsDrawn = drawPath(path, 10);

        qDebug() << "Drew" << cellsDrawn << "path cells";
        successfulConnections++;
    }

    qDebug() << "\nSuccessfully connected" << successfulConnections
             << "out of" << fans.size() << "fans";

    if (successfulConnections > 0)
    {
        QMessageBox::information(this, "Fans Connected",
                                 QString("Successfully connected %1 out of %2 fan(s) to their nearest sockets!")
                                     .arg(successfulConnections).arg(fans.size()));
    }

    qDebug() << "==================================";
    ui->widget->update();
}
void MainWindow::on_lightUpBulbsButton_clicked()
{
    qDebug() << "=== LIGHTING UP BULBS ===";

    // Find all bulbs
    vector<pair<int,int>> bulbs;
    for (int r = 0; r < grid.getrows(); r++) {
        for (int c = 0; c < grid.getcols(); c++) {
            if (grid.getcell(r, c) == 9) {
                bulbs.push_back({r, c});
            }
        }
    }

    if (bulbs.empty()) {
        QMessageBox::information(this, "Light Up", "No bulbs found!");
        return;
    }

    // For each bulb, find nearest socket and use AStarFan for correct L-shape routing
    for (auto& bulb : bulbs) {
        pair<int,int> socket = findNearestSocket(bulb.first, bulb.second);

        if (socket.first == -1) continue;
        
        // Use AStarFan logic (which now prefers open space and straight lines)
        AStarFan astar(&grid);
        auto path = astar.findPath(socket.first, socket.second, bulb.first, bulb.second);
        
        if (!path.empty()) {
             drawPath(path, 11); // 11 = Bulb Wire
        }
    }

    QMessageBox::information(this, "Success",
                             QString("Lit up %1 bulb(s)!").arg(bulbs.size()));
    ui->widget->update();
}

void MainWindow::drawZigzagPath(int r1, int c1, int r2, int c2, int pathType)
{
    // Crazy zigzag: go horizontal first, then vertical
    int currentR = r1;
    int currentC = c1;

    // Move horizontally
    while (currentC != c2) {
        int val = grid.getcell(currentR, currentC);
        if (val == 0 || val == 5 || val == 8 || val == 10) {
            grid.setCell(currentR, currentC, pathType);
        }
        currentC += (c2 > c1) ? 1 : -1;
    }

    // Move vertically
    while (currentR != r2) {
        int val = grid.getcell(currentR, currentC);
        if (val == 0 || val == 5 || val == 8 || val == 10) {
            grid.setCell(currentR, currentC, pathType);
        }
        currentR += (r2 > r1) ? 1 : -1;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
