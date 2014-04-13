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

/*  消息回调函数  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void PictrueLast();
void PictrueNext();
bool ReDrawing();
bool initial();
bool Shutdown();

/*  将类名保存在全局变量中  */
char szClassName[ ] = "PictureViewer";

// 控制结构指针
DirectXControl *glDxc = nullptr;

// 窗口设置
const DWORD gwWidth = 800;
const DWORD gwHeight =  600;

// 菜单句柄
HMENU ghMenuHandle = nullptr;
// 菜单显影
bool gbMenuIsShow = false;
// 窗口句柄
HWND gHwnd = nullptr;
// 程序句柄
HINSTANCE gHInstance = nullptr;
// 当前显示文件名
string gFileName;

// 类型
typedef unsigned long DWORD;

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
        return -1;
    // 指针指向
    glDxc = &dxc;
    initial();

    // 确保运行在32位色深下      // 无所谓了 反证全屏
//    if ( glDxc->GetPixelFormat() != 32 )
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
    glDxc = nullptr;
    // 程序结束会自动析构dxc局部对象


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
            glDxc->PrimaryHide();
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
            if ( (static_cast<int> HIWORD(lParam) < 20) && ( !gbMenuIsShow ) ) // 暂时不检测窗口
            {
                gbMenuIsShow = SetMenu( hwnd, ghMenuHandle);
            }
            if ( (static_cast<int> HIWORD(lParam) > 20) && ( gbMenuIsShow ) )
            {
                gbMenuIsShow = !SetMenu( hwnd, nullptr);
            }
            break;

        case WM_KEYDOWN:
            if ( KEYDOWN( VK_ESCAPE ) )
            {
                // 发送退出消息
                PostMessage( hwnd, WM_DESTROY, 0, 0);
                return 0;
            }
            if ( KEYDOWN( VK_LEFT ) )
            {
                // 左箭头
                PictrueLast();
            }
            if ( KEYDOWN( VK_RIGHT ) )
            {
                // 右箭头
                PictrueNext();
            }
            // 按键按下
            break;

        case WM_SIZE:
            // 大小改变
            // 重新获取表面指针
//            clog << "WM_SIZE:PrimaryReFlash() " << endl;
//            clog << glDxc->PrimaryReFlash() << endl;
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
//                        glDxc->PrimaryHide();
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
//                        glDxc->PrimaryShow();
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
                case MENU_FILE_ID_OPEN:
                    break;

                case MENU_FILE_ID_CLOSE:
                    break;

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

                case MENU_HELP_ID_ABOUT:
//                    MessageBox(
//                               hwnd,
//                               "Picture Viewer\n\n\nDesign by Jeremie\n\n\nPower by DirectX",
//                               "About Picture Viewer",
//                               MB_OK | MB_ICONEXCLAMATION
//                               );
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
    // 测试像素位数【结果为32】
    clog << glDxc->GetPixelFormat() << endl;
    {
        RECT a = glDxc->GetMainWindowClientRect();
        clog << "left " << a.left << endl;
        clog << "right " << a.right << endl;
        clog << "top " << a.top << endl;
        clog << "bottom " << a.bottom << endl;
    }
//    {   // Test
//        RECT MainWindowRect;
//        RECT MainWindowClientRect;
//        GetWindowRect( gHwnd, &(MainWindowRect) );
//        AdjustWindowRect(
//            &(MainWindowClientRect),
//            GetWindowStyle(gHwnd),
//            GetMenu(gHwnd) != nullptr
//            );
//        cout << endl;
//        cout << "left\t" << MainWindowRect.left << endl;
//        cout << "right\t" << MainWindowRect.right << endl;
//        cout << "top\t" << MainWindowRect.top << endl;
//        cout << "bottom\t" << MainWindowRect.bottom << endl;
//        cout << endl;
//        cout << "left\t" << MainWindowClientRect.left << endl;          // -3
//        cout << "right\t" << MainWindowClientRect.right << endl;         // 3
//        cout << "top\t" << MainWindowClientRect.top << endl;           // -44 -25
//        cout << "bottom\t" << MainWindowClientRect.bottom << endl;    // 3
//        cout << endl;
//        cout << endl;
//
////        真实窗口用户区矩形为：
////        RL - CRL
////        RR - CRR
////        RT - CRT
////        RB - CRB
//
//    }
    ReDrawing();
    return;
}

//下一张图
void PictrueNext()
{
    ReDrawing();
    glDxc->TestPaint();
    return;
}


// 重绘
bool ReDrawing()
{
    // for now test if user is hitting ESC and send WM_CLOSE
    if (KEYDOWN(VK_ESCAPE))
    {
        PostMessage(gHwnd,WM_CLOSE,0,0);
    } // end if

    glDxc->TestPaint();

    return true;
}


// 初始化
bool initial()
{
    // 设置显示模式
    glDxc->SetDisplayMode(
                          static_cast<DWORD> (800),
                          static_cast<DWORD> (600),
                          static_cast<DWORD> (32)
                          );
    glDxc->PrimaryShow();
    //TODO 读文件
    // 设置 gFileName
    return true;
}

// 结束
bool Shutdown()
{
    glDxc->PrimaryHide();
    return true;
}






