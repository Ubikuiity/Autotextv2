#pragma once
#include <wx/wx.h>

class HomePanel : public wxPanel
{
public:
    HomePanel(wxFrame* parent);

private:
    wxButton* startStopButton;
    
    void StartStopDetection(wxCommandEvent& evt);

};
