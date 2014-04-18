#include "PngControl.h"

// ���õ��� clogerr �� FILE
//#include <iostream>
#include <cstdio>
using namespace std;
// �����������װ��
#include "CLogErr.h"
using namespace CLogErr::clog;

// png �����
#include <png.h>   // lpng1610
// ������ļ� http://www.libpng.org/pub/png/libpng-manual.txt

// ע�� ��������Ҫ���� libpng.a �� libz.a



// Note: ÿ��������Ҫsetjmpһ�£�Ȩ��try

PngControl::PngControl():
    AllReady(false),
    OpenOK(false),
    png_ptr(nullptr),
    info_ptr(nullptr),
    p_png_file(nullptr),
    MaxHeight(0),
    MaxWidth(0)
{
    if ( !this->PngLibCreate() )
    {
        AllReady = false;
        return;
    }
    AllReady = true;
    return;
}


PngControl::PngControl( unsigned int MaxHeighty, unsigned int MaxWidthx ):
    AllReady(false),
    HalfReady(false),
    OpenOK(false),
    png_ptr(nullptr),
    info_ptr(nullptr),
    p_png_file(nullptr),
    MaxHeight(MaxHeighty),
    MaxWidth(MaxWidthx)
{

    if ( !this->PngLibCreate() )
    {
        clogerr << "PngControl:ctor:PngLibCreateFailed" << endl;
        AllReady = false;
        return;
    }

    HalfReady = true;

    if ( !this->SetMaxHW( MaxHeight, MaxWidth) )
    {
        clogerr << "PngControl:ctor:SetMaxHWFailed" << endl;
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
    if ( !AreInitiSccess() || AreOpenSccess() )
    {
        clogerr << "PngControl:OpenPngFile:PremiseFailed"
                << "\n!AreInitiSccess: " << !AreInitiSccess()
                << "\tAreOpenSccess: " << AreOpenSccess() << endl;
        return false;
    }
    // �쳣����
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        // �쳣

        this->PngLibRelase();

        clogerr << "PngControl:OpenPngFile:PnglibFail" << endl;

        return false;
    }else{
        // ����

        // �½��ļ�ָ��
        // �ļ�ָ��Ҫ���Զ����ƶ���ʽ��
        this->p_png_file = fopen( File.c_str(), "rb");
        if ( !(this->p_png_file) )
        {
            // ��ʧ��
            // png_destroy_read_struct(& this->png_ptr, & this->info_ptr, nullptr);
            clogerr << "PngControl:OpenPngFile:FileOpenFail" << endl;
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


PCppUCHAR PngControl::GetPngPixelArray()
{
    if ( !AreInitiSccess() || !AreOpenSccess() )
    {
        clogerr << "PngControl:GetPngPixelArray:PremiseFailed" << endl;
        return nullptr;
    }
    // png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    return  png_get_rows(png_ptr, info_ptr);
}




bool PngControl::ReStartPngLib(unsigned int MaxHeighty, unsigned int MaxWidthx)
{
    if ( !this->ReStartPngLib() )
    {
        return false;
    }
    HalfReady = true;
    if ( !this->SetMaxHW( MaxHeighty, MaxWidthx ) )
    {
        return false;
    }
    return true;
}


bool PngControl::ReStartPngLib()
{
    clogerr << "PngControl:ReStartPngLib" << endl;
    this->PngLibRelase();
    return this->PngLibCreate();
}


bool PngControl::PngLibCreate()
{
    if ( AreInitiSccess() || AreOpenSccess() )
    {
        clogerr << "PngControl:PngLibCreate:PremiseFailed" << endl;
        return false;
    }

    // png_ptr �� info_ptr ��Ӧ���� png ����ڲ�����
    // ��Ӧ��ֱ�Ӷ�ȡ  ��ȡ�������ʱ����ʹ�ÿ��Դ���API

    png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if ( png_ptr == nullptr )
    {
        clogerr << "PngControl:PngLibCreate:CreateReadStructFail" << endl;
        return false;
    }


    info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr == nullptr)
    {
//        png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
        png_destroy_read_struct(& this->png_ptr, nullptr, nullptr);
        clogerr << "PngControl:PngLibCreate:CreateInfoStructFail" << endl;
        png_ptr = nullptr;
        return false;
    }

    AllReady = true;
    HalfReady = true;

    clogerr << "PngControl:PngLibCreate:Create" << endl;

    return true;
}

bool PngControl::PngLibRelase()
{

    clogerr << "PngControl:PngLibRelase:Relase" << endl;

    // �ͷ�
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    this->png_ptr = nullptr;
    this->info_ptr = nullptr;
    AllReady = false;
    HalfReady = false;
    OpenOK = false;

    return true;
}

bool PngControl::SetMaxHW( unsigned int MaxHeighty, unsigned int MaxWidthx )
{
    if ( !this->HalfReady && !AreOpenSccess() )
    {
        clogerr << "PngControl:SetMaxHW:PremiseFailed" << endl;
        return false;
    }

    this->MaxHeight = MaxHeighty;
    this->MaxWidth = MaxWidthx;

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        // �쳣

        this->PngLibRelase();

        clogerr << "PngControl:SetMaxHW:PngSetUserLimits:PnglibFail" << endl;

        return false;
    }else{
        // ����
        png_set_user_limits ( png_ptr, MaxWidth, MaxHeight);
    }

    return true;
}



PCUINT PngControl::GetPngWidth()
{
    if ( !AreInitiSccess() || !AreOpenSccess() )
    {
        clogerr << "PngControl:GetPngWidth:PremiseFailed" << endl;
        return 0;
    }
    return png_get_image_width(png_ptr, info_ptr);
}

PCUINT PngControl::GetPngHeight()
{
    if ( !AreInitiSccess() || !AreOpenSccess() )
    {
        clogerr << "PngControl:GetPngHeight:PremiseFailed" << endl;
        return 0;
    }
    return png_get_image_height(png_ptr, info_ptr);
}

PCUCHAR PngControl::GetPngBitDepth()
{
    if ( !AreInitiSccess() || !AreOpenSccess() )
    {
        clogerr << "PngControl:GetPngBitDepth:PremiseFailed" << endl;
        return 0;
    }
    return png_get_bit_depth(png_ptr, info_ptr);
}

PCUCHAR PngControl::GetPngColorType()
{
    if ( !AreInitiSccess() || !AreOpenSccess() )
    {
        clogerr << "PngControl:GetPngColorType:PremiseFailed" << endl;
        return 0;
    }
    return png_get_color_type(png_ptr, info_ptr);
}

PCUCHAR PngControl::GetPngChannels()
{
    if ( !AreInitiSccess() || !AreOpenSccess() )
    {
        clogerr << "PngControl:GetPngChannels:PremiseFailed" << endl;
        return 0;
    }
    return png_get_channels(png_ptr, info_ptr);
}

