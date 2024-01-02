#include "./headers/mainApp.hpp"

bool MyApp::OnInit()
{
    // Creating icon in task tray
    mainTrayIcon* trayIcon = new mainTrayIcon();

    return true;
}
