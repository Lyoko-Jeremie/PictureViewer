
// ����ͷ�ļ�
#include "DirectXControl.h"
// ��׼�쳣������
#include <stdexcept>
#include <iostream>
using namespace std;
// Windowsͷ�ļ�
#include <windows.h>
#include <windowsx.h>
// DirectXͷ�ļ�
#include <ddraw.h>
// �ǵ�����Ŀ����ѡ�������ӿ��ļ��ļ�ddraw.lib
// ��������Ŀͷ�ļ�����·�������Direct��������include�ļ���


// ������RGB�����
// this builds a 16 bit color value in 5.5.5 format (1-bit alpha mode)
#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))
// this builds a 24 bit color value in 8.8.8 format
#define _RGB24BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) )
// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))




// ��ʼ��DirectDraw�ṹ��
//#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }
inline void DirectXControl::DDRAW_INIT_STRUCT( DDPIXELFORMAT &ddstruct)
{
    memset(&ddstruct,0,sizeof(ddstruct));
    ddstruct.dwSize=sizeof(ddstruct);
    return;
}
inline void DirectXControl::DDRAW_INIT_STRUCT( DDSURFACEDESC2 &ddstruct)
{
    memset(&ddstruct,0,sizeof(ddstruct));
    ddstruct.dwSize=sizeof(ddstruct);
    return;
}

DirectXControl::DirectXControl( HWND hwnd, bool IsFullScreen ):
    FullScreen(IsFullScreen),
    lpdd7(nullptr),
    MainWindowHandle(hwnd),
    MainWindowRect( {0,0,0,0} ),
    MainWindowClientRect( {0,0,0,0} ),
    MainWindowBPP(0)
{
    this->AllReady = false;
    // �򿪽ӿ�
    if ( FAILED( DirectDrawCreateEx( NULL, (void**)&lpdd7, IID_IDirectDraw7, NULL ) ) )
    {
        throw runtime_error("DirectXControl():DirectDrawCreateEx()fail");
    }
    // ���ý�������
    // DDSCL_NORMAL ����
    // DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE ȫ��|����ģʽ
    // ����ģʽ����������ȫ��ģʽ��^_^��
    // DDSCL_ALLOWREBOOT ��������ģʽ�µ�Ctrl-Alt-Del�ȼ�
    if ( this->FullScreen )
    {
        if (
            FAILED(
                    lpdd7 -> SetCooperativeLevel(
                                                this->MainWindowHandle,
                                                DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT
                                                )
                   )
            )
        {
            throw runtime_error("DirectXControl():SetCooperativeLevel()fail");
        }
    }else{
        if (
            FAILED(
                    lpdd7 -> SetCooperativeLevel(
                                                this->MainWindowHandle,
                                                DDSCL_NORMAL
                                                )
                   )
            )
        {
            throw runtime_error("DirectXControl():SetCooperativeLevel()fail");
        }
    }
    // ��ʼ���ṹ��
    DDRAW_INIT_STRUCT(this->ddsd);
    // ������Ч��Ŀ
    this->ddsd.dwFlags = DDSD_CAPS;
    // ����Ϊ����ʾ����
    this->ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    // ��������ʾ����
    if (FAILED( this->lpdd7->CreateSurface(&ddsd, &lpddsprimary, NULL)))
    {
        throw runtime_error("DirectXControl():CreateSurface()fail");
    }
    this->AllReady = true;
}

DirectXControl::~DirectXControl()
{
    // ����ʾ����
    if (lpddsprimary)
       {
       lpddsprimary->Release();
       lpddsprimary = NULL;
       }

    // �ӿ�
    if (lpdd7)
       {
       lpdd7->Release();
       lpdd7 = NULL;
       }
}

// ���ع�����
bool DirectXControl::AreInitiSccess()
{
    return this->AllReady;
}

// ������ʾģʽ
bool DirectXControl::SetDisplayMode(
                                    DWORD dwWidth,
                                    DWORD dwHeight,
                                    DWORD dwBPP,
                                    DWORD dwRefreshRate, //=0
                                    DWORD dwFlag //=0
                                    )
{
    if ( FAILED( this->lpdd7->SetDisplayMode(
                                             dwWidth,
                                             dwHeight,
                                             dwBPP,
                                             dwRefreshRate,
                                             dwFlag )
                )
        )
    {
        throw runtime_error("DirectXControl().SetDisplayMode():SetDisplayMode()fail");
    }
//    this->dwWidth = dwWidth;
//    this->dwHeight = dwHeight;
//    this->dwBPP = dwBPP;
//    this->dwRefreshRate = dwRefreshRate;
    return true;
}

// ��ȡ�������ظ�ʽ
int DirectXControl::GetPixelFormat()
{
    //���ظ�ʽ
    DDPIXELFORMAT ddpixelformat;
    // ��պͳ�ʼ�����ظ�ʽ
    this->DDRAW_INIT_STRUCT(ddpixelformat);
    // ��ȡ���ظ�ʽ
    this->lpddsprimary->GetPixelFormat( &ddpixelformat );
    // ����
    this->MainWindowBPP = ddpixelformat.dwRGBBitCount;
    return this->MainWindowBPP;
}


// ��ȡ�����û�������
RECT DirectXControl::GetMainWindowClientRect()
{
    // ��ȡ�������ھ���
    GetWindowRect( this->MainWindowHandle, &(this->MainWindowRect) );
    // �����û�������
    RECT Style;
    AdjustWindowRect(
                    &(Style),
                    GetWindowStyle(this->MainWindowHandle),
                    GetMenu(this->MainWindowHandle) != nullptr //,
//                    GetWindowExStyle(this->MainWindowHandle)
                    );
    // ������ʵֵ
    if ( this->FullScreen )
    {
        // ȫ��ģʽ��û�н����� AdjustWindowRect ��������Ч
        this->MainWindowClientRect = this->MainWindowRect;
        return this->MainWindowRect;
    }else{
        // ��ȫ��ģʽҪȥ�� AdjustWindowRect �ļ�����
        this->MainWindowClientRect.left = this->MainWindowRect.left - Style.left;
        this->MainWindowClientRect.right = this->MainWindowRect.right - Style.right;
        this->MainWindowClientRect.top = this->MainWindowRect.top - Style.top;
        this->MainWindowClientRect.bottom = this->MainWindowRect.bottom - Style.bottom;
        return this->MainWindowClientRect;
    }
/*
ע��
{   // Test
    RECT MainWindowRect;
    RECT MainWindowClientRect;
    GetWindowRect( gHwnd, &(MainWindowRect) );
    AdjustWindowRect(
        &(MainWindowClientRect),
        GetWindowStyle(gHwnd),
        GetMenu(gHwnd) != nullptr //,
//                    GetWindowExStyle(this->MainWindowHandle)
        );
    cout << endl;
    cout << "left\t" << MainWindowRect.left << endl;
    cout << "right\t" << MainWindowRect.right << endl;
    cout << "top\t" << MainWindowRect.top << endl;
    cout << "bottom\t" << MainWindowRect.bottom << endl;
    cout << endl;
    cout << "left\t" << MainWindowClientRect.left << endl;          // -3
    cout << "right\t" << MainWindowClientRect.right << endl;         // 3
    cout << "top\t" << MainWindowClientRect.top << endl;           // -44 -25
    cout << "bottom\t" << MainWindowClientRect.bottom << endl;    // 3
    cout << endl;
    cout << endl;

    ��ʵ�����û�������Ϊ��
    RL - CRL
    RR - CRR
    RT - CRT
    RB - CRB

    But��
    ��ȫ��ģʽ��AdjustWindowRect�޷�������

}
*/
}





bool DirectXControl::TestPaint()
{

    // ��ȡ����
    RECT client = this->GetMainWindowClientRect();

    // ��ȡ���
    this->DDRAW_INIT_STRUCT(ddsd);


    clog << "Now Lock" << endl;

    // ��������
    this->lpddsprimary->Lock(NULL,&(this->ddsd),
                                DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);

    clog << "Lock!" << endl;

    // ��ȡ����ָ��
    UCHAR *primary_buffer = (UCHAR *)ddsd.lpSurface;

    cout << "ddsd.lPitch " << ddsd.lPitch << endl;
    cout << "*primary_buffer " << (long)primary_buffer << endl;

    // 32λ��ͼ
//    int x = (client.right - client.left) + client.left;
//    int y = (client.bottom - client.top) + client.top;
//    for ( int i = 0; 100!=i; ++i)
//    {
//        ++x;
//        ++y;
//        DWORD color = _RGB32BIT(0,128, 428, 428);
//        *((DWORD *)(primary_buffer + x*4 + y*ddsd.lPitch)) = color;
//    }

    clog << "Now Unlock" << endl;

    // ��������
    if ( FAILED( this->lpddsprimary->Unlock(NULL)))
        throw runtime_error("Cant Unlock primary");
    clog << "Unlock!" << endl;
    return true;
}


















