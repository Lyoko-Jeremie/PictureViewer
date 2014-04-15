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


typedef unsigned char UCHAR;
typedef UCHAR *pUCHAR;
typedef pUCHAR *ppUCHAR;


class PngControl
{
    public:

        PngControl( unsigned int MaxHeighty, unsigned int MaxWidthx );
        ~PngControl();

        // 检测是否初始化成功
        bool AreInitiSccess();

        bool AreOpenSccess();

        // 打开并读取文件
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
//        // channels 通道数，这里可以知道基本格式
//        // 返回值对应的通道类型
//        // 1 (GRAY, PALETTE),
//        // 2 (GRAY_ALPHA),
//        // 3 (RGB),
//        // 4 (RGB_ALPHA or RGB + filler byte)
//        png_byte channels = png_get_channels(png_ptr, info_ptr);

    protected:

        bool PngLibCreate();

        bool PngLibRelase();

        bool SetMaxHW( unsigned int MaxHeighty, unsigned int MaxWidthx );

        // 初始化成功标志
        bool AllReady;
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
