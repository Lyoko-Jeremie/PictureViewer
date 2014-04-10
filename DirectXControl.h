#ifndef DIRECTXCONTROL_H
#define DIRECTXCONTROL_H

// 确保 DirectX 的GUID能够使用
// 例如IID_IDirectDraw7
#define INITGUID

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
        bool AreInitiSccess();
    protected:
    private:
        bool AllReady;
        LPDIRECTDRAW lpdd;
        LPDIRECTDRAW7 lpdd7;
        HWND MainWindowHandle;
};

#endif // DIRECTXCONTROL_H
