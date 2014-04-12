
// 包含头文件
#include "DirectXControl.h"
// 标准异常处理类
#include <stdexcept>
#include <iostream>
using namespace std;
// Windows头文件
#include <windows.h>
#include <windowsx.h>
// DirectX头文件
#include <ddraw.h>
// 记得在项目编译选项中链接库文件文件ddraw.lib
// 并且在项目头文件搜索路径中添加Direct开发包的include文件夹


// 下面是RGB构造宏
// this builds a 16 bit color value in 5.5.5 format (1-bit alpha mode)
#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))
// this builds a 24 bit color value in 8.8.8 format
#define _RGB24BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) )
// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))




// 初始化DirectDraw结构体
//#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }
inline void DirectXControl::DDRAW_INIT_STRUCT( DDPIXELFORMAT &ddstruct)
{
    memset(&ddstruct,0,sizeof(ddstruct));
    ddstruct.dwSize=sizeof(ddstruct);
    return;
}
inline void DirectXControl::DDRAW_INIT_STRUCT( DDSURFACEDESC2 &ddstruct)
{
    memset(&ddstruct,0,sizeof(ddstruct));
    ddstruct.dwSize=sizeof(ddstruct);
    return;
}

DirectXControl::DirectXControl( HWND hwnd, bool IsFullScreen ):
    FullScreen(IsFullScreen),
    lpdd7(nullptr),
    MainWindowHandle(hwnd),
    MainWindowRect( {0,0,0,0} ),
    MainWindowClientRect( {0,0,0,0} ),
    MainWindowBPP(0)
{
    this->AllReady = false;
    // 打开接口
    if ( FAILED( DirectDrawCreateEx( NULL, (void**)&lpdd7, IID_IDirectDraw7, NULL ) ) )
    {
        throw runtime_error("DirectXControl():DirectDrawCreateEx()fail");
    }
    // 设置交互级别
    // DDSCL_NORMAL 窗口
    // DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE 全屏|排他模式
    // 排他模式才是真正的全屏模式【^_^】
    // DDSCL_ALLOWREBOOT 允许排他模式下的Ctrl-Alt-Del热键
    if ( this->FullScreen )
    {
        if (
            FAILED(
                    lpdd7 -> SetCooperativeLevel(
                                                this->MainWindowHandle,
                                                DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT
                                                )
                   )
            )
        {
            throw runtime_error("DirectXControl():SetCooperativeLevel()fail");
        }
    }else{
        if (
            FAILED(
                    lpdd7 -> SetCooperativeLevel(
                                                this->MainWindowHandle,
                                                DDSCL_NORMAL
                                                )
                   )
            )
        {
            throw runtime_error("DirectXControl():SetCooperativeLevel()fail");
        }
    }
    // 初始化结构体
    DDRAW_INIT_STRUCT(this->ddsd);
    // 设置有效项目
    this->ddsd.dwFlags = DDSD_CAPS;
    // 设置为主显示表面
    this->ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    // 创建主显示表面
    if (FAILED( this->lpdd7->CreateSurface(&ddsd, &lpddsprimary, NULL)))
    {
        throw runtime_error("DirectXControl():CreateSurface()fail");
    }
    this->AllReady = true;
}

DirectXControl::~DirectXControl()
{
    // 主显示表面
    if (lpddsprimary)
       {
       lpddsprimary->Release();
       lpddsprimary = NULL;
       }

    // 接口
    if (lpdd7)
       {
       lpdd7->Release();
       lpdd7 = NULL;
       }
}

// 返回构造结果
bool DirectXControl::AreInitiSccess()
{
    return this->AllReady;
}

// 设置显示模式
bool DirectXControl::SetDisplayMode(
                                    DWORD dwWidth,
                                    DWORD dwHeight,
                                    DWORD dwBPP,
                                    DWORD dwRefreshRate, //=0
                                    DWORD dwFlag //=0
                                    )
{
    if ( FAILED( this->lpdd7->SetDisplayMode(
                                             dwWidth,
                                             dwHeight,
                                             dwBPP,
                                             dwRefreshRate,
                                             dwFlag )
                )
        )
    {
        throw runtime_error("DirectXControl().SetDisplayMode():SetDisplayMode()fail");
    }
//    this->dwWidth = dwWidth;
//    this->dwHeight = dwHeight;
//    this->dwBPP = dwBPP;
//    this->dwRefreshRate = dwRefreshRate;
    return true;
}

// 获取窗口像素格式
int DirectXControl::GetPixelFormat()
{
    //像素格式
    DDPIXELFORMAT ddpixelformat;
    // 清空和初始化像素格式
    this->DDRAW_INIT_STRUCT(ddpixelformat);
    // 获取像素格式
    this->lpddsprimary->GetPixelFormat( &ddpixelformat );
    // 保存
    this->MainWindowBPP = ddpixelformat.dwRGBBitCount;
    return this->MainWindowBPP;
}


// 获取窗口用户区矩形
RECT DirectXControl::GetMainWindowClientRect()
{
    // 获取整个窗口矩形
    GetWindowRect( this->MainWindowHandle, &(this->MainWindowRect) );
    // 计算用户区矩形
    RECT Style;
    AdjustWindowRect(
                    &(Style),
                    GetWindowStyle(this->MainWindowHandle),
                    GetMenu(this->MainWindowHandle) != nullptr //,
//                    GetWindowExStyle(this->MainWindowHandle)
                    );
    // 计算真实值
    if ( this->FullScreen )
    {
        // 全屏模式下没有界面风格 AdjustWindowRect 计算结果无效
        this->MainWindowClientRect = this->MainWindowRect;
        return this->MainWindowRect;
    }else{
        // 非全屏模式要去掉 AdjustWindowRect 的计算结果
        this->MainWindowClientRect.left = this->MainWindowRect.left - Style.left;
        this->MainWindowClientRect.right = this->MainWindowRect.right - Style.right;
        this->MainWindowClientRect.top = this->MainWindowRect.top - Style.top;
        this->MainWindowClientRect.bottom = this->MainWindowRect.bottom - Style.bottom;
        return this->MainWindowClientRect;
    }
/*
注：
{   // Test
    RECT MainWindowRect;
    RECT MainWindowClientRect;
    GetWindowRect( gHwnd, &(MainWindowRect) );
    AdjustWindowRect(
        &(MainWindowClientRect),
        GetWindowStyle(gHwnd),
        GetMenu(gHwnd) != nullptr //,
//                    GetWindowExStyle(this->MainWindowHandle)
        );
    cout << endl;
    cout << "left\t" << MainWindowRect.left << endl;
    cout << "right\t" << MainWindowRect.right << endl;
    cout << "top\t" << MainWindowRect.top << endl;
    cout << "bottom\t" << MainWindowRect.bottom << endl;
    cout << endl;
    cout << "left\t" << MainWindowClientRect.left << endl;          // -3
    cout << "right\t" << MainWindowClientRect.right << endl;         // 3
    cout << "top\t" << MainWindowClientRect.top << endl;           // -44 -25
    cout << "bottom\t" << MainWindowClientRect.bottom << endl;    // 3
    cout << endl;
    cout << endl;

    真实窗口用户区矩形为：
    RL - CRL
    RR - CRR
    RT - CRT
    RB - CRB

    But：
    在全屏模式下AdjustWindowRect无法计算结果

}
*/
}





bool DirectXControl::TestPaint()
{

    // 获取区域
    RECT client = this->GetMainWindowClientRect();

    // 获取句柄
    this->DDRAW_INIT_STRUCT(ddsd);


    clog << "Now Lock" << endl;

    // 加锁表面
    this->lpddsprimary->Lock(NULL,&(this->ddsd),
                                DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);

    clog << "Lock!" << endl;

    // 获取表面指针
    UCHAR *primary_buffer = (UCHAR *)ddsd.lpSurface;

    cout << "ddsd.lPitch " << ddsd.lPitch << endl;
    cout << "*primary_buffer " << (long)primary_buffer << endl;

    // 32位绘图
//    int x = (client.right - client.left) + client.left;
//    int y = (client.bottom - client.top) + client.top;
//    for ( int i = 0; 100!=i; ++i)
//    {
//        ++x;
//        ++y;
//        DWORD color = _RGB32BIT(0,128, 428, 428);
//        *((DWORD *)(primary_buffer + x*4 + y*ddsd.lPitch)) = color;
//    }

    clog << "Now Unlock" << endl;

    // 解锁表面
    if ( FAILED( this->lpddsprimary->Unlock(NULL)))
        throw runtime_error("Cant Unlock primary");
    clog << "Unlock!" << endl;
    return true;
}


















