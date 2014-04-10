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

