#include "PngControl.h"

// 所用到的 clog 和 FILE
#include <iostream>
#include <cstdio>
using namespace std;

// png 解码库
#include <png.h>   // lpng1610
// 库帮助文件 http://www.libpng.org/pub/png/libpng-manual.txt

// 注意 在链接中要链接 libpng.a 和 libz.a



// Note: 每个函数都要setjmp一下，权作try


PngControl::PngControl( unsigned int MaxHeighty, unsigned int MaxWidthx ):
    AllReady(false),
    OpenOK(false),
    png_ptr(nullptr),
    info_ptr(nullptr),
    p_png_file(nullptr),
    MaxHeight(MaxHeighty),
    MaxWidth(MaxWidthx)
{

    if ( !this->PngLibCreate() )
    {
        AllReady = false;
        return;
    }

    if ( !this->SetMaxHW( MaxHeight, MaxWidth) )
    {
        AllReady = false;
        return;
    }

    AllReady = true;
    return;
}

PngControl::~PngControl()
{
    this->PngLibRelase();
}

// 返回构造结果
bool PngControl::AreInitiSccess()
{
    return this->AllReady;
}

bool PngControl::AreOpenSccess()
{
    return this->OpenOK;
}


bool PngControl::OpenPngFile(string File)
{
    if ( AreInitiSccess() && !AreOpenSccess() )
    {
        clog << "PngControl:OpenPngFile:PremiseFailed" << endl;
        return false;
    }
    // 异常处理
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        // 异常

        this->PngLibRelase();

        clog << "PngControl:OpenPngFile:PnglibFail" << endl;

        return false;
    }else{
        // 正常

        // 新建文件指针
        // 文件指针要求以二进制读方式打开
        this->p_png_file = fopen( "a.png", "rb");
        if ( !(this->p_png_file) )
        {
            // 打开失败
            // png_destroy_read_struct(& this->png_ptr, & this->info_ptr, nullptr);
            clog << "PngControl:OpenPngFile:FileOpenFail" << endl;
            return false;
        }

        // 设置文件来源流
        png_init_io( png_ptr, p_png_file);

        // 读取
        png_read_png (
            png_ptr,
            info_ptr,
            PNG_TRANSFORM_SWAP_ALPHA | PNG_TRANSFORM_GRAY_TO_RGB,
            nullptr
            );

        // 最后记得关闭文件
        fclose( this->p_png_file );
        this->p_png_file = nullptr;

    }
    OpenOK = true;
    return true;
}


ppUCHAR PngControl::GetPngPixelArray()
{
    if ( !AreInitiSccess() || !AreOpenSccess() )
    {
        clog << "PngControl:GetPngPixelArray:PremiseFailed" << endl;
        return nullptr;
    }
    // png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    return  png_get_rows(png_ptr, info_ptr);
}




bool PngControl::ReStartPngLib(unsigned int MaxHeighty, unsigned int MaxWidthx)
{
    if ( !this->SetMaxHW( MaxHeighty, MaxWidthx ) )
    {
        return false;
    }
    if ( !this->ReStartPngLib() )
    {
        return false;
    }
    return true;
}


bool PngControl::ReStartPngLib()
{
    this->PngLibRelase();
    return this->PngLibCreate();
}


bool PngControl::PngLibCreate()
{
    if ( AreInitiSccess() || AreOpenSccess() )
    {
        clog << "PngControl:PngLibCreate:PremiseFailed" << endl;
        return false;
    }

    // png_ptr 和 info_ptr 都应该是 png 库的内部数据
    // 不应该直接读取  获取相关数据时尽量使用库自带的API

    png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if ( png_ptr == nullptr )
    {
        clog << "PngControl:PngLibCreate:CreateReadStructFail" << endl;
        return false;
    }


    info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr == nullptr)
    {
//        png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
        png_destroy_read_struct(& this->png_ptr, nullptr, nullptr);
        clog << "PngControl:PngLibCreate:CreateInfoStructFail" << endl;
        png_ptr = nullptr;
        return false;
    }

    AllReady = true;

    return true;
}

bool PngControl::PngLibRelase()
{

    // 释放
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    this->png_ptr = nullptr;
    this->info_ptr = nullptr;
    AllReady = false;
    OpenOK = false;

    return true;
}

bool PngControl::SetMaxHW( unsigned int MaxHeighty, unsigned int MaxWidthx )
{
    if ( AreInitiSccess() && !AreOpenSccess() )
    {
        clog << "PngControl:OpenPngFile:PremiseFailed" << endl;
        return false;
    }

    this->MaxHeight = MaxHeighty;
    this->MaxWidth = MaxWidthx;

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        // 异常

        this->PngLibRelase();

        clog << "PngControl:SetMaxHW:PngSetUserLimits:PnglibFail" << endl;

        return false;
    }else{
        // 正常
        png_set_user_limits ( png_ptr, MaxWidth, MaxHeight);
    }

    return true;
}




