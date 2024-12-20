/********************************************************************
*
* Description:  implementation for StormLib - linux port
*       intended to be used in GLdiablo
*       
*   ---->   StormLib was originally developed for Windows by
*       Ladislav Zezula (www.zezula.net), and he did
*       a _great_ job! Thanks Ladislav!
*
*   this is currently a quick and dirty hack to get it working
*    don't expect beauty and/or miracles :)
*
*   these are function wraps to execute Windows API calls
*        as native Macintosh file calls (open/close/read/write/...)
*
*   continue you work: added some wrapping functions for GNU/Linux  by XPinguin
*
* Author: Marko Friedemann <marko.friedemann@bmx-chemnitz.de>
* Created at: Mon Jan 29 19:01:37 CEST 2001
* Computer: whiplash.flachland-chemnitz.de 
* System: Linux 2.4.0 on i686
*    
* Copyright (c) 2001 BMX-Chemnitz.DE All rights reserved.
*
********************************************************************/

#ifndef _WIN32
#include "StormLib.h"

int globalerr;

// time_t is number of seconds since 1.1.1970, UTC.
// 1 second = 10000000 (decimal) in FILETIME
static void ConvertTimeTToFileTime(LPFILETIME pFT, time_t crt_time)
{
    LARGE_INTEGER ft;
    LARGE_INTEGER seconds;

    // Set the start to 1.1.1970 00:00:00
    ft.HighPart = 0x019DB1DE;
    ft.LowPart = 0xD53E8000;

    // Add number of seconds
    seconds.QuadPart = 10000000;
    seconds.QuadPart *= crt_time;
    ft.QuadPart += seconds.QuadPart;

    // Return the result FILETIME
    pFT->dwHighDateTime = ft.HighPart;
    pFT->dwLowDateTime = ft.LowPart;
}

void SetLastError(int err)
{
    globalerr = err;
}

int GetLastError()
{
    return(globalerr);
}

char *ErrString(int err)
{
    switch (err) {
    case ERROR_INVALID_FUNCTION:
        return "function not implemented";
    case ERROR_FILE_NOT_FOUND:
        return "file not found";
    case ERROR_ACCESS_DENIED:
        return "access denied";
    case ERROR_NOT_ENOUGH_MEMORY:
        return "not enough memory";
    case ERROR_BAD_FORMAT:
        return "bad format";
    case ERROR_NO_MORE_FILES:
        return "no more files";
    case ERROR_HANDLE_EOF:
        return "access beyound EOF";
    case ERROR_HANDLE_DISK_FULL:
        return "no space left on device";
    case ERROR_INVALID_PARAMETER:
        return "invalid parameter";
    case ERROR_INVALID_HANDLE:
        return "invalid handle";
    case ERROR_DISK_FULL:
        return "no space left on device";
    case ERROR_ALREADY_EXISTS:
        return "file exists";
    case ERROR_CAN_NOT_COMPLETE:
        return "operation cannot be completed";
    default:
        return "unknown error";
    }
}

HANDLE CreateFile(const char *sFileName, DWORD ulMode, DWORD ulSharing, void *pSecAttrib, DWORD ulCreation, DWORD ulFlags, HANDLE hFile)
{
    switch (ulCreation)
    {
    case MPQ_OPEN_EXISTING:
        return (HANDLE)open(sFileName, O_RDONLY | O_LARGEFILE);

    case MPQ_OPEN_ALWAYS:
        return (HANDLE)open(sFileName, O_RDWR | O_CREAT);

    case MPQ_CREATE_ALWAYS:
    case MPQ_CREATE_NEW:
        return (HANDLE)open(sFileName, O_RDWR | O_CREAT | O_TRUNC);

    default:
        return INVALID_HANDLE_VALUE;
    }
}

BOOL CloseHandle(HANDLE hFile)
{
    return (close((intptr_t)hFile) == 0);
}

DWORD GetFileSize(HANDLE hFile, DWORD *ulOffSetHigh)
{
    // Fix by Taiche : removed the hFile == NULL test because the CreateFile function above
    // can return a HANDLE equal to 0 WHICH IS A LEGAL VALUE and does not mean the handle is NULL.
    if (hFile == INVALID_HANDLE_VALUE) 
    {
        return 0xffffffff;
    }

    struct stat64 fileinfo;
    fstat64((intptr_t)hFile, &fileinfo);
    
    // Fix by Ladik: If "ulOffSetHigh" is not NULL, it needs to be set 
    // to higher 32 bits of a file size.
    // TODO: Could some Linux programmer verify this ?
    if(ulOffSetHigh != NULL)
        *ulOffSetHigh = (fileinfo.st_size >> 32);

    return (DWORD)fileinfo.st_size;
}

DWORD SetFilePointer(HANDLE hFile, LONG lOffSetLow, LONG *pOffSetHigh, DWORD ulMethod)
{
    off64_t nFileOffset = (DWORD)lOffSetLow;

    if(pOffSetHigh != NULL)
        nFileOffset |= (*(off64_t *)pOffSetHigh) << 32;

    return lseek64((intptr_t)hFile, nFileOffset, ulMethod);
}

BOOL SetEndOfFile(HANDLE hFile)
{
    return (ftruncate((intptr_t)hFile, lseek((intptr_t)hFile, 0, SEEK_CUR)) == 0);
}

BOOL GetFileTime(HANDLE hFile,                 /* handle to file */
                 LPFILETIME lpCreationTime,    /* file creation time */
                 LPFILETIME lpLastAccessTime,  /* file accessed time */
                 LPFILETIME lpLastWriteTime)   /* file modified time */
{
    struct stat file_stats;
    int filedesc = (int)(size_t)(hFile);

    if(fstat(filedesc, &file_stats) == -1)
        return FALSE;

    if(lpCreationTime != NULL)
        ConvertTimeTToFileTime(lpCreationTime, file_stats.st_ctime);
    if(lpLastAccessTime != NULL)
        ConvertTimeTToFileTime(lpLastAccessTime, file_stats.st_atime);
    if(lpLastWriteTime != NULL)
        ConvertTimeTToFileTime(lpLastWriteTime, file_stats.st_mtime);
    return TRUE;
}

BOOL ReadFile(HANDLE hFile, void *pBuffer, DWORD ulLen, DWORD *ulRead, void *pOverLapped)
{
    ssize_t count;
    if ((count = read((intptr_t)hFile, pBuffer, ulLen)) == -1) {
        *ulRead = 0;
        return false;
    }
    *ulRead = count;
    return true;
}

BOOL WriteFile(HANDLE hFile, const void *pBuffer, DWORD ulLen, DWORD *ulWritten, void *pOverLapped)
{
    ssize_t count;
    if ((count = write((intptr_t)hFile, pBuffer, ulLen)) == -1) {
        *ulWritten = 0;
        return false;
    }
    *ulWritten = count;
    return true;
}

// Check if a memory block is accessible for reading
BOOL IsBadReadPtr(const void * ptr, int size)
{
    return FALSE;
}

// Returns attributes of a file
DWORD GetFileAttributes(const char * szFileName)
{
    return 0;
}

BOOL DeleteFile(const char *lpFileName)
{
    return (BOOL)remove(lpFileName);
}

BOOL MoveFile(const char *lpExistingFileName, const char *lpNewFileName)
{
    return rename(lpExistingFileName, lpNewFileName);
}	

#endif
