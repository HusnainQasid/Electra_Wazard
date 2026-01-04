#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QInputDialog>
#include <QTimer>
#include <vector>
#include "grid.h"

class MainWindow;

class GridWidget : public QWidget {
    Q_OBJECT
public:
    explicit GridWidget(QWidget *parent = nullptr);
    void setGrid(Grid *g);     
    void setMode(int m);       
    int getcellsize() const;
    void setPath(const std::vector<std::pair<int,int>>& path);
    void setFanPath(const std::vector<std::pair<int,int>>& path);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override; // Added for Ghost Tool

private:
    Grid *grid;   
    int cellSize = 12;  
    int currentMode;    
    std::vector<std::pair<int,int>> currentPath;
    std::vector<std::pair<int,int>> fanSuggestions;
    std::vector<std::pair<int,int>> currentFanPath;
    
    // Mouse Hover for Ghost Tool
    int hoverRow = -1;
    int hoverCol = -1;
    
    // Centering offsets
    int offsetX = 0;
    int offsetY = 0;
    
    // Animation
    QTimer *animationTimer;
    int animationPhase = 0;
};

#endif // GRIDWIDGET_H
