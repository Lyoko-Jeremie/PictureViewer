#ifndef PNGCONTROL_H
#define PNGCONTROL_H

// 所用到的 clog 和 FILE
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

// png 解码库
#include <png.h>   // lpng1610
// 库帮助文件 http://www.libpng.org/pub/png/libpng-manual.txt

// 注意 在链接中要链接 libpng.a 和 libz.a


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

        // 检测是否初始化成功
        bool AreInitiSccess();

        bool AreOpenSccess();

        // 打开并读取文件
        bool OpenPngFile( string File);

        bool ReStartPngLib( unsigned int MaxHeighty, unsigned int MaxWidthx );
        bool ReStartPngLib();

        // row_pointers 总长度为
        // UCHAR 的 height * width * channels(RGB=3 , ARGB = 4) * bit_depth(24bit=8)
        PCppUCHAR GetPngPixelArray();

        PCUINT GetPngWidth();

        PCUINT GetPngHeight();

        PCUCHAR GetPngBitDepth();

        PCUCHAR GetPngColorType();

        // channels 通道数，这里可以知道基本格式
        // 返回值对应的通道类型
        // 1 (GRAY, PALETTE),
        // 2 (GRAY_ALPHA),
        // 3 (RGB),
        // 4 (RGB_ALPHA or RGB + filler byte)
        PCUCHAR GetPngChannels();

    protected:

        bool PngLibCreate();

        bool PngLibRelase();

        bool SetMaxHW( unsigned int MaxHeighty, unsigned int MaxWidthx );

        // 初始化成功标志
        bool AllReady;

        // 成功一半
        bool HalfReady;

        // Note: OpenOK 只能在 AllReady 为真时为真
        // 文件数据读取成功标志
        bool OpenOK;

        // png_ptr 和 info_ptr 都应该是 png 库的内部数据
        // 不应该直接读取  获取相关数据时尽量使用库自带的API
        png_structp png_ptr;
        png_infop info_ptr;

        FILE *p_png_file;

        // 最大可接受尺寸
        unsigned int MaxHeight;
        unsigned int MaxWidth;

    private:

};

#endif // PNGCONTROL_H
