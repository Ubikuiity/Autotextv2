#include "headers/homePanel.hpp"

HomePanel::HomePanel(wxFrame* parent) : wxPanel(parent)
{
    // ----- Layout -----

    // Left Part
    this->startStopButton = new wxButton(this, wxID_ANY, "Start/Stop Engine", wxPoint(20, 20), wxSize(100, 25));
    wxStaticText* typedKeys = new wxStaticText(this, wxID_ANY, "", wxPoint(20, 50), wxSize(100, 50));
    typedKeys->SetLabelText("Hello, app is working ...");

    // Right part
    wxCheckBox* optionCheckBox = new wxCheckBox(this, wxID_ANY, "Option 1 here", wxPoint(150, 30));

    startStopButton->Bind(wxEVT_BUTTON, &HomePanel::StartStopDetection, this);
}

// ----- Events -----

void HomePanel::StartStopDetection(wxCommandEvent& evt)
{
    this->startStopButton->Enable(false);
    // TODO Send info to regexEngine
    Sleep(1000);
    this->startStopButton->Enable(true);
}
