#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in srv;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    srv.sin_family = AF_INET;
    srv.sin_port = htons(18888);

    connect(sock, (struct sockaddr *)&srv, sizeof(srv));
    send(sock, "start", 5, 0);
    // send(sock, "\n",1, 0);    

    struct pollfd pofd;
    pofd.fd = sock;
    pofd.events = POLLIN;

    Mat recv_mat(800, 1920, CV_8UC3);
    char input_buffer;
    int count, len;

    while(1) {
        if(poll(&pofd, 1, -1) < 0) {
            printf("poll error !\n");
            break;
        }

        if(pofd.revents) {
            len = recv(sock, recv_mat.data, 800*1920*3, 0);
            send(sock, "ok", 2, 0);
            imshow("recv", recv_mat);
            waitKey(10);
        }
        
    }


    // close(sock);
    return 0;
}