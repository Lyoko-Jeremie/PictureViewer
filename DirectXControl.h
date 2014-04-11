#ifndef DIRECTXCONTROL_H
#define DIRECTXCONTROL_H

// ȷ�� DirectX ��GUID�ܹ�ʹ��
// ����IID_IDirectDraw7
#define INITGUID

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
        DirectXControl( HWND hwnd );
        ~DirectXControl();
        // ����Ƿ��ʼ���ɹ�
        bool AreInitiSccess();
        // ������ʾģʽ
        bool SetDisplayMode( DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate, DWORD dwFlag );
    protected:
        // ׼����ɱ�־
        bool AllReady;
        // �豸���
        LPDIRECTDRAW lpdd;
        LPDIRECTDRAW7 lpdd7;
        // ���ھ��
        HWND MainWindowHandle;
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
