#include "headers/mainTrayIcon.hpp"

using namespace std;

int callbackWrapper(char* c, void* params);

mainTrayIcon::mainTrayIcon() : wxTaskBarIcon(), mMotorIsRunning(false)
{
    // Need to create Appdata folder and subfolders
    int appDataCreated = createAppDataRessourcesIfNotExistent();

    // Creating Logger
    string filename;  // name of file where we are going to log
    GetLogfileName(&filename);
    string logFolder = getAppdataAsString() + "/" + PATHS_PROGRAMNAME + "/" + PATHS_LOGFOLDER;
    string logPath = logFolder + "/" + filename;

    int logsCleared = clearOldLogs(logFolder, 5);  // Removes all logs older than 5 days

    this->mLogger = new Logger(logPath);  // Creating Logger
    this->mLogger->log("> Started Logging <");
    this->mLogger->log("AppData files created (code) : " + to_string(appDataCreated));
    this->mLogger->log("Removed " + to_string(logsCleared) + " old log files");

    this->startEmbeddedMotor();

    // UI tasks
    this->myIcon = wxIcon(wxIconLocation(getPathOfExeAsString() + "/" + PATHS_RESFOLDER + "/" + PATHS_ICONFILE));
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
        this->mFrame = new MainFrame("Autotext", this);
        this->mFrame->SetSize(wxSize(350, 330));
        this->mFrame->SetIcon(myIcon);
        this->mFrame->Show(true);
    }
}

void mainTrayIcon::OnExit(wxCommandEvent& event)
{
    if (this->mFrame)
    {
        this->mFrame->Close();
    }
    this->stopEmbeddedMotor();
    this->mLogger->log("> Exiting program <");
    delete this->mLogger;
    this->mLogger = NULL;
    this->Destroy();
}

// Used by subFrame to warn the trayIcon that it has been closed
void mainTrayIcon::informSubFrameClosing()
{
    this->mFrame = NULL;
}

// RegexMotor Function

// returns true is motor is running, false otherwise
bool mainTrayIcon::getMotorState()
{
    return this->mMotorIsRunning;
}

// Starts motor if stopped, stops it if started
// Returns true if a change was made, false otherwise
bool mainTrayIcon::startStopEmbeddedMotor()
{
    if (this->mMotorIsRunning)
    {
        if (stopEmbeddedMotor())
        {
            this->mFrame->SetStatusText("Detection disabled");
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (startEmbeddedMotor())
        {
            this->mFrame->SetStatusText("Detection active");
            return true;
        }
        else
        {
            return false;
        }
    }
}

// Starts the motor inside the trayIcon
bool mainTrayIcon::startEmbeddedMotor()
{
    if(this->mMotorIsRunning)  // Pass if motor is already running
    {
        this->mLogger->log("Motor not started because already running");
        return false;
    }
    // Reading YAML file
    this->mLogger->log("Starting motor ...");
    this->mLogger->log("Reading content of yaml input file");

    string yamlWordPath = getAppdataAsString() + "/" + PATHS_PROGRAMNAME + "/" + PATHS_WORDSFILE;

    this->mPatterns = getWordPatternsFromFile(yamlWordPath.c_str());
    if (this->mPatterns == NULL)
    {
        this->mLogger->log("Coudn't start motor because there are no words detected in yaml file");
        return false;
    }

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

    this->mLogger->log("Motor started successfully");

    this->mMotorIsRunning = true;
    return true;
}

bool mainTrayIcon::stopEmbeddedMotor()
{
    if(!this->mMotorIsRunning)  // Pass if motor is not running
    {
        this->mLogger->log("Motor not stoped because not running");
        return false;
    }
    // Clear memory of the regex motor
    this->mLogger->log("Stopping motor and clearing memory ...");

    StopReceiver(this->mReceiver);
    this->mLogger->log("Destroyed Receiver");

    UnHook();
    this->mLogger->log("Removed Hook");

    destroyMotor(this->mMotor);
    this->mLogger->log("Destroyed regex Motor");

    destroyWordPatterns(this->mPatterns);
    this->mLogger->log("Destroyed words pattern");
    this->mLogger->log("Motor was stopped successfully");

    this->mMotorIsRunning = false;
    return true;
}

// Wrapper for nextStep function to send it to thread as a callback
int callbackWrapper(char* c, void* params)
{
    paramsForCallback* paramsCasted = (paramsForCallback*) params;  // Casting argument to correct type
    reMotor* motor = paramsCasted->motor;
    wordPatterns* patterns = paramsCasted->patterns;

    if(*c == '\b')  // Handle the special case of backspaces
    {
        undoLastStep(motor);
        return 0;
    }

    nextStep(motor, *c);  // Calls next step
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
