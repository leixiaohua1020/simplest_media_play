最简单的多媒体播放的例子
Simplest Media Play

雷霄骅 Lei Xiaohua
leixiaohua1020@126.com
中国传媒大学/数字电视技术
Communication University of China / Digital TV Technology
http://blog.csdn.net/leixiaohua1020

本工程包含了使用各种API（Direct3D，OpenGL，GDI，DirectSound，SDL2）播放多媒体例子。
其中音频输入为PCM采样数据。输出至系统的声卡播放出来。
视频输入为YUV/RGB像素数据。输出至显示器上的一个窗口播放出来。
通过本工程的代码初学者可以快速学习使用这几个API播放视频和音频的技术。
一共包括了如下几个子工程：
simplest_audio_play_directsound: 	使用DirectSound播放PCM音频采样数据。
simplest_audio_play_sdl2: 			使用SDL2播放PCM音频采样数据。
simplest_video_play_direct3d: 		使用Direct3D的Surface播放RGB/YUV视频像素数据。
simplest_video_play_direct3d_texture:使用Direct3D的Texture播放RGB视频像素数据。
simplest_video_play_gdi: 			使用GDI播放RGB/YUV视频像素数据。
simplest_video_play_opengl: 		使用OpenGL播放RGB/YUV视频像素数据。
simplest_video_play_opengl_texture:	使用OpenGL的Texture播放YUV视频像素数据。
simplest_video_play_sdl2: 			使用SDL2播放RGB/YUV视频像素数据。


This project contains several examples about video/audio's playing. It uses different
API (Direct3D, OpenGL, GDI, DirectSound, SDL2) to output video/audio to system devices.
In audio examples, input data is PCM raw audio data.
In video examples, input data is YUV/RGB raw pixel data.
Through these examples, the beginner of video/audio programmer can learn the usage of
these API easyly.
It contains following programs:
simplest_audio_play_directsound: 	Play PCM raw audio data using DirectSound.
simplest_audio_play_sdl2: 			Play PCM raw audio data using SDL2.
simplest_video_play_direct3d: 		Play RGB/YUV raw video data using Direct3D's Surface.
simplest_video_play_direct3d_texture:Play RGB raw video data using Direct3D's Texture.
simplest_video_play_gdi: 			Play RGB/YUV raw video data using GDI.
simplest_video_play_opengl: 		Play RGB/YUV raw video data using OpenGL.
simplest_video_play_opengl_texture:	Play YUV raw video data using OpenGL.
simplest_video_play_sdl2: 			Play RGB/YUV raw video data using SDL2.



