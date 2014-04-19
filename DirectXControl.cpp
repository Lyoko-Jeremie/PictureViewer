
// ����ͷ�ļ�
#include "DirectXControl.h"
// ��׼�쳣������
#include <stdexcept>
//#include <iostream>
using namespace std;
// �����������װ��
#include "CLogErr.h"
using namespace CLogErr::clog;
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
    MainWindowBPP(0),
    lpddsprimary(nullptr),
    lpddsback(nullptr),
    BaseX(0),
    BaseY(0),
    PureColorD(0)
{
    // ��ʼ���ṹ��
    DDRAW_INIT_STRUCT(this->ddsd);

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
    this->ChangeBackGroudColor();
    this->AllReady = true;
}

DirectXControl::~DirectXControl()
{
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
    if ( !lpddsprimary )     // ����ָ����Ч
    {
        throw logic_error("DirectXControl().GetPixelFormat():Nullptr:lpddsprimary");
        return 0;
    }
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





bool DirectXControl::TestPaint( int type, unsigned int runtimes )
{

    if ( lpddsprimary )     // ����ָ����Ч
    {
        // ��ȡ����
        RECT client = this->GetMainWindowClientRect();

        // ��ȡ���
        this->DDRAW_INIT_STRUCT(ddsd);


        clogerr << "Now Lock" << endl;

        // ��������
        if ( SUCCEEDED(
                       this->lpddsprimary->Lock(
                                                NULL,&(this->ddsd),
                                                DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
                                                NULL
                                                )
                        )
            )
        {


            // �ɹ�����
            clogerr << "Lock!" << endl;


            clogerr << "ddsd.lPitch " << ddsd.lPitch << endl;

            // ddsd.lPitch=3200 ��Ļ���800 ����32λ��Ҫ����4
            LONG lPitch32 = ddsd.lPitch >> 2;

            // 32λ����
            if ( 32 == this->GetPixelFormat() )
            {
                // ��ȡ����ָ��
                DWORD *primary_buffer = (DWORD *)ddsd.lpSurface;
                // 32λ��ͼ

                if ( 2 == type )
                {
                    for ( unsigned int r = 0; r != runtimes; ++r)
                    {
                        int x = rand() % (client.right - client.left -100) + client.left;
                        int y = rand() % (client.bottom - client.top -100) + client.top;
                        clogerr << "x " << x << endl;
                        clogerr << "y " << y << endl;
                        DWORD color = _RGB32BIT( 0, rand() %100+100, rand() %100+100, rand() %100+100);
                        for ( int i = 0; 100!=i; ++i)
                        {
                            ++x;
                            ++y;
                            if ( (x+y*lPitch32) >= ( (client.right - client.left) * (client.bottom - client.top) ) )
                            {
                                // ������Χ
                                break;
                            }
                //            *((DWORD *)(primary_buffer + x*4 + y*ddsd.lPitch)) = color;
                            primary_buffer[ x+y*lPitch32 ] = color;
                        }
                    }
                }

                if ( 1 == type )
                {
                    DWORD color = _RGB32BIT( 0, rand() %255, rand() %255, rand() %255);
                    int ax = rand() % (client.right - client.left) + client.left;
                    int bx = rand() % (client.right - ax) + ax;
                    int ay = rand() % (client.bottom - client.top) + client.top;
                    int by = rand() % (client.bottom - ay) + ay;
                    for ( int y = ay ; y != by ; ++y)
                    {
                        for ( int x = ax ; x != bx ; ++x)
                        {
                            if ( (x+y*lPitch32) >= ( (client.right - client.left) * (client.bottom - client.top) ) )
                            {
                                // ������Χ
                                break;
                            }
                            primary_buffer[ x+y*lPitch32 ] = color;
                        }
                    }
                }


            }   // End of 32λ����

            clogerr << "Now Unlock" << endl;

            // ��������
            if ( FAILED( this->lpddsprimary->Unlock(NULL)))
                throw runtime_error("Cant Unlock primary");

            clogerr << "Unlock!" << endl;


        }else
        {
            // ����ʧ��
            clogerr << "Lock Fail" << endl;
//            throw runtime_error("Cant Lock primary");
            return false;
        }
    }else{
        clogerr << "Lock Fail" << endl;
    }



    return true;
}











bool DirectXControl::PrimaryShow()
{
    if ( !lpdd7 )
    {
        throw logic_error("DirectXControl().PrimaryShow():Nullptr:lpdd7");
        return false;
    }
    // ��ʼ���ṹ��
    DDRAW_INIT_STRUCT(this->ddsd);
    // ������Ч��Ŀ
    this->ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    // ���ú󱻻�������
    this->ddsd.dwBackBufferCount = 1;
    // ����Ϊ����ʾ����
    this->ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
    // ��������ʾ����
    if (FAILED( this->lpdd7->CreateSurface(&ddsd, &lpddsprimary, nullptr)))
    {
        lpddsprimary = nullptr;
        return false;
    }
    // ��������ʾ����
    ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
    if (FAILED( this->lpddsprimary->GetAttachedSurface( &ddsd.ddsCaps, &lpddsback)))
    {
        lpddsback = nullptr;
        return false;
    }
    return true;
}

bool DirectXControl::PrimaryHide()
{
    // �󱸱���
    if (lpddsback)
    {
        lpddsback->Release();
        lpddsback = nullptr;
    }
    // ����ʾ����
    if (lpddsprimary)
    {
        lpddsprimary->Release();
        lpddsprimary = nullptr;
    }
    return true;
}

bool DirectXControl::PrimaryReFlash()
{
    // ���»�ȡ�û�������
    this->GetMainWindowClientRect();

    // ˢ��������ָ��
    this->PrimaryHide();
    // ����ֵ�ǻ�ȡ���
    return this->PrimaryShow();
}




// channels ͨ�������������֪��������ʽ
// ����ֵ��Ӧ��ͨ������
// 1 (GRAY, PALETTE),
// 2 (GRAY_ALPHA),
// 3 (RGB),
// 4 (RGB_ALPHA or RGB + filler byte)
// UCHAR **ppImage �Ĵ�С height * width * channels(RGB=3 , ARGB = 4) * bit_depth(24bit=8)
bool DirectXControl::PaintImage(
                                UCHAR **ppImage,
                                unsigned int Wide,
                                unsigned int Height,
                                UCHAR BitDepth,
                                UCHAR ColorType,
                                UCHAR Channels
                                )
{

    // Ԥ���
    // ColorType ����
    // BitDepth=8
    // Channels= 3 or 4     ��3Ϊ24λ  4Ϊ32λ��
    if ( !(8 == BitDepth && ( 3 == Channels || 4 == Channels ) ) )
    {
        clogerr << "DirectXControl:PaintImage:PremiseFailed"
                << "\nBitDepth: " << static_cast<int>(BitDepth)
                << "\tChannels: " << static_cast<int>(Channels) << endl;
        return false;
    }

    if ( lpddsback )     // ����ָ����Ч
    {
        // ��ȡ����
        RECT client = this->GetMainWindowClientRect();

        // ��ȡ���
        this->DDRAW_INIT_STRUCT(ddsd);


        clogerr << "Now Lock" << endl;

        // ��������
        if ( SUCCEEDED(
                        this->lpddsback->Lock(
                                                NULL,&(this->ddsd),
                                                DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
                                                NULL
                                                )
                        )
            )
        {
            // �ɹ�����
            clogerr << "Lock!" << endl;


            clogerr << "ddsd.lPitch " << ddsd.lPitch << endl;

            // ddsd.lPitch=3200 ��Ļ���800 ����32λ��Ҫ����4
            LONG lPitch32 = ddsd.lPitch >> 2;

            // 32λ����
            if ( 32 == this->GetPixelFormat() )
            {
                // ��ȡ����ָ��
                DWORD *back_buffer = (DWORD *)ddsd.lpSurface;
                // 32λ��ͼ


                unsigned int UserHeight = static_cast<unsigned int>(client.bottom - client.top);
                unsigned int UserWide = static_cast<unsigned int>(client.right - client.left);

                unsigned int ImageHeight = Height;
                unsigned int ImageWide = Wide;

                // ��ձ���

                for ( unsigned int y = 0; UserHeight > y; ++y)
                {
                    for ( unsigned int x = 0; UserWide > x; ++x)
                    {
                        back_buffer[ x + y*lPitch32 ] = this->PureColorD;
                    }
                }



                if ( this->BaseX >= 0 && this->BaseY >= 0 )
                {   // �����ƶ�
                    unsigned int Yi = 0;
                    for ( unsigned int y = this->BaseY; UserHeight > y; ++y, ++Yi)
                    {
                        UCHAR *pImage = nullptr;
                        if ( ImageHeight > Yi )
                        {
                            // δԽ��
                            pImage = ppImage[Yi];
                        }
                        unsigned int Xi = 0;
                        for ( unsigned int x = this->BaseX; UserWide > x; ++x, ++Xi)
                        {
                            if ( ImageWide > Xi && pImage != nullptr )
                            {
                                unsigned int ImageBit = Xi * Channels;
                                if ( 3 == Channels )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        0,
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2]
                                                                                                        );
                                }
//                                if ( 4 == Channels )
                                if ( 4 == Channels && 0 != pImage[ImageBit] )
                                {
//                                    // ��� Alpha ͸������㷨        ��ʱʵ�ֲ���
//                                    ARGB a(
//                                            pImage[ImageBit],
//                                            pImage[ImageBit+1] * pImage[ImageBit] ,
//                                            pImage[ImageBit+2] * pImage[ImageBit] ,
//                                            pImage[ImageBit+3] * pImage[ImageBit]
//                                            );
//                                    ARGB b( this->PureColorU );
//                                    b.Red *= b.Alpha * ( 255 - a.Alpha ) ;
//                                    b.Green *= b.Alpha * ( 255 - a.Alpha ) ;
//                                    b.Blue *= b.Alpha * ( 255 - a.Alpha ) ;
//                                    UCHAR alpha = a.Alpha + b.Alpha - (a.Alpha * b.Alpha) ;
//                                    if ( alpha )
//                                    {
//                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
//                                                                            255,
//                                                                            (a.Red + b.Red) /alpha,
//                                                                            (a.Green + b.Green) /alpha,
//                                                                            (a.Blue + b.Blue) /alpha
//                                                                            );
//                                    }
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2],
                                                                                                        pImage[ImageBit+3]
                                                                                                        );
                                }
                            }
                        }
                    }
                }
                if ( this->BaseX < 0 && this->BaseY >= 0 )
                {   // �����ƶ�
                    unsigned int Yi = 0;
                    for ( unsigned int y = this->BaseY; UserHeight > y; ++y, ++Yi)
                    {
                        UCHAR *pImage = nullptr;
                        if ( ImageHeight > Yi )
                        {
                            // δԽ��
                            pImage = ppImage[Yi];
                        }
                        unsigned int Xi = - this->BaseX;
                        for ( unsigned int x = 0; UserWide > x; ++x, ++Xi)
                        {
                            if ( ImageWide > Xi && pImage != nullptr )
                            {
                                unsigned int ImageBit = Xi * Channels;
                                if ( 3 == Channels )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        0,
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2]
                                                                                                        );
                                }
                                if ( 4 == Channels && 0 != pImage[ImageBit] )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2],
                                                                                                        pImage[ImageBit+3]
                                                                                                        );
                                }
                            }
                        }
                    }
                }
                if ( this->BaseX >= 0 && this->BaseY < 0 )
                {   // �����ƶ�
                    unsigned int Yi = - this->BaseY;
                    for ( unsigned int y = 0; UserHeight > y; ++y, ++Yi)
                    {
                        UCHAR *pImage = nullptr;
                        if ( ImageHeight > Yi )
                        {
                            // δԽ��
                            pImage = ppImage[Yi];
                        }
                        unsigned int Xi = 0;
                        for ( unsigned int x = this->BaseX; UserWide > x; ++x, ++Xi)
                        {
                            if ( ImageWide > Xi && pImage != nullptr )
                            {
                                unsigned int ImageBit = Xi * Channels;
                                if ( 3 == Channels )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        0,
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2]
                                                                                                        );
                                }
                                if ( 4 == Channels && 0 != pImage[ImageBit] )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2],
                                                                                                        pImage[ImageBit+3]
                                                                                                        );
                                }
                            }
                        }
                    }
                }
                if ( this->BaseX < 0 && this->BaseY < 0 )
                {   // �����ƶ�
                    unsigned int Yi = - this->BaseY;
                    for ( unsigned int y = 0; UserHeight > y; ++y, ++Yi)
                    {
                        UCHAR *pImage = nullptr;
                        if ( ImageHeight > Yi )
                        {
                            // δԽ��
                            pImage = ppImage[Yi];
                        }
                        unsigned int Xi = -  this->BaseX;
                        for ( unsigned int x =0; UserWide > x; ++x, ++Xi)
                        {
                            if ( ImageWide > Xi && pImage != nullptr )
                            {
                                unsigned int ImageBit = Xi * Channels;
                                if ( 3 == Channels )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        0,
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2]
                                                                                                        );
                                }
                                if ( 4 == Channels && 0 != pImage[ImageBit] )
                                {
                                    back_buffer[ x + y*lPitch32 ] = _RGB32BIT(
                                                                                                        pImage[ImageBit],
                                                                                                        pImage[ImageBit+1],
                                                                                                        pImage[ImageBit+2],
                                                                                                        pImage[ImageBit+3]
                                                                                                        );
                                }
                            }
                        }
                    }
                }



            }   // End of 32λ����

            clogerr << "Now Unlock" << endl;

            // ��������
            if ( FAILED( this->lpddsback->Unlock(NULL)))
                throw runtime_error("Cant Unlock primary");

            clogerr << "Unlock!" << endl;

            // ����
            while ( FAILED( this->lpddsprimary->Flip( nullptr, DDFLIP_WAIT ) ) )
                /*Empty*/;

        }else
        {
            // ����ʧ��
            clogerr << "Lock Fail" << endl;
//            throw runtime_error("Cant Lock primary");
            return false;
        }
    }else{
        clogerr << "Lock Fail" << endl;
    }



    return true;
}



bool DirectXControl::ClearScreen()
{

    if ( lpddsback )     // ����ָ����Ч
    {
        // ��ȡ����
        RECT client = this->GetMainWindowClientRect();

        // ��ȡ���
        this->DDRAW_INIT_STRUCT(ddsd);


        clogerr << "Now Lock" << endl;

        // ���ɫ
        this->ChangeBackGroudColor();


        // �������涼Ҫ���
        for ( int h = 0; 2 != h; ++h)
        {
            // ��������
            if ( SUCCEEDED(
                           this->lpddsback->Lock(
                                                    NULL,&(this->ddsd),
                                                    DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
                                                    NULL
                                                    )
                            )
                )
            {
                // �ɹ�����
                clogerr << "Lock!" << endl;


                clogerr << "ddsd.lPitch " << ddsd.lPitch << endl;

                // ddsd.lPitch=3200 ��Ļ���800 ����32λ��Ҫ����4
                LONG lPitch32 = ddsd.lPitch >> 2;

                // 32λ����
                if ( 32 == this->GetPixelFormat() )
                {
                    // ��ȡ����ָ��
                    DWORD *back_buffer = (DWORD *)ddsd.lpSurface;
                    // 32λ��ͼ

                    // ȫ��
                    unsigned int yLimit = static_cast<unsigned int>(client.bottom);
                    unsigned int xLimit = static_cast<unsigned int>(client.right);


                    for ( unsigned int y = 0; yLimit != y; ++y)
                    {
                        for ( unsigned int x = 0; xLimit != x; ++x)
                        {
                            back_buffer[ x + y*lPitch32 ] = PureColorD;
                        }
                    }



                }   // End of 32λ����

                clogerr << "Now Unlock" << endl;

                // ��������
                if ( FAILED( this->lpddsback->Unlock(NULL)))
                    throw runtime_error("Cant Unlock primary");

                clogerr << "Unlock!" << endl;

                // ����
                while ( FAILED( this->lpddsprimary->Flip( nullptr, DDFLIP_WAIT ) ) )
                    /*Empty*/;

            }else{
                // ����ʧ��
                clogerr << "Lock Fail" << endl;
    //            throw runtime_error("Cant Lock primary");
                return false;
            }
        }
    }else{
        clogerr << "Lock Fail" << endl;
    }



    return true;
}





bool DirectXControl::AddBaseX(int i)
{
    clogerr << "AddBaseY" << i << endl;
    this->BaseX += i;
    return true;
}
bool DirectXControl::AddBaseY(int i)
{
    clogerr << "AddBaseY" << i << endl;
    this->BaseY += i;
    return true;
}
bool DirectXControl::ReBase()
{
    clogerr << "ReBase" << endl;
    this->BaseX = 0;
    this->BaseY = 0;
    return true;
}





bool DirectXControl::ChangeBackGroudColor()
{
    // �������ɫ
    this->PureColorU.Alpha =  rand()%255;
    this->PureColorU.Red =  rand()%255;
    this->PureColorU.Green =  rand()%255;
    this->PureColorU.Blue =  rand()%255;
    // ͬ���� PureColorD
    this->PureColorD = _RGB32BIT(
                                this->PureColorU.Alpha,
                                this->PureColorU.Red,
                                this->PureColorU.Green,
                                this->PureColorU.Blue
                                );
    return true;
}


DWORD DirectXControl::GetBackGroudColor()
{
    return this->PureColorD;
}

bool DirectXControl::SetBackGroudColor( UCHAR Red, UCHAR Green, UCHAR Blue, UCHAR Alpha /* = 0 */ )
{
    this->PureColorU.Alpha = Alpha;
    this->PureColorU.Red = Red;
    this->PureColorU.Green = Green;
    this->PureColorU.Blue = Blue;
    return true;
}






