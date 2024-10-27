#ifndef TCHAR_MACROS
#define TCHAR_MACROS
#include <string>
#include <tchar.h>

typedef std::basic_string<TCHAR> tstring;

#ifdef UNICODE
#define TEXTCMP(text1,text2) wcscmp(text1,text2)

#else
#define TEXTCMP(text1,text2) strcmp(text1,text2)
#endif
#endif