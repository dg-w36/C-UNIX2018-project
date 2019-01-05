#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <string.h>

#define COL 1920
#define ROW 800
#define PIXEL_SIZE 3

using namespace std;
using namespace cv;

int main(){

	Mat frame;
	VideoCapture cap;
    cap.open("./../sample.mp4");    

    uchar buffer[COL*ROW][3];
    uchar r_buffer[COL*ROW];

    if(!cap.isOpened())
        return -1;
    else {
        cap>>frame;
    }

    // save image
    memcpy(buffer, frame.data, COL*ROW*PIXEL_SIZE);
    for(int i=0;i<COL*ROW;i++) {
        r_buffer[i] = buffer[i][0];
    }
    Mat out_frame1(ROW, COL, CV_8UC1, r_buffer);
    imshow("test1", out_frame1);
    waitKey();

    for(int i=0;i<COL*ROW;i++) {
        r_buffer[i] = buffer[i][1];
    }    
    Mat out_frame2(ROW, COL, CV_8UC1, r_buffer);
    imshow("test2", out_frame2);
    waitKey();
   
    for(int i=0;i<COL*ROW;i++) {
        r_buffer[i] = buffer[i][0];
    }
    Mat out_frame3(ROW, COL, CV_8UC1, r_buffer);
    imshow("test3", out_frame3);
    waitKey();

    int i = 0;
    while(1) {
        cap>>frame;
        if(frame.empty())//如果某帧为空则退出循环
             break;
        imshow("video", frame);
        if(waitKey(20)  == 27){
            break;
        }
    }



    printf("image element size is %lu\n", frame.elemSize());
    printf("channal num is %d\n",frame.channels());
    printf("image shape is %d * %d\n",frame.cols, frame.rows);

	return 0;
}