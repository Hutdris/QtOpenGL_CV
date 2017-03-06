/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
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
    QLCDNumber *lcdNumber;
    QPushButton *icpButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_6;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1247, 886);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 40, 101, 31));
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
        openGLWidget->setGeometry(QRect(310, 20, 911, 611));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);
        openGLWidget->setMinimumSize(QSize(680, 480));
        openGLWidget->setLayoutDirection(Qt::RightToLeft);
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(310, 660, 911, 171));
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(200, 650, 64, 23));
        icpButton = new QPushButton(centralWidget);
        icpButton->setObjectName(QStringLiteral("icpButton"));
        icpButton->setGeometry(QRect(30, 490, 56, 17));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(30, 520, 131, 23));
        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(40, 550, 101, 21));
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
        lcdNumber->raise();
        icpButton->raise();
        pushButton_3->raise();
        pushButton_6->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1247, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton, SIGNAL(clicked()), openGLWidget, SLOT(animate()));
        QObject::connect(openGLWidget, SIGNAL(getZoomRatio(int)), lcdNumber, SLOT(display(int)));
        QObject::connect(icpButton, SIGNAL(clicked()), MainWindow, SLOT(runICP()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), MainWindow, SLOT(camerasDisplay()));
        QObject::connect(pushButton_6, SIGNAL(clicked()), MainWindow, SLOT(initCameras()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "Import STL", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "Import ", Q_NULLPTR));
        upperButton->setText(QApplication::translate("MainWindow", "Upper", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("MainWindow", "End&Save", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MainWindow", "Rcording", Q_NULLPTR));
        lowerButton->setText(QApplication::translate("MainWindow", "Lower", Q_NULLPTR));
        centerButton->setText(QApplication::translate("MainWindow", "CenterLabel", Q_NULLPTR));
        upperLabel->setText(QString());
        lowerLabel->setText(QString());
        centerLabel->setText(QString());
        icpButton->setText(QApplication::translate("MainWindow", "Run ICP", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "Camera display", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("MainWindow", "initCameras", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
