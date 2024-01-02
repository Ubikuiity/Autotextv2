#pragma once

#include <wx/wx.h>
#include "ID.hpp"
#include "../subPanels/headers/homePanel.hpp"
#include "../subPanels/headers/editPanel.hpp"

class MainFrame : public wxFrame
{
    private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void ChangeToEditPanel(wxCommandEvent& event);
    void ChangeToHomePanel(wxCommandEvent& event);
    HomePanel* hPanel;
    EditPanel* ePanel;
    wxPanel* currentPanel;
    wxBoxSizer* sizer;

    MainFrame*& trayIconPtrToThis;

    public:
    MainFrame(const wxString& title, MainFrame*& trayIconFrame);
    void OnCloseWindow(wxCloseEvent& event);
};
