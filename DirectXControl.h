#ifndef DIRECTXCONTROL_H
#define DIRECTXCONTROL_H

// 确保 DirectX 的GUID能够使用
// 例如IID_IDirectDraw7
#define INITGUID

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
        DirectXControl( HWND hwnd );
        ~DirectXControl();
        // 检测是否初始化成功
        bool AreInitiSccess();
        // 设置显示模式
        bool SetDisplayMode( DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate, DWORD dwFlag );
    protected:
        // 准备完成标志
        bool AllReady;
        // 设备句柄
        LPDIRECTDRAW lpdd;
        LPDIRECTDRAW7 lpdd7;
        // 窗口句柄
        HWND MainWindowHandle;
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
