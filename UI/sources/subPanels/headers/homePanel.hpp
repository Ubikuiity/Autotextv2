#pragma once
#include <wx/wx.h>
#include <string>

class HomePanel;  // Mutual inclusion, we must define class before inclusion
#include "../../headers/mainFrame.hpp"

class HomePanel : public wxPanel
{
    private:
    wxButton* startStopButton;
    MainFrame* directParent;
    
    void StartStopDetection(wxCommandEvent& evt);
    void updateButtonText();

    public:
    HomePanel(MainFrame* parent);
    void addCaracterToTypedCaracters(char c);
};
