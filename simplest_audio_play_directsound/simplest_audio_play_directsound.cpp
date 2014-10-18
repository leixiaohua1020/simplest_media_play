/**
 * 最简单的DirectSound播放音频的例子（DirectSound播放PCM）
 * Simplest Audio Play DirectSound (DirectSound play PCM) 
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序使用DirectSound播放PCM音频采样数据。
 * 是最简单的DirectSound播放音频的教程。
 *
 * 函数调用步骤如下：
 *
 * [初始化]
 * DirectSoundCreate8()：创建一个DirectSound对象。
 * SetCooperativeLevel()：设置协作权限，不然没有声音。
 * IDirectSound8->CreateSoundBuffer()：创建一个主缓冲区对象。
 * IDirectSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8..)：
 *			创建一个副缓冲区对象，用来存储要播放的声音数据文件。
 * IDirectSoundBuffer8->QueryInterface(IID_IDirectSoundNotify..)：
 *			创建通知对象，通知应用程序指定播放位置已经达到。
 * IDirectSoundNotify8->SetNotificationPositions()：设置通知位置。
 * IDirectSoundBuffer8->SetCurrentPosition()：设置播放的起始点。
 * IDirectSoundBuffer8->Play()：开始播放。
 *
 * [循环播放数据]
 * IDirectSoundBuffer8->Lock()：锁定副缓冲区，准备写入数据。
 * fread()：读取数据。
 * IDirectSoundBuffer8->Unlock()：解锁副缓冲区。
 * WaitForMultipleObjects()：等待“播放位置已经达到”的通知。
 *
 * This software plays PCM raw audio data using DirectSound.
 * It's the simplest tutorial about DirectSound.
 *
 * The process is shown as follows:
 *
 * [Init]
 * DirectSoundCreate8(): Init DirectSound object.
 * SetCooperativeLevel(): Must set, or we won't hear sound.
 * IDirectSound8->CreateSoundBuffer(): Create primary sound buffer.
 * IDirectSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8..): 
 *			Create secondary sound buffer.
 * IDirectSoundBuffer8->QueryInterface(IID_IDirectSoundNotify..): 
 *			Create Notification object.
 * IDirectSoundNotify8->SetNotificationPositions():
 *			Set Notification Positions.
 * IDirectSoundBuffer8->SetCurrentPosition(): Set position to start.
 * IDirectSoundBuffer8->Play(): Begin to play.
 *
 * [Loop to play data]
 * IDirectSoundBuffer8->Lock(): Lock secondary buffer.
 * fread(): get PCM data.
 * IDirectSoundBuffer8->Unlock(): UnLock secondary buffer.
 * WaitForMultipleObjects(): Wait for Notifications.
 */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dsound.h>


#define MAX_AUDIO_BUF 4 
#define BUFFERNOTIFYSIZE 192000 


int sample_rate=44100;	//PCM sample rate
int channels=2;			//PCM channel number
int bits_per_sample=16;	//bits per sample

BOOL main(int argc,char * argv[])
{
	int i;
	FILE * fp;
	if((fp=fopen("../NocturneNo2inEflat_44.1k_s16le.pcm","rb"))==NULL){
		printf("cannot open this file\n");
		return -1;
	}

	IDirectSound8 *m_pDS=0;					
	IDirectSoundBuffer8 *m_pDSBuffer8=NULL;	//used to manage sound buffers.
	IDirectSoundBuffer *m_pDSBuffer=NULL;	
	IDirectSoundNotify8 *m_pDSNotify=0;		
	DSBPOSITIONNOTIFY m_pDSPosNotify[MAX_AUDIO_BUF];
	HANDLE m_event[MAX_AUDIO_BUF];

	SetConsoleTitle(TEXT("Simplest Audio Play DirectSound"));//Console Title
	//Init DirectSound
	if(FAILED(DirectSoundCreate8(NULL,&m_pDS,NULL)))
		return FALSE;
	if(FAILED(m_pDS->SetCooperativeLevel(FindWindow(NULL,TEXT("Simplest Audio Play DirectSound")),DSSCL_NORMAL)))
		return FALSE;


	DSBUFFERDESC dsbd;
	memset(&dsbd,0,sizeof(dsbd));
	dsbd.dwSize=sizeof(dsbd);
	dsbd.dwFlags=DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY |DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwBufferBytes=MAX_AUDIO_BUF*BUFFERNOTIFYSIZE; 
	dsbd.lpwfxFormat=(WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	dsbd.lpwfxFormat->wFormatTag=WAVE_FORMAT_PCM;   
	/* format type */
	(dsbd.lpwfxFormat)->nChannels=channels;          
	/* number of channels (i.e. mono, stereo...) */
	(dsbd.lpwfxFormat)->nSamplesPerSec=sample_rate;     
	/* sample rate */
	(dsbd.lpwfxFormat)->nAvgBytesPerSec=sample_rate*(bits_per_sample/8)*channels; 
	/* for buffer estimation */
	(dsbd.lpwfxFormat)->nBlockAlign=(bits_per_sample/8)*channels;        
	/* block size of data */
	(dsbd.lpwfxFormat)->wBitsPerSample=bits_per_sample;     
	/* number of bits per sample of mono data */
	(dsbd.lpwfxFormat)->cbSize=0;

	//Creates a sound buffer object to manage audio samples. 
	HRESULT hr1;
	if( FAILED(m_pDS->CreateSoundBuffer(&dsbd,&m_pDSBuffer,NULL))){   
		return FALSE;
	}
	if( FAILED(m_pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8,(LPVOID*)&m_pDSBuffer8))){
		return FALSE ;
	}
	//Get IDirectSoundNotify8
	if(FAILED(m_pDSBuffer8->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&m_pDSNotify))){
		return FALSE ;
	}
	for(i =0;i<MAX_AUDIO_BUF;i++){
		m_pDSPosNotify[i].dwOffset =i*BUFFERNOTIFYSIZE;
		m_event[i]=::CreateEvent(NULL,false,false,NULL); 
		m_pDSPosNotify[i].hEventNotify=m_event[i];
	}
	m_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF,m_pDSPosNotify);
	m_pDSNotify->Release();

	//Start Playing
	BOOL isPlaying =TRUE;
	LPVOID buf=NULL;
	DWORD  buf_len=0;
	DWORD res=WAIT_OBJECT_0;
	DWORD offset=BUFFERNOTIFYSIZE;

	m_pDSBuffer8->SetCurrentPosition(0);
	m_pDSBuffer8->Play(0,0,DSBPLAY_LOOPING);
	//Loop
	while(isPlaying){
		if((res >=WAIT_OBJECT_0)&&(res <=WAIT_OBJECT_0+3)){
			m_pDSBuffer8->Lock(offset,BUFFERNOTIFYSIZE,&buf,&buf_len,NULL,NULL,0);
			if(fread(buf,1,buf_len,fp)!=buf_len){
				//File End
				//Loop:
				fseek(fp, 0, SEEK_SET);
				fread(buf,1,buf_len,fp);
				//Close:
				//isPlaying=0;
			}
			offset+=buf_len;
			offset %= (BUFFERNOTIFYSIZE * MAX_AUDIO_BUF);
			printf("this is %7d of buffer\n",offset);
			m_pDSBuffer8->Unlock(buf,buf_len,NULL,0);
		}
		res = WaitForMultipleObjects (MAX_AUDIO_BUF, m_event, FALSE, INFINITE);
	}

	return 0;
}
