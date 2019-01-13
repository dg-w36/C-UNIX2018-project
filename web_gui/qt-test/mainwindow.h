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
#include <sys/stat.h>

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
    int *color_mode ;

public slots :
    void update_img();
    void socket_connect();
    void record_pic();
    void change_colormode();

private:
    Ui::MainWindow *ui;

    // image update
    uchar * tmp_img;
    QGraphicsScene scene_color;
    QGraphicsScene scene_gray;
    QImage tmp_qimage1 = QImage(960, 400, QImage::Format_RGB888);
    QImage tmp_qimage2 = QImage(960, 400, QImage::Format_Grayscale8);


    // image capture
    int record_status;
    char * fn = "/tmp/pipefifo";
    int pipe_fd[2];


};

#endif // MAINWINDOW_H
