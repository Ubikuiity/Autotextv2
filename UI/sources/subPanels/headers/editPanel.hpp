#pragma once
#include <wx/wx.h>
#include <string>

extern "C"
{
    #include "../../../../submodules/yamlHandler/sources/headers/all.h"
}

class EditPanel : public wxPanel
{
public:
    EditPanel(wxFrame* parent);

private:
    wxListBox* boxListOfKeyWords;
    wxButton* removeButton;
    wxTextCtrl* keyWordControl;
    wxTextCtrl* replacerControl;
    
    wxArrayString keyWordsArray;  // This array will store the editable keywords
    wxArrayString replacersArray;  // This array will store the editable replacer strings

    // Events

    void addKeyWord(wxCommandEvent& event);
    void removeKeyWord(wxCommandEvent& event);
    void saveKeyWordsFromUI(wxCommandEvent& event);
    void changeSelectedWord(wxCommandEvent& event);
    void replacerTextChange(wxCommandEvent& event);
    void keyWordTextChange(wxCommandEvent& event);
    void getWordsFromFile();
};
