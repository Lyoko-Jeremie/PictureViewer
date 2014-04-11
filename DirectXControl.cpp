#include "DirectXControl.h"

// ����ͷ�ļ�
#include <ddraw.h>
// �ǵ�����Ŀ����ѡ�������ӿ��ļ��ļ�ddraw.lib
// ��������Ŀͷ�ļ�����·�������Direct��������include�ļ���

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






