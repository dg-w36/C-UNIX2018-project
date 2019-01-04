#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
	Mat frame;
	VideoCapture cap;
    cap.open("./../sample.mp4");

    if(!cap.isOpened())
        return -1;
    
    while(1) {
        cap>>frame;//等价于cap.read(frame);
        if(frame.empty())//如果某帧为空则退出循环
             break;
        imshow("video", frame);
        waitKey(16);//每帧延时20毫秒
    }
	return 0;
}