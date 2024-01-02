#pragma once

#include <wx/wx.h>
#include <wx/taskbar.h>
#include "ID.hpp"
#include "mainFrame.hpp"

class mainTrayIcon : public wxTaskBarIcon
{
    private:
    wxIcon myIcon;
    MainFrame* mFrame;
    void OnHome(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    public:
    mainTrayIcon();
    virtual wxMenu* CreatePopupMenu();
};
