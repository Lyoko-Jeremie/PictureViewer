#include "FileList.h"

#include <string>
#include <vector>
//#include <iostream>
#include <stdexcept>
using namespace std;
// 测试用输出包装类
#include "CLogErr.h"
using namespace CLogErr::clog;

#include <windows.h>
#include <winbase.h>    // 核心API

FileList::FileList( string path ):
    AllReady(false),
    MainPath(path),
    MainFileList(),
    ReadHandle(nullptr),
    ReadStruct(),
    pReadStruct(&ReadStruct)
{
    this->CtorList();
}

FileList::~FileList()
{
    this->AllReady = false;
}

bool FileList::ReFlash()
{
    this->MainFileList.clear();
    return this->CtorList();
}

string FileList::GetFileOnlyName(const unsigned int i)
{
    if ( i < this->GetSize() )
    {
        clogerr << "PngListSize: " << this->GetSize() << endl;
        return this->MainFileList.at(i);
    }
    clogerr << "PngListSize: " << this->GetSize() << " < " << i << endl;
    throw out_of_range("FileList:GetFileOnlyName:OutOfRange: " + i );
    return "";
}

string FileList::GetFileFullPath(const unsigned int i)
{
    return this->MainPath + this->GetFileOnlyName(i);
}

size_t FileList::GetSize()
{
    return this->MainFileList.size();
}


bool FileList::CtorList()
{
    this->ReadHandle = FindFirstFile( this->MainPath.c_str(), this->pReadStruct);
    if ( INVALID_HANDLE_VALUE == this->ReadHandle )
    {
        clogerr << "FileList:ReFlash:ReadHandle:INVALID_HANDLE_VALUE{MayBeNotFindOrPathWrong}" << endl;
        this->pReadStruct = nullptr;
        this->AllReady = false;
        return false;
    }
    do{
        int t = GetLastError();
        if ( ERROR_FILE_NOT_FOUND == t )
        {
            clogerr << "FileList:ReFlash:ERROR_FILE_NOT_FOUND" << endl;
        }
        if (  !(this->ReadStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  )
        {
            // 不是文件夹
            clogerr << this->ReadStruct.cFileName << endl;
            this->MainFileList.push_back( this->ReadStruct.cFileName );
        }

    }while ( FindNextFile( this->ReadHandle, this->pReadStruct ) );

    if ( !FindClose(this->ReadHandle) )
    {
        clogerr << "CloseFail" << endl;
        return false;
    }
    this->ReadHandle = nullptr;
    this->AllReady = true;
    return true;
}







