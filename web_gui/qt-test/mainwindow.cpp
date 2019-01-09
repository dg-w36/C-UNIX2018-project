#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>




using namespace cv;
int start_socket();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pofd.fd = start_socket();
    pofd.events = POLLIN;

    connect(&socket_timer,SIGNAL(timeout()), this, SLOT(check_socket()));
    socket_timer.start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int start_socket(){
    int sock;
    struct sockaddr_in srv;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    srv.sin_addr.s_addr = inet_addr("59.66.94.25");
    srv.sin_family = AF_INET;
    srv.sin_port = htons(18888);

    connect(sock, (struct sockaddr *)&srv, sizeof(srv));
    send(sock, "start", 5, 0);
    // send(sock, "\n",1, 0);
    return sock;
}

void MainWindow::check_socket() {

    QImage img;
    Mat recv_mat(800, 1920, CV_8UC3);
    int len,total;

    if(poll(&pofd, 1, -1) < 0) {
        printf("poll error !\n");
    }

    if(pofd.revents) {
        len = recv(pofd.fd, recv_mat.data, 800*1920*3, 0);
        total = len;
        while(total < 800*1920*3){
            len = recv(pofd.fd, recv_mat.data+total, 800*1920*3-total, 0);
            total += len;
        }

        send(pofd.fd, "ok", 2, 0);

        img =QImage((const unsigned char*)(recv_mat.data),recv_mat.cols,recv_mat.rows,recv_mat.cols*recv_mat.channels(),
                    QImage::Format_RGB888);

        QGraphicsScene *scene = new QGraphicsScene;
        scene->addPixmap(QPixmap::fromImage(img));
        ui->image->setScene(scene);
        //ui->graphicsView->resize(image.width() + 5, image.height() + 5);
        ui->image->show();
//        imshow("recv", recv_mat);
//        if(waitKey(10) == 27) {
//            break;
//        }
//    }
    }
}
