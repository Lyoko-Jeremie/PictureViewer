#ifndef FILELIST_H
#define FILELIST_H

#include <string>
#include <vector>
using namespace std;

#include <windows.h>
#include <winbase.h>    // 核心API

class FileList
{
    public:
        FileList( string path);     // 文件搜索路径   包含掩码
        virtual ~FileList();
        bool ReFlash();
        string GetFileOnlyName( const unsigned int i);
        string GetFileFullPath( const unsigned int i);
        size_t GetSize();
    protected:
        bool AllReady;
        string MainPath;
        vector<string> MainFileList;
        HANDLE ReadHandle;
        WIN32_FIND_DATA ReadStruct;
        LPWIN32_FIND_DATA pReadStruct;
        bool CtorList();
    private:
};

#endif // FILELIST_H
