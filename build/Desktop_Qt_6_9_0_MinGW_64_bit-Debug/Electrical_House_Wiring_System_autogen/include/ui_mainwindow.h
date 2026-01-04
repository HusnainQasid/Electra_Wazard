/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <C:\Users\acer\OneDrive\Documents\Electrical_House_Wiring_System\gridwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    GridWidget *widget;
    QComboBox *devicecomboBox;
    QPushButton *previewPathButton_;
    QPushButton *autoPlaceFansButton;
    QPushButton *connectFansButton;
    QPushButton *lightUpButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        widget = new GridWidget(centralwidget);
        widget->setObjectName("widget");
        devicecomboBox = new QComboBox(widget);
        devicecomboBox->addItem(QString());
        devicecomboBox->addItem(QString());
        devicecomboBox->addItem(QString());
        devicecomboBox->addItem(QString());
        devicecomboBox->addItem(QString());
        devicecomboBox->addItem(QString());
        devicecomboBox->setObjectName("devicecomboBox");
        devicecomboBox->setGeometry(QRect(610, 160, 111, 21));
        previewPathButton_ = new QPushButton(widget);
        previewPathButton_->setObjectName("previewPathButton_");
        previewPathButton_->setGeometry(QRect(610, 0, 111, 24));
        autoPlaceFansButton = new QPushButton(widget);
        autoPlaceFansButton->setObjectName("autoPlaceFansButton");
        autoPlaceFansButton->setGeometry(QRect(610, 40, 111, 24));
        connectFansButton = new QPushButton(widget);
        connectFansButton->setObjectName("connectFansButton");
        connectFansButton->setGeometry(QRect(610, 80, 111, 24));
        lightUpButton = new QPushButton(widget);
        lightUpButton->setObjectName("lightUpButton");
        lightUpButton->setGeometry(QRect(610, 120, 111, 24));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        devicecomboBox->setItemText(0, QCoreApplication::translate("MainWindow", "wall", nullptr));
        devicecomboBox->setItemText(1, QCoreApplication::translate("MainWindow", "socket", nullptr));
        devicecomboBox->setItemText(2, QCoreApplication::translate("MainWindow", "main board", nullptr));
        devicecomboBox->setItemText(3, QCoreApplication::translate("MainWindow", "fan", nullptr));
        devicecomboBox->setItemText(4, QCoreApplication::translate("MainWindow", "bulb", nullptr));
        devicecomboBox->setItemText(5, QCoreApplication::translate("MainWindow", "redtricted", nullptr));

        previewPathButton_->setText(QCoreApplication::translate("MainWindow", "Draw Wire", nullptr));
        autoPlaceFansButton->setText(QCoreApplication::translate("MainWindow", "Place Fan", nullptr));
        connectFansButton->setText(QCoreApplication::translate("MainWindow", "Connect Fans", nullptr));
        lightUpButton->setText(QCoreApplication::translate("MainWindow", "Light Up bulbs", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
