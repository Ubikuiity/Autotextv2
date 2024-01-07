# name the installer
OutFile "Installer.exe"

# define the directory to install to, the desktop in this case as specified
# by the predefined $DESKTOP variable
InstallDir $PROGRAMFILES\Autotext

Page components
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

# default section
Section "Autotext"
    # define the output path for this file
    SetOutPath $INSTDIR

    WriteUninstaller "uninstallAutotext.exe"

    # Local directory containing files
    !cd "..\build\UI\Debug"

    # Put files in install directory
    File keyBoardHook.dll
    File uiMain.exe
    File words.yaml

    CreateDirectory $INSTDIR\res
    File /oname=res\icon.ico res\icon.ico

    # Registry entries
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext" "DisplayName" "Autotext"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext" "DisplayIcon" "$INSTDIR\uiMain.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext" "UninstallString" "$INSTDIR\uninstallAutotext.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext" "InstallLocation" "$INSTDIR"
SectionEnd

Section "Startup Menu Icon"
    CreateShortCut "$SMPROGRAMS\Autotext.lnk" "$INSTDIR\uiMain.exe"
SectionEnd

# Seems this section gets flagged by Windows defender, maybe I should do it on user only
Section "Start Autotext with system"
    # For current user : HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "Autotext" "$INSTDIR\uiMain.exe"
    # Add registry entry to HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
SectionEnd

Section "Uninstall"
    # Remove only files of Autotext
    Delete "$INSTDIR\keyBoardHook.dll"
    Delete "$INSTDIR\uiMain.exe"
    Delete "$INSTDIR\words.yaml"

    Delete "$INSTDIR\res\icon.ico"
    RMDir "$INSTDIR\res"

    Delete "$INSTDIR\uninstallAutotext.exe"

    # This removes the directory only if empty
    RMDir "$INSTDIR"

    # Remove possible start menu shortcut
    Delete "$SMPROGRAMS\Autotext.lnk"

    # Delete registry entries
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext"
    # Next line raises error flag if value doesn't exist, not a problem since we don't catch these flags
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "Autotext"
SectionEnd
