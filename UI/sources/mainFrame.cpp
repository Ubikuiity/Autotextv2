#include "./headers/mainFrame.hpp"
#include <wx/event.h>

MainFrame::MainFrame(const wxString& title, mainTrayIcon* trayIconPtr) : wxFrame(NULL, wxID_ANY, title),
parentTrayIcon(trayIconPtr), motorStoppedByEdition(false)
{
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnCloseWindow, this);

    // Home list
    wxMenu *menuHome = new wxMenu;
    menuHome->Append(ID_FRAME_HOME_MENU, "&Home",
                    "Home page of autotext");  // Home button
    menuHome->Append(wxID_EXIT);  // Quit button

    // Edit list
    wxMenu *menuEdit = new wxMenu;
    menuEdit->Append(ID_FRAME_EDIT_WORDS, "&Edit words",
                    "Edit the key words");  // Edit words button
    menuEdit->Append(ID_FRAME_EDIT_FILE, "&Edit file",
                    "Manually edit the words file");  // Edit file button

    wxMenu *menuHelp = new wxMenu;  // Menu named "Help"
    menuHelp->Append(wxID_ABOUT);

    // Main menu bar
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuHome, "&Home");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar(menuBar);
 
    CreateStatusBar();  // Creates bottom status bar
    SetStatusText("Welcome to Autotext");

    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::ChangeToEditPanel, this, ID_FRAME_EDIT_WORDS);
    Bind(wxEVT_MENU, &MainFrame::ChangeToHomePanel, this, ID_FRAME_HOME_MENU);

    // ----- Creating panels -----
    this->sizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(this->sizer);
    this->hPanel = new HomePanel(this);
    this->currentPanel = this->hPanel;
    this->ePanel = new EditPanel(this);
    ePanel->Hide();

    // Don't understand why I need this part but otherwise we have gray stripes around the window
    this->sizer->Add(this->hPanel);
    this->sizer->Detach(0);
    this->sizer->Prepend(this->hPanel, 1, wxGROW);
    this->sizer->Layout();
}

void MainFrame::ChangeToHomePanel(wxCommandEvent& event)
{
    this->sizer->Detach(0);
    this->currentPanel->Hide();
    this->sizer->Prepend(this->hPanel, 1, wxGROW);
    this->hPanel->Show();
    this->currentPanel = this->hPanel;
    this->sizer->Layout();

    if (this->motorStoppedByEdition)
    {
        if (this->parentTrayIcon->startEmbeddedMotor())
        {
            SetStatusText("Detection has been reactivated");
        }
        else
        {
            SetStatusText("Coudn't start detection (no keywords ?)");
        }
        this->motorStoppedByEdition = false;
    }
    this->hPanel->UpdateButtonText();
}

void MainFrame::ChangeToEditPanel(wxCommandEvent& event)
{
    this->sizer->Detach(0);
    this->currentPanel->Hide();
    this->sizer->Prepend(this->ePanel, 1, wxGROW);
    this->ePanel->Show();
    this->currentPanel = this->ePanel;
    this->sizer->Layout();

    if (this->parentTrayIcon->getMotorState())  // If motor is running
    {
        this->parentTrayIcon->stopEmbeddedMotor();
        SetStatusText("Detection stopped while editing");
        this->motorStoppedByEdition = true;
    }
}

mainTrayIcon* MainFrame::GetParentTrayIcon()
{
    return this->parentTrayIcon;
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnCloseWindow(wxCloseEvent& event)
{
    if (this->motorStoppedByEdition)
    {
        this->parentTrayIcon->startEmbeddedMotor();
    }
    this->parentTrayIcon->informSubFrameClosing();  // Send information to parent that windows has been closed
    event.Skip();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Custom Autotext\nBy Ubikuiity\nCode available on Github",
                "Autotext v2", wxICON_INFORMATION, this);
}
