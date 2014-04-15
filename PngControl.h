#ifndef PNGCONTROL_H
#define PNGCONTROL_H

// ���õ��� clog �� FILE
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

// png �����
#include <png.h>   // lpng1610
// ������ļ� http://www.libpng.org/pub/png/libpng-manual.txt

// ע�� ��������Ҫ���� libpng.a �� libz.a


typedef unsigned char PCUCHAR;
typedef PCUCHAR *PCpUCHAR;
typedef PCpUCHAR *PCppUCHAR;

typedef unsigned int PCUINT;


class PngControl
{
    public:

        PngControl( unsigned int MaxHeighty, unsigned int MaxWidthx );
        PngControl();
        ~PngControl();

        // ����Ƿ��ʼ���ɹ�
        bool AreInitiSccess();

        bool AreOpenSccess();

        // �򿪲���ȡ�ļ�
        bool OpenPngFile( string File);

        bool ReStartPngLib( unsigned int MaxHeighty, unsigned int MaxWidthx );
        bool ReStartPngLib();

        // row_pointers �ܳ���Ϊ
        // UCHAR �� height * width * channels(RGB=3 , ARGB = 4) * bit_depth(24bit=8)
        PCppUCHAR GetPngPixelArray();

        PCUINT GetPngWidth();

        PCUINT GetPngHeight();

        PCUCHAR GetPngBitDepth();

        PCUCHAR GetPngColorType();

        // channels ͨ�������������֪��������ʽ
        // ����ֵ��Ӧ��ͨ������
        // 1 (GRAY, PALETTE),
        // 2 (GRAY_ALPHA),
        // 3 (RGB),
        // 4 (RGB_ALPHA or RGB + filler byte)
        PCUCHAR GetPngChannels();

    protected:

        bool PngLibCreate();

        bool PngLibRelase();

        bool SetMaxHW( unsigned int MaxHeighty, unsigned int MaxWidthx );

        // ��ʼ���ɹ���־
        bool AllReady;

        // �ɹ�һ��
        bool HalfReady;

        // Note: OpenOK ֻ���� AllReady Ϊ��ʱΪ��
        // �ļ����ݶ�ȡ�ɹ���־
        bool OpenOK;

        // png_ptr �� info_ptr ��Ӧ���� png ����ڲ�����
        // ��Ӧ��ֱ�Ӷ�ȡ  ��ȡ�������ʱ����ʹ�ÿ��Դ���API
        png_structp png_ptr;
        png_infop info_ptr;

        FILE *p_png_file;

        // ���ɽ��ܳߴ�
        unsigned int MaxHeight;
        unsigned int MaxWidth;

    private:

};

#endif // PNGCONTROL_H
