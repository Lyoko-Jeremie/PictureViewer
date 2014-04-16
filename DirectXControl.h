#ifndef DIRECTXCONTROL_H
#define DIRECTXCONTROL_H

// 确保 DirectX 的GUID能够使用
// 例如IID_IDirectDraw7
#define INITGUID
// 或者加入dxguid.lib库

// WIN SDK
#include <windows.h>
// 包含头文件
#include <ddraw.h>
// 记得在项目编译选项中链接库文件文件ddraw.lib
// 并且在项目头文件搜索路径中添加Direct开发包的include文件夹


// 基于对象编程 只是对所需的DirectX操作进行包装而已
class DirectXControl
{
    public:
        DirectXControl( HWND hwnd, bool IsFullScreen );
        ~DirectXControl();
        // 检测是否初始化成功
        bool AreInitiSccess();

        // 设置显示模式   Note:显示模式的改变影响整个屏幕，所以在窗口模式使用会失效【抛异常】
        bool SetDisplayMode( DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate = 0, DWORD dwFlag = 0 );

        // 获取像素格式
        int GetPixelFormat();

        // 用户区坐标
        RECT GetMainWindowClientRect();

        bool TestPaint( int type, unsigned int runtimes = 3);
        bool PaintImage( UCHAR **ppImage, unsigned int Wide, unsigned int Height, UCHAR BitDepth, UCHAR ColorType, UCHAR Channels );
        bool ClearScreen();

        bool PrimaryShow();

        bool PrimaryHide();

        bool PrimaryReFlash();

        bool AddBaseX( int i);
        bool AddBaseY( int i);
        bool ReBase();


    protected:

        inline void DDRAW_INIT_STRUCT( DDPIXELFORMAT &ddstruct);
        inline void DDRAW_INIT_STRUCT( DDSURFACEDESC2 &ddstruct);
        // 准备完成标志
        bool AllReady;
        // 全屏标志
        bool FullScreen;
        // 设备句柄
//        LPDIRECTDRAW lpdd;
        LPDIRECTDRAW7 lpdd7;
        // 绘图表面结构体
        DDSURFACEDESC2 ddsd;
        // 窗口句柄
        HWND MainWindowHandle;
        // 窗口坐标
        RECT MainWindowRect;
        // 用户区坐标
        RECT MainWindowClientRect;
        // 用户区BPP【像素】
        int MainWindowBPP;
        // 绘制表面
        LPDIRECTDRAWSURFACE7 lpddsprimary;
        // 图像基偏移
        int BaseX;
        int BaseY;

        DWORD PureColor;

//        // 设置宽高
//        DWORD dwWidth;
//        DWORD dwHeight;
//        // 设置色深
//        DWORD dwBPP;
//        // 刷新率
//        DWORD dwRefreshRate;
    private:
};

#endif // DIRECTXCONTROL_H
