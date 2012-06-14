
//#include "stdafx.h"

#include "h-genutils.h"

#include <Ogre.h>


// convert a char* to a wchar*
// warning: the return value points to a fixed buffer, whose contents change with
// every call to this function.
WCHAR* hGenUtils::convertCStringToWString(const char* string)
{
    const int MAX_STRINGZ=500;
    static WCHAR wtext[MAX_STRINGZ+2];

    if (strlen(string)>MAX_STRINGZ)
    {
        throw("hGenUtils::convertCStringToWString buffer isn't big enough");
    }

    // convert text to wchar
    if (MultiByteToWideChar(
        CP_ACP,// ansi code page
		0,// flags
        string,// orig string
        -1,// calculate len
        wtext,// where to put the string
        MAX_STRINGZ)// maximum allowed path
        ==0)
    {
        throw("hGenUtils::convertCStringToWString failed with no extra error info");
    }

    return wtext;
}