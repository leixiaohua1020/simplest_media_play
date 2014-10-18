 最简单的GDI播放视频的例子（GDI播放RGB/YUV）
 Simplest Video Play GDI (GDI play RGB/YUV) 

 雷霄骅 Lei Xiaohua
 leixiaohua1020@126.com
 中国传媒大学/数字电视技术
 Communication University of China / Digital TV Technology
 http://blog.csdn.net/leixiaohua1020

 本程序使用GDI播放RGB/YUV视频像素数据。GDI实际上只能直接播放RGB数据。
 因此如果输入数据为YUV420P的话，需要先转换为RGB数据之后再进行播放。
 
 函数调用步骤如下：
 GetDC()：获得显示设备的句柄。
 像素数据格式的转换（如果需要的话）
 设置BMP文件头...
 StretchDIBits()：指定BMP文件头，以及像素数据，绘制。
 ReleaseDC()：释放显示设备的句柄。

 在该示例程序中，包含了像素转换的几个工具函数，以及“大端”，
 “小端”（字节顺序）相互转换的函数。

 This software plays RGB/YUV raw video data using GDI.
 In fact GDI only can draw RGB data. So If input data is
 YUV420P, it need to be convert to RGB first.
 It's the simplest GDI tutorial (About video playback).

 The process is shown as follows:

 GetDC()：retrieves a handle to a device context (DC).
 Convert pixel data format(if needed).
 Set BMP Header...
 StretchDIBits()：Set pixel data and BMP data and begin to draw.
 ReleaseDC()：release the handle.

 In this program there are some functions about conversion
 between pixel format and conversion between "Big Endian" and 
 "Little Endian".