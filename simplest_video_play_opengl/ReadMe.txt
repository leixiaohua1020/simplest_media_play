 最简单的OpenGL播放视频的例子（OpenGL播放RGB/YUV）
 Simplest Video Play OpenGL (OpenGL play RGB/YUV) 

 雷霄骅 Lei Xiaohua
 leixiaohua1020@126.com
 中国传媒大学/数字电视技术
 Communication University of China / Digital TV Technology
 http://blog.csdn.net/leixiaohua1020

 本程序使用OpenGL播放RGB/YUV视频像素数据。本程序实际上只能
 播放RGB（RGB24，BGR24，BGRA）数据。如果输入数据为YUV420P
 数据的话，需要先转换为RGB数据之后再进行播放。
 本程序是最简单的使用OpenGL播放像素数据的例子，适合OpenGL新手学习。

 函数调用步骤如下：

 [初始化]
 glutInit()：初始化glut库。
 glutInitDisplayMode()：设置显示模式。
 glutCreateWindow()：创建一个窗口。
 glutDisplayFunc()：设置绘图函数（重绘的时候调用）。
 glutTimerFunc()：设置定时器。
 glutMainLoop()：进入消息循环。

 [循环渲染数据]
 glRasterPos3f(),glPixelZoom()：调整显示位置，图像大小。
 glDrawPixels()：绘制。
 glutSwapBuffers()：显示。

 This software plays RGB/YUV raw video data using OpenGL. This
 software support show RGB (RGB24, BGR24, BGRA) data on the screen.
 If the input data is YUV420P, it need to be convert to RGB first.
 This program is the simplest example about play raw video data
 using OpenGL, Suitable for the beginner of OpenGL.

 The process is shown as follows:

 [Init]
 glutInit()：Init glut library.
 glutInitDisplayMode(): Set display mode.
 glutCreateWindow(): Create a window.
 glutDisplayFunc(): Set the display callback.
 glutTimerFunc(): Set timer.
 glutMainLoop(): Start message loop.

 [Loop to Render data]
 glRasterPos3f(),glPixelZoom(): Change picture's size and position.
 glDrawPixels(): draw.
 glutSwapBuffers(): show.