#　图像接受端程序
## 实现功能介绍
通过ＱＴ实现ＧＵＩ界面，包括视频显示区，ｉｐ输入区，以及开始连接，灰度和彩色切换，以及视频录制功能．第一和第二是图像采集卡的基本功能，第二个功能是通过实现最简单的灰度和彩色转换功能，来设计一定的图像处理的接口．

## 实现方法
上述三个按键的功能，分别对应三个不同的进程．
1. 连接对应着图像缓存进程，即通过ｓｏｃｋｅｔ对图像不断进行接收和缓存．
2. 色彩切换按键对应着图像处理进程．色彩切换按键可以控制一个参数，程序中为color_mode,　图像处理进程根据该参数的取值，来进行不同的图像处理功能
3. 视频录制功能对应着视频存储进程，该按键可以产生开始或者结束的信号，与视频存储进程进行通信，控制视频录制的进行．
   
## 实验方法
通过ｑｔ　ｃｒｅａｔｅｒ软件打开该ｑｔ工程，进行编译即可执行．填写服务器的地址，点击ｃｏｎｎｅｃｔ就可以出现视频图像．（先打开图像发送服务器）
需要注意的是，在ｑｔ的配置文件，uset.pro 中，opencv 库的路径可能需要重新配置，才能编译通过．
