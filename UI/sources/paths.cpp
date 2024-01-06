#include "headers/paths.hpp"

using namespace std;

string getPathOfExeAsString()
{
    wchar_t pBuf[256];
    size_t len = sizeof(pBuf);

    int bytes = GetModuleFileName(NULL, pBuf, len);  // This gets path of current file
    wstring ws(pBuf);

    //setup converter from wide string to string
    using convert_type = codecvt_utf8<wchar_t>;
    wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    string res = converter.to_bytes(ws);
    
    // Remove last part of the path (filename)
    int i1 = res.find_last_of("\\");
    int i2 = res.find_last_of("/");
    return res.substr(0, (i1>i2) ? i1 : i2);
}
