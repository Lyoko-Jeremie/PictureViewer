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
        bool PaintImage(
                        UCHAR **ppImage,
                        unsigned int Wide,
                        unsigned int Height,
                        UCHAR BitDepth,
                        UCHAR ColorType,
                        UCHAR Channels,
                        bool AutoBackGroudColor
                        );
        bool ClearScreen();

        bool PrimaryShow();

        bool PrimaryHide();

        bool PrimaryReFlash();

        bool AddBaseX( int i);
        bool AddBaseY( int i);
        bool ReBase();
        bool ChangeBackGroudColor();
        DWORD GetBackGroudColor();
        bool SetBackGroudColor( UCHAR Red, UCHAR Green, UCHAR Blue, UCHAR Alpha = 0 );


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
        // 后备表面
        LPDIRECTDRAWSURFACE7 lpddsback;
        // 图像基偏移
        int BaseX;
        int BaseY;

        // 纯色 DWORD 版
        DWORD PureColorD;
        // 纯色 ARGB 版        Note:两个应该同步
        struct ARGB{
            UCHAR Alpha;
            UCHAR Red;
            UCHAR Green;
            UCHAR Blue;
            ARGB():Alpha(0),Red(0),Green(0),Blue(0) {}
            ARGB( const UCHAR &alpha, const UCHAR &red, const UCHAR &green, const UCHAR &blue ):
                        Alpha(alpha),Red(red),Green(green),Blue(blue) {}
            ARGB( const DWORD &s ):Alpha(0),Red(0),Green(0),Blue(0)
            {
                this->Blue = static_cast<UCHAR>( s & 0xff );
                this->Green = static_cast<UCHAR>( ( s >> 8 ) & 0xff );
                this->Red = static_cast<UCHAR>( ( s >> 16 ) & 0xff );
                this->Alpha = static_cast<UCHAR>( ( s >> 24 ) & 0xff );
            }
            ARGB( const ARGB &s ):
                Alpha(s.Alpha),Red(s.Red),Green(s.Green),Blue(s.Blue){}
            DWORD to_ARGB32()
            {
                return ((Blue) + ((Green) << 8) + ((Red) << 16) + ((Alpha) << 24));
            }
        } PureColorU;

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
