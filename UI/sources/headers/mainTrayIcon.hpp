#pragma once

#include <wx/wx.h>
#include <wx/taskbar.h>
#include "ID.hpp"
#include "mainFrame.hpp"

// Headers for regexMotor
extern "C"
{
    #include "../../../submodules/regexMotor/sources/headers/useMotor.h"
    #include "../../../submodules/caracterReceiver/sources/headers/caracterReceiver.h"
    #include "../../../submodules/sendKeyboardStrikes/sources/headers/keyboardIn.h"
    #include "../../../submodules/yamlHandler/sources/headers/all.h"
}
#include "../../../submodules/logger/sources/headers/logger.h"

#include "../../../DLLs/headers/hook.h"

#include <string>

class mainTrayIcon : public wxTaskBarIcon
{
    private:
    wxIcon myIcon;
    MainFrame* mFrame;
    void OnHome(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    // Regex Motor variables
    Logger* mLogger;
    wordPatterns* mPatterns;
    reMotor* mMotor;
    threadProperties* mReceiver;

    public:
    mainTrayIcon();
    virtual wxMenu* CreatePopupMenu();
};

typedef struct paramsForCallback paramsForCallback;
struct paramsForCallback
{
    reMotor* motor;
    wordPatterns* patterns;
};
