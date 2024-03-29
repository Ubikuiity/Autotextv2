#pragma once

#include <wx/wx.h>
#include <wx/taskbar.h>
#include <filesystem>
#include "ID.hpp"
#include "paths.hpp"

class mainTrayIcon;  // Since we have mutual inclusions, we must declare the class exist so mainFrame can use it
#include "mainFrame.hpp"

// Headers for regexMotor
extern "C"
{
    #include "../../../submodules/regexMotor/sources/headers/useMotor.h"
    #include "../../../submodules/caracterReceiver/sources/headers/caracterReceiver.h"
    #include "../../../submodules/sendKeyboardStrikes/sources/headers/keyboardIn.h"
    #include "../../../submodules/yamlHandler/sources/headers/all.h"
}
#include "../../../submodules/logger/sources/headers/logger.hpp"

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
    bool mMotorIsRunning;

    public:
    mainTrayIcon();
    virtual wxMenu* CreatePopupMenu();
    void informSubFrameClosing();

    // Regex Motor functionnalities
    bool getMotorState();
    bool startStopEmbeddedMotor();
    bool startEmbeddedMotor();
    bool stopEmbeddedMotor();
};

typedef struct paramsForCallback paramsForCallback;
struct paramsForCallback
{
    reMotor* motor;
    wordPatterns* patterns;
};
