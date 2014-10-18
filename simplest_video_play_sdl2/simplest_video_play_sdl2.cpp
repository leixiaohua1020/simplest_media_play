/**
 * 最简单的SDL2播放视频的例子（SDL2播放RGB/YUV）
 * Simplest Video Play SDL2 (SDL2 play RGB/YUV) 
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序使用SDL2播放RGB/YUV视频像素数据。SDL实际上是对底层绘图
 * API（Direct3D，OpenGL）的封装，使用起来明显简单于直接调用底层
 * API。
 *
 * 函数调用步骤如下：
 *
 * [初始化]
 * SDL_Init()：初始化SDL。
 * SDL_CreateWindow()：创建窗口（Window）。
 * SDL_CreateRenderer()：基于窗口创建渲染器（Render）。
 * SDL_CreateTexture()：创建纹理（Texture）。
 *
 * [循环渲染数据]
 * SDL_UpdateTexture()：设置纹理的数据。
 * SDL_RenderCopy()：纹理复制给渲染器。
 * SDL_RenderPresent()：显示。
 *
 * This software plays RGB/YUV raw video data using SDL2.
 * SDL is a wrapper of low-level API (Direct3D, OpenGL).
 * Use SDL is much easier than directly call these low-level API.  
 *
 * The process is shown as follows:
 *
 * [Init]
 * SDL_Init(): Init SDL.
 * SDL_CreateWindow(): Create a Window.
 * SDL_CreateRenderer(): Create a Render.
 * SDL_CreateTexture(): Create a Texture.
 *
 * [Loop to Render data]
 * SDL_UpdateTexture(): Set Texture's data.
 * SDL_RenderCopy(): Copy Texture to Render.
 * SDL_RenderPresent(): Show.
 */

#include <stdio.h>

extern "C"
{
#include "sdl/SDL.h"
};

//set '1' to choose a type of file to play
#define LOAD_BGRA    1
#define LOAD_RGB24   0
#define LOAD_BGR24   0
#define LOAD_YUV420P 0

#define HAS_BORDER	 1

//Bit per Pixel
#if LOAD_BGRA
const int bpp=32;
#elif LOAD_RGB24|LOAD_BGR24
const int bpp=24;
#elif LOAD_YUV420P
const int bpp=12;
#endif

const int screen_w=500,screen_h=500;
const int pixel_w=320,pixel_h=180;

unsigned char buffer[pixel_w*pixel_h*bpp/8];
//BPP=32
unsigned char buffer_convert[pixel_w*pixel_h*4];

//Convert RGB24/BGR24 to RGB32/BGR32
//And change Endian if needed
void CONVERT_24to32(unsigned char *image_in,unsigned char *image_out,int w,int h){
	for(int i =0;i<h;i++)
		for(int j=0;j<w;j++){
			//Big Endian or Small Endian?
			//"ARGB" order:high bit -> low bit.
			//ARGB Format Big Endian (low address save high MSB, here is A) in memory : A|R|G|B
			//ARGB Format Little Endian (low address save low MSB, here is B) in memory : B|G|R|A
			if(SDL_BYTEORDER==SDL_LIL_ENDIAN){
				//Little Endian (x86): R|G|B --> B|G|R|A
				image_out[(i*w+j)*4+0]=image_in[(i*w+j)*3+2];
				image_out[(i*w+j)*4+1]=image_in[(i*w+j)*3+1];
				image_out[(i*w+j)*4+2]=image_in[(i*w+j)*3];
				image_out[(i*w+j)*4+3]='0';
			}else{
				//Big Endian: R|G|B --> A|R|G|B
				image_out[(i*w+j)*4]='0';
				memcpy(image_out+(i*w+j)*4+1,image_in+(i*w+j)*3,3);
			}
		}
}


//Refresh Event
#define REFRESH_EVENT  (SDL_USEREVENT + 1)

int thread_exit=0;

int refresh_video(void *opaque){
	while (thread_exit==0) {
		SDL_Event event;
		event.type = REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 

	SDL_Window *screen; 
	//SDL 2.0 Support for multiple windows
	screen = SDL_CreateWindow("Simplest Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h,SDL_WINDOW_OPENGL);
	if(!screen) {  
		printf("SDL: could not create window - exiting:%s\n",SDL_GetError());  
		return -1;
	}
	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);  

	Uint32 pixformat=0;
#if LOAD_BGRA
	//Note: ARGB8888 in "Little Endian" system stores as B|G|R|A
	pixformat= SDL_PIXELFORMAT_ARGB8888;  
#elif LOAD_RGB24
	pixformat= SDL_PIXELFORMAT_RGB888;  
#elif LOAD_BGR24
	pixformat= SDL_PIXELFORMAT_BGR888;  
#elif LOAD_YUV420P
	//IYUV: Y + U + V  (3 planes)
	//YV12: Y + V + U  (3 planes)
	pixformat= SDL_PIXELFORMAT_IYUV;  
#endif

	SDL_Texture* sdlTexture = SDL_CreateTexture(sdlRenderer,pixformat, SDL_TEXTUREACCESS_STREAMING,pixel_w,pixel_h);

	int border=0;
	//Make picture smaller, Add a "border"
#if HAS_BORDER
	border=30;
#endif
	SDL_Rect sdlRect;  
	sdlRect.x = 0+border;  
	sdlRect.y = 0+border;  
	sdlRect.w = screen_w-border*2;  
	sdlRect.h = screen_h-border*2;  

	FILE *fp=NULL;
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
		printf("cannot open this file\n");
		return -1;
	}

	SDL_Thread *refresh_thread = SDL_CreateThread(refresh_video,NULL,NULL);
	SDL_Event event;
	while(1){
		//Wait
		SDL_WaitEvent(&event);
		if(event.type==REFRESH_EVENT){
			if (fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp) != pixel_w*pixel_h*bpp/8){
				// Loop
				fseek(fp, 0, SEEK_SET);
				fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp);
			}

#if LOAD_BGRA
			//We don't need to change Endian
			//Because input BGRA pixel data(B|G|R|A) is same as ARGB8888 in Little Endian (B|G|R|A)
			SDL_UpdateTexture( sdlTexture, NULL, buffer, pixel_w*4);  
#elif LOAD_RGB24|LOAD_BGR24
			//change 24bit to 32 bit
			//and in Windows we need to change Endian
			CONVERT_24to32(buffer,buffer_convert,pixel_w,pixel_h);
			SDL_UpdateTexture( sdlTexture, NULL, buffer_convert, pixel_w*4);  
#elif LOAD_YUV420P
			SDL_UpdateTexture( sdlTexture, NULL, buffer, pixel_w);  
#endif
			
			SDL_RenderClear( sdlRenderer );   
			SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, &sdlRect);  
			SDL_RenderPresent( sdlRenderer );  
			//Delay 40ms
			SDL_Delay(40);
			
		}else if(event.type==SDL_QUIT){
			break;
		}
	}

	return 0;
}
