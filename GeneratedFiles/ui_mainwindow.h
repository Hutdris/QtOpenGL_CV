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
#include <QtWidgets/QCheckBox>
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
    QCheckBox *RecCheckBox;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1247, 886);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
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
        textBrowser->setGeometry(QRect(310, 710, 911, 121));
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(230, 790, 64, 23));
        icpButton = new QPushButton(centralWidget);
        icpButton->setObjectName(QStringLiteral("icpButton"));
        icpButton->setGeometry(QRect(30, 490, 56, 17));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(30, 520, 131, 23));
        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(40, 550, 101, 21));
        RecCheckBox = new QCheckBox(centralWidget);
        RecCheckBox->setObjectName(QStringLiteral("RecCheckBox"));
        RecCheckBox->setGeometry(QRect(110, 490, 101, 21));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 590, 121, 31));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1247, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);
        QObject::connect(openGLWidget, SIGNAL(getZoomRatio(int)), lcdNumber, SLOT(display(int)));
        QObject::connect(icpButton, SIGNAL(clicked()), MainWindow, SLOT(runICP()));
        QObject::connect(pushButton_6, SIGNAL(clicked()), MainWindow, SLOT(initCameras()));
        QObject::connect(pushButton_5, SIGNAL(clicked()), MainWindow, SLOT(recording()));
        QObject::connect(pushButton, SIGNAL(clicked()), openGLWidget, SLOT(tracing_output_reset()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), MainWindow, SLOT(reset_init_pos()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        upperButton->setText(QApplication::translate("MainWindow", "Upper", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Pause", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "Start/Stop Recording", 0));
        lowerButton->setText(QApplication::translate("MainWindow", "Lower", 0));
        centerButton->setText(QApplication::translate("MainWindow", "CenterLabel", 0));
        upperLabel->setText(QString());
        lowerLabel->setText(QString());
        centerLabel->setText(QString());
        icpButton->setText(QApplication::translate("MainWindow", "Run ICP", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Set first frame", 0));
        pushButton_6->setText(QApplication::translate("MainWindow", "initCameras", 0));
        RecCheckBox->setText(QApplication::translate("MainWindow", "Recording", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Tracing reset/save", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
