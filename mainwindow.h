#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <map>
#include<QLabel>
#include<QVBoxLayout>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool fanPlacementMode;

    struct Room {
        std::vector<std::pair<int,int>> cells;
        int minRow, maxRow, minCol, maxCol;
    };

    // Public methods called from GridWidget
    Room detectRoomAt(int r, int c);
    void placeFansInRoom(const Room& room, int fanCount);
    void exitFanPlacementMode();

private slots:
    void on_previewPathButton_clicked();
    void on_autoPlaceFansButton_clicked();
    void on_connectFansButton_clicked();
    void on_lightUpBulbsButton_clicked();

private:
    Ui::MainWindow *ui;

    // Data members
    std::vector<std::pair<int,int>> rememberedSockets;
    std::map<std::pair<int,int>, int> originalCellValues;

    // UI enhancement members
    QLabel* deviceCountLabel;
    QLabel* connectionCountLabel;

    // Helper functions for room detection
    void floodFill(int r, int c, std::vector<std::vector<bool>>& visited, Room& room);

    // Helper functions for socket/fan operations
    double calculateDistance(int r1, int c1, int r2, int c2);
    std::pair<int,int> findNearestSocket(int fanR, int fanC);

    // New helper functions to reduce code duplication
    std::vector<std::pair<int,int>> getAllSockets();
    void restorePaths();
    int drawPath(const std::vector<std::pair<int,int>>& path, int pathType);

    // UI enhancement functions
    void setupEnhancedUI();
    void createLegendPanel();
    void addLegendItem(QVBoxLayout* layout, QString icon, QString name,
                       QString color, QString description);
    void addWireLegendItem(QVBoxLayout* layout, QString name,
                           QString color, QString description);
    void updateStatusBar();
    void drawZigzagPath(int r1, int c1, int r2, int c2, int pathType);
};

#endif // MAINWINDOW_H
