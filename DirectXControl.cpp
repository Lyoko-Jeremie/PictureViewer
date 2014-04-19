
// 包含头文件
#include "DirectXControl.h"
// 标准异常处理类
#include <stdexcept>
//#include <iostream>
using namespace std;
// 测试用输出包装类
#include "CLogErr.h"
using namespace CLogErr::clog;
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
    MainWindowBPP(0),
    lpddsprimary(nullptr),
    lpddsback(nullptr),
    BaseX(0),
    BaseY(0),
    PureColorD(0)
{
    // 初始化结构体
    DDRAW_INIT_STRUCT(this->ddsd);

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
    this->ChangeBackGroudColor();
    this->AllReady = true;
}

DirectXControl::~DirectXControl()
{
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
    if ( !lpddsprimary )     // 表面指针无效
    {
        throw logic_error("DirectXControl().GetPixelFormat():Nullptr:lpddsprimary");
        return 0;
    }
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





bool DirectXControl::TestPaint( int type, unsigned int runtimes )
{

    if ( lpddsprimary )     // 表面指针有效
    {
        // 获取区域
        RECT client = this->GetMainWindowClientRect();

        // 获取句柄
        this->DDRAW_INIT_STRUCT(ddsd);


        clogerr << "Now Lock" << endl;

        // 加锁表面
        if ( SUCCEEDED(
                       this->lpddsprimary->Lock(
                                                NULL,&(this->ddsd),
                                                DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
                                                NULL
                                                )
                        )
            )
        {


            // 成功加锁
            clogerr << "Lock!" << endl;


            clogerr << "ddsd.lPitch " << ddsd.lPitch << endl;

            // ddsd.lPitch=3200 屏幕宽度800 所以32位下要除以4
            LONG lPitch32 = ddsd.lPitch >> 2;

            // 32位保护
            if ( 32 == this->GetPixelFormat() )
            {
                // 获取表面指针
                DWORD *primary_buffer = (DWORD *)ddsd.lpSurface;
                // 32位绘图

                if ( 2 == type )
                {
                    for ( unsigned int r = 0; r != runtimes; ++r)
                    {
                        int x = rand() % (client.right - client.left -100) + client.left;
                        int y = rand() % (client.bottom - client.top -100) + client.top;
                        clogerr << "x " << x << endl;
                        clogerr << "y " << y << endl;
                        DWORD color = _RGB32BIT( 0, rand() %100+100, rand() %100+100, rand() %100+100);
                        for ( int i = 0; 100!=i; ++i)
                        {
                            ++x;
                            ++y;
                            if ( (x+y*lPitch32) >= ( (client.right - client.left) * (client.bottom - client.top) ) )
                            {
                                // 超出范围
                                break;
                            }
                //            *((DWORD *)(primary_buffer + x*4 + y*ddsd.lPitch)) = color;
                            primary_buffer[ x+y*lPitch32 ] = color;
                        }
                    }
                }

                if ( 1 == type )
                {
                    DWORD color = _RGB32BIT( 0, rand() %255, rand() %255, rand() %255);
                    int ax = rand() % (client.right - client.left) + client.left;
                    int bx = rand() % (client.right - ax) + ax;
                    int ay = rand() % (client.bottom - client.top) + client.top;
                    int by = rand() % (client.bottom - ay) + ay;
                    for ( int y = ay ; y != by ; ++y)
                    {
                        for ( int x = ax ; x != bx ; ++x)
                        {
                            if ( (x+y*lPitch32) >= ( (client.right - client.left) * (client.bottom - client.top) ) )
                            {
                                // 超出范围
                                break;
                            }
                            primary_buffer[ x+y*lPitch32 ] = color;
                        }
                    }
                }


            }   // End of 32位保护

            clogerr << "Now Unlock" << endl;

            // 解锁表面
            if ( FAILED( this->lpddsprimary->Unlock(NULL)))
                throw runtime_error("Cant Unlock primary");

            clogerr << "Unlock!" << endl;


        }else
        {
            // 加锁失败
            clogerr << "Lock Fail" << endl;
//            throw runtime_error("Cant Lock primary");
            return false;
        }
    }else{
        clogerr << "Lock Fail" << endl;
    }



    return true;
}











bool DirectXControl::PrimaryShow()
{
    if ( !lpdd7 )
    {
        throw logic_error("DirectXControl().PrimaryShow():Nullptr:lpdd7");
        return false;
    }
    // 初始化结构体
    DDRAW_INIT_STRUCT(this->ddsd);
    // 设置有效项目
    this->ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    // 设置后被缓冲数量
    this->ddsd.dwBackBufferCount = 1;
    // 设置为主显示表面
    this->ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
    // 创建主显示表面
    if (FAILED( this->lpdd7->CreateSurface(&ddsd, &lpddsprimary, nullptr)))
    {
        lpddsprimary = nullptr;
        return false;
    }
    // 创建后备显示表面
    ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
    if (FAILED( this->lpddsprimary->GetAttachedSurface( &ddsd.ddsCaps, &lpddsback)))
    {
        lpddsback = nullptr;
        return false;
    }
    return true;
}

bool DirectXControl::PrimaryHide()
{
    // 后备表面
    if (lpddsback)
    {
        lpddsback->Release();
        lpddsback = nullptr;
    }
    // 主显示表面
    if (lpddsprimary)
    {
        lpddsprimary->Release();
        lpddsprimary = nullptr;
    }
    return true;
}

bool DirectXControl::PrimaryReFlash()
{
    // 重新获取用户区矩形
    this->GetMainWindowClientRect();

    // 刷新主表面指针
    this->PrimaryHide();
    // 返回值是获取结果
    return this->PrimaryShow();
}




// channels 通道数，这里可以知道基本格式
// 返回值对应的通道类型
// 1 (GRAY, PALETTE),
// 2 (GRAY_ALPHA),
// 3 (RGB),
// 4 (RGB_ALPHA or RGB + filler byte)
// UCHAR **ppImage 的大小 height * width * channels(RGB=3 , ARGB = 4) * bit_depth(24bit=8)
bool DirectXControl::PaintImage(
                                UCHAR **ppImage,
                                unsigned int Wide,
                                unsigned int Height,
                                UCHAR BitDepth,
                                UCHAR ColorType,
                                UCHAR Channels
                                )
{

    // 预检查
    // ColorType 不管
    // BitDepth=8
    // Channels= 3 or 4     【3为24位  4为32位】
    if ( !(8 == BitDepth && ( 3 == Channels || 4 == Channels ) ) )
    {
        clogerr << "DirectXControl:PaintImage:PremiseFailed"
                << "\nBitDepth: " << static_cast<int>(BitDepth)
                << "\tChannels: " << static_cast<int>(Channels) << endl;
        return false;
    }

    if ( lpddsback )     // 表面指针有效
    {
        // 获取区域
        RECT client = this->GetMainWindowClientRect();

        // 获取句柄
        this->DDRAW_INIT_STRUCT(ddsd);


        clogerr << "Now Lock" << endl;

        // 加锁表面
        if ( SUCCEEDED(
                        this->lpddsback->Lock(
                                                NULL,&(this->ddsd),
                                                DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
                                                NULL
                                                )
                        )
            )
        {
            // 成功加锁
            clogerr << "Lock!" << endl;


            clogerr << "ddsd.lPitch " << ddsd.lPitch << endl;

            // ddsd.lPitch=3200 屏幕宽度800 所以32位下要除以4
            LONG lPitch32 = ddsd.lPitch >> 2;

            // 32位保护
            if ( 32 == this->GetPixelFormat() )
            {
                // 获取表面指针
                DWORD *back_buffer = (DWORD *)ddsd.lpSurface;
                // 32位绘图


                unsigned int UserHeight = static_cast<unsigned int>(client.bottom - client.top);
                unsigned int UserWide = static_cast<unsigned int>(client.right - client.left);

                unsigned int ImageHeight = Height;
                unsigned int ImageWide = Wide;

                // 清空表面

                for ( unsigned int y = 0; UserHeight > y; ++y)
                {
                    for ( unsigned int x = 0; UserWide > x; ++x)
                    {
                        back_buffer[ x + y*lPitch32 ] = this->PureColorD;
                    }
                }



                if ( this->BaseX >= 0 && this->BaseY >= 0 )
                {   // 右下移动
                    unsigned int Yi = 0;
                    for ( unsigned int y = this->BaseY; UserHeight > y; ++y, ++Yi)
                    {
                        UCHAR *pImage = nullptr;
                        if ( ImageHeight > Yi )
                        {
                            // 未越界
                            pImage = ppImage[Yi];
                        }
                        unsigned int Xi = 0;
                        for ( unsigned int x = this->BaseX; UserWide > x; ++x, ++Xi)
                        {
                            if ( ImageWide > Xi && pImage != nullptr )
                            {
                                unsigned int ImageBit = Xi * Channels;
                                if ( 3 == Channels )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        0,
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2]
                                                                                                        );
                                }
//                                if ( 4 == Channels )
                                if ( 4 == Channels && 0 != pImage[ImageBit] )
                                {
//                                    // 添加 Alpha 透明混合算法        暂时实现不了
//                                    ARGB a(
//                                            pImage[ImageBit],
//                                            pImage[ImageBit+1] * pImage[ImageBit] ,
//                                            pImage[ImageBit+2] * pImage[ImageBit] ,
//                                            pImage[ImageBit+3] * pImage[ImageBit]
//                                            );
//                                    ARGB b( this->PureColorU );
//                                    b.Red *= b.Alpha * ( 255 - a.Alpha ) ;
//                                    b.Green *= b.Alpha * ( 255 - a.Alpha ) ;
//                                    b.Blue *= b.Alpha * ( 255 - a.Alpha ) ;
//                                    UCHAR alpha = a.Alpha + b.Alpha - (a.Alpha * b.Alpha) ;
//                                    if ( alpha )
//                                    {
//                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
//                                                                            255,
//                                                                            (a.Red + b.Red) /alpha,
//                                                                            (a.Green + b.Green) /alpha,
//                                                                            (a.Blue + b.Blue) /alpha
//                                                                            );
//                                    }
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2],
                                                                                                        pImage[ImageBit+3]
                                                                                                        );
                                }
                            }
                        }
                    }
                }
                if ( this->BaseX < 0 && this->BaseY >= 0 )
                {   // 左下移动
                    unsigned int Yi = 0;
                    for ( unsigned int y = this->BaseY; UserHeight > y; ++y, ++Yi)
                    {
                        UCHAR *pImage = nullptr;
                        if ( ImageHeight > Yi )
                        {
                            // 未越界
                            pImage = ppImage[Yi];
                        }
                        unsigned int Xi = - this->BaseX;
                        for ( unsigned int x = 0; UserWide > x; ++x, ++Xi)
                        {
                            if ( ImageWide > Xi && pImage != nullptr )
                            {
                                unsigned int ImageBit = Xi * Channels;
                                if ( 3 == Channels )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        0,
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2]
                                                                                                        );
                                }
                                if ( 4 == Channels && 0 != pImage[ImageBit] )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2],
                                                                                                        pImage[ImageBit+3]
                                                                                                        );
                                }
                            }
                        }
                    }
                }
                if ( this->BaseX >= 0 && this->BaseY < 0 )
                {   // 右上移动
                    unsigned int Yi = - this->BaseY;
                    for ( unsigned int y = 0; UserHeight > y; ++y, ++Yi)
                    {
                        UCHAR *pImage = nullptr;
                        if ( ImageHeight > Yi )
                        {
                            // 未越界
                            pImage = ppImage[Yi];
                        }
                        unsigned int Xi = 0;
                        for ( unsigned int x = this->BaseX; UserWide > x; ++x, ++Xi)
                        {
                            if ( ImageWide > Xi && pImage != nullptr )
                            {
                                unsigned int ImageBit = Xi * Channels;
                                if ( 3 == Channels )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        0,
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2]
                                                                                                        );
                                }
                                if ( 4 == Channels && 0 != pImage[ImageBit] )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2],
                                                                                                        pImage[ImageBit+3]
                                                                                                        );
                                }
                            }
                        }
                    }
                }
                if ( this->BaseX < 0 && this->BaseY < 0 )
                {   // 左上移动
                    unsigned int Yi = - this->BaseY;
                    for ( unsigned int y = 0; UserHeight > y; ++y, ++Yi)
                    {
                        UCHAR *pImage = nullptr;
                        if ( ImageHeight > Yi )
                        {
                            // 未越界
                            pImage = ppImage[Yi];
                        }
                        unsigned int Xi = -  this->BaseX;
                        for ( unsigned int x =0; UserWide > x; ++x, ++Xi)
                        {
                            if ( ImageWide > Xi && pImage != nullptr )
                            {
                                unsigned int ImageBit = Xi * Channels;
                                if ( 3 == Channels )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        0,
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2]
                                                                                                        );
                                }
                                if ( 4 == Channels && 0 != pImage[ImageBit] )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2],
                                                                                                        pImage[ImageBit+3]
                                                                                                        );
                                }
                            }
                        }
                    }
                }



            }   // End of 32位保护

            clogerr << "Now Unlock" << endl;

            // 解锁表面
            if ( FAILED( this->lpddsback->Unlock(NULL)))
                throw runtime_error("Cant Unlock primary");

            clogerr << "Unlock!" << endl;

            // 交换
            while ( FAILED( this->lpddsprimary->Flip( nullptr, DDFLIP_WAIT ) ) )
                /*Empty*/;

        }else
        {
            // 加锁失败
            clogerr << "Lock Fail" << endl;
//            throw runtime_error("Cant Lock primary");
            return false;
        }
    }else{
        clogerr << "Lock Fail" << endl;
    }



    return true;
}



bool DirectXControl::ClearScreen()
{

    if ( lpddsback )     // 表面指针有效
    {
        // 获取区域
        RECT client = this->GetMainWindowClientRect();

        // 获取句柄
        this->DDRAW_INIT_STRUCT(ddsd);


        clogerr << "Now Lock" << endl;

        // 随机色
        this->ChangeBackGroudColor();


        // 两个表面都要清空
        for ( int h = 0; 2 != h; ++h)
        {
            // 加锁表面
            if ( SUCCEEDED(
                           this->lpddsback->Lock(
                                                    NULL,&(this->ddsd),
                                                    DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
                                                    NULL
                                                    )
                            )
                )
            {
                // 成功加锁
                clogerr << "Lock!" << endl;


                clogerr << "ddsd.lPitch " << ddsd.lPitch << endl;

                // ddsd.lPitch=3200 屏幕宽度800 所以32位下要除以4
                LONG lPitch32 = ddsd.lPitch >> 2;

                // 32位保护
                if ( 32 == this->GetPixelFormat() )
                {
                    // 获取表面指针
                    DWORD *back_buffer = (DWORD *)ddsd.lpSurface;
                    // 32位绘图

                    // 全屏
                    unsigned int yLimit = static_cast<unsigned int>(client.bottom);
                    unsigned int xLimit = static_cast<unsigned int>(client.right);


                    for ( unsigned int y = 0; yLimit != y; ++y)
                    {
                        for ( unsigned int x = 0; xLimit != x; ++x)
                        {
                            back_buffer[ x + y*lPitch32 ] = PureColorD;
                        }
                    }



                }   // End of 32位保护

                clogerr << "Now Unlock" << endl;

                // 解锁表面
                if ( FAILED( this->lpddsback->Unlock(NULL)))
                    throw runtime_error("Cant Unlock primary");

                clogerr << "Unlock!" << endl;

                // 交换
                while ( FAILED( this->lpddsprimary->Flip( nullptr, DDFLIP_WAIT ) ) )
                    /*Empty*/;

            }else{
                // 加锁失败
                clogerr << "Lock Fail" << endl;
    //            throw runtime_error("Cant Lock primary");
                return false;
            }
        }
    }else{
        clogerr << "Lock Fail" << endl;
    }



    return true;
}





bool DirectXControl::AddBaseX(int i)
{
    clogerr << "AddBaseY" << i << endl;
    this->BaseX += i;
    return true;
}
bool DirectXControl::AddBaseY(int i)
{
    clogerr << "AddBaseY" << i << endl;
    this->BaseY += i;
    return true;
}
bool DirectXControl::ReBase()
{
    clogerr << "ReBase" << endl;
    this->BaseX = 0;
    this->BaseY = 0;
    return true;
}





bool DirectXControl::ChangeBackGroudColor()
{
    // 创建随机色
    this->PureColorU.Alpha =  rand()%255;
    this->PureColorU.Red =  rand()%255;
    this->PureColorU.Green =  rand()%255;
    this->PureColorU.Blue =  rand()%255;
    // 同步到 PureColorD
    this->PureColorD = _RGB32BIT(
                                this->PureColorU.Alpha,
                                this->PureColorU.Red,
                                this->PureColorU.Green,
                                this->PureColorU.Blue
                                );
    return true;
}


DWORD DirectXControl::GetBackGroudColor()
{
    return this->PureColorD;
}

bool DirectXControl::SetBackGroudColor( UCHAR Red, UCHAR Green, UCHAR Blue, UCHAR Alpha /* = 0 */ )
{
    this->PureColorU.Alpha = Alpha;
    this->PureColorU.Red = Red;
    this->PureColorU.Green = Green;
    this->PureColorU.Blue = Blue;
    return true;
}






