#ifndef DIRECTXCONTROL_H
#define DIRECTXCONTROL_H

// ȷ�� DirectX ��GUID�ܹ�ʹ��
// ����IID_IDirectDraw7
#define INITGUID
// ���߼���dxguid.lib��

// WIN SDK
#include <windows.h>
// ����ͷ�ļ�
#include <ddraw.h>
// �ǵ�����Ŀ����ѡ�������ӿ��ļ��ļ�ddraw.lib
// ��������Ŀͷ�ļ�����·�������Direct��������include�ļ���


// ���ڶ����� ֻ�Ƕ������DirectX�������а�װ����
class DirectXControl
{
    public:
        DirectXControl( HWND hwnd, bool IsFullScreen );
        ~DirectXControl();
        // ����Ƿ��ʼ���ɹ�
        bool AreInitiSccess();

        // ������ʾģʽ   Note:��ʾģʽ�ĸı�Ӱ��������Ļ�������ڴ���ģʽʹ�û�ʧЧ�����쳣��
        bool SetDisplayMode( DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate = 0, DWORD dwFlag = 0 );

        // ��ȡ���ظ�ʽ
        int GetPixelFormat();

        // �û�������
        RECT GetMainWindowClientRect();

        bool TestPaint( int type, unsigned int runtimes = 3);
        bool PaintImage( UCHAR **ppImage, unsigned int Wide, unsigned int Height, UCHAR BitDepth, UCHAR ColorType, UCHAR Channels );
        bool ClearScreen();

        bool PrimaryShow();

        bool PrimaryHide();

        bool PrimaryReFlash();

        bool AddBaseX( int i);
        bool AddBaseY( int i);
        bool ReBase();


    protected:

        inline void DDRAW_INIT_STRUCT( DDPIXELFORMAT &ddstruct);
        inline void DDRAW_INIT_STRUCT( DDSURFACEDESC2 &ddstruct);
        // ׼����ɱ�־
        bool AllReady;
        // ȫ����־
        bool FullScreen;
        // �豸���
//        LPDIRECTDRAW lpdd;
        LPDIRECTDRAW7 lpdd7;
        // ��ͼ����ṹ��
        DDSURFACEDESC2 ddsd;
        // ���ھ��
        HWND MainWindowHandle;
        // ��������
        RECT MainWindowRect;
        // �û�������
        RECT MainWindowClientRect;
        // �û���BPP�����ء�
        int MainWindowBPP;
        // ���Ʊ���
        LPDIRECTDRAWSURFACE7 lpddsprimary;
        // ͼ���ƫ��
        int BaseX;
        int BaseY;

        DWORD PureColor;

//        // ���ÿ��
//        DWORD dwWidth;
//        DWORD dwHeight;
//        // ����ɫ��
//        DWORD dwBPP;
//        // ˢ����
//        DWORD dwRefreshRate;
    private:
};

#endif // DIRECTXCONTROL_H
