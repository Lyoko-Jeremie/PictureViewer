// WIN SDK
#include <windows.h>
#include <windowsx.h>
// C++String
#include <string>
#include <iostream>
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

/*  ��Ϣ�ص�����  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void PictrueLast();
void PictrueNext();
bool ReDrawing();
bool initial();
bool Shutdown();
bool OpenIndexPngFile( unsigned int Index);
bool ClosePngFile();

/*  ������������ȫ�ֱ�����  */
char szClassName[ ] = "PictureViewer";

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
// Demo����
int giDemoType = 0;


// �˵����
HMENU ghMenuHandle = nullptr;
// �˵���Ӱ
bool gbMenuIsShow = false;
// ���ھ��
HWND gHwnd = nullptr;
// ������
HINSTANCE gHInstance = nullptr;

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
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);         // ���ָ��
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
        clog << "DirectXControl Create Fail." << endl;
        return -1;
    }
    // ָ��ָ��
    gpDxc = &dxc;

    // ��ʼ���ļ��б�
    FileList FileListDate(".\\png\\*.png");
    if ( !FileListDate.GetSize() )
    {
        clog << "No Png File." << endl;
        giShowType = 0;
//        return -1;
    }
    gpFileListDate = &FileListDate;

    // ��ʼ��ͼ��ṹ�����
    IMAGE ImageDate;
    gpImageDate = &ImageDate;

    // ��ʼ��Png��
    PngControl PngCDate( 4000, 2000);   // ��Լһ����Ļ������
    if ( !PngCDate.AreInitiSccess() )
    {
        clog << "Png Lib Initial Fail." << endl;
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
        /**< CPU�������ӳ� */
        Sleep(30);
        ReDrawing();
    }

    Shutdown();
    // �ͷ�
    gpDxc = nullptr;
    // ����������Զ�����dxc�ֲ�����
    gpFileListDate = nullptr;


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
            // ���������һ��
            gpDxc->PrimaryHide();
            PostQuitMessage (0);    // ���� WM_QUIT ��Ϣ�˳���������
            break;

        case WM_PAINT:
            // �ػ�
            {
//                PAINTSTRUCT ps;
                // ����� ���ñ�����ˢ�ػ���������
                // hdc Ϊͼ�λ������
//                HDC hdc = BeginPaint( hwnd, &ps);
                // �ػ����
//                EndPaint( hwnd, &ps);
            }
            break;

        // �����Ϣ
        // �����Ϣ������������ڴ����û���
        case WM_MOUSEMOVE:
            // ����ƶ�����Ϣ
            // x= static_cast<int> LOWORD(lParam)
            // y= static_cast<int> HIWORD(lParam)
            // ����= static_cast<int> LOWORD(wParam)
            if ( (static_cast<int> HIWORD(lParam) < 20) && ( !gbMenuIsShow ) ) // ��ʱ����ⴰ��
            {
                gbMenuIsShow = SetMenu( hwnd, ghMenuHandle);
            }
            if ( (static_cast<int> HIWORD(lParam) > 20) && ( gbMenuIsShow ) )
            {
                gbMenuIsShow = !SetMenu( hwnd, nullptr);
            }
            break;

//        case WM_KEYDOWN:
//            // ���ﲻ�������鷳
//            // ��������
//            break;

        case WM_SIZE:
            // ��С�ı�
            // ���»�ȡ����ָ��
//            clog << "WM_SIZE:PrimaryReFlash() " << endl;
//            clog << gpDxc->PrimaryReFlash() << endl;
            // Windows��Ϣ����������ڶ���...�������.FuckYouMother!1

            break;

        case WM_MOVE:
            // �����ƶ�
            // TODO ��Ӧmove �������û�����ͼ
            break;

//        case WM_ACTIVATE:     // ���ٴ���״̬��
            {   // ���ڻ״̬�ı�
                /**< P:˭��������    reinterpret_cast<HWND> lParam  Note:�ײ�ת������Ҫ������  */
                if ( gHwnd == reinterpret_cast<HWND> (lParam) )
                {   // ��������
                    if ( WA_INACTIVE==LOWORD(wParam) )
                    {
//                        clog << "WM_ACTIVATE:PrimaryHide()" << endl;
//                        gpDxc->PrimaryHide();
                        /**< ȡ������ */
                        /**< ������С�� */
                        if ( static_cast<bool> ( HIWORD(wParam) ) )
                        {
                            /**< ��С���� */
                        }
                        else
                        {
                            /**< ֻ��ʧȥ���� */
                        }
                    }
                    else
                    {
//                        clog << "WM_ACTIVATE:PrimaryShow()" << endl;
//                        gpDxc->PrimaryShow();
                        /**< ������ */
                        /**< ���Լ��ʽ */
                        if ( WA_ACTIVE==LOWORD(wParam))
                        {
                            /**< ����꼤�� */
                        }
                        else
                        {
                            if (WA_CLICKACTIVE==LOWORD(wParam))
                            {
                                /**< ����꼤�� */
                            }
                        }
                    }
                }
            }
            break;


        case WM_CREATE:
            // ���ڴ���
            break;

        case WM_COMMAND:
            // �˵���Ϣ
            {
                // ��������Ĳ˵���Ϣ
                switch ( LOWORD( wParam ) )
                {
                case MENU_FILE_ID_OPEN:
                    break;

                case MENU_FILE_ID_CLOSE:
                    break;

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
                    break;

                case MENU_DEMO_ID_RAND_COLOER_LINE:
                    break;

                case MENU_HELP_ID_ABOUT:
//                    MessageBox(
//                               hwnd,
//                               "Picture Viewer\n\n\nDesign by Jeremie\n\n\nPower by DirectX",
//                               "About Picture Viewer",
//                               MB_OK | MB_ICONEXCLAMATION
//                               );
                    // TODO ���� �� ��ͼ����

                    // png : png_get_copyright(NULL)

                    // A "png_get_copyright" function is available, for convenient use in "about"
                    // boxes and the like:
                    //
                    //     printf("%s", png_get_copyright(NULL));
                    //
                    // Also, the PNG logo (in PNG format, of course) is supplied in the
                    // files "pngbar.png" and "pngbar.jpg (88x31) and "pngnow.png" (98x31).

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
/*  Test
//    // ��������λ�������Ϊ32��
//    clog << gpDxc->GetPixelFormat() << endl;
//    {
//        RECT a = gpDxc->GetMainWindowClientRect();
//        clog << "left " << a.left << endl;
//        clog << "right " << a.right << endl;
//        clog << "top " << a.top << endl;
//        clog << "bottom " << a.bottom << endl;
//    }
//    {   // Test
//        RECT MainWindowRect;
//        RECT MainWindowClientRect;
//        GetWindowRect( gHwnd, &(MainWindowRect) );
//        AdjustWindowRect(
//            &(MainWindowClientRect),
//            GetWindowStyle(gHwnd),
//            GetMenu(gHwnd) != nullptr
//            );
//        cout << endl;
//        cout << "left\t" << MainWindowRect.left << endl;
//        cout << "right\t" << MainWindowRect.right << endl;
//        cout << "top\t" << MainWindowRect.top << endl;
//        cout << "bottom\t" << MainWindowRect.bottom << endl;
//        cout << endl;
//        cout << "left\t" << MainWindowClientRect.left << endl;          // -3
//        cout << "right\t" << MainWindowClientRect.right << endl;         // 3
//        cout << "top\t" << MainWindowClientRect.top << endl;           // -44 -25
//        cout << "bottom\t" << MainWindowClientRect.bottom << endl;    // 3
//        cout << endl;
//        cout << endl;
//
////        ��ʵ�����û�������Ϊ��
////        RL - CRL
////        RR - CRR
////        RT - CRT
////        RB - CRB
//
//    }
*/
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
    // for now test if user is hitting ESC and send WM_CLOSE
    if (KEYDOWN(VK_ESCAPE))
    {
        PostMessage(gHwnd,WM_CLOSE,0,0);
    } // end if
    if ( KEYDOWN( VK_ESCAPE ) )
    {
        // �����˳���Ϣ
        PostMessage( gHwnd, WM_DESTROY, 0, 0);
        return 0;
    }
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
    DrawObject( giShowType, gpImageDate, giDemoType);
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
        }
        else
        {
            // ��ʧ�ܣ���ʾDemo
            giShowType = 0;
        }

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
        gpDxc->TestPaint(demo);
        return true;
    }
    if ( 1 == object )
    {
        if ( KEYDOWN( VK_LEFT ) )
        {
            gpDxc->SetBaseX( gpDxc->GetBaseX() - 5 );
            // ���ͷ
        }
        if ( KEYDOWN( VK_RIGHT ) )
        {
            gpDxc->SetBaseX( gpDxc->GetBaseX() + 5 );
            // �Ҽ�ͷ
        }
        if ( KEYDOWN( VK_UP ) )
        {
            gpDxc->SetBaseY( gpDxc->GetBaseY() - 5 );
            // �ϼ�ͷ
        }
        if ( KEYDOWN( VK_DOWN ) )
        {
            gpDxc->SetBaseY( gpDxc->GetBaseY() + 5 );
            // �¼�ͷ
        }
        if ( VK_HOME )
        {
            gpDxc->SetBaseX( 0 );
            gpDxc->SetBaseY( 0 );
            // Home
        }
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
                    atImageDate.Height
                    &&
                    atImageDate.Width
                    )
                {
                    // ȫ��Ϊ0
                    // UCHAR �� height * width * channels(RGB=3 , ARGB = 4) * bit_depth(24bit=8)
                    clog << "OpenIndexPngFile:PngAreOpen" << endl;
                    clog << "\nWidth: " << atImageDate.Width
                            << "\tBitDepth: " << atImageDate.BitDepth
                            << "\tColorType: " << atImageDate.ColorType
                            << "\tChannels: " << atImageDate.Channels << endl;
                    giShowType = 1;
                }else{
                    // �ָ��ṹ��
                    atImageDate.Width = 0;
                    atImageDate.Height = 0;
                    atImageDate.BitDepth = 0;
                    atImageDate.ColorType = 0;
                    atImageDate.Channels = 0;
                    atImageDate.ppImage = nullptr;
                    gpDxc->SetBaseX( 0 );
                    gpDxc->SetBaseY( 0 );
                }

            }
        }else{
            clog << "OpenIndexPngFile:PngAreOpenFail" << endl;
            gpPngCDate->ReStartPngLib();
        }

    return true;
}


bool ClosePngFile()
{
    clog << "ClosePngFile" << endl;
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
    gpDxc->SetBaseX( 0 );
    gpDxc->SetBaseY( 0 );
    return true;
}



