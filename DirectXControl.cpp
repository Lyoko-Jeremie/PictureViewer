#include "DirectXControl.h"

// 包含头文件
#include <ddraw.h>
// 记得在项目编译选项中链接库文件文件ddraw.lib
// 并且在项目头文件搜索路径中添加Direct开发包的include文件夹

DirectXControl::DirectXControl( HWND hwnd )
{
    this->AllReady = false;
    this->lpdd = nullptr;
    this->lpdd7 = nullptr;
    this->MainWindowHandle = hwnd;
    if ( FAILED( DirectDrawCreateEx( NULL, (void**)&lpdd7, IID_IDirectDraw7, NULL ) ) )
    {
        return;
    }
    if ( FAILED( lpdd7 -> SetCooperativeLevel( this->MainWindowHandle, DDSCL_NORMAL ) ) )
    {
        return;
    }
    this->AllReady = true;
}

DirectXControl::~DirectXControl()
{
    if (!this->AllReady)
        return;
    lpdd7 -> Release();
}

bool DirectXControl::AreInitiSccess()
{
    return this->AllReady;
}


bool DirectXControl::SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate = 0, DWORD dwFlag = 0)
{
    if ( FAILED( this->lpdd7->SetDisplayMode( dwWidth, dwHeight, dwBPP, dwRefreshRate, dwFlag ) ) )
    {
        return false;
    }
//    this->dwWidth = dwWidth;
//    this->dwHeight = dwHeight;
//    this->dwBPP = dwBPP;
//    this->dwRefreshRate = dwRefreshRate;
    return true;
}






