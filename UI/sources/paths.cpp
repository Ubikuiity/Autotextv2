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

string getAppdataAsString()
{
    wchar_t* pBuf;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, NULL, &pBuf);  // This does a malloc for pBuf inside

    wstring ws(pBuf);
    //setup converter from wide string to string
    using convert_type = codecvt_utf8<wchar_t>;
    wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    string res = converter.to_bytes(ws);

    CoTaskMemFree(pBuf);  // Frees the malloc done earlier by Windows API call
    
    return res;
}

// Create basic ressources needed for the program to work
// Returns an int with each bit giving if a file / folder was created
// 1 for AppData folder, 2 for log Folder, 4 for words.yaml file
int createAppDataRessourcesIfNotExistent()
{
    string appDataFolder = getAppdataAsString();
    appDataFolder = appDataFolder + "/" + PATHS_PROGRAMNAME;

    int res = 0;

    res += filesystem::create_directory(appDataFolder);  // Main Appdata folder

    string logFolder = appDataFolder + "/" + PATHS_LOGFOLDER;
    res += filesystem::create_directory(logFolder) * 2;  // Log folder

    string wordsPath = appDataFolder + "/" + PATHS_WORDSFILE;
    if (!filesystem::exists(wordsPath))  // If file doesn't exists, we create empty file
    {
        res += 4;
        FILE* f = fopen(wordsPath.c_str(), "w");
        fclose(f);
    }
    return res;
}
