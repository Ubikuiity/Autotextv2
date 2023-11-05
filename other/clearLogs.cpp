#include <string>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
using namespace std;

int main(/* int argc, char* argv[] */)
{
    string logFolder = "log";
    fs::path logFolderPath(logFolder);

    int cmpt = 0;
    for (const auto subFile : fs::directory_iterator(logFolderPath))
    {
        fs::path filePath = subFile.path();
        if (filePath.extension() == ".log")
        {
            if(! fs::remove(filePath))  // Removes the file and goes in 'if' in case something went wrong
            {
                cout << "Something went wrong while removing : " << filePath << endl;
            }
            else cmpt ++;
        }
    }
    cout << "Removed " << to_string(cmpt) << " entries in " << fs::absolute(logFolderPath) << endl;

    return 0;
}
