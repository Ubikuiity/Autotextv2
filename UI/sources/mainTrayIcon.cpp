#include "headers/mainTrayIcon.hpp"

using namespace std;

int callbackWrapper(char* c, void* params);

mainTrayIcon::mainTrayIcon() : wxTaskBarIcon()
{
    // Regex Motor Tasks

    // Creating Logger
    string filename;  // name of file where we are going to log
    GetLogfileName(&filename);
    string prefix = "D:\\VisualStudioProjects\\Autotextv2\\output\\log";  // Hardcoded path here
    this->mLogger = new Logger(prefix + "\\" + filename);  // Creating Logger
    this->mLogger->log("> Started Logging <");

    // Reading YAML file
    this->mLogger->log("Reading content of yaml input file");

    char yamlWordPath[] = "D:\\VisualStudioProjects\\Autotextv2\\wordsTest.yaml";
    this->mPatterns = getWordPatternsFromFile(yamlWordPath);

    this->mLogger->log("Data read successfully !");

    // Creating regex Motor
    this->mMotor = createMotor(this->mPatterns->words);
    this->mLogger->log("Motor created");

    paramsForCallback* myParams = (paramsForCallback *) malloc(sizeof(paramsForCallback));
    myParams->motor = this->mMotor;
    myParams->patterns = this->mPatterns;

    installhook();
    this->mLogger->log("Keyboard hook injected");

    this->mReceiver = StartReceiverAsThread(callbackWrapper, (void*) myParams, 0);
    this->mLogger->log("Receiver started with motor step as callback");

    // Finally, UI tasks
    this->myIcon = wxIcon(wxIconLocation("D:/VisualStudioProjects/Autotextv2/UI/icon.ico"));
    this->SetIcon(this->myIcon);

    this->mFrame = NULL;
}

wxMenu* mainTrayIcon::CreatePopupMenu()
{
    wxMenu *trayMenu = new wxMenu;
    trayMenu->Append(ID_TRAY_HOME, "&Home");  // Home button
    trayMenu->Append(ID_TRAY_EXIT, "&Exit");  // Exit button

    Bind(wxEVT_MENU, &mainTrayIcon::OnHome, this, ID_TRAY_HOME);
    Bind(wxEVT_MENU, &mainTrayIcon::OnExit, this, ID_TRAY_EXIT);

    return trayMenu;
}

void mainTrayIcon::OnHome(wxCommandEvent& event)
{
    if (this->mFrame)  // If frame already exists
    {
        bool wasMaximized = this->mFrame->IsMaximized();
        if (this->mFrame->IsIconized())  // If window has been minimized
        {
            this->mFrame->Restore();  // This will also restore to normal size if the window was maximized
        }
        if (wasMaximized)
        {
            this->mFrame->Maximize();
        }
        this->mFrame->SetFocus();
        this->mFrame->Show();
    }
    else
    {
        // Creating frame
        this->mFrame = new MainFrame("Autotext", this->mFrame);
        this->mFrame->SetSize(wxSize(350, 330));
        this->mFrame->SetIcon(myIcon);
        this->mFrame->Show(true);
    }
}

void mainTrayIcon::OnExit(wxCommandEvent& event)
{
    // Clear memory of the regex motor
    this->mLogger->log("Clearing memory ...");

    StopReceiver(this->mReceiver);
    this->mLogger->log("Destroyed Receiver");

    UnHook();
    this->mLogger->log("Removed Hook");

    destroyMotor(this->mMotor);
    this->mLogger->log("Destroyed regex Motor");

    destroyWordPatterns(this->mPatterns);
    this->mLogger->log("Destroyed words pattern");

    this->mLogger->log("> Exiting program <");

    if (this->mFrame)
    {
        this->mFrame->Close();
    }
    this->Destroy();
}

// RegexMotor Function
// Wrapper for nextStep function to send it to thread as a callback
int callbackWrapper(char* c, void* params)
{
    paramsForCallback* paramsCasted = (paramsForCallback*) params;  // Casting argument to correct type
    reMotor* motor = paramsCasted->motor;
    wordPatterns* patterns = paramsCasted->patterns;

    if(*c == '\b')  // Handle the special case of backspaces
    {
        undoLastStep(motor);
        plotMotor(motor);  // Display motor
        return 0;
    }

    nextStep(motor, *c);  // Calls next step
    plotMotor(motor);  // Display motor
    if (int finalStateIndex = checkAndGetFinals(motor))  // If we hit a final value
    {
        int convertedStateIndex = getIndexOfValue(motor->FinalStates, finalStateIndex);
        char* wordDetected = getStrListeValue(motor->FinalStrings, convertedStateIndex);
        char* replacer = getReplacerFromWord(patterns, wordDetected);
        sendBackspacesInputs(strlen(wordDetected));
        writeGivenString(replacer, strlen(replacer));

        restartMotorForNextRun(motor);
        return 1;
    }
    return 0;
}
