#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define COL 1920
#define ROW 800
#define PIXEL_SIZE 3
#define BUFFER_SIZE 2
#define MAX_CONNECT 5

using namespace std;
using namespace cv;

int main(){
    // alloc the buffer to store the video file
    uchar * v_buffer = (uchar *)mmap(NULL, 3*BUFFER_SIZE*COL*ROW*3, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    int * index_buffer = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    uchar * send_buffer = (uchar *)mmap(NULL, BUFFER_SIZE*COL*ROW*3, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    
    *index_buffer = 0;
    if(fork() == 0) {  // load frame process, just like the DMA
        Mat frame;
        VideoCapture cap;
        int cols, rows, element_size;
        cap.open("./../sample.mp4");  // load movie file

        if(!cap.isOpened())
            return -1;

        while(1) {  // load frame to buffer
            cap>>frame;
            // save image
            memcpy(&v_buffer[((*index_buffer)%3)*(COL*ROW*PIXEL_SIZE)], frame.data, COL*ROW*PIXEL_SIZE);
            (*index_buffer) = (*index_buffer) + 1;
            if(frame.empty()) {
                printf("Video file is no frame\n");
                break;
            }
            usleep(33333); //30fps
        }   

        // printf("image element size is %lu\n", frame.elemSize());
        // printf("channal num is %d\n",frame.channels());
        // printf("image shape is %d * %d\n",frame.cols, frame.rows);
        return 0;
    }

    Mat out_frame(ROW,COL,CV_8UC3);
    int index = 0;

    int srv_sock, clt_sock, c, read_size, i,j,count;
    struct sockaddr_in srv, clt;
    char buf[2000],buf2[2000];

    char nametable[MAX_CONNECT][100] = {0};
    char oldname[100];

    srv_sock = socket(AF_INET, SOCK_STREAM, 0); // 创建套接字
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = INADDR_ANY; // 在所有地址上监听
    srv.sin_port = htons(18888);
    // 监听 18888 端口
    // 绑定套接字
    bind(srv_sock, (struct sockaddr *)&srv, sizeof(srv)); 
    listen(srv_sock, 3);
    // 接受客户端连接
    c = sizeof(struct sockaddr_in);
    
    struct pollfd fds[MAX_CONNECT + 1];
    fds[0].fd = srv_sock; // 加入到fd set 中,进行监听
    fds[0].events= POLLIN;

    for(i=1;i<MAX_CONNECT+1;i++){ // 初始没有监视的连接fd
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    
    count = 0;
    int is_send = 0;
    while(1) {
        if(poll(fds, MAX_CONNECT+1, -1) < 0) {
            printf("poll error !\n");
            break;
        }
        if(fds[0].revents) { // 检查是否有新连接
            for(i=1; i<MAX_CONNECT+1; i++) {
                if(fds[i].fd == -1) {
                    fds[i].fd  = accept(srv_sock, (struct sockaddr *)&clt, (socklen_t *)&c);
                    index = *index_buffer -1; // have some problem
                    break;
                }
            }
        }

        is_send = 0;  
        // 检查客户端输入
        for(i=1;i<MAX_CONNECT+1;i++){ 
            if(fds[i].revents) { // 检查是否有客户端输入,目前协议简单,然和信息都视为画面申请
                read_size = recv(fds[i].fd, buf, sizeof(buf), 0); // 接收
                buf[read_size] = '\0';

                if (read_size == 0) {
                    puts("Client disconnected");
                    fds[i].fd = -1;
                    break;
                } else if (read_size == -1) {
                    fds[i].fd = -1;
                    perror("recv failed");
                    break;
                }
                
                while(*index_buffer < index){ } // 这里类似与fifo的两个指针,跳出循环表示有新的帧被缓存了
                
                memcpy(out_frame.data, &v_buffer[((index)%3)*(COL*ROW*PIXEL_SIZE)], COL*ROW*PIXEL_SIZE); 
                // 从fifo中取出数据,准备发送
                if(index >= 3) {
                    *index_buffer = *index_buffer - 3;
                    index = index - 3; 
                }
                // index = index + 1;
                
                // printf("send to %d\n", j);
                send(fds[i].fd, out_frame.data, COL*ROW*PIXEL_SIZE, 0);
                is_send = 1;
            }
        }
        if(is_send) {
            index++;
        }


    }
    close(clt_sock); 
    close(srv_sock);
	return 0;
}