#include "headers/homePanel.hpp"

HomePanel::HomePanel(MainFrame* parent) : wxPanel(parent), directParent(parent)
{
    // ----- Layout -----

    // Left Part
    this->startStopButton = new wxButton(this, wxID_ANY, "", wxPoint(20, 20), wxSize(100, 25));
    this->updateButtonText();

    // Right part
    wxCheckBox* optionCheckBox = new wxCheckBox(this, wxID_ANY, "Option 1 here", wxPoint(150, 30));

    startStopButton->Bind(wxEVT_BUTTON, &HomePanel::StartStopDetection, this);
}

// ----- Events -----

void HomePanel::updateButtonText()
{
    if (this->directParent->GetParentTrayIcon()->getMotorState())
    {
        this->startStopButton->SetLabelText("Stop detection");
    }
    else
    {
        this->startStopButton->SetLabelText("Start detection");
    }
}

void HomePanel::StartStopDetection(wxCommandEvent& evt)
{
    this->startStopButton->Enable(false);
    this->directParent->GetParentTrayIcon()->startStopEmbeddedMotor();
    updateButtonText();
    this->startStopButton->Enable(true);
}
