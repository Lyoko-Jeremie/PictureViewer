// WIN SDK
#include <windows.h>
#include <windowsx.h>
// C++String
#include <string>
//#include <iostream>
#include <stdexcept>
using namespace std;
// �˵���Դ
#include "RC.h"
// DirectXControl��װ��
#include "DirectXControl.h"
// �ļ���Ϣ�����б��װ��
#include "FileList.h"
// Png�������װ��
#include "PngControl.h"
// �����������װ��
#include "CLogErr.h"
using namespace CLogErr::clog;

/*  ��Ϣ�ص�����  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void PictrueLast();
void PictrueNext();
bool ReDrawing();
bool initial();
bool Shutdown();
bool OpenIndexPngFile( unsigned int Index);
bool ClosePngFile();
bool DrawAbout();
HCURSOR LoadAnimatedCursor(UINT nID, LPCTSTR pszResouceType);
bool ExitAPPMessage();


/*  ������������ȫ�ֱ�����  */
char szClassName[ ] = "PictureViewer";

// ������Ϣ
string gsAboutString = "\
\n\
About:\n\
Designer : Jeremie\n\
Programmer : Jeremie\n\
Power By : Win32API DirectDraw libpng zlib\n\
Thanks my maids.\n\
\n\
                                                CC-BY-SA 3.0\n\
\n\
";

// ������Ϣ
string gsHelpString = "\
\n\
���� :\n\
��һ��ͼ PageUp / ���Ϲ���\n\
��һ��ͼ PageDown / ���¹���\n\
�ƶ� �������� / ����϶�\n\
����λ�� Home / ���˫��\n\
�ı䱳����ɫ Delete / �Ҽ�����\n\
\n\
�˳� Esc / ���+�Ҽ�\n\
���� End\n\
\n\
";

// ���ƽṹָ��
DirectXControl *gpDxc = nullptr;
// Png��ָ��
PngControl *gpPngCDate = nullptr;
// �ļ��б�ָ��
FileList *gpFileListDate = nullptr;
// ��ǰ�ļ��б�������
size_t gsFileIndex = 0;

// ��ǰ��ʾ����
int giShowType = 0;
// Demo����   ��ʼֵ 1
int giDemoType = 1;


// ����϶��ò���
bool gbMouseLeftButtonDown = false;
int giMouseX = 0;
int giMouseY = 0;


// �˵����
HMENU ghMenuHandle = nullptr;
// �˵���Ӱ
bool gbMenuIsShow = false;
// ���ھ��
HWND gHwnd = nullptr;
// ������
HINSTANCE gHInstance = nullptr;

// �����Դ
HCURSOR ghCurosrMove = nullptr;
HCURSOR ghCurosrArrow = nullptr;

// ����
typedef unsigned long DWORD;
typedef unsigned char UCHAR;

// ͼ��ṹ��
struct IMAGE{
    public:
        PCppUCHAR ppImage;
        PCUINT Width;
        PCUINT Height;
        PCUCHAR BitDepth;
        PCUCHAR ColorType;
        PCUCHAR Channels;
        IMAGE():ppImage(nullptr),Width(0),Height(0),BitDepth(0),ColorType(0),Channels(0){}
};
typedef IMAGE *pIMAGE;

// ȫ��ͼ��ṹ��ָ��
pIMAGE gpImageDate = nullptr;



bool DrawObject( int object, pIMAGE pImage = nullptr, int demo = 0);


// ��������
//#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
//#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
// ������⺯��
inline bool KEYUP(int vk_code)
{
    return (GetAsyncKeyState(vk_code) & 0x8000) ? false : true;
}
inline bool KEYDOWN(int vk_code)
{
    return (GetAsyncKeyState(vk_code) & 0x8000) ? true : false;
}

// WinMain������
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    gHInstance = hThisInstance;     // ��������
    HWND hwnd;               /* ���ھ�� */
    MSG messages;            /* ��Ϣ�ṹ�� */
    WNDCLASSEX wincl;        /* �����ڽṹ�� */

    /* ���촰���ڽṹ�� */
    wincl.hInstance = hThisInstance;          // hInstance �ֶ�
    wincl.lpszClassName = szClassName;        // ��������
    wincl.lpfnWndProc = WindowProcedure;      /* �ص�����ָ�� */
    // ����������ʽ
    // CS_DBLCLKS ����˫����Ϣ
    // CS_HREDRAW CS_VREDRAW �ڸı���ʱˢ��
    // CS_OWNDC ��ȡ�豸������
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;
    wincl.cbSize = sizeof (WNDCLASSEX);       // ������ṹ���С

    /* ʹ��Ĭ�ϵ�icon�����ָ�� */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);       // ͼ��
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);     // ͼ�� С
    wincl.hCursor = nullptr;         // ���ָ��
    wincl.lpszMenuName = NULL;   //"MainMenu";                 // C���ASCII�˵���Դ���ַ���
    // ���������Ǹ��ӵ�����ʱ����Ϣ����0����
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
//    // ������ɫ ʹ��Ĭ�ϱ���ɫ��ˢ
//    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wincl.hbrBackground = static_cast<HBRUSH> ( GetStockObject(WHITE_BRUSH) );

    // ע�ᴰ���࣬ʧ�����˳�����
    if (!RegisterClassEx (&wincl))
        return -1;

    // ������ע����ϣ����� Windows��
    if ( ! (hwnd = CreateWindowEx (
                0,                   // ������ʽ ��չ��ʽ��־
                szClassName,         /* ���� */
                "Picture Viewer",    /* ���ڱ��� C����ַ��� */
                // ������ۺ���Ϊ��ͨ�ñ�־
                // WS_OVERLAPPEDWINDOW ���õı�׼���
                // WS_OVERLAPPED ֻ�б��������ɸı��С�Ĵ���
                // WS_SYSMENU ϵͳ��ť
                // WS_MINIMIZEBOX ��С����ť
    //            WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
                // WS_POPUP ����ʽ���� WS_VISIBLE �������ɼ�
                WS_POPUP | WS_VISIBLE ,
                // ����XY���� CW_USEDEFAULTΪ�Զ�
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                // ���ڿ��     // ��ȫ��ģʽ�¶��Ѿ�����ν�� ����������Direct��
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                // ������ָ�� ��Ϊ������Ӵ���
                HWND_DESKTOP,
                NULL,                /* �˵���� */
                hThisInstance,       /* hInstance */
                NULL                 /* �߼����� */
                )
            )
        )
    {
        return -1;
    }

    gHwnd = hwnd;

    // ������Դ
    // LoadAnimatedCursor(CURSOR_ID_MOVE, RT_ANICURSOR);
    // RT_ANICURSOR ����Դ��������
    ghCurosrMove = LoadAnimatedCursor(CURSOR_ID_MOVE, RT_ANICURSOR);
    // SetCursor(ghCurosrMove);     ���ط���

    // ����Ĭ�����   Ĭ����nullptr��
    ghCurosrArrow = LoadCursor( nullptr, IDC_ARROW);
    // Ĭ�����
    SetCursor(ghCurosrArrow);

    // ��ʾ����
    //ShowWindow (hwnd, nCmdShow);

    ghMenuHandle = LoadMenu( hThisInstance, "MainMenu");
    // hwnd ���ھ��    ghMenuHandle �˵����
    SetMenu( hwnd, ghMenuHandle);
    gbMenuIsShow = true;

    // �������ƶ���
    DirectXControl dxc(hwnd,true);
    // ����Ƿ񴴽��ɹ�
    if ( !dxc.AreInitiSccess() )
    {
        clogerr << "DirectXControl Create Fail.";
        clogerr << endl;
        return -1;
    }
    // ָ��ָ��
    gpDxc = &dxc;

    // ��ʼ���ļ��б�
    FileList FileListDate(".\\png\\*.png");
    if ( !FileListDate.GetSize() )
    {
        clogerr << "No Png File." << endl;
        giShowType = 0;
    }
    gpFileListDate = &FileListDate;

    // ��ʼ��ͼ��ṹ�����
    IMAGE ImageDate;
    gpImageDate = &ImageDate;

    // ��ʼ��Png��
    PngControl PngCDate( 4000, 2000);   // ��Լһ����Ļ������
    if ( !PngCDate.AreInitiSccess() )
    {
        clogerr << "Png Lib Initial Fail." << endl;
        return -1;
    }
    gpPngCDate = &PngCDate;

    initial();


    // ȷ��������32λɫ����      // ����ν�� ��֤ȫ��
//    if ( gpDxc->GetPixelFormat() != 32 )
//    {
////        throw runtime_error("Must run in 32-bit color depth environments.");
//        return 0;
//    }

//    // ��Ϣѭ����ֱ��GetMessage()����0
//    while (GetMessage (&messages, NULL, 0, 0))
//    {
//        // �����ⰴ����Ϣת��Ϊ�ַ���Ϣ
//        TranslateMessage(&messages);
//        // ������Ϣ����Ϣ�ص�����
//        DispatchMessage(&messages);
//    }

    // peekģʽ��Ϣѭ��
    while ( true )
    {
        if ( PeekMessage( &messages, 0, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT==messages.message)
                break;
            TranslateMessage(&messages);
            DispatchMessage(&messages);
        }
        #ifndef NDEBUG
        /**< CPU�������ӳ� */
        Sleep(30);
        #else
//        DWORD start_time = GetTickCount();
        #endif // NDEBUG
        ReDrawing();

        #ifdef NDEBUG
//        while ( ( GetTickCount() - start_time ) < 33 )
//            /*Enmpty*/;
        #endif // NDEBUG
    }

    Shutdown();
    // �ͷ�
    gpDxc = nullptr;
    // ����������Զ�����dxc�ֲ�����
    gpFileListDate = nullptr;

    gpImageDate = nullptr;

    gpPngCDate = nullptr;


    // ���򷵻�ֵ������PostQuitMessage()�����Ĳ���
    return messages.wParam;
}




// ��Ϣ�ص�����
// hwnd ��Ϣ��Դ���ھ��
// message ��Ϣ��ʶ��
// wParam lParam ��Ϣ
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  // ������Ϣ
    {
        case WM_CLOSE:
        case WM_DESTROY:
            // ���� ���ڹرհ�ť����
            ExitAPPMessage();
            break;

        // �����Ϣ
        // �����Ϣ������������ڴ����û���
        case WM_MOUSEMOVE:
            // ����ƶ�����Ϣ
            // x= static_cast<int> LOWORD(lParam)
            // y= static_cast<int> HIWORD(lParam)
            // ����= static_cast<int> LOWORD(wParam)
            if ( gbMouseLeftButtonDown )
            {   // ����   ����
                int NewMouseX = static_cast<int> LOWORD(lParam);
                int NewMouseY = static_cast<int> HIWORD(lParam);
                // ����λ��
                gpDxc->AddBaseX( NewMouseX - giMouseX );
                gpDxc->AddBaseY( NewMouseY - giMouseY );
                // ˢ��
                giMouseX = NewMouseX;
                giMouseY = NewMouseY;
            }
            else
            {
                if ( (static_cast<int> HIWORD(lParam) < 20) && ( !gbMenuIsShow ) ) // ��ʱ����ⴰ��
                {
                    // ���ͼ��
                    gpDxc->PrimaryReFlash();
                    gbMenuIsShow = SetMenu( hwnd, ghMenuHandle);
                }
                if ( (static_cast<int> HIWORD(lParam) > 20) && ( gbMenuIsShow ) )
                {
                    gbMenuIsShow = !SetMenu( hwnd, nullptr);
                }
            }
            break;

        case WM_LBUTTONDOWN:
            // ��ʼ����
            SetCursor(ghCurosrMove);    // �������
            gbMouseLeftButtonDown = true;
            // x= static_cast<int> LOWORD(lParam)
            // y= static_cast<int> HIWORD(lParam)
            giMouseX = static_cast<int> LOWORD(lParam);
            giMouseY = static_cast<int> HIWORD(lParam);
            // �������
            break;

        case WM_LBUTTONUP:
            // ���ٽ���
            SetCursor(ghCurosrArrow);   // �ָ����
            gbMouseLeftButtonDown = false;
            giMouseX = 0;
            giMouseY = 0;
            // �������
            break;

        case WM_LBUTTONDBLCLK:
            gpDxc->ReBase();
            // ���˫����Ϣ
            break;

        case WM_RBUTTONDOWN:
            gpDxc->ChangeBackGroudColor();
            if ( true == gbMouseLeftButtonDown )
            {
                // ���˳�ָ��
                ExitAPPMessage();
            }
            // �Ҽ���Ϣ
            break;

        case WM_MOUSEWHEEL :
            // HIWORD(wParam) == WHEEL_DELTA�ı��� && ��Ϊ��
//            clogerr << "WM_MOUSEWHEEL " << static_cast<short>( HIWORD(wParam) ) << endl;
            if ( HIWORD(wParam) < 0 )
            {
                PictrueLast();
            }
            if ( HIWORD(wParam) > 0 )
            {
                PictrueNext();
            }
            // �����¼�
            break;

        case WM_KEYDOWN:
            if ( KEYDOWN( VK_NEXT ) )
            {
                // Page Down
                PictrueNext();
            }
            if ( KEYDOWN( VK_PRIOR ) )
            {
                // Page Up
                PictrueLast();
            }
            if ( KEYDOWN( VK_HOME ) )
            {
                gpDxc->ReBase();
                // Home
            }
            if ( KEYDOWN( VK_DELETE ) )
            {
                gpDxc->ChangeBackGroudColor();
                // Home
            }
            if ( KEYDOWN( VK_END ) )
            {
                // �͹��ڵĲ˵���һ��
                ClosePngFile();
                gpDxc->PrimaryReFlash();
                giShowType = 2;
                // End
            }
            // ��������
            break;

        case WM_SIZE:
            // ��С�ı�
            // ���»�ȡ����ָ��
//            clogerr << "WM_SIZE:PrimaryReFlash() " << endl;
//            clogerr << gpDxc->PrimaryReFlash() << endl;
            // Windows��Ϣ��TM�����ڶ���...�������.FuckMicrosoft!

            break;

        case WM_COMMAND:
            // �˵���Ϣ
            {
                // ��������Ĳ˵���Ϣ
                switch ( LOWORD( wParam ) )
                {
//                case MENU_FILE_ID_OPEN:
//                    break;
//
//                case MENU_FILE_ID_CLOSE:
//                    break;

                case MENU_FILE_ID_EXIT:
                    // �����˳���Ϣ
                    PostMessage( hwnd, WM_DESTROY, 0, 0);
                    break;

                case MENU_EXPLORER_ID_LAST:
                    PictrueLast();
                    break;

                case MENU_EXPLORER_ID_NEXT:
                    PictrueNext();
                    break;

                case MENU_DEMO_ID_RAND_COLOER_RECT:
                    gpDxc->ClearScreen();
                    ClosePngFile();
                    giShowType = 0;
                    giDemoType = 1;
                    break;

                case MENU_DEMO_ID_RAND_COLOER_LINE:
                    gpDxc->ClearScreen();
                    ClosePngFile();
                    giShowType = 0;
                    giDemoType = 2;
                    break;

                case MENU_HELP_ID_ABOUT:

                    ClosePngFile();
                    gpDxc->PrimaryReFlash();
                    giShowType = 2;

                    break;

                default:
                    // Nothing.....
                    return DefWindowProc (hwnd, message, wParam, lParam);
                    break;
                }
            }
          break;

        default:                      // ����Ĭ����Ϣ����
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}



// ��һ��ͼ
void PictrueLast()
{
    // ���
    ClosePngFile();
    // ����
    gpDxc->ClearScreen();
    // ��
    if ( !gpFileListDate->GetSize() )
    {
        // û���ļ�
        giShowType = 0;
        return;
    }
    size_t index = ( 0 != gsFileIndex ? gsFileIndex - 1 : gpFileListDate->GetSize() - 1 );
    if ( OpenIndexPngFile( index ) )
    {
        giShowType = 1;
    }
    else
    {
        giShowType = 0;
    }

    ReDrawing();
    return;
}

//��һ��ͼ
void PictrueNext()
{
    // ���
    ClosePngFile();
    // ����
    gpDxc->ClearScreen();
    // ��
    if ( !gpFileListDate->GetSize() )
    {
        // û���ļ�
        giShowType = 0;
        return;
    }
    size_t index = ( gpFileListDate->GetSize() - 1 != gsFileIndex ? gsFileIndex + 1 : 0 );
    if ( OpenIndexPngFile( index ) )
    {
        giShowType = 1;
    }
    else
    {
        giShowType = 0;
    }

    ReDrawing();
    return;
}


// �ػ�
bool ReDrawing()
{
    if ( KEYDOWN( VK_ESCAPE ) )
    {
        ExitAPPMessage();
        return 0;
    }
    if ( 1 == giShowType )
    {
        if ( KEYDOWN( VK_LEFT ) )
        {
            gpDxc->AddBaseX( - 5 );
            // ���ͷ
        }
        if ( KEYDOWN( VK_RIGHT ) )
        {
            gpDxc->AddBaseX( 5 );
            // �Ҽ�ͷ
        }
        if ( KEYDOWN( VK_UP ) )
        {
            gpDxc->AddBaseY( -5 );
            // �ϼ�ͷ
        }
        if ( KEYDOWN( VK_DOWN ) )
        {
            gpDxc->AddBaseY( 5 );
            // �¼�ͷ
        }
    }

    if ( GetMenu( gHwnd ) == nullptr )
    {
        // û�в˵��Ż�ͼ
        DrawObject( giShowType, gpImageDate, giDemoType);
    }
    return true;
}


// ��ʼ��
bool initial()
{

    // ��ȡ��ǰ��Ļ����
    unsigned int ScreenWide = GetSystemMetrics( SM_CXSCREEN );
    unsigned int ScreenHeight = GetSystemMetrics( SM_CYSCREEN );

    // ������ʾģʽ
    gpDxc->SetDisplayMode(
                          static_cast<DWORD> (ScreenWide),
                          static_cast<DWORD> (ScreenHeight),
                          static_cast<DWORD> (32)
                          );
    gpDxc->PrimaryShow();

    // ��һ��ͼ
    if ( gpFileListDate->GetSize() )
    {
        // ��ͼƬ

        if ( OpenIndexPngFile(0) )
        {
            // �ɹ��򿪣���ʾͼƬ
            giShowType = 1;
            clogerr << "OpenIndexPngFileSeccess" << endl;
        }
        else
        {
            // ��ʧ�ܣ���ʾDemo
            giShowType = 0;
            clogerr << "OpenIndexPngFileFail" << endl;
        }

    }else{
        clogerr << "No Png File." << endl;
        giShowType = 0;
    }

    return true;
}

// ����
bool Shutdown()
{
    gpDxc->PrimaryHide();
    return true;
}


// ���ƶ���
// object:  null = -1 , demo = 0 , Image = 1 , about = 2
// pImage λͼָ��
bool DrawObject( int object, pIMAGE pImage /*= nullptr*/, int demo /*= 0*/)
{
    if ( -1 == object )
    {
        // Empty
        return true;
    }
    if ( 0 == object )
    {

        // �� Demo ��ͼ�����ػ�
        #ifdef NDEBUG
        static DWORD start_time;
        #endif // NDEBUG

        gpDxc->TestPaint(demo);

        #ifdef NDEBUG
        while ( ( GetTickCount() - start_time ) < 11 )
            Sleep(1);
        // DWORD start_time = GetTickCount();
        start_time = GetTickCount();
        #endif // NDEBUG

        return true;
    }
    if ( 1 == object )
    {
        // ���ŵ�����
        IMAGE &atImageDate = *gpImageDate;
        gpDxc->PaintImage(
                          atImageDate.ppImage,
                          atImageDate.Width,
                          atImageDate.Height,
                          atImageDate.BitDepth,
                          atImageDate.ColorType,
                          atImageDate.Channels
                          );
        return true;
    }
    if ( 2 == object )
    {
        DrawAbout();
        // ���� ����Ϣ
        return true;
    }
    return true;
}



bool OpenIndexPngFile( unsigned int Index)
{
        // ��ȡ
        gsFileIndex = Index;
        if ( gpPngCDate->OpenPngFile( ".\\png\\" + gpFileListDate->GetFileOnlyName( gsFileIndex ) ) )
        {
            // ��ȷ��
            if ( gpPngCDate->AreOpenSccess() && gpPngCDate->AreOpenSccess() )
            {
                // ���ŵ�����
                IMAGE &atImageDate = *gpImageDate;

                // ��ṹ��
                atImageDate.Width = gpPngCDate->GetPngWidth();
                atImageDate.Height = gpPngCDate->GetPngHeight();
                atImageDate.BitDepth = gpPngCDate->GetPngBitDepth();
                atImageDate.ColorType = gpPngCDate->GetPngColorType();
                atImageDate.Channels = gpPngCDate->GetPngChannels();
                atImageDate.ppImage = gpPngCDate->GetPngPixelArray();

                // ��Ч�Լ��
                if (
                    atImageDate.BitDepth
                    &&
                    atImageDate.Channels
                    &&
                    atImageDate.ColorType
                    &&
                    atImageDate.Height
                    &&
                    atImageDate.Width
                    )
                {
                    // ȫ��Ϊ0
                    // UCHAR �� height * width * channels(RGB=3 , ARGB = 4) * bit_depth(24bit=8)
                    clogerr << "OpenIndexPngFile:PngAreOpen" << endl;
                    clogerr << "\nWidth: "
                            << static_cast<unsigned int>(atImageDate.Width)
                            << "\tBitDepth: " << static_cast<unsigned int>(atImageDate.BitDepth)
                            << "\tColorType: " << static_cast<unsigned int>(atImageDate.ColorType)
                            << "\tChannels: " << static_cast<unsigned int>(atImageDate.Channels) << endl;
                    giShowType = 1;
                }else{
                    // �ָ��ṹ��
                    atImageDate.Width = 0;
                    atImageDate.Height = 0;
                    atImageDate.BitDepth = 0;
                    atImageDate.ColorType = 0;
                    atImageDate.Channels = 0;
                    atImageDate.ppImage = nullptr;
                    gpDxc->ReBase();
                }

            }
        }else{
            clogerr << "OpenIndexPngFile:PngAreOpenFail" << endl;
            gpPngCDate->ReStartPngLib();
            return false;
        }

    return true;
}


bool ClosePngFile()
{
    clogerr << "ClosePngFile" << endl;
    giShowType = -1;    // ����ʾ
    gpPngCDate->ReStartPngLib();
    // ���ŵ�����
    IMAGE &atImageDate = *gpImageDate;
    // �ָ��ṹ��
    atImageDate.Width = 0;
    atImageDate.Height = 0;
    atImageDate.BitDepth = 0;
    atImageDate.ColorType = 0;
    atImageDate.Channels = 0;
    atImageDate.ppImage = nullptr;
    gpDxc->ReBase();
    return true;
}


bool DrawAbout()
{
    HDC gdc = nullptr;
    if ( nullptr != ( gdc = GetDC(gHwnd) ) )
    {
        // ��Ч

        // ��ȡ�û�������
        RECT rect;
        GetClientRect( gHwnd, &rect);

        // ����
        rect.left += 20;
        rect.right += 20;
        string AboutString = gsHelpString
                                        + gsAboutString
                                        + "\n\nThe lib Copyright :\n"
                                        + "\n\n" + gpPngCDate->PngLibCopyright()
                                        + "\n\n" + gpPngCDate->PngZLibCopyright()
                                        ;
        DrawText( gdc, AboutString.c_str(), AboutString.size(), &rect, DT_LEFT);

        // �ͷ�
        ReleaseDC( gHwnd, gdc);

        // ʹ��Ч
        ValidateRect( gHwnd, &rect);
        return true;
    }
    clogerr << "DrawAboutFail." << endl;
    return false;
}






//HCURSOR hCursor = LoadAnimatedCursor(IDR_MYTYPE1, _T("MyType"));
//HCURSOR hCursor = LoadAnimatedCursor(IDR_MYTYPE1, "MyType");


//HCURSOR LoadAnimatedCursor(UINT nID, LPCTSTR pszResouceType)
// ���� ani ��̬����� ���ߺ���
HCURSOR LoadAnimatedCursor(UINT nID, LPCTSTR pszResouceType)
{
    HCURSOR hCursor = NULL;
//    HINSTANCE hInstance = AfxGetInstanceHandle();
    HINSTANCE hInstance = gHInstance;
    if (hInstance)
    {
        HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(nID), pszResouceType );
        DWORD dwResourceSize = SizeofResource(hInstance, hResource);
        if (dwResourceSize>0)
        {
            clogerr << "LoadResource" << endl;
            HGLOBAL hRsrcGlobal = LoadResource(hInstance, hResource);
            if (hRsrcGlobal)
            {
                clogerr << "LockResource" << endl;
                LPBYTE pResource = (LPBYTE)LockResource(hRsrcGlobal);
                if (pResource)
                {
                    clogerr << "CreateIconFromResource" << endl;
                    hCursor = (HCURSOR)CreateIconFromResource(pResource, dwResourceSize, FALSE, 0x00030000);
//                    UnlockResource(pResource);        // ����Ч��
                }
                FreeResource(hRsrcGlobal);
            }
        }else{
        clogerr << "dwResourceSize !> 0" << endl;
        }
    }
    return hCursor;
}

// �˳���Ϣ
bool ExitAPPMessage()
{
    // ���������һ��
    gpDxc->PrimaryHide();   // ֹͣ��ʾ
    PostQuitMessage (0);    // ���� WM_QUIT ��Ϣ�˳���������
    return true;
}




