# name the installer
OutFile "Installer.exe"

# define the directory to install to, the desktop in this case as specified
# by the predefined $DESKTOP variable
InstallDir $PROGRAMFILES\Autotext

Page components
Page directory
Page Instfiles

# default section
Section "Autotext"
    # define the output path for this file
    SetOutPath $INSTDIR

    # Local directory containing files
    !cd "..\build\UI\Debug"

    # Put files in install directory
    File keyBoardHook.dll
    File uiMain.exe
    File words.yaml

    CreateDirectory $INSTDIR\res
    File /oname=res\icon.ico res\icon.ico
SectionEnd

Section "Startup Menu Icon"
    CreateShortCut "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Autotext.lnk" $INSTDIR\uiMain.exe
SectionEnd

# Seems this section gets flagged by Windows defender, maybe I should do it on user only
Section "Start Autotext with system"
    # For current user : HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "Autotext" "$INSTDIR\uiMain.exe"
    # Add registry entry to HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
SectionEnd
