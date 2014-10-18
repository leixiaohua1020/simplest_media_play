 最简单的Direct3D播放视频的例子（Direct3D[Surface]播放RGB/YUV）
 Simplest Video Play Direct3D (Direct3D[Surface] play RGB/YUV)

 雷霄骅 Lei Xiaohua
 leixiaohua1020@126.com
 中国传媒大学/数字电视技术
 Communication University of China / Digital TV Technology
 http://blog.csdn.net/leixiaohua1020

 本程序使用Direct3D播放RGB/YUV视频像素数据。使用D3D中的Surface渲染数据。
 是最简单的Direct3D播放视频的教程。
 使用Surface渲染视频相对于另一种方法（使用Texture）来说，更加简单，适合上手。
 函数调用步骤如下：

 [初始化]
 Direct3DCreate9()：获得IDirect3D9
 IDirect3D9->CreateDevice()：通过IDirect3D9创建Device（设备）。
 IDirect3DDevice9->CreateOffscreenPlainSurface()：通过Device创建一个Surface（离屏表面）。

 [循环渲染数据]
 IDirect3DSurface9->LockRect()：锁定离屏表面。
 memcpy()：填充数据
 IDirect3DSurface9->UnLockRect()：解锁离屏表面。
 IDirect3DDevice9->BeginScene()：开始绘制。
 IDirect3DDevice9->GetBackBuffer()：获得后备缓冲。
 IDirect3DDevice9->StretchRect()：拷贝Surface数据至后备缓冲。
 IDirect3DDevice9->EndScene()：结束绘制。
 IDirect3DDevice9->Present()：显示出来。

 This software play RGB/YUV raw video data using Direct3D. It uses Surface 
 in D3D to render the pixel data. Compared to another method (use Texture), 
 it is more simple and suitable for the beginner of Direct3D.
 The process is shown as follows:

 [Init]
 Direct3DCreate9(): Get IDirect3D9.
 IDirect3D9->CreateDevice(): Create a Device.
 IDirect3DDevice9->CreateOffscreenPlainSurface(): Create a Offscreen Surface.

 [Loop to Render data]
 IDirect3DSurface9->LockRect(): Lock the Offscreen Surface.
 memcpy(): Fill pixel data...
 IDirect3DSurface9->UnLockRect(): UnLock the Offscreen Surface.
 IDirect3DDevice9->BeginScene(): Begin drawing.
 IDirect3DDevice9->GetBackBuffer(): Get BackBuffer.
 IDirect3DDevice9->StretchRect(): Copy Surface data to BackBuffer.
 IDirect3DDevice9->EndScene(): End drawing.
 IDirect3DDevice9->Present(): Show on the screen.