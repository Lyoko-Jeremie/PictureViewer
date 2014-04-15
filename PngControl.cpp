#include "PngControl.h"

// ���õ��� clog �� FILE
#include <iostream>
#include <cstdio>
using namespace std;

// png �����
#include <png.h>   // lpng1610
// ������ļ� http://www.libpng.org/pub/png/libpng-manual.txt

// ע�� ��������Ҫ���� libpng.a �� libz.a



// Note: ÿ��������Ҫsetjmpһ�£�Ȩ��try


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

// ���ع�����
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
    // �쳣����
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        // �쳣

        this->PngLibRelase();

        clog << "PngControl:OpenPngFile:PnglibFail" << endl;

        return false;
    }else{
        // ����

        // �½��ļ�ָ��
        // �ļ�ָ��Ҫ���Զ����ƶ���ʽ��
        this->p_png_file = fopen( "a.png", "rb");
        if ( !(this->p_png_file) )
        {
            // ��ʧ��
            // png_destroy_read_struct(& this->png_ptr, & this->info_ptr, nullptr);
            clog << "PngControl:OpenPngFile:FileOpenFail" << endl;
            return false;
        }

        // �����ļ���Դ��
        png_init_io( png_ptr, p_png_file);

        // ��ȡ
        png_read_png (
            png_ptr,
            info_ptr,
            PNG_TRANSFORM_SWAP_ALPHA | PNG_TRANSFORM_GRAY_TO_RGB,
            nullptr
            );

        // ���ǵùر��ļ�
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

    // png_ptr �� info_ptr ��Ӧ���� png ����ڲ�����
    // ��Ӧ��ֱ�Ӷ�ȡ  ��ȡ�������ʱ����ʹ�ÿ��Դ���API

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

    // �ͷ�
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
        // �쳣

        this->PngLibRelase();

        clog << "PngControl:SetMaxHW:PngSetUserLimits:PnglibFail" << endl;

        return false;
    }else{
        // ����
        png_set_user_limits ( png_ptr, MaxWidth, MaxHeight);
    }

    return true;
}




