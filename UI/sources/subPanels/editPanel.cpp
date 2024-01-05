#include "./headers/editPanel.hpp"

void wipeContentOfFile(char* pathOfFile);

EditPanel::EditPanel(wxFrame* parent) : wxPanel(parent)
{
    this->getWordsFromFile();

    this->boxListOfKeyWords = new wxListBox(this, wxID_ANY, wxPoint(10, 10), wxSize(100, 150), this->keyWordsArray, wxLB_SINGLE|wxLB_HSCROLL);
    
    wxButton* addButton = new wxButton(this, wxID_ANY, "Add", wxPoint(10, 165), wxSize(100, 25));
    this->removeButton = new wxButton(this, wxID_ANY, "Remove", wxPoint(10, 190), wxSize(100, 25));
    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save", wxPoint(10, 215), wxSize(100, 25));

    this->keyWordControl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(125, 10), wxSize(200, 20));
    this->replacerControl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(125, 35), wxSize(200, 205), wxTE_MULTILINE);

    addButton->Bind(wxEVT_BUTTON, &EditPanel::addKeyWord, this);
    this->removeButton->Bind(wxEVT_BUTTON, &EditPanel::removeKeyWord, this);
    saveButton->Bind(wxEVT_BUTTON, &EditPanel::saveKeyWordsFromUI, this);

    this->boxListOfKeyWords->Bind(wxEVT_LISTBOX, &EditPanel::changeSelectedWord, this);
    this->keyWordControl->Bind(wxEVT_TEXT, &EditPanel::keyWordTextChange, this);
    this->replacerControl->Bind(wxEVT_TEXT, &EditPanel::replacerTextChange, this);
}

void EditPanel::addKeyWord(wxCommandEvent& event)
{
    this->keyWordsArray.Add("New");
    this->replacersArray.Add("");

    this->boxListOfKeyWords->Append("New");
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
        return;  // No item is selected, we skip
    }
    // Remove the selected element
    this->keyWordsArray.RemoveAt(activeIndex);
    this->replacersArray.RemoveAt(activeIndex);

    this->boxListOfKeyWords->Delete(activeIndex);
    if (activeIndex  > 0)
    {
        this->boxListOfKeyWords->SetSelection(activeIndex - 1);
        wxCommandEvent dummyEvent;
        this->changeSelectedWord(dummyEvent);
    }

    this->removeButton->Enable();
}

void EditPanel::changeSelectedWord(wxCommandEvent& event)
{
    int activeIndex = this->boxListOfKeyWords->GetSelection();
    if(activeIndex == wxNOT_FOUND)
    {
        return;  // No item is selected, we skip
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
    char yamlWordPath[] = "D:\\VisualStudioProjects\\Autotextv2\\wordsTest.yaml";  // Hardcoded path here
    wordPatterns* filePatterns = getWordPatternsFromFile(yamlWordPath);

    // Resets arrays (Probably not necessary)
    this->keyWordsArray.Clear();
    this->replacersArray.Clear();

    for(int i=0; i<lengthStrList(filePatterns->words); i++)
    {
        this->keyWordsArray.Add(getStrListeValue(filePatterns->words, i));
        this->replacersArray.Add(getStrListeValue(filePatterns->replacements, i));
    }

    destroyWordPatterns(filePatterns);
}

// Save edited words to file
// TODO add backup file ?
void EditPanel::saveKeyWordsFromUI(wxCommandEvent& event)
{
    char yamlWordPath[] = "D:\\VisualStudioProjects\\Autotextv2\\wordsTest.yaml";  // Hardcoded path here
    int nbOfWords = this->keyWordsArray.GetCount();
    if (nbOfWords < 1)  // If there is 0 item in the list
    {
        wipeContentOfFile(yamlWordPath);
        wxMessageBox("Data saved successfully (0 keywords saved)", "Save", wxOK | wxICON_INFORMATION);
        return;
    }
    wordPatterns* editedPatterns = createWordPatternsFromFirstPair(this->keyWordsArray[0].mb_str(), this->replacersArray[0].mb_str());

    for(int i=1; i < nbOfWords; i++)
    {
        appendPattern(editedPatterns, this->keyWordsArray[i].mb_str(), this->replacersArray[i].mb_str());
    }

    writeFileFromWordPatterns(yamlWordPath, editedPatterns);

    destroyWordPatterns(editedPatterns);  // We free the memory

    wxMessageBox("Data saved successfully", "Save", wxOK | wxICON_INFORMATION);
}

// Utils

void wipeContentOfFile(char* pathOfFile)
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
