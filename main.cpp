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

/*  ��Ϣ�ص�����  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void PictrueLast();
void PictrueNext();
bool ReDrawing();
bool initial();
bool Shutdown();

/*  ������������ȫ�ֱ�����  */
char szClassName[ ] = "PictureViewer";

// ���ƽṹָ��
DirectXControl *glDxc = nullptr;

// ��������
const DWORD gwWidth = 800;
const DWORD gwHeight =  600;

// �˵����
HMENU ghMenuHandle = nullptr;
// �˵���Ӱ
bool gbMenuIsShow = false;
// ���ھ��
HWND gHwnd = nullptr;
// ������
HINSTANCE gHInstance = nullptr;
// ��ǰ��ʾ�ļ���
string gFileName;

// ����
typedef unsigned long DWORD;

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
        return -1;
    // ָ��ָ��
    glDxc = &dxc;
    initial();

    // ȷ��������32λɫ����      // ����ν�� ��֤ȫ��
//    if ( glDxc->GetPixelFormat() != 32 )
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
    glDxc = nullptr;
    // ����������Զ�����dxc�ֲ�����


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
            glDxc->PrimaryHide();
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

        case WM_KEYDOWN:
            if ( KEYDOWN( VK_ESCAPE ) )
            {
                // �����˳���Ϣ
                PostMessage( hwnd, WM_DESTROY, 0, 0);
                return 0;
            }
            if ( KEYDOWN( VK_LEFT ) )
            {
                // ���ͷ
                PictrueLast();
            }
            if ( KEYDOWN( VK_RIGHT ) )
            {
                // �Ҽ�ͷ
                PictrueNext();
            }
            // ��������
            break;

        case WM_SIZE:
            // ��С�ı�
            // ���»�ȡ����ָ��
//            clog << "WM_SIZE:PrimaryReFlash() " << endl;
//            clog << glDxc->PrimaryReFlash() << endl;
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
//                        glDxc->PrimaryHide();
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
//                        glDxc->PrimaryShow();
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

                case MENU_HELP_ID_ABOUT:
//                    MessageBox(
//                               hwnd,
//                               "Picture Viewer\n\n\nDesign by Jeremie\n\n\nPower by DirectX",
//                               "About Picture Viewer",
//                               MB_OK | MB_ICONEXCLAMATION
//                               );
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
    // ��������λ�������Ϊ32��
    clog << glDxc->GetPixelFormat() << endl;
    {
        RECT a = glDxc->GetMainWindowClientRect();
        clog << "left " << a.left << endl;
        clog << "right " << a.right << endl;
        clog << "top " << a.top << endl;
        clog << "bottom " << a.bottom << endl;
    }
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
    ReDrawing();
    return;
}

//��һ��ͼ
void PictrueNext()
{
    ReDrawing();
    glDxc->TestPaint();
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

    glDxc->TestPaint();

    return true;
}


// ��ʼ��
bool initial()
{
    // ������ʾģʽ
    glDxc->SetDisplayMode(
                          static_cast<DWORD> (800),
                          static_cast<DWORD> (600),
                          static_cast<DWORD> (32)
                          );
    glDxc->PrimaryShow();
    //TODO ���ļ�
    // ���� gFileName
    return true;
}

// ����
bool Shutdown()
{
    glDxc->PrimaryHide();
    return true;
}






