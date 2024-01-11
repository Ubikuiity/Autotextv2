#include "./headers/editPanel.hpp"

using namespace std;

void wipeContentOfFile(const char* pathOfFile);

EditPanel::EditPanel(wxFrame* parent) : wxPanel(parent)
{
    this->getWordsFromFile();

    wxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);  // Main sizer
    wxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);  // Sizer containing list of keywords + buttons
    wxSizer* keywordSizer = new wxBoxSizer(wxHORIZONTAL);  // Sizer containing keyword
    wxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);  // Sizer containing right part (keyword + replacer)

    this->boxListOfKeyWords = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(100, 150), this->keyWordsArray, wxLB_SINGLE|wxLB_HSCROLL);

    wxButton* addButton = new wxButton(this, wxID_ANY, "Add", wxDefaultPosition, wxDefaultSize);
    this->removeButton = new wxButton(this, wxID_ANY, "Remove", wxDefaultPosition, wxDefaultSize);
    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save", wxDefaultPosition, wxDefaultSize);

    leftSizer->Add(this->boxListOfKeyWords, 1, wxEXPAND);
    leftSizer->Add(addButton, 0, wxTOP | wxALIGN_CENTER, 2);
    leftSizer->Add(this->removeButton, 0, wxTOP | wxALIGN_CENTER, 2);
    leftSizer->Add(saveButton, 0, wxTOP | wxALIGN_CENTER, 2);

    wxStaticText* keywordCaption = new wxStaticText(this, wxID_ANY, "Keyword : ");
    this->keyWordControl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0L, wxTextValidator(wxFILTER_ALPHANUMERIC));
    wxStaticText* replacerCaption = new wxStaticText(this, wxID_ANY, "Replacer : ");
    this->replacerControl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    keywordSizer->Add(keywordCaption, 0, wxEXPAND | wxTOP, 3);
    keywordSizer->Add(this->keyWordControl, 1, wxEXPAND);

    rightSizer->Add(keywordSizer, 0, wxALIGN_LEFT | wxEXPAND | wxDOWN, 5);
    rightSizer->Add(replacerCaption, 0, wxTOP, 5);
    rightSizer->Add(this->replacerControl, 1, wxALIGN_LEFT | wxEXPAND);

    mainSizer->Add(leftSizer, 0, wxEXPAND | wxLEFT | wxTOP | wxDOWN, 5);
    mainSizer->Add(rightSizer, 1, wxEXPAND | wxALL, 5);

    this->SetSizerAndFit(mainSizer);

    addButton->Bind(wxEVT_BUTTON, &EditPanel::addKeyWord, this);
    this->removeButton->Bind(wxEVT_BUTTON, &EditPanel::removeKeyWord, this);
    saveButton->Bind(wxEVT_BUTTON, &EditPanel::saveKeyWordsFromUI, this);

    this->boxListOfKeyWords->Bind(wxEVT_LISTBOX, &EditPanel::changeSelectedWord, this);
    this->keyWordControl->Bind(wxEVT_TEXT, &EditPanel::keyWordTextChange, this);
    this->replacerControl->Bind(wxEVT_TEXT, &EditPanel::replacerTextChange, this);
}

void EditPanel::addKeyWord(wxCommandEvent& event)
{
    this->keyWordsArray.Add("new");
    this->replacersArray.Add("Put replacer here...");

    this->boxListOfKeyWords->Append("new");
    this->boxListOfKeyWords->SetSelection(this->keyWordsArray.GetCount()-1);  // Set selection on last item ("New")
    wxCommandEvent dummyEvent;
    this->changeSelectedWord(dummyEvent);
}

void EditPanel::removeKeyWord(wxCommandEvent& event)
{
    this->removeButton->Disable();
    int activeIndex = this->boxListOfKeyWords->GetSelection();
    if(activeIndex == wxNOT_FOUND)
    {
        this->removeButton->Enable();
        return;  // No item is selected, we skip
    }
    // Remove the selected element
    this->keyWordsArray.RemoveAt(activeIndex);
    this->replacersArray.RemoveAt(activeIndex);

    this->boxListOfKeyWords->Delete(activeIndex);
    if (activeIndex  > 0)
    {
        this->boxListOfKeyWords->SetSelection(activeIndex - 1);  // Change selection to upper value
    }

    wxCommandEvent dummyEvent;
    this->changeSelectedWord(dummyEvent);  // Refresh the right text boxes

    this->removeButton->Enable();
}

void EditPanel::changeSelectedWord(wxCommandEvent& event)
{
    int activeIndex = this->boxListOfKeyWords->GetSelection();
    if(activeIndex == wxNOT_FOUND)  // No items is selected
    {
        this->keyWordControl->ChangeValue("");  // Empty the fields
        this->replacerControl->ChangeValue("");  // Empty the fields
        return;
    }
    this->keyWordControl->ChangeValue(this->keyWordsArray[activeIndex]);
    this->replacerControl->ChangeValue(this->replacersArray[activeIndex]);
}

void EditPanel::replacerTextChange(wxCommandEvent& event)
{
    int activeIndex = this->boxListOfKeyWords->GetSelection();
    if(activeIndex == wxNOT_FOUND)
    {
        return;  // No item is selected, we skip
    }
    wxString newReplacer = this->replacerControl->GetValue();
    this->replacersArray[activeIndex] = newReplacer;
}

void EditPanel::keyWordTextChange(wxCommandEvent& event)
{
    int activeIndex = this->boxListOfKeyWords->GetSelection();
    if(activeIndex == wxNOT_FOUND)
    {
        return;  // No item is selected, we skip
    }
    wxString newKeyword = this->keyWordControl->GetValue();
    this->keyWordsArray[activeIndex] = newKeyword;
    
    // Now we change the value inside the left list box
    this->boxListOfKeyWords->SetString(activeIndex, newKeyword);
}

// File edits function

void EditPanel::getWordsFromFile()
{
    string yamlWordPath = getAppdataAsString() + "/" + PATHS_PROGRAMNAME + "/" + PATHS_WORDSFILE;
    wordPatterns* filePatterns = getWordPatternsFromFile(yamlWordPath.c_str());

    // Resets arrays (Probably not necessary)
    this->keyWordsArray.Clear();
    this->replacersArray.Clear();

    if (filePatterns != NULL)  // If no patterns were found, we skip
    {
        for(int i=0; i<lengthStrList(filePatterns->words); i++)
        {
            this->keyWordsArray.Add(getStrListeValue(filePatterns->words, i));
            this->replacersArray.Add(getStrListeValue(filePatterns->replacements, i));
        }
        destroyWordPatterns(filePatterns);
    }
}

// Save edited words to file
// TODO add backup file ?
void EditPanel::saveKeyWordsFromUI(wxCommandEvent& event)
{
    string yamlWordPath = getAppdataAsString() + "/" + PATHS_PROGRAMNAME + "/" + PATHS_WORDSFILE;
    int nbOfWords = this->keyWordsArray.GetCount();
    if (nbOfWords < 1)  // If there is 0 item in the list
    {
        wipeContentOfFile(yamlWordPath.c_str());
        wxMessageBox("Data saved successfully (0 keywords saved)", "Save", wxOK | wxICON_INFORMATION);
        return;
    }
    wordPatterns* editedPatterns = createWordPatternsFromFirstPair(this->keyWordsArray[0].mb_str(), this->replacersArray[0].mb_str());

    for(int i=1; i < nbOfWords; i++)
    {
        appendPattern(editedPatterns, this->keyWordsArray[i].mb_str(), this->replacersArray[i].mb_str());
    }

    writeFileFromWordPatterns(yamlWordPath.c_str(), editedPatterns);

    destroyWordPatterns(editedPatterns);  // We free the memory

    wxString message = wxString::Format("Data saved successfully.\n%i words saved", nbOfWords);
    wxMessageBox(message, "Save", wxOK | wxICON_INFORMATION);
}

// Utils

void wipeContentOfFile(const char* pathOfFile)
{
    FILE* file = NULL;
    file = fopen(pathOfFile, "w");  // This clears the file
    if (file == NULL)
    {
        printf("Something unexpected happend when opening");
        return;
    }
    fclose(file);
}
