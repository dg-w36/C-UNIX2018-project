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

    // 共享内存模拟数据fifo 
    uchar * v_buffer = (uchar *)mmap(NULL, 3*1920*800*3, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    int * index_buffer = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);

    *index_buffer = 0; // 初始fifo指针为0

    w.v_buffer = v_buffer;
    w.index_buffer = index_buffer;

    int index = (*index_buffer) == 0 ? (*index_buffer):(*index_buffer)-1;
    Mat tmp_img(800,1920, CV_8UC3);
    uchar * out = (uchar *)mmap(NULL, 400*960*3, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    // 色彩模式用共享内存实现,可以在多个进程间通信
    int * color_mode = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    w.color_mode = color_mode;
    *color_mode = 1; // initial is color mode

    w.set_Qimage_buffer(out);

    // timer for image update
    QTimer socket_timer;
    QObject::connect(&socket_timer, SIGNAL(timeout()), &w, SLOT(update_img()));
    socket_timer.start(30);

    // sub process 2 for do image process
    // if the buffer index change, process 2 start
    int proc2 = fork();
    if(proc2 == 0) {
        Mat output_img(400,960, CV_8UC3);
        Mat gray_img(400,960, CV_8UC1);
        while(1) {
            if(*index_buffer > index) { // 图像处理不是由按键触发的,只要有数据开始缓存,就会出发图像处理和显示
//                memcpy(tmp_img.data, &v_buffer[(index%3)*(COL*ROW*PIXEL_SIZE)], COL*ROW*PIXEL_SIZE);
                tmp_img.data = &v_buffer[(index%3)*(COL*ROW*PIXEL_SIZE)];
                resize(tmp_img, output_img, output_img.size());

                if(*color_mode) { // 根据该控制参数控制数据处理的方法,其他图像处理可以同样方法加入
                    cvtColor(output_img, output_img, CV_RGB2BGR);
                }
                else {
                    cvtColor(output_img, gray_img, CV_RGB2GRAY);
                }

                index++;
                if(index >= 3) {
                    index = index % 3;
                    *index_buffer = (*index_buffer) % 3;
                }

                if(*color_mode) {
//                    qDebug("in color mode\n");
                    memcpy(out, output_img.data, 400*960*3);
                }
                else {
//                    qDebug("in gray mode\n");
                    memcpy(out, gray_img.data, 400*960);
                }
            }
            usleep(16667); // 30 帧工作
        }
    }
    else{
        qDebug("image processing proc is %d\n", proc2);
    }

    return a.exec();
}
