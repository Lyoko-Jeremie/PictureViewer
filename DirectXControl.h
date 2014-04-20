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
        bool PaintImage(
                        UCHAR **ppImage,
                        unsigned int Wide,
                        unsigned int Height,
                        UCHAR BitDepth,
                        UCHAR ColorType,
                        UCHAR Channels,
                        bool AutoBackGroudColor
                        );
        bool ClearScreen();

        bool PrimaryShow();

        bool PrimaryHide();

        bool PrimaryReFlash();

        bool AddBaseX( int i);
        bool AddBaseY( int i);
        bool ReBase();
        bool ChangeBackGroudColor();
        DWORD GetBackGroudColor();
        bool SetBackGroudColor( UCHAR Red, UCHAR Green, UCHAR Blue, UCHAR Alpha = 0 );


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
        // �󱸱���
        LPDIRECTDRAWSURFACE7 lpddsback;
        // ͼ���ƫ��
        int BaseX;
        int BaseY;

        // ��ɫ DWORD ��
        DWORD PureColorD;
        // ��ɫ ARGB ��        Note:����Ӧ��ͬ��
        struct ARGB{
            UCHAR Alpha;
            UCHAR Red;
            UCHAR Green;
            UCHAR Blue;
            ARGB():Alpha(0),Red(0),Green(0),Blue(0) {}
            ARGB( const UCHAR &alpha, const UCHAR &red, const UCHAR &green, const UCHAR &blue ):
                        Alpha(alpha),Red(red),Green(green),Blue(blue) {}
            ARGB( const DWORD &s ):Alpha(0),Red(0),Green(0),Blue(0)
            {
                this->Blue = static_cast<UCHAR>( s & 0xff );
                this->Green = static_cast<UCHAR>( ( s >> 8 ) & 0xff );
                this->Red = static_cast<UCHAR>( ( s >> 16 ) & 0xff );
                this->Alpha = static_cast<UCHAR>( ( s >> 24 ) & 0xff );
            }
            ARGB( const ARGB &s ):
                Alpha(s.Alpha),Red(s.Red),Green(s.Green),Blue(s.Blue){}
            DWORD to_ARGB32()
            {
                return ((Blue) + ((Green) << 8) + ((Red) << 16) + ((Alpha) << 24));
            }
        } PureColorU;

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
