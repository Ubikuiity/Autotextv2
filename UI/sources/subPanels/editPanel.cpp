#include "./headers/editPanel.hpp"

EditPanel::EditPanel(wxFrame* parent) : wxPanel(parent)
{
    this->keyWordsArray = {"New", "Hello"};  // TODO Create from actual words !
    this->boxListOfKeyWords = new wxListBox(this, wxID_ANY, wxPoint(10, 10), wxSize(100, 150), keyWordsArray, wxLB_SINGLE|wxLB_HSCROLL);
    
    wxButton* addButton = new wxButton(this, wxID_ANY, "Add", wxPoint(10, 165), wxSize(100, 25));
    this->removeButton = new wxButton(this, wxID_ANY, "Remove", wxPoint(10, 190), wxSize(100, 25));
    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save", wxPoint(10, 215), wxSize(100, 25));

    wxTextCtrl* keyWordControl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(125, 10), wxSize(200, 20));
    wxTextCtrl* replacerControl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(125, 35), wxSize(200, 205));

    addButton->Bind(wxEVT_BUTTON, &EditPanel::addKeyWord, this);
    this->removeButton->Bind(wxEVT_BUTTON, &EditPanel::removeKeyWord, this);
    saveButton->Bind(wxEVT_BUTTON, &EditPanel::saveKeyWordsFromUI, this);
}

void EditPanel::addKeyWord(wxCommandEvent& event)
{
    this->boxListOfKeyWords->Append("New");
    this->keyWordsArray.Add("New");
}

void EditPanel::removeKeyWord(wxCommandEvent& event)
{
    int activeIndex = this->boxListOfKeyWords->GetSelection();
    if(activeIndex == wxNOT_FOUND)
    {
        return;  // No item is selected, we skip
    }
    this->removeButton->Disable();
    this->keyWordsArray.RemoveAt(activeIndex);  // Remove the selected element
    this->boxListOfKeyWords->Delete(activeIndex);
    this->removeButton->Enable();
}

void EditPanel::saveKeyWordsFromUI(wxCommandEvent& event)
{
    std::string text = "Strings are : \n";
    for(int i=0; i < this->keyWordsArray.GetCount(); i++)
    {
        text += keyWordsArray[i] + "\n";
    }
    wxMessageBox(text, "Save", wxOK | wxICON_INFORMATION);
}
