#include "stdafx.h"
#include <cstdlib>

char* wchar_to_char(wchar_t* src)
{
    char* ret;
    ret = new char[wcslen(src) + 1];
    wcstombs(ret, src, wcslen(src));
    ret[wcslen(src)] = 0;
    return ret;
}

char* cstring_to_char(CString str)
{
    wchar_t* wstr;
    char* cstr;

    wstr = str.GetBuffer(str.GetLength());
    cstr = wchar_to_char(wstr);

    return cstr;
}