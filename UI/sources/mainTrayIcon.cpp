#include "headers/mainTrayIcon.hpp"

mainTrayIcon::mainTrayIcon() : wxTaskBarIcon()
{
    this->myIcon = wxIcon(wxIconLocation("D:/VisualStudioProjects/Autotextv2/UI/icon.ico"));
    this->SetIcon(this->myIcon);

    this->mFrame = NULL;
}

wxMenu* mainTrayIcon::CreatePopupMenu()
{
    wxMenu *trayMenu = new wxMenu;
    trayMenu->Append(ID_TRAY_HOME, "&Home");  // Home button
    trayMenu->Append(ID_TRAY_EXIT, "&Exit");  // Exit button

    Bind(wxEVT_MENU, &mainTrayIcon::OnHome, this, ID_TRAY_HOME);
    Bind(wxEVT_MENU, &mainTrayIcon::OnExit, this, ID_TRAY_EXIT);

    return trayMenu;
}

void mainTrayIcon::OnHome(wxCommandEvent& event)
{
    if (this->mFrame)  // If frame already exists
    {
        bool wasMaximized = this->mFrame->IsMaximized();
        if (this->mFrame->IsIconized())  // If window has been minimized
        {
            this->mFrame->Restore();  // This will also restore to normal size if the window was maximized
        }
        if (wasMaximized)
        {
            this->mFrame->Maximize();
        }
        this->mFrame->SetFocus();
        this->mFrame->Show();
    }
    else
    {
        // Creating frame
        this->mFrame = new MainFrame("Autotext", this->mFrame);
        this->mFrame->SetSize(wxSize(350, 330));
        this->mFrame->SetIcon(myIcon);
        this->mFrame->Show(true);
    }
}

void mainTrayIcon::OnExit(wxCommandEvent& event)
{
    if (this->mFrame)
    {
        this->mFrame->Close();
    }
    this->Destroy();
}
