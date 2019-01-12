#include "mainwindow.h"
#include <opencv2/opencv.hpp>
#include <string.h>

using namespace cv;

int start_socket(QString data, uchar * v_buffer, int * index_buffer);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->connect_socket, SIGNAL(clicked()), this, SLOT(socket_connect()));
    QObject::connect(ui->record, SIGNAL(clicked()), this, SLOT(record_pic()));

    ui->image->setScene(&scene);
    ui->image->show();

    record_status = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::update_img() {
    memcpy(tmp_qimage.bits(), tmp_img, 960*400*3);
    scene.addPixmap(QPixmap::fromImage(tmp_qimage));
    ui->image->setScene(&scene);
    ui->image->resize(tmp_qimage.width() + 5, tmp_qimage.height() + 5);
    ui->image->show();
}

void MainWindow::set_Qimage_buffer(uchar * pt) {
     tmp_img = pt;
}

void MainWindow::socket_connect() {
    QString data = ui->server_ip->toPlainText();

    if(sock == -1) {
        sock = start_socket(data, v_buffer, index_buffer);
    }
}

int start_socket(QString data, uchar * v_buffer, int * index_buffer) {
    int sock;
    struct sockaddr_in srv;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    qDebug("ip = %s\n", data.toLatin1().data());
    srv.sin_addr.s_addr = inet_addr(data.toLatin1().data());
    srv.sin_family = AF_INET;
    srv.sin_port = htons(18888);

    int result;
    result = connect(sock, (struct sockaddr *)&srv, sizeof(srv));

    if(result < 0) {
        close(sock);
        return -1;
    }
    else {
        send(sock, "start", 5, 0);
        int proc1 = fork();
        if(proc1 == 0) { // subprocess 1 to recv image and cache it
            struct pollfd pofd;
            pofd.fd = sock;
            pofd.events = POLLIN;
            Mat recv_mat(800, 1920, CV_8UC3);
            int len,total;

            while(1) {
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
                    memcpy(&v_buffer[((*index_buffer)%3)*(800*1920*3)], recv_mat.data, 800*1920*3);
                    (*index_buffer)++;

    //                qDebug("recv ok\n");
                }
           }
        }
        else{
            qDebug("image recive pid is:%d\n", proc1);
        }
        return sock;
    }

}

void MainWindow::record_pic() {
    if(record_status == 0){
        record_status = 1;
    }
    else{
        record_status = 0;
        // stop signal
    }
}
