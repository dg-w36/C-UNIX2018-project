#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <QMainWindow>
#include <QTimer>
#include <QObject>

#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
//    void update_img(QImage img);
    void set_Qimage_buffer(uchar * pt);
    ~MainWindow();
    int sock = -1;
    uchar * v_buffer;
    int * index_buffer;

public slots :
    void update_img();
    void socket_connect();
    void record_pic();

private:
    Ui::MainWindow *ui;
    uchar * tmp_img;
    QGraphicsScene scene;
    QImage tmp_qimage = QImage(960, 400, QImage::Format_RGB888);
    int record_status;
};

#endif // MAINWINDOW_H
