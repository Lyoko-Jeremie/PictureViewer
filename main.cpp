// WIN SDK
#include <windows.h>
#include <windowsx.h>
// C++String
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;
// 菜单资源
#include "RC.h"
// DirectXControl包装类
#include "DirectXControl.h"
// 文件信息数据列表包装类
#include "FileList.h"
// Png解析库包装类
#include "PngControl.h"

/*  消息回调函数  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void PictrueLast();
void PictrueNext();
bool ReDrawing();
bool initial();
bool Shutdown();
bool OpenIndexPngFile( unsigned int Index);
bool ClosePngFile();
bool DrawAbout();

/*  将类名保存在全局变量中  */
char szClassName[ ] = "PictureViewer";

// 关于信息
string gsAboutString = "\
\n\
About:\n\
Designer : Jeremie\n\
Programmer : Jeremie\n\
Power By : Win32API DirectDraw libpng zlib\n\
Thanks my maids.\n\
\n\
                                                CC-BY-SA 3.0\n\
\n\
";

// 帮助信息
string gsHelpString = "\
\n\
Help Inform :\n\
PageUp to Last Picture\n\
PageDown to Next Picture\n\
Up to Move Up\n\
Down to Move Down\n\
Left to Move Left\n\
Right to Move Right\n\
Hone to ReSet Move\n\
Delete to Change Backgroud Color\n\
\n\
Esc to Exit\n\
End to Help.\n\
\n\
";

// 控制结构指针
DirectXControl *gpDxc = nullptr;
// Png库指针
PngControl *gpPngCDate = nullptr;
// 文件列表指针
FileList *gpFileListDate = nullptr;
// 当前文件列表编号索引
size_t gsFileIndex = 0;

// 当前显示内容
int giShowType = 0;
// Demo内容
int giDemoType = 1;


// 菜单句柄
HMENU ghMenuHandle = nullptr;
// 菜单显影
bool gbMenuIsShow = false;
// 窗口句柄
HWND gHwnd = nullptr;
// 程序句柄
HINSTANCE gHInstance = nullptr;

// 类型
typedef unsigned long DWORD;
typedef unsigned char UCHAR;

// 图像结构体
struct IMAGE{
    public:
        PCppUCHAR ppImage;
        PCUINT Width;
        PCUINT Height;
        PCUCHAR BitDepth;
        PCUCHAR ColorType;
        PCUCHAR Channels;
        IMAGE():ppImage(nullptr),Width(0),Height(0),BitDepth(0),ColorType(0),Channels(0){}
};
typedef IMAGE *pIMAGE;

// 全局图像结构体指针
pIMAGE gpImageDate = nullptr;



bool DrawObject( int object, pIMAGE pImage = nullptr, int demo = 0);


// 按键检测宏
//#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
//#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
// 按键检测函数
inline bool KEYUP(int vk_code)
{
    return (GetAsyncKeyState(vk_code) & 0x8000) ? false : true;
}
inline bool KEYDOWN(int vk_code)
{
    return (GetAsyncKeyState(vk_code) & 0x8000) ? true : false;
}

// WinMain主函数
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    gHInstance = hThisInstance;     // 本程序句柄
    HWND hwnd;               /* 窗口句柄 */
    MSG messages;            /* 消息结构体 */
    WNDCLASSEX wincl;        /* 窗口内结构体 */

    /* 构造窗口内结构体 */
    wincl.hInstance = hThisInstance;          // hInstance 字段
    wincl.lpszClassName = szClassName;        // 窗口类名
    wincl.lpfnWndProc = WindowProcedure;      /* 回调函数指针 */
    // 窗口属性样式
    // CS_DBLCLKS 接受双击消息
    // CS_HREDRAW CS_VREDRAW 在改变宽高时刷新
    // CS_OWNDC 获取设备描述表
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;
    wincl.cbSize = sizeof (WNDCLASSEX);       // 窗口类结构体大小

    /* 使用默认的icon和鼠标指针 */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);       // 图标
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);     // 图标 小
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);         // 鼠标指针
    wincl.lpszMenuName = NULL;   //"MainMenu";                 // C风格ASCII菜单资源名字符串
    // 下面两个是附加的运行时间消息，置0即可
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
//    // 背景颜色 使用默认背景色画刷
//    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wincl.hbrBackground = static_cast<HBRUSH> ( GetStockObject(WHITE_BRUSH) );

    // 注册窗口类，失败则退出程序
    if (!RegisterClassEx (&wincl))
        return -1;

    // 窗口类注册完毕，创建 Windows类
    if ( ! (hwnd = CreateWindowEx (
                0,                   // 窗口样式 扩展样式标志
                szClassName,         /* 类名 */
                "Picture Viewer",    /* 窗口标题 C风格字符串 */
                // 窗口外观和行为的通用标志
                // WS_OVERLAPPEDWINDOW 常用的标准风格
                // WS_OVERLAPPED 只有标题栏不可改变大小的窗口
                // WS_SYSMENU 系统按钮
                // WS_MINIMIZEBOX 最小化按钮
    //            WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
                // WS_POPUP 弹出式窗口 WS_VISIBLE 创建即可见
                WS_POPUP | WS_VISIBLE ,
                // 窗口XY坐标 CW_USEDEFAULT为自动
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                // 窗口宽高     // 在全屏模式下都已经无所谓了 具体设置在Direct下
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                // 父窗口指针 作为桌面的子窗口
                HWND_DESKTOP,
                NULL,                /* 菜单句柄 */
                hThisInstance,       /* hInstance */
                NULL                 /* 高级特征 */
                )
            )
        )
    {
        return -1;
    }

    gHwnd = hwnd;

    // 显示窗口
    //ShowWindow (hwnd, nCmdShow);

    ghMenuHandle = LoadMenu( hThisInstance, "MainMenu");
    // hwnd 窗口句柄    ghMenuHandle 菜单句柄
    SetMenu( hwnd, ghMenuHandle);
    gbMenuIsShow = true;

    // 创建控制对象
    DirectXControl dxc(hwnd,true);
    // 检测是否创建成功
    if ( !dxc.AreInitiSccess() )
    {
        clog << "DirectXControl Create Fail." << endl;
        return -1;
    }
    // 指针指向
    gpDxc = &dxc;

    // 初始化文件列表
    FileList FileListDate(".\\png\\*.png");
    if ( !FileListDate.GetSize() )
    {
        clog << "No Png File." << endl;
        giShowType = 0;
    }
    gpFileListDate = &FileListDate;

    // 初始化图像结构体对象
    IMAGE ImageDate;
    gpImageDate = &ImageDate;

    // 初始化Png库
    PngControl PngCDate( 4000, 2000);   // 大约一般屏幕两倍大
    if ( !PngCDate.AreInitiSccess() )
    {
        clog << "Png Lib Initial Fail." << endl;
        return -1;
    }
    gpPngCDate = &PngCDate;

    initial();


    // 确保运行在32位色深下      // 无所谓了 反证全屏
//    if ( gpDxc->GetPixelFormat() != 32 )
//    {
////        throw runtime_error("Must run in 32-bit color depth environments.");
//        return 0;
//    }

//    // 消息循环，直到GetMessage()返回0
//    while (GetMessage (&messages, NULL, 0, 0))
//    {
//        // 将虚拟按键消息转换为字符消息
//        TranslateMessage(&messages);
//        // 发送消息到消息回调函数
//        DispatchMessage(&messages);
//    }

    // peek模式消息循环
    while ( true )
    {
        if ( PeekMessage( &messages, 0, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT==messages.message)
                break;
            TranslateMessage(&messages);
            DispatchMessage(&messages);
        }
        /**< CPU防满载延迟 */
        Sleep(30);
        ReDrawing();
    }

    Shutdown();
    // 释放
    gpDxc = nullptr;
    // 程序结束会自动析构dxc局部对象
    gpFileListDate = nullptr;

    gpImageDate = nullptr;

    gpPngCDate = nullptr;


    // 程序返回值，返回PostQuitMessage()函数的参数
    return messages.wParam;
}




// 消息回调函数
// hwnd 消息来源窗口句柄
// message 消息标识符
// wParam lParam 信息
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  // 处理消息
    {
        case WM_CLOSE:
        case WM_DESTROY:
            // 销毁 窗口关闭按钮按下
            // 在这里清除一切
            gpDxc->PrimaryHide();
            PostQuitMessage (0);    // 发送 WM_QUIT 消息退出整个程序
            break;

        case WM_PAINT:
            // 重绘
            {
//                PAINTSTRUCT ps;
                // 激活窗口 并用背景画刷重绘整个窗口
                // hdc 为图形环境句柄
//                HDC hdc = BeginPaint( hwnd, &ps);
                // 重绘代码
//                EndPaint( hwnd, &ps);
            }
            break;

        // 鼠标消息
        // 鼠标消息坐标相对于所在窗口用户区
        case WM_MOUSEMOVE:
            // 鼠标移动中消息
            // x= static_cast<int> LOWORD(lParam)
            // y= static_cast<int> HIWORD(lParam)
            // 按键= static_cast<int> LOWORD(wParam)
            if ( (static_cast<int> HIWORD(lParam) < 50) && ( !gbMenuIsShow ) ) // 暂时不检测窗口
            {
                gbMenuIsShow = SetMenu( hwnd, ghMenuHandle);
            }
            if ( (static_cast<int> HIWORD(lParam) > 50) && ( gbMenuIsShow ) )
            {
                gbMenuIsShow = !SetMenu( hwnd, nullptr);
            }
            break;

        case WM_KEYDOWN:
            if ( KEYDOWN( VK_NEXT ) )
            {
                // Page Down
                PictrueNext();
            }
            if ( KEYDOWN( VK_PRIOR ) )
            {
                // Page Up
                PictrueLast();
            }
            if ( KEYDOWN( VK_HOME ) )
            {
                gpDxc->ReBase();
                // Home
            }
            if ( KEYDOWN( VK_DELETE ) )
            {
                gpDxc->ChangeBackGroudColor();
                // Home
            }
            if ( KEYDOWN( VK_END ) )
            {
                // 和关于的菜单项一致
                ClosePngFile();
                gpDxc->PrimaryReFlash();
                giShowType = 2;
                // End
            }
            // 按键按下
            break;

        case WM_SIZE:
            // 大小改变
            // 重新获取表面指针
//            clog << "WM_SIZE:PrimaryReFlash() " << endl;
//            clog << gpDxc->PrimaryReFlash() << endl;
            // Windows消息你他妈就是在逗我...你个逗比.FuckYouMother!1

            break;

        case WM_MOVE:
            // 窗口移动
            // TODO 响应move 并且在用户区绘图
            break;

//        case WM_ACTIVATE:     // 不再处理活动状态了
            {   // 窗口活动状态改变
                /**< P:谁被激活了    reinterpret_cast<HWND> lParam  Note:底层转换必须要这样做  */
                if ( gHwnd == reinterpret_cast<HWND> (lParam) )
                {   // 是主窗口
                    if ( WA_INACTIVE==LOWORD(wParam) )
                    {
//                        clog << "WM_ACTIVATE:PrimaryHide()" << endl;
//                        gpDxc->PrimaryHide();
                        /**< 取消激活 */
                        /**< 测试最小化 */
                        if ( static_cast<bool> ( HIWORD(wParam) ) )
                        {
                            /**< 最小化了 */
                        }
                        else
                        {
                            /**< 只是失去焦点 */
                        }
                    }
                    else
                    {
//                        clog << "WM_ACTIVATE:PrimaryShow()" << endl;
//                        gpDxc->PrimaryShow();
                        /**< 被激活 */
                        /**< 测试激活方式 */
                        if ( WA_ACTIVE==LOWORD(wParam))
                        {
                            /**< 非鼠标激活 */
                        }
                        else
                        {
                            if (WA_CLICKACTIVE==LOWORD(wParam))
                            {
                                /**< 由鼠标激活 */
                            }
                        }
                    }
                }
            }
            break;


        case WM_CREATE:
            // 窗口创建
            break;

        case WM_COMMAND:
            // 菜单消息
            {
                // 解析具体的菜单消息
                switch ( LOWORD( wParam ) )
                {
//                case MENU_FILE_ID_OPEN:
//                    break;
//
//                case MENU_FILE_ID_CLOSE:
//                    break;

                case MENU_FILE_ID_EXIT:
                    // 发送退出消息
                    PostMessage( hwnd, WM_DESTROY, 0, 0);
                    break;

                case MENU_EXPLORER_ID_LAST:
                    PictrueLast();
                    break;

                case MENU_EXPLORER_ID_NEXT:
                    PictrueNext();
                    break;

                case MENU_DEMO_ID_RAND_COLOER_RECT:
                    gpDxc->ClearScreen();
                    ClosePngFile();
                    giShowType = 0;
                    giDemoType = 1;
                    break;

                case MENU_DEMO_ID_RAND_COLOER_LINE:
                    gpDxc->ClearScreen();
                    ClosePngFile();
                    giShowType = 0;
                    giDemoType = 2;
                    break;

                case MENU_HELP_ID_ABOUT:

                    ClosePngFile();
                    gpDxc->PrimaryReFlash();
                    giShowType = 2;

                    break;

                default:
                    // Nothing.....
                    return DefWindowProc (hwnd, message, wParam, lParam);
                    break;
                }
            }
          break;

        default:                      // 调用默认消息函数
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}



// 上一张图
void PictrueLast()
{
    // 清空
    ClosePngFile();
    // 清屏
    gpDxc->ClearScreen();
    // 打开
    if ( !gpFileListDate->GetSize() )
    {
        // 没有文件
        giShowType = 0;
        return;
    }
    size_t index = ( 0 != gsFileIndex ? gsFileIndex - 1 : gpFileListDate->GetSize() - 1 );
    if ( OpenIndexPngFile( index ) )
    {
        giShowType = 1;
    }
    else
    {
        giShowType = 0;
    }

    ReDrawing();
    return;
}

//下一张图
void PictrueNext()
{
    // 清空
    ClosePngFile();
    // 清屏
    gpDxc->ClearScreen();
    // 打开
    if ( !gpFileListDate->GetSize() )
    {
        // 没有文件
        giShowType = 0;
        return;
    }
    size_t index = ( gpFileListDate->GetSize() - 1 != gsFileIndex ? gsFileIndex + 1 : 0 );
    if ( OpenIndexPngFile( index ) )
    {
        giShowType = 1;
    }
    else
    {
        giShowType = 0;
    }

    ReDrawing();
    return;
}


// 重绘
bool ReDrawing()
{
    if ( KEYDOWN( VK_ESCAPE ) )
    {
        // 发送退出消息
        PostMessage( gHwnd, WM_DESTROY, 0, 0);
        return 0;
    }
    if ( 1 == giShowType )
    {
        if ( KEYDOWN( VK_LEFT ) )
        {
            gpDxc->AddBaseX( - 5 );
            // 左箭头
        }
        if ( KEYDOWN( VK_RIGHT ) )
        {
            gpDxc->AddBaseX( 5 );
            // 右箭头
        }
        if ( KEYDOWN( VK_UP ) )
        {
            gpDxc->AddBaseY( -5 );
            // 上箭头
        }
        if ( KEYDOWN( VK_DOWN ) )
        {
            gpDxc->AddBaseY( 5 );
            // 下箭头
        }
    }

    DrawObject( giShowType, gpImageDate, giDemoType);
    return true;
}


// 初始化
bool initial()
{

    // 获取当前屏幕设置
    unsigned int ScreenWide = GetSystemMetrics( SM_CXSCREEN );
    unsigned int ScreenHeight = GetSystemMetrics( SM_CYSCREEN );

    // 设置显示模式
    gpDxc->SetDisplayMode(
                          static_cast<DWORD> (ScreenWide),
                          static_cast<DWORD> (ScreenHeight),
                          static_cast<DWORD> (32)
                          );
    gpDxc->PrimaryShow();

    // 第一张图
    if ( gpFileListDate->GetSize() )
    {
        // 有图片

        if ( OpenIndexPngFile(0) )
        {
            // 成功打开，显示图片
            giShowType = 1;
            clog << "OpenIndexPngFileSeccess" << endl;
        }
        else
        {
            // 打开失败，显示Demo
            giShowType = 0;
            clog << "OpenIndexPngFileFail" << endl;
        }

    }else{
        clog << "No Png File." << endl;
        giShowType = 0;
    }

    return true;
}

// 结束
bool Shutdown()
{
    gpDxc->PrimaryHide();
    return true;
}


// 绘制对象
// object:  null = -1 , demo = 0 , Image = 1 , about = 2
// pImage 位图指针
bool DrawObject( int object, pIMAGE pImage /*= nullptr*/, int demo /*= 0*/)
{
    if ( -1 == object )
    {
        // Empty
        return true;
    }
    if ( 0 == object )
    {
        gpDxc->TestPaint(demo);
        return true;
    }
    if ( 1 == object )
    {
        // 附着到对象
        IMAGE &atImageDate = *gpImageDate;
        gpDxc->PaintImage(
                          atImageDate.ppImage,
                          atImageDate.Width,
                          atImageDate.Height,
                          atImageDate.BitDepth,
                          atImageDate.ColorType,
                          atImageDate.Channels
                          );
        return true;
    }
    if ( 2 == object )
    {
        DrawAbout();
        // 关于 等信息
        return true;
    }
    return true;
}



bool OpenIndexPngFile( unsigned int Index)
{
        // 读取
        gsFileIndex = Index;
        if ( gpPngCDate->OpenPngFile( ".\\png\\" + gpFileListDate->GetFileOnlyName( gsFileIndex ) ) )
        {
            // 正确打开
            if ( gpPngCDate->AreOpenSccess() && gpPngCDate->AreOpenSccess() )
            {
                // 附着到对象
                IMAGE &atImageDate = *gpImageDate;

                // 填结构体
                atImageDate.Width = gpPngCDate->GetPngWidth();
                atImageDate.Height = gpPngCDate->GetPngHeight();
                atImageDate.BitDepth = gpPngCDate->GetPngBitDepth();
                atImageDate.ColorType = gpPngCDate->GetPngColorType();
                atImageDate.Channels = gpPngCDate->GetPngChannels();
                atImageDate.ppImage = gpPngCDate->GetPngPixelArray();

                // 有效性检测
                if (
                    atImageDate.BitDepth
                    &&
                    atImageDate.Channels
                    &&
                    atImageDate.ColorType
                    &&
                    atImageDate.Height
                    &&
                    atImageDate.Height
                    &&
                    atImageDate.Width
                    )
                {
                    // 全不为0
                    // UCHAR 的 height * width * channels(RGB=3 , ARGB = 4) * bit_depth(24bit=8)
                    clog << "OpenIndexPngFile:PngAreOpen" << endl;
                    clog << "\nWidth: " << static_cast<unsigned int>(atImageDate.Width)
                            << "\tBitDepth: " << static_cast<unsigned int>(atImageDate.BitDepth)
                            << "\tColorType: " << static_cast<unsigned int>(atImageDate.ColorType)
                            << "\tChannels: " << static_cast<unsigned int>(atImageDate.Channels) << endl;
                    giShowType = 1;
                }else{
                    // 恢复结构体
                    atImageDate.Width = 0;
                    atImageDate.Height = 0;
                    atImageDate.BitDepth = 0;
                    atImageDate.ColorType = 0;
                    atImageDate.Channels = 0;
                    atImageDate.ppImage = nullptr;
                    gpDxc->ReBase();
                }

            }
        }else{
            clog << "OpenIndexPngFile:PngAreOpenFail" << endl;
            gpPngCDate->ReStartPngLib();
            return false;
        }

    return true;
}


bool ClosePngFile()
{
    clog << "ClosePngFile" << endl;
    giShowType = -1;    // 不显示
    gpPngCDate->ReStartPngLib();
    // 附着到对象
    IMAGE &atImageDate = *gpImageDate;
    // 恢复结构体
    atImageDate.Width = 0;
    atImageDate.Height = 0;
    atImageDate.BitDepth = 0;
    atImageDate.ColorType = 0;
    atImageDate.Channels = 0;
    atImageDate.ppImage = nullptr;
    gpDxc->ReBase();
    return true;
}


bool DrawAbout()
{
    HDC gdc = nullptr;
    if ( nullptr != ( gdc = GetDC(gHwnd) ) )
    {
        // 有效

        // 获取用户区矩形
        RECT rect;
        GetClientRect( gHwnd, &rect);

        // 绘制
        rect.left += 20;
        rect.right += 20;
        string AboutString = gsHelpString
                                        + gsAboutString
                                        + "\n\nThe lib Copyright :\n"
                                        + "\n\n" + gpPngCDate->PngLibCopyright()
                                        + "\n\n" + gpPngCDate->PngZLibCopyright()
                                        ;
        DrawText( gdc, AboutString.c_str(), AboutString.size(), &rect, DT_LEFT);

        // 释放
        ReleaseDC( gHwnd, gdc);

        // 使有效
        ValidateRect( gHwnd, &rect);
        return true;
    }
    clog << "DrawAboutFail." << endl;
    return false;
}







