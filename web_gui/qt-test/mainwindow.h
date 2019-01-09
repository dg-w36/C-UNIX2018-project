#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <arpa/inet.h>

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void check_socket();

private:
    Ui::MainWindow *ui;
    struct pollfd pofd;
    QTimer socket_timer;
};

#endif // MAINWINDOW_H
