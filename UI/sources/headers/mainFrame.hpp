#pragma once

#include <wx/wx.h>
#include "ID.hpp"

class MainFrame;  // Since we have mutual inclusions, we must declare class so mainTrayIcon knows what it is
#include "mainTrayIcon.hpp"
#include "../subPanels/headers/homePanel.hpp"
#include "../subPanels/headers/editPanel.hpp"

class MainFrame : public wxFrame
{
    private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void ChangeToEditPanel(wxCommandEvent& event);
    void ChangeToHomePanel(wxCommandEvent& event);
    void DisplayEditHelp(wxCommandEvent& event);
    HomePanel* hPanel;
    EditPanel* ePanel;
    wxPanel* currentPanel;
    wxBoxSizer* sizer;

    mainTrayIcon* parentTrayIcon;
    bool motorStoppedByEdition;

    public:
    MainFrame(const wxString& title, mainTrayIcon* trayIconPtr);
    void OnCloseWindow(wxCloseEvent& event);
    mainTrayIcon* GetParentTrayIcon();
};
