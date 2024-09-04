#ifndef TCHAR_MACROS
#define TCHAR_MACROS
#include <string>
#include <tchar.h>

typedef std::basic_string<TCHAR> tstring;

inline TCHAR* strToTchar(std::string str);

#ifdef UNICODE
#define TEXTCMP(text1,text2) wcscmp(text1,text2)

TCHAR* strToTchar(std::string str){
    TCHAR *tch=new TCHAR[str.size()+1];
    // Terminate TCHAR string
    tch[str.size()]=0;
    std::copy(str.begin(),str.end(),tch);
    return tch;
}
#else
#define TEXTCMP(text1,text2) strcmp(text1,text2)

TCHAR* strToTchar(std::string str){
    return str.c_str();
}

#endif
#endif