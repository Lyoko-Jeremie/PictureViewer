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


typedef unsigned char UCHAR;
typedef UCHAR *pUCHAR;
typedef pUCHAR *ppUCHAR;


class PngControl
{
    public:

        PngControl( unsigned int MaxHeighty, unsigned int MaxWidthx );
        ~PngControl();

        // ����Ƿ��ʼ���ɹ�
        bool AreInitiSccess();

        bool AreOpenSccess();

        // �򿪲���ȡ�ļ�
        bool OpenPngFile( string File);

        bool ReStartPngLib( unsigned int MaxHeighty, unsigned int MaxWidthx );
        bool ReStartPngLib();

        ppUCHAR GetPngPixelArray();

//        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
//
//        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
//
//        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
//
//        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
//
//        // channels ͨ�������������֪��������ʽ
//        // ����ֵ��Ӧ��ͨ������
//        // 1 (GRAY, PALETTE),
//        // 2 (GRAY_ALPHA),
//        // 3 (RGB),
//        // 4 (RGB_ALPHA or RGB + filler byte)
//        png_byte channels = png_get_channels(png_ptr, info_ptr);

    protected:

        bool PngLibCreate();

        bool PngLibRelase();

        bool SetMaxHW( unsigned int MaxHeighty, unsigned int MaxWidthx );

        // ��ʼ���ɹ���־
        bool AllReady;
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
