#pragma once
#include <wx/wx.h>
#include <string>
#include <Windows.h>

class HomePanel;  // Mutual inclusion, we must define class before inclusion
#include "../../headers/mainFrame.hpp"

class HomePanel : public wxPanel
{
    private:
    wxButton* startStopButton;
    wxCheckBox* startupStatusCheckBox;

    MainFrame* directParent;
    
    void StartStopDetection(wxCommandEvent& evt);
    void ChangeStartupStatus(wxCommandEvent& evt);
    bool GetStartupStatusFromRegistry();

    public:
    HomePanel(MainFrame* parent);
    void UpdateButtonText();
};
