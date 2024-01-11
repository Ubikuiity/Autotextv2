#include "headers/homePanel.hpp"

BYTE* RegGetBinaryValue(HKEY hkey, const char* registryPath, const char* value, int* retSize);

HomePanel::HomePanel(MainFrame* parent) : wxPanel(parent), directParent(parent)
{
    // ----- Layout -----

    // Left Part
    this->startStopButton = new wxButton(this, wxID_ANY, "", wxPoint(20, 20), wxSize(100, 25));
    this->UpdateButtonText();

    // Right part
    this->startupStatusCheckBox = new wxCheckBox(this, wxID_ANY, "Start Autotext with Windows", wxPoint(150, 30));

    this->startupStatusCheckBox->SetValue(this->GetStartupStatusFromRegistry());
    
    startStopButton->Bind(wxEVT_BUTTON, &HomePanel::StartStopDetection, this);
    this->startupStatusCheckBox->Bind(wxEVT_CHECKBOX, &HomePanel::ChangeStartupStatus, this);
}

// ----- Events -----

void HomePanel::UpdateButtonText()
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

void HomePanel::ChangeStartupStatus(wxCommandEvent& evt)
{
    HKEY key;
    BOOL fResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StartupApproved\\Run", 0, KEY_WRITE, &key);
    if (ERROR_SUCCESS == fResult)
    {
        if (this->startupStatusCheckBox->IsChecked())  // We need to activate startup
        {
            DWORD dwData[] = {0x2,0x0,0x0};  // 2 Enables the startup
            RegSetValueEx(key, __TEXT("Autotext"), 0, REG_BINARY, (LPBYTE)&dwData, sizeof(dwData));
        }
        else  // We need to disable startup on Autotext
        {
            DWORD dwData[] = {0x3,0x0,0x0};  // 3 disables the startup
            RegSetValueEx(key, __TEXT("Autotext"), 0, REG_BINARY, (LPBYTE)&dwData, sizeof(dwData));
        }
    }
    RegCloseKey(HKEY_CURRENT_USER);
}

void HomePanel::StartStopDetection(wxCommandEvent& evt)
{
    this->startStopButton->Enable(false);
    if (!this->directParent->GetParentTrayIcon()->startStopEmbeddedMotor())  // If we coudn't change the status
    {
        wxMessageBox("Coudn't start/stop engine.\n\
Probable reason is because there are no keywords to detect.\n\
Otherwise check logs in AppData folder.",
            "Start / stop failed",
            wxICON_EXCLAMATION);
    }
    UpdateButtonText();
    this->startStopButton->Enable(true);
}

// --- Utils ---

// Check windows registry to find is Autotext startup has been disabled
// Returns true if startup is activated, false otherwise
bool HomePanel::GetStartupStatusFromRegistry()
{
    int resSize;
    std::string regPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StartupApproved\\Run";
    std::string regValue = "Autotext";

    BYTE* regStatus = RegGetBinaryValue(HKEY_CURRENT_USER, 
        regPath.c_str(),
        regValue.c_str(), &resSize);

    if (regStatus == NULL)  // If function failed, return default value
    {
        return false;
    }
    return (regStatus[0] == 2);  // First byte of reg value is 3 if deactivated and 2 if activated
}


// Retrieve binary data from Windows registry, returns the pointer to the bytes read, and the length read (retSize)
BYTE* RegGetBinaryValue(HKEY hkey, const char* registryPath, const char* value, int* retSize)
{
    HKEY hKey;
    BYTE* pData = NULL;

    // Open the registry key
    if (RegOpenKeyExA(HKEY_CURRENT_USER, registryPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD dwType;

        // Get the size of the binary data
        if (RegQueryValueExA(hKey, value, NULL, &dwType, NULL, (DWORD*)retSize) == ERROR_SUCCESS)
        {
            // Allocate memory for the binary data
            pData = new BYTE[*retSize];

            // Read the binary data using RegGetValue
            RegGetValueA(hKey, NULL, value, RRF_RT_REG_BINARY, &dwType, pData, (DWORD*)retSize);
        }

        // Close the registry key
        RegCloseKey(hKey);
    }
    return pData;
}
