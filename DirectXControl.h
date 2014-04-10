#ifndef DIRECTXCONTROL_H
#define DIRECTXCONTROL_H

// ȷ�� DirectX ��GUID�ܹ�ʹ��
// ����IID_IDirectDraw7
#define INITGUID

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
        bool AreInitiSccess();
    protected:
    private:
        bool AllReady;
        LPDIRECTDRAW lpdd;
        LPDIRECTDRAW7 lpdd7;
        HWND MainWindowHandle;
};

#endif // DIRECTXCONTROL_H
