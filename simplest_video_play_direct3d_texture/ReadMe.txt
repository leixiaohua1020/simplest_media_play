 最简单的Direct3D播放视频的例子(Direct3D[Texture]播放RGB)
 Simplest Video Play Direct3D (Direct3D[Texture] play RGB)

 雷霄骅 Lei Xiaohua
 leixiaohua1020@126.com
 中国传媒大学/数字电视技术
 Communication University of China / Digital TV Technology
 http://blog.csdn.net/leixiaohua1020

 本程序使用Direct3D播放RGB/YUV视频像素数据。使用D3D中的Texture渲染数据。
 相对于使用Surface渲染视频数据来说，使用Texture渲染视频数据功能更加灵活，
 但是学习起来也会相对复杂一些。

 函数调用步骤如下：

 [初始化]
 Direct3DCreate9()：获得IDirect3D9
 IDirect3D9->CreateDevice()：通过IDirect3D9创建Device（设备）
 IDirect3DDevice9->CreateTexture()：通过Device创建一个Texture（纹理）。
 IDirect3DDevice9->CreateVertexBuffer()：通过Device创建一个VertexBuffer（顶点缓存）。
 IDirect3DVertexBuffer9->Lock()：锁定顶点缓存。
 memcpy()：填充顶点缓存。
 IDirect3DVertexBuffer9->Unlock()：解锁顶点缓存。

 [循环渲染数据]
 IDirect3DTexture9->LockRect()：锁定纹理。
 memcpy()：填充纹理数据
 IDirect3DTexture9->UnLockRect()：解锁纹理。
 IDirect3DDevice9->BeginScene()：开始绘制。
 IDirect3DDevice9->SetTexture()：设置当前要渲染的纹理。
 IDirect3DDevice9->SetStreamSource()：绑定VertexBuffer。
 IDirect3DDevice9->SetFVF()：设置Vertex格式。
 IDirect3DDevice9->DrawPrimitive()：渲染。
 IDirect3DDevice9->EndScene()：结束绘制。
 IDirect3DDevice9->Present()：显示出来。

 This software plays RGB/YUV raw video data using Direct3D.
 It uses Texture in D3D to render the pixel data.
 Compared to another method (use Surface), it's more flexible
 but a little difficult.

 The process is shown as follows:

 [Init]
 Direct3DCreate9()：Get IDirect3D9.
 IDirect3D9->CreateDevice()：Create a Device.
 IDirect3DDevice9->CreateTexture()：Create a Texture.
 IDirect3DDevice9->CreateVertexBuffer()：Create a VertexBuffer.
 IDirect3DVertexBuffer9->Lock()：Lock VertexBuffer.
 memcpy()：Fill VertexBuffer.
 IDirect3DVertexBuffer9->Unlock()：UnLock VertexBuffer.

 [Loop to Render data]
 IDirect3DTexture9->LockRect()：Lock Texture.
 memcpy()：Fill pixel data...
 IDirect3DTexture9->UnLockRect()：UnLock Texture.
 IDirect3DDevice9->BeginScene()：Begin to draw.
 IDirect3DDevice9->SetTexture()：Set current Texture.
 IDirect3DDevice9->SetStreamSource()：Bind VertexBuffer.
 IDirect3DDevice9->SetFVF()：Set Vertex Format.
 IDirect3DDevice9->DrawPrimitive()：Render.
 IDirect3DDevice9->EndScene()：End drawing.
 IDirect3DDevice9->Present()：Show on the screen.