#ifndef PNG_LIB_CONTROL
#define PNG_LIB_CONTROL

// ���õ��� clog �� FILE
#include <iostream>
#include <cstdio>
using namespace std;

// png �����
#include <png.h>   // lpng1610
// ������ļ� http://www.libpng.org/pub/png/libpng-manual.txt

// ע�� ��������Ҫ���� libpng.a �� libz.a

#define PNGERROR -1
#define PNGSUCCESS 0

// ��������
typedef unsigned char UCHAR;
typedef unsigned char **UCHAR_ARRARY;

int PngOpen( FILE &pFile, UCHAR_ARRARY *pDate )
{
    return PNGSUCCESS;
}


#endif // PNG_LIB_CONTROL



