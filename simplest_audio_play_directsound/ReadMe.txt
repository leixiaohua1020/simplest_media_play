 最简单的DirectSound播放音频的例子（DirectSound播放PCM）
 Simplest Audio Play DirectSound (DirectSound play PCM) 

 雷霄骅 Lei Xiaohua
 leixiaohua1020@126.com
 中国传媒大学/数字电视技术
 Communication University of China / Digital TV Technology
 http://blog.csdn.net/leixiaohua1020

 本程序使用DirectSound播放PCM音频采样数据。
 是最简单的DirectSound播放音频的教程。

 函数调用步骤如下：

 [初始化]
 DirectSoundCreate8()：创建一个DirectSound对象。
 SetCooperativeLevel()：设置协作权限，不然没有声音。
 IDirectSound8->CreateSoundBuffer()：创建一个主缓冲区对象。
 IDirectSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8..)：
			创建一个副缓冲区对象，用来存储要播放的声音数据文件。
 IDirectSoundBuffer8->QueryInterface(IID_IDirectSoundNotify..)：
			创建通知对象，通知应用程序指定播放位置已经达到。
 IDirectSoundNotify8->SetNotificationPositions()：设置通知位置。
 IDirectSoundBuffer8->SetCurrentPosition()：设置播放的起始点。
 IDirectSoundBuffer8->Play()：开始播放。

 [循环播放数据]
 IDirectSoundBuffer8->Lock()：锁定副缓冲区，准备写入数据。
 fread()：读取数据。
 IDirectSoundBuffer8->Unlock()：解锁副缓冲区。
 WaitForMultipleObjects()：等待“播放位置已经达到”的通知。

 This software plays PCM raw audio data using DirectSound.
 It's the simplest tutorial about DirectSound.

 The process is shown as follows:

 [Init]
 DirectSoundCreate8(): Init DirectSound object.
 SetCooperativeLevel(): Must set, or we won't hear sound.
 IDirectSound8->CreateSoundBuffer(): Create primary sound buffer.
 IDirectSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8..): 
			Create secondary sound buffer.
 IDirectSoundBuffer8->QueryInterface(IID_IDirectSoundNotify..): 
			Create Notification object.
 IDirectSoundNotify8->SetNotificationPositions():
			Set Notification Positions.
 IDirectSoundBuffer8->SetCurrentPosition(): Set position to start.
 IDirectSoundBuffer8->Play(): Begin to play.

 [Loop to play data]
 IDirectSoundBuffer8->Lock(): Lock secondary buffer.
 fread(): get PCM data.
 IDirectSoundBuffer8->Unlock(): UnLock secondary buffer.
 WaitForMultipleObjects(): Wait for Notifications.