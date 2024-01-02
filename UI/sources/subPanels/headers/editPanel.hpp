#pragma once
#include <wx/wx.h>
#include <string>

class EditPanel : public wxPanel
{
public:
    EditPanel(wxFrame* parent);

private:
    wxListBox* boxListOfKeyWords;
    wxArrayString keyWordsArray;
    wxButton* removeButton;  // We need pointer for this button to lock it when we remove to make sure we don't mess up

    void addKeyWord(wxCommandEvent& event);
    void removeKeyWord(wxCommandEvent& event);
    void saveKeyWordsFromUI(wxCommandEvent& event);
};
