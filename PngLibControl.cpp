#ifndef PNG_LIB_CONTROL
#define PNG_LIB_CONTROL

// 所用到的 clog 和 FILE
#include <iostream>
#include <cstdio>
using namespace std;

// png 解码库
#include <png.h>   // lpng1610
// 库帮助文件 http://www.libpng.org/pub/png/libpng-manual.txt

// 注意 在链接中要链接 libpng.a 和 libz.a

#define PNGERROR -1
#define PNGSUCCESS 0

// 数据类型
typedef unsigned char UCHAR;
typedef unsigned char **UCHAR_ARRARY;

int PngOpen( FILE &pFile, UCHAR_ARRARY *pDate )
{
    return PNGSUCCESS;
}


#endif // PNG_LIB_CONTROL



