#include "gridwidget.h"
#include "mainwindow.h"
#include <cmath>

GridWidget::GridWidget(QWidget *parent)
    : QWidget(parent)
{
    grid = nullptr;
    currentMode = 0; // nothing selected
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    
    // ANTIMATION SETUP
    animationPhase = 0;
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, [=](){
        animationPhase = (animationPhase + 1) % 20; // Cycle 0-19
        update();
    });
    animationTimer->start(50); // 20fps update for smooth flow
}

void GridWidget::setGrid(Grid *g) {
    grid = g;
}
void GridWidget::setPath(const std::vector<std::pair<int,int>>& path)
{
    currentPath = path;
    update();
}

void GridWidget::setMode(int m) {
    currentMode = m;
}
int GridWidget::getcellsize()const{
    return cellSize;
}

void GridWidget::paintEvent(QPaintEvent *)
{
    if (!grid) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. Background (Floor Plan Style)
    // Deep blue-gray background for the "blueprint" look
    painter.fillRect(rect(), QColor(30, 30, 46));

    // DYNAMIC SCALING & CENTERING
    // We calculate these every paint event to ensure responsiveness
    int rows = grid->getrows();
    int cols = grid->getcols();
    
    if (rows > 0 && cols > 0) {
        int cellW = width() / cols;
        int cellH = height() / rows;
        cellSize = std::min(cellW, cellH); // Keep squares
        
        // ensure minimum readability
        if (cellSize < 5) cellSize = 5; 
        
        offsetX = (width() - (cols * cellSize)) / 2;
        offsetY = (height() - (rows * cellSize)) / 2;
    }

    // Apply translation for centering
    painter.translate(offsetX, offsetY);

    // Subtle Grid Dots (instead of lines)
    if (cellSize > 4) // Only if large enough
    {
        painter.setPen(QPen(QColor(255, 255, 255, 20), 1));
        for (int r = 0; r <= rows; r++) {
            for (int c = 0; c <= cols; c++) {
                painter.drawPoint(c * cellSize, r * cellSize);
            }
        }
    }

    // 2. Draw Components
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int value = grid->getcell(r, c);
            QRect cellRect(c * cellSize, r * cellSize, cellSize, cellSize);
            QPoint center = cellRect.center();

            // --- WALLS (Continuous Connections + WIDER + 3D Shadow)
            if (value == 5) 
            {
                painter.setBrush(QColor(20, 20, 30, 100)); // Semi-transparent black
                painter.setPen(Qt::NoPen);
                
                double gap = cellSize * 0.15;
                double thick = cellSize * 0.7; 
                
                // Draw Shadow first
                QRectF shadowBlock(c * cellSize + gap + 4, r * cellSize + gap + 4, thick, thick);
                painter.drawRect(shadowBlock);

                // Main Wall
                painter.setBrush(QColor(108, 112, 134)); // Wall color
                
                // Draw center block
                QRectF centerBlock(c * cellSize + gap, r * cellSize + gap, thick, thick);
                painter.drawRect(centerBlock);

                // Check neighbors to connect
                bool up = (r > 0 && grid->getcell(r-1, c) == 5);
                bool down = (r < rows-1 && grid->getcell(r+1, c) == 5);
                bool left = (c > 0 && grid->getcell(r, c-1) == 5);
                bool right = (c < cols-1 && grid->getcell(r, c+1) == 5);

                if (up) painter.drawRect(QRectF(c * cellSize + gap, r * cellSize, thick, gap));
                if (down) painter.drawRect(QRectF(c * cellSize + gap, r * cellSize + gap + thick, thick, gap));
                if (left) painter.drawRect(QRectF(c * cellSize, r * cellSize + gap, gap, thick));
                if (right) painter.drawRect(QRectF(c * cellSize + gap + thick, r * cellSize + gap, gap, thick));
            }
            // --- SOCKETS ---
            else if (value == 2) 
            {
                painter.setBrush(QColor(250, 179, 135)); // Soft peach/orange
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(center, cellSize/3, cellSize/3);
                
                // Icon details
                painter.setBrush(QColor(30, 30, 46));
                painter.drawEllipse(center.x() - cellSize/8, center.y(), cellSize/10, cellSize/10);
                painter.drawEllipse(center.x() + cellSize/8, center.y(), cellSize/10, cellSize/10);
            }
            // --- MAIN BOARD ---
            else if (value == 3) 
            {
                painter.setBrush(QColor(137, 180, 250)); // Blue
                painter.setPen(QPen(QColor(255,255,255), 2));
                painter.drawRoundedRect(cellRect.adjusted(2,2,-2,-2), 4, 4);
                
                // Lightning bolt
                QPolygonF bolt;
                bolt << QPointF(center.x(), center.y()-cellSize/3)
                     << QPointF(center.x()-cellSize/4, center.y())
                     << QPointF(center.x()+cellSize/6, center.y())
                     << QPointF(center.x()-cellSize/6, center.y()+cellSize/3);
                painter.drawPolyline(bolt);
            }
            // --- FANS ---
            else if (value == 6) 
            {
                painter.setBrush(QColor(148, 226, 213)); // Teal
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(center, cellSize/4, cellSize/4);
                
                // Blades
                for(int i=0; i<3; i++) {
                    QPolygonF blade;
                    double angle = (i * 2 * 3.14159 / 3) + (animationPhase * 0.1); // SPINNING ANIMATION!
                    blade << QPointF(center) 
                          << QPointF(center.x() + cos(angle)*cellSize/1.5, center.y() + sin(angle)*cellSize/1.5)
                          << QPointF(center.x() + cos(angle+0.3)*cellSize/1.5, center.y() + sin(angle+0.3)*cellSize/1.5);
                    painter.drawPolygon(blade);
                }
            }
            // --- BULBS ---
            else if (value == 9) 
            {
                painter.setBrush(QColor(249, 226, 175)); // Yellow
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(center, cellSize/3, cellSize/3);
                
                // Glow ring (Pulsing)
                int pulse = abs((animationPhase % 20) - 10); // 0->10->0
                painter.setBrush(Qt::NoBrush);
                painter.setPen(QPen(QColor(249, 226, 175, 100 + pulse*5), 2));
                painter.drawEllipse(center, cellSize/2 + pulse/2, cellSize/2 + pulse/2);
            }
            // --- RESTRICTED ---
            else if (value == 7) 
            {
                painter.setPen(QPen(QColor(243, 139, 168), 2)); // Red/Pink
                painter.drawLine(cellRect.topLeft(), cellRect.bottomRight());
                painter.drawLine(cellRect.topRight(), cellRect.bottomLeft());
            }
            // --- WIRES (Main, Fan, Bulb) ---
            else if (value == 8 || value == 10 || value == 11) 
            {
                QColor wireColor;
                if (value == 8) wireColor = QColor(166, 227, 161); // Green
                else if (value == 10) wireColor = QColor(250, 179, 135); // Peach
                else wireColor = QColor(249, 226, 175); // Gold

                painter.setPen(QPen(wireColor, 2));
                
                // Helper lambda for wire drawing
                auto drawWire = [&](bool cond, QPoint p1, QPoint p2) {
                     if (cond) painter.drawLine(p1, p2);
                };

                // Check connections
                bool up = grid->getcell(r-1, c) == value; // Simplified neighbor check for speed/visuals
                bool down = grid->getcell(r+1, c) == value;
                bool left = grid->getcell(r, c-1) == value;
                bool right = grid->getcell(r, c+1) == value;
                
                // Check device connection compatibility (simplified)
                if (r>0 && (grid->getcell(r-1,c)==2 || grid->getcell(r-1,c)==3 || grid->getcell(r-1,c)==6 || grid->getcell(r-1,c)==9)) up=true;
                if (r<rows-1 && (grid->getcell(r+1,c)==2 || grid->getcell(r+1,c)==3 || grid->getcell(r+1,c)==6 || grid->getcell(r+1,c)==9)) down=true;
                if (c>0 && (grid->getcell(r,c-1)==2 || grid->getcell(r,c-1)==3 || grid->getcell(r,c-1)==6 || grid->getcell(r,c-1)==9)) left=true;
                if (c<cols-1 && (grid->getcell(r,c+1)==2 || grid->getcell(r,c+1)==3 || grid->getcell(r,c+1)==6 || grid->getcell(r,c+1)==9)) right=true;

                drawWire(up, center, QPoint(center.x(), cellRect.top()));
                drawWire(down, center, QPoint(center.x(), cellRect.bottom()));
                drawWire(left, center, QPoint(cellRect.left(), center.y()));
                drawWire(right, center, QPoint(cellRect.right(), center.y()));
                
                painter.setBrush(wireColor);
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(center, 3, 3); // Slightly larger node

                // --- ANIMATED CURRENT FLOW ---
                // Draw a second dashed white line moving along the wire
                QPen dashPen(QColor(255, 255, 255, 150), 2);
                QVector<qreal> dashes;
                dashes << 3 << 5; // dash 3px, space 5px
                dashPen.setDashPattern(dashes);
                dashPen.setDashOffset(animationPhase); // ANIMATE OFFSET!
                painter.setPen(dashPen);

                drawWire(up, center, QPoint(center.x(), cellRect.top()));
                drawWire(down, center, QPoint(center.x(), cellRect.bottom()));
                drawWire(left, center, QPoint(cellRect.left(), center.y()));
                drawWire(right, center, QPoint(cellRect.right(), center.y()));
            }
        }
        
        // Draw Fan Paths (Overlay)
        if (currentFanPath.size() > 1) {
             painter.setPen(QPen(QColor(148, 226, 213), 2, Qt::DashLine));
             for (size_t i = 1; i < currentFanPath.size(); i++) {
                 QPoint p1(currentFanPath[i-1].second * cellSize + cellSize/2,
                           currentFanPath[i-1].first  * cellSize + cellSize/2);
                 QPoint p2(currentFanPath[i].second * cellSize + cellSize/2,
                           currentFanPath[i].first  * cellSize + cellSize/2);
                 painter.drawLine(p1, p2);
             }
        }
    }
    
    // --- GHOST TOOL PREVIEW ---
    if (hoverRow >= 0 && hoverCol >= 0 && hoverRow < rows && hoverCol < cols && currentMode != 0)
    {
        QRect cellRect(hoverCol * cellSize, hoverRow * cellSize, cellSize, cellSize);
        QPoint center = cellRect.center();
        
        // Ghost style: Semi-transparent
        painter.setOpacity(0.5); 
        
        if (currentMode == 5) // Wall
        {
            painter.setBrush(QColor(108, 112, 134));
            painter.setPen(Qt::NoPen);
            double gap = cellSize * 0.15;
            double thick = cellSize * 0.7; 
            painter.drawRect(QRectF(hoverCol * cellSize + gap, hoverRow * cellSize + gap, thick, thick));
        }
        else if (currentMode == 2) // Socket
        {
            painter.setBrush(QColor(250, 179, 135));
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(center, cellSize/3, cellSize/3);
        }
        else if (currentMode == 3) // Main Board
        {
            painter.setBrush(QColor(137, 180, 250));
            painter.setPen(QPen(QColor(255,255,255), 2));
            painter.drawRoundedRect(cellRect.adjusted(2,2,-2,-2), 4, 4);
        }
        else if (currentMode == 6) // Fan
        {
            painter.setBrush(QColor(148, 226, 213));
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(center, cellSize/4, cellSize/4);
            // Static blades for ghost
            for(int i=0; i<3; i++) {
                QPolygonF blade;
                double angle = i * 2 * 3.14159 / 3;
                blade << QPointF(center) 
                        << QPointF(center.x() + cos(angle)*cellSize/1.5, center.y() + sin(angle)*cellSize/1.5)
                        << QPointF(center.x() + cos(angle+0.3)*cellSize/1.5, center.y() + sin(angle+0.3)*cellSize/1.5);
                painter.drawPolygon(blade);
            }
        }
        else if (currentMode == 9) // Bulb
        {
            painter.setBrush(QColor(249, 226, 175));
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(center, cellSize/3, cellSize/3);
        }
        else if (currentMode == 7) // Restricted
        {
            painter.setPen(QPen(QColor(243, 139, 168), 2));
            painter.drawLine(cellRect.topLeft(), cellRect.bottomRight());
            painter.drawLine(cellRect.topRight(), cellRect.bottomLeft());
        }
        
        painter.setOpacity(1.0); // Reset opacity
    }
}


void GridWidget::setFanPath(const std::vector<std::pair<int,int>>& path)
{
    currentFanPath = path;
    update();
}

void GridWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!grid) return;
    
    QPointF pos = event->position();
    
    // Calculate Hover Cell using Offset
    int c = static_cast<int>(pos.x() - offsetX) / cellSize;
    int r = static_cast<int>(pos.y() - offsetY) / cellSize;
    
    if (r != hoverRow || c != hoverCol) {
        hoverRow = r;
        hoverCol = c;
        update(); // Trigger repaint to show ghost at new position
    }
}

void GridWidget::mousePressEvent(QMouseEvent *event) {
    if (!grid) return;

    QPointF pos = event->position();
    
    // ADJUST FOR OFFSET AND CELL SIZE
    int c = static_cast<int>(pos.x() - offsetX) / cellSize;
    int r = static_cast<int>(pos.y() - offsetY) / cellSize;

    qDebug() << "=== MOUSE CLICK ===";
    qDebug() << "Raw pos:" << pos << " Offset: (" << offsetX << "," << offsetY << ") CellSize:" << cellSize;
    qDebug() << "Mapped grid position: (" << r << "," << c << ")";

    if (!grid->isvalidcell(r, c)) {
        qDebug() << "Invalid cell!";
        return;
    }

    // Check if parent MainWindow is in fan placement mode
    MainWindow* mainWin = qobject_cast<MainWindow*>(window());

    // ... (rest of logic unchanged) ...
    qDebug() << "MainWindow found:" << (mainWin != nullptr);
    if (mainWin) {
        qDebug() << "Fan placement mode:" << mainWin->fanPlacementMode;
    }

    if (mainWin && mainWin->fanPlacementMode)
    {
        qDebug() << ">>> ENTERING FAN PLACEMENT MODE";
        qDebug() << "Room clicked at (" << r << "," << c << ")";
        qDebug() << "Cell value at click:" << grid->getcell(r, c);

        // Detect the room at this position
        MainWindow::Room room = mainWin->detectRoomAt(r, c);

        qDebug() << "Room cells found:" << room.cells.size();

        if (room.cells.empty())
        {
            qDebug() << "ERROR: No valid room at this position";
            QMessageBox::warning(this, "Fan Placement",
                                 "Cannot detect room at this position.\nMake sure you clicked inside a room surrounded by walls.");
            mainWin->exitFanPlacementMode();
            setCursor(Qt::ArrowCursor);
            return;
        }

        // Ask user how many fans
        bool ok;
        int fanCount = QInputDialog::getInt(
            this,
            "Fan Placement",
            QString("Room size: %1 cells\nHow many fans to place?")
                .arg(room.cells.size()),
            1,      // default
            0,      // min
            10,     // max
            1,      // step
            &ok
            );

        qDebug() << "User selected fans:" << fanCount << ", OK pressed:" << ok;

        if (ok && fanCount > 0)
        {
            qDebug() << "Placing" << fanCount << "fans...";
            mainWin->placeFansInRoom(room, fanCount);
            update();
        }

        // Exit fan placement mode
        qDebug() << "Exiting fan placement mode";
        mainWin->exitFanPlacementMode();
        setCursor(Qt::ArrowCursor);

        return;  // Important: return here to prevent normal placement
    }

    // Regular device placement mode
    qDebug() << "Normal placement mode, currentMode:" << currentMode;

    int currentCell = grid->getcell(r, c);

    switch(currentMode) {
    case 5:
        grid->setwall(r, c);
        break;

    case 2: // Socket - MUST be on wall
        if (currentCell == 5) {
            grid->setsocket(r, c);
        } else {
            QMessageBox::warning(this, "Invalid Placement",
                                 "⚠️ Sockets can only be placed on WALLS!");
        }
        break;

    case 3: // Main Board - MUST be on wall
        if (currentCell == 5) {
            grid->setmainboard(r, c);
        } else {
            QMessageBox::warning(this, "Invalid Placement",
                                 "⚠️ Main Board can only be placed on WALLS!");
        }
        break;

    case 6: // Fan - CANNOT be on wall
        if (currentCell != 5 && currentCell != 8 && currentCell != 10) {
            grid->setfan(r, c);
        } else {
            QMessageBox::warning(this, "Invalid Placement",
                                 "⚠️ Fans cannot be placed on walls!");
        }
        break;

    case 9: // Bulb - CANNOT be on wall
        if (currentCell != 5 && currentCell != 8 && currentCell != 10) {
            grid->setbulb(r, c);
        } else {
            QMessageBox::warning(this, "Invalid Placement",
                                 "⚠️ Bulbs cannot be placed on walls!");
        }
        break;

    case 7:
        grid->setrestrictedd(r, c);
        break;
    }
    update();
    qDebug() << "==================";
}

