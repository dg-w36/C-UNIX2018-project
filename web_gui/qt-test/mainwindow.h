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

public slots :
    void update_img();

private:
    Ui::MainWindow *ui;
    uchar * tmp_img;
};

#endif // MAINWINDOW_H
