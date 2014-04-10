// WIN SDK
#include <windows.h>
// 菜单资源
#include "MainMenu.h"
// DirectXControl包装类
#include "DirectXControl.h"

/*  消息回调函数  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  将类名保存在全局变量中  */
char szClassName[ ] = "PictureViewer";

// 按键检测宏
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// 控制结构指针
DirectXControl *gdxc = nullptr;

// WinMain主函数
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
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
    wincl.lpszMenuName = "MainMenu";                 // C风格ASCII菜单资源名字符串
    // 下面两个是附加的运行时间消息，置0即可
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    // 背景颜色 使用默认背景色画刷
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    // 注册窗口类，失败则退出程序
    if (!RegisterClassEx (&wincl))
        return -1;

    // 窗口类注册完毕，创建 Windows类
    hwnd = CreateWindowEx (
           0,                   // 窗口样式 扩展样式标志
           szClassName,         /* 类名 */
           "Picture Viewer",    /* 窗口标题 C风格字符串 */
           // 窗口外观和行为的通用标志
           // WS_OVERLAPPEDWINDOW 常用的标准风格
           WS_OVERLAPPEDWINDOW,
           // 窗口XY坐标 CW_USEDEFAULT为自动
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           // 窗口宽高 CW_USEDEFAULT为自动
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           // 父窗口指针 作为桌面的子窗口
           HWND_DESKTOP,
           LoadMenu( hThisInstance, "MainMenu") ,                /* 菜单句柄 */
           hThisInstance,       /* hInstance */
           NULL                 /* 高级特征 */
           );

    // 显示窗口
    ShowWindow (hwnd, nCmdShow);

    // 创建控制对象
    DirectXControl dxc(hwnd);
    // 检测是否创建成功
    if ( !dxc.AreInitiSccess() )
        return -1;
    // 指针指向
    gdxc = &dxc;

    // 消息循环，直到GetMessage()返回0
    while (GetMessage (&messages, NULL, 0, 0))
    {
        // 将虚拟按键消息转换为字符消息
        TranslateMessage(&messages);
        // 发送消息到消息回调函数
        DispatchMessage(&messages);
    }

    // 释放
    gdxc = nullptr;
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
        case WM_DESTROY:
            // 销毁 窗口关闭按钮按下
            // 在这里清除一切
            PostQuitMessage (0);    // 发送 WM_QUIT 消息退出整个程序
            break;

        case WM_PAINT:
            // 重绘
            {
                PAINTSTRUCT ps;
                // 激活窗口 并用背景画刷重绘整个窗口
                // hdc 为图形环境句柄
                HDC hdc = BeginPaint( hwnd, &ps);
                // 重绘代码
                EndPaint( hwnd, &ps);
            }
            break;

        case WM_KEYDOWN:
            // 按键按下
            break;

        case WM_SIZE:
            // 大小改变
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
                    PostMessage( hwnd, WM_DESTROY, 0, 0);
                    break;

                case MENU_EXPLORER_ID_LAST:
                    break;

                case MENU_EXPLORER_ID_NEXT:
                    break;

                case MENU_HELP_ID_ABOUT:
                    MessageBox(
                               hwnd,
                               "Picture Viewer\n\n\nDesign by Jeremie\n\n\nPower by DirectX",
                               "About Picture Viewer",
                               MB_OK | MB_ICONEXCLAMATION
                               );
                    break;

                default:
                    // Nothing.....
                    break;
                }
            }
          break;

        default:                      // 调用默认消息函数
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}











