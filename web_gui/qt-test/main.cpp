#include "mainwindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>
#include <string.h>

#define COL 1920
#define ROW 800
#define PIXEL_SIZE 3
#define BUFFER_SIZE 2

using namespace cv;

int start_socket();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    uchar * v_buffer = (uchar *)mmap(NULL, 3*1920*800*3, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    int * index_buffer = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);

    *index_buffer = 0;
    struct pollfd pofd;
    pofd.fd = start_socket();
    pofd.events = POLLIN;

    if(fork() == 0) { // subprocess 1
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
                memcpy(&v_buffer[((*index_buffer)%3)*(COL*ROW*PIXEL_SIZE)], recv_mat.data, COL*ROW*PIXEL_SIZE);
                (*index_buffer)++;

                qDebug("recv ok\n");
            }
       }
    }

    int index = (*index_buffer) == 0 ? (*index_buffer):(*index_buffer)-1;
    Mat output_img(400,960, CV_8UC3);
    Mat tmp_img(800,1920, CV_8UC3);

    uchar * out = (uchar *)mmap(NULL, 400*960*3, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    w.set_Qimage_buffer(out);

    QTimer socket_timer;
    QObject::connect(&socket_timer, SIGNAL(timeout()), &w, SLOT(update_img()));
    socket_timer.start(30);

    if(fork() == 0) {
        while(1) {
            if(*index_buffer > index) {
                memcpy(tmp_img.data, &v_buffer[(index%3)*(COL*ROW*PIXEL_SIZE)], COL*ROW*PIXEL_SIZE);
                resize(tmp_img, output_img, output_img.size());
                cvtColor(output_img, output_img, CV_RGB2BGR);

                index++;
                if(index >= 3) {
                    index = index % 3;
                    (*index_buffer) = (*index_buffer) % 3;
                }

                memcpy(out, output_img.data, 400*960*3);
            }
        }
    }

    return a.exec();
}

int start_socket(){
    int sock;
    struct sockaddr_in srv;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    srv.sin_family = AF_INET;
    srv.sin_port = htons(18888);

    connect(sock, (struct sockaddr *)&srv, sizeof(srv));
    send(sock, "start", 5, 0);
    // send(sock, "\n",1, 0);
    return sock;
}
