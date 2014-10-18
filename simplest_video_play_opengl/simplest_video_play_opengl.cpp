/**
 * 最简单的OpenGL播放视频的例子（OpenGL播放RGB/YUV）
 * Simplest Video Play OpenGL (OpenGL play RGB/YUV) 
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序使用OpenGL播放RGB/YUV视频像素数据。本程序实际上只能
 * 播放RGB（RGB24，BGR24，BGRA）数据。如果输入数据为YUV420P
 * 数据的话，需要先转换为RGB数据之后再进行播放。
 * 本程序是最简单的使用OpenGL播放像素数据的例子，适合OpenGL新手学习。
 *
 * 函数调用步骤如下：
 *
 * [初始化]
 * glutInit()：初始化glut库。
 * glutInitDisplayMode()：设置显示模式。
 * glutCreateWindow()：创建一个窗口。
 * glutDisplayFunc()：设置绘图函数（重绘的时候调用）。
 * glutTimerFunc()：设置定时器。
 * glutMainLoop()：进入消息循环。
 *
 * [循环渲染数据]
 * glRasterPos3f(),glPixelZoom()：调整显示位置，图像大小。
 * glDrawPixels()：绘制。
 * glutSwapBuffers()：显示。
 *
 * This software plays RGB/YUV raw video data using OpenGL. This
 * software support show RGB (RGB24, BGR24, BGRA) data on the screen.
 * If the input data is YUV420P, it need to be convert to RGB first.
 * This program is the simplest example about play raw video data
 * using OpenGL, Suitable for the beginner of OpenGL.
 *
 * The process is shown as follows:
 *
 * [Init]
 * glutInit()：Init glut library.
 * glutInitDisplayMode(): Set display mode.
 * glutCreateWindow(): Create a window.
 * glutDisplayFunc(): Set the display callback.
 * glutTimerFunc(): Set timer.
 * glutMainLoop(): Start message loop.
 *
 * [Loop to Render data]
 * glRasterPos3f(),glPixelZoom(): Change picture's size and position.
 * glDrawPixels(): draw.
 * glutSwapBuffers(): show.
 */

#include <stdio.h>

#include "glew.h"
#include "glut.h"

#include <stdlib.h>
#include <malloc.h>
#include <string.h>

//set '1' to choose a type of file to play
#define LOAD_RGB24   1
#define LOAD_BGR24   0
#define LOAD_BGRA    0
#define LOAD_YUV420P 0

int screen_w=500,screen_h=500;
const int pixel_w = 320, pixel_h = 180;
//Bit per Pixel
#if LOAD_BGRA
const int bpp=32;
#elif LOAD_RGB24|LOAD_BGR24
const int bpp=24;
#elif LOAD_YUV420P
const int bpp=12;
#endif
//YUV file
FILE *fp = NULL;
unsigned char buffer[pixel_w*pixel_h*bpp/8];
unsigned char buffer_convert[pixel_w*pixel_h*3];

inline unsigned char CONVERT_ADJUST(double tmp)
{
	return (unsigned char)((tmp >= 0 && tmp <= 255)?tmp:(tmp < 0 ? 0 : 255));
}
//YUV420P to RGB24
void CONVERT_YUV420PtoRGB24(unsigned char* yuv_src,unsigned char* rgb_dst,int nWidth,int nHeight)
{
	unsigned char *tmpbuf=(unsigned char *)malloc(nWidth*nHeight*3);
	unsigned char Y,U,V,R,G,B;
	unsigned char* y_planar,*u_planar,*v_planar;
	int rgb_width , u_width;
	rgb_width = nWidth * 3;
	u_width = (nWidth >> 1);
	int ypSize = nWidth * nHeight;
	int upSize = (ypSize>>2);
	int offSet = 0;

	y_planar = yuv_src;
	u_planar = yuv_src + ypSize;
	v_planar = u_planar + upSize;

	for(int i = 0; i < nHeight; i++)
	{
		for(int j = 0; j < nWidth; j ++)
		{
			// Get the Y value from the y planar
			Y = *(y_planar + nWidth * i + j);
			// Get the V value from the u planar
			offSet = (i>>1) * (u_width) + (j>>1);
			V = *(u_planar + offSet);
			// Get the U value from the v planar
			U = *(v_planar + offSet);

			// Cacular the R,G,B values
			// Method 1
			R = CONVERT_ADJUST((Y + (1.4075 * (V - 128))));
			G = CONVERT_ADJUST((Y - (0.3455 * (U - 128) - 0.7169 * (V - 128))));
			B = CONVERT_ADJUST((Y + (1.7790 * (U - 128))));
			/*
			// The following formulas are from MicroSoft' MSDN
			int C,D,E;
			// Method 2
			C = Y - 16;
			D = U - 128;
			E = V - 128;
			R = CONVERT_ADJUST(( 298 * C + 409 * E + 128) >> 8);
			G = CONVERT_ADJUST(( 298 * C - 100 * D - 208 * E + 128) >> 8);
			B = CONVERT_ADJUST(( 298 * C + 516 * D + 128) >> 8);
			R = ((R - 128) * .6 + 128 )>255?255:(R - 128) * .6 + 128; 
			G = ((G - 128) * .6 + 128 )>255?255:(G - 128) * .6 + 128; 
			B = ((B - 128) * .6 + 128 )>255?255:(B - 128) * .6 + 128; 
			*/
			offSet = rgb_width * i + j * 3;

			rgb_dst[offSet] = B;
			rgb_dst[offSet + 1] = G;
			rgb_dst[offSet + 2] = R;
		}
	}
	free(tmpbuf);
}

void display(void){
    if (fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp) != pixel_w*pixel_h*bpp/8){
        // Loop
        fseek(fp, 0, SEEK_SET);
        fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp);
    }

	//Make picture full of window
	//Move to(-1.0,1.0)
	glRasterPos3f(-1.0f,1.0f,0);
	//Zoom, Flip
	glPixelZoom((float)screen_w/(float)pixel_w, -(float)screen_h/pixel_h);
	

#if LOAD_BGRA
	glDrawPixels(pixel_w, pixel_h,GL_BGRA, GL_UNSIGNED_BYTE, buffer);
#elif LOAD_RGB24
	glDrawPixels(pixel_w, pixel_h,GL_RGB, GL_UNSIGNED_BYTE, buffer);
#elif LOAD_BGR24
	glDrawPixels(pixel_w, pixel_h,GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
#elif LOAD_YUV420P
	CONVERT_YUV420PtoRGB24(buffer,buffer_convert,pixel_w,pixel_h);
	glDrawPixels(pixel_w, pixel_h,GL_RGB, GL_UNSIGNED_BYTE, buffer_convert);
#endif
	//GLUT_DOUBLE
	glutSwapBuffers();

	//GLUT_SINGLE
	//glFlush();
}

void timeFunc(int value){
    display();
    // Present frame every 40 ms
    glutTimerFunc(40, timeFunc, 0);
}


int main(int argc, char* argv[])
{
#if LOAD_BGRA
	fp=fopen("../test_bgra_320x180.rgb","rb+");
#elif LOAD_RGB24
	fp=fopen("../test_rgb24_320x180.rgb","rb+");
#elif LOAD_BGR24
	fp=fopen("../test_bgr24_320x180.rgb","rb+");
#elif LOAD_YUV420P
	fp=fopen("../test_yuv420p_320x180.yuv","rb+");
#endif
	if(fp==NULL){
		printf("Cannot open this file.\n");
		return -1;
	}

    // GLUT init
    glutInit(&argc, argv);  
	//Double, Use glutSwapBuffers() to show
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
	//Single, Use glFlush() to show
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(screen_w, screen_h);
    glutCreateWindow("Simplest Video Play OpenGL");
	printf("Simplest Video Play OpenGL\n");
	printf("Lei Xiaohua\n");
	printf("http://blog.csdn.net/leixiaohua1020\n");
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    glutDisplayFunc(&display);
    glutTimerFunc(40, timeFunc, 0); 
    
    // Start!
    glutMainLoop();

    return 0;
}






