/**
 * 最简单的GDI播放视频的例子（GDI播放RGB/YUV）
 * Simplest Video Play GDI (GDI play RGB/YUV) 
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序使用GDI播放RGB/YUV视频像素数据。GDI实际上只能直接播放RGB数据。
 * 因此如果输入数据为YUV420P的话，需要先转换为RGB数据之后再进行播放。
 * 
 * 函数调用步骤如下：
 * GetDC()：获得显示设备的句柄。
 * 像素数据格式的转换（如果需要的话）
 * 设置BMP文件头...
 * StretchDIBits()：指定BMP文件头，以及像素数据，绘制。
 * ReleaseDC()：释放显示设备的句柄。
 *
 * 在该示例程序中，包含了像素转换的几个工具函数，以及“大端”，
 * “小端”（字节顺序）相互转换的函数。
 *
 * This software plays RGB/YUV raw video data using GDI.
 * In fact GDI only can draw RGB data. So If input data is
 * YUV420P, it need to be convert to RGB first.
 * It's the simplest GDI tutorial (About video playback).
 *
 * The process is shown as follows:
 *
 * GetDC()：retrieves a handle to a device context (DC).
 * Convert pixel data format(if needed).
 * Set BMP Header...
 * StretchDIBits()：Set pixel data and BMP data and begin to draw.
 * ReleaseDC()：release the handle.
 *
 * In this program there are some functions about conversion
 * between pixel format and conversion between "Big Endian" and 
 * "Little Endian".
 */

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

//set '1' to choose a type of file to play
#define LOAD_BGRA    1
#define LOAD_RGB24   0
#define LOAD_BGR24   0
#define LOAD_YUV420P 0

//Width, Height
const int screen_w=500,screen_h=500;
const int pixel_w=320,pixel_h=180;

//Bit per Pixel
#if LOAD_BGRA
const int bpp=32;
#elif LOAD_RGB24|LOAD_BGR24
const int bpp=24;
#elif LOAD_YUV420P
const int bpp=12;
#endif

FILE *fp=NULL;

//Storage frame data
unsigned char buffer[pixel_w*pixel_h*bpp/8];

unsigned char buffer_convert[pixel_w*pixel_h*3];

//Not Efficient, Just an example
//change endian of a pixel (32bit)
void CHANGE_ENDIAN_32(unsigned char *data){
	char temp3,temp2;
	temp3=data[3];
	temp2=data[2];
	data[3]=data[0];
	data[2]=data[1];
	data[0]=temp3;
	data[1]=temp2;
}
//change endian of a pixel (24bit)
void CHANGE_ENDIAN_24(unsigned char *data){
	char temp2=data[2];
	data[2]=data[0];
	data[0]=temp2;
}

//RGBA to RGB24 (or BGRA to BGR24)
void CONVERT_RGBA32toRGB24(unsigned char *image,int w,int h){
	for(int i =0;i<h;i++)
		for(int j=0;j<w;j++){
			memcpy(image+(i*w+j)*3,image+(i*w+j)*4,3);
		}
}
//RGB24 to BGR24
void CONVERT_RGB24toBGR24(unsigned char *image,int w,int h){
	for(int i =0;i<h;i++)
		for(int j=0;j<w;j++){
			char temp2;
			temp2=image[(i*w+j)*3+2];
			image[(i*w+j)*3+2]=image[(i*w+j)*3+0];
			image[(i*w+j)*3+0]=temp2;
		}
}

//Change endian of a picture
void CHANGE_ENDIAN_PIC(unsigned char *image,int w,int h,int bpp){
	unsigned char *pixeldata=NULL;
	for(int i =0;i<h;i++)
		for(int j=0;j<w;j++){
			pixeldata=image+(i*w+j)*bpp/8;
			if(bpp==32){
				CHANGE_ENDIAN_32(pixeldata);
			}else if(bpp==24){
				CHANGE_ENDIAN_24(pixeldata);
			}
		}
}

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



bool Render(HWND hwnd)
{
	//Read Pixel Data
	if (fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp) != pixel_w*pixel_h*bpp/8){
		// Loop
		fseek(fp, 0, SEEK_SET);
		fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp);
	}
	
	HDC hdc=GetDC(hwnd);

	//Note:
	//Big Endian or Small Endian?
	//ARGB order:high bit -> low bit.
	//ARGB Format Big Endian (low address save high MSB, here is A) in memory : A|R|G|B
	//ARGB Format Little Endian (low address save low MSB, here is B) in memory : B|G|R|A
	
	//Microsoft Windows is Little Endian
	//So we must change the order
#if LOAD_BGRA
	CONVERT_RGBA32toRGB24(buffer,pixel_w,pixel_h);
	//we don't need to change endian
	//Because input BGR24 pixel data(B|G|R) is same as RGB in Little Endian (B|G|R)
#elif LOAD_RGB24
	//Change to Little Endian
	CHANGE_ENDIAN_PIC(buffer,pixel_w,pixel_h,24);
#elif LOAD_BGR24
	//In fact we don't need to do anything.
	//Because input BGR24 pixel data(B|G|R) is same as RGB in Little Endian (B|G|R)
	//CONVERT_RGB24toBGR24(buffer,pixel_w,pixel_h);
	//CHANGE_ENDIAN_PIC(buffer,pixel_w,pixel_h,24);
#elif LOAD_YUV420P
	//YUV Need to Convert to RGB first
	//YUV420P to RGB24
	CONVERT_YUV420PtoRGB24(buffer,buffer_convert,pixel_w,pixel_h);
	//Change to Little Endian
	CHANGE_ENDIAN_PIC(buffer_convert,pixel_w,pixel_h,24);
#endif

	//BMP Header
	BITMAPINFO m_bmphdr={0};
	DWORD dwBmpHdr = sizeof(BITMAPINFO);
	//24bit
	m_bmphdr.bmiHeader.biBitCount = 24;
	m_bmphdr.bmiHeader.biClrImportant = 0;
	m_bmphdr.bmiHeader.biSize = dwBmpHdr;
	m_bmphdr.bmiHeader.biSizeImage = 0;
	m_bmphdr.bmiHeader.biWidth = pixel_w;
	//Notice: BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
	//So we must set reverse biHeight to show image correctly.
	m_bmphdr.bmiHeader.biHeight = -pixel_h;
	m_bmphdr.bmiHeader.biXPelsPerMeter = 0;
	m_bmphdr.bmiHeader.biYPelsPerMeter = 0;
	m_bmphdr.bmiHeader.biClrUsed = 0;
	m_bmphdr.bmiHeader.biPlanes = 1;
	m_bmphdr.bmiHeader.biCompression = BI_RGB;
	//Draw data
#if LOAD_YUV420P
	//YUV420P data convert to another buffer
	int nResult = StretchDIBits(hdc,
		0,0,
		screen_w,screen_h,
		0, 0,
		pixel_w, pixel_h,
		buffer_convert,
		&m_bmphdr,
		DIB_RGB_COLORS,
		SRCCOPY);
#else
	//Draw data
	int nResult = StretchDIBits(hdc,
		0,0,
		screen_w,screen_h,
		0, 0,
		pixel_w, pixel_h,
		buffer,
		&m_bmphdr,
		DIB_RGB_COLORS,
		SRCCOPY);
#endif

	ReleaseDC(hwnd,hdc);

	return true;
}


LRESULT WINAPI MyWndProc(HWND hwnd, UINT msg, WPARAM wparma, LPARAM lparam)
{
	switch(msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparma, lparam);
}

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = (WNDPROC)MyWndProc;
	wc.lpszClassName = _T("GDI");
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wc);

	HWND hwnd = NULL;
	hwnd = CreateWindow(_T("GDI"), _T("Simplest Video Play GDI"), WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, NULL);
	if (hwnd==NULL){
		return -1;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

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

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT){
		//PeekMessage不同于GetMessage，是非阻塞函数。
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			Sleep(40);
			Render(hwnd);
		}
	}


	UnregisterClass(_T("GDI"), hInstance);
	return 0;
}