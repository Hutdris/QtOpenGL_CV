/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "OpenGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *upperButton;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *lowerButton;
    QPushButton *centerButton;
    QLabel *upperLabel;
    QLabel *lowerLabel;
    QLabel *centerLabel;
    OpenGLWidget *openGLWidget;
    QTextBrowser *textBrowser;
    QSlider *ZoomBar;
    QLCDNumber *lcdNumber;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1259, 878);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 40, 171, 51));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(30, 110, 171, 51));
        upperButton = new QPushButton(centralWidget);
        upperButton->setObjectName(QStringLiteral("upperButton"));
        upperButton->setGeometry(QRect(30, 170, 51, 51));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(30, 420, 171, 51));
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(30, 360, 171, 51));
        lowerButton = new QPushButton(centralWidget);
        lowerButton->setObjectName(QStringLiteral("lowerButton"));
        lowerButton->setGeometry(QRect(30, 230, 51, 51));
        centerButton = new QPushButton(centralWidget);
        centerButton->setObjectName(QStringLiteral("centerButton"));
        centerButton->setGeometry(QRect(30, 290, 51, 51));
        upperLabel = new QLabel(centralWidget);
        upperLabel->setObjectName(QStringLiteral("upperLabel"));
        upperLabel->setGeometry(QRect(90, 180, 111, 21));
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        font.setPointSize(20);
        upperLabel->setFont(font);
        upperLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lowerLabel = new QLabel(centralWidget);
        lowerLabel->setObjectName(QStringLiteral("lowerLabel"));
        lowerLabel->setGeometry(QRect(90, 240, 121, 31));
        lowerLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        centerLabel = new QLabel(centralWidget);
        centerLabel->setObjectName(QStringLiteral("centerLabel"));
        centerLabel->setGeometry(QRect(90, 300, 121, 31));
        centerLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        openGLWidget = new OpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(310, 20, 911, 631));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(310, 680, 911, 141));
        ZoomBar = new QSlider(centralWidget);
        ZoomBar->setObjectName(QStringLiteral("ZoomBar"));
        ZoomBar->setGeometry(QRect(260, 30, 31, 441));
        ZoomBar->setMinimum(1);
        ZoomBar->setMaximum(100);
        ZoomBar->setValue(100);
        ZoomBar->setOrientation(Qt::Vertical);
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(230, 490, 64, 23));
        MainWindow->setCentralWidget(centralWidget);
        pushButton_2->raise();
        upperButton->raise();
        pushButton_4->raise();
        pushButton_5->raise();
        lowerButton->raise();
        centerButton->raise();
        upperLabel->raise();
        lowerLabel->raise();
        centerLabel->raise();
        openGLWidget->raise();
        textBrowser->raise();
        pushButton->raise();
        ZoomBar->raise();
        lcdNumber->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1259, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton, SIGNAL(clicked()), openGLWidget, SLOT(animate()));
        QObject::connect(ZoomBar, SIGNAL(valueChanged(int)), openGLWidget, SLOT(setZoomRatio(int)));
        QObject::connect(ZoomBar, SIGNAL(valueChanged(int)), lcdNumber, SLOT(display(int)));


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Import STL", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Import ", 0));
        upperButton->setText(QApplication::translate("MainWindow", "Upper", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "End&Save", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "Rcording", 0));
        lowerButton->setText(QApplication::translate("MainWindow", "Lower", 0));
        centerButton->setText(QApplication::translate("MainWindow", "CenterLabel", 0));
        upperLabel->setText(QString());
        lowerLabel->setText(QString());
        centerLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
