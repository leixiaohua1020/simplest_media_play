 最简单的SDL2播放音频的例子（SDL2播放PCM）
 Simplest Audio Play SDL2 (SDL2 play PCM) 

 雷霄骅 Lei Xiaohua
 leixiaohua1020@126.com
 中国传媒大学/数字电视技术
 Communication University of China / Digital TV Technology
 http://blog.csdn.net/leixiaohua1020

 本程序使用SDL2播放PCM音频采样数据。SDL实际上是对底层绘图
 API（Direct3D，OpenGL）的封装，使用起来明显简单于直接调用底层
 API。

 函数调用步骤如下: 

 [初始化]
 SDL_Init(): 初始化SDL。
 SDL_OpenAudio(): 根据参数（存储于SDL_AudioSpec）打开音频设备。

 [循环播放数据]
 SDL_PauseAudio(): 播放音频数据。
 SDL_Delay(): 延时等待播放完成。

 This software plays PCM raw audio data using SDL2.
 SDL is a wrapper of low-level API (DirectSound).
 Use SDL is much easier than directly call these low-level API.

 The process is shown as follows:

 [Init]
 SDL_Init(): Init SDL.
 SDL_OpenAudio(): Opens the audio device with the desired 
					parameters (In SDL_AudioSpec).

 [Loop to play data]
 SDL_PauseAudio(): Play Audio.
 SDL_Delay(): Wait for completetion of playback.