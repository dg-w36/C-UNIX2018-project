/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGraphicsView *image;
    QPushButton *connect_socket;
    QPushButton *do_process;
    QPushButton *record;
    QPlainTextEdit *server_ip;
    QLabel *label_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1053, 556);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        image = new QGraphicsView(centralWidget);
        image->setObjectName(QStringLiteral("image"));
        image->setGeometry(QRect(20, 10, 980, 420));
        connect_socket = new QPushButton(centralWidget);
        connect_socket->setObjectName(QStringLiteral("connect_socket"));
        connect_socket->setGeometry(QRect(170, 440, 91, 31));
        do_process = new QPushButton(centralWidget);
        do_process->setObjectName(QStringLiteral("do_process"));
        do_process->setGeometry(QRect(280, 440, 111, 31));
        record = new QPushButton(centralWidget);
        record->setObjectName(QStringLiteral("record"));
        record->setGeometry(QRect(410, 440, 101, 31));
        server_ip = new QPlainTextEdit(centralWidget);
        server_ip->setObjectName(QStringLiteral("server_ip"));
        server_ip->setGeometry(QRect(20, 440, 141, 31));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(530, 450, 121, 16));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1053, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        connect_socket->setText(QApplication::translate("MainWindow", "connect", Q_NULLPTR));
        do_process->setText(QApplication::translate("MainWindow", "Change to gray", Q_NULLPTR));
        record->setText(QApplication::translate("MainWindow", "start_record", Q_NULLPTR));
        server_ip->setPlainText(QApplication::translate("MainWindow", "127.0.0.1", Q_NULLPTR));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
