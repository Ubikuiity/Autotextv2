# name the installer
OutFile "AutotextInstaller.exe"

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

    File "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Redist\MSVC\14.29.30133\vcredist_x64.exe"
    ExecWait "$INSTDIR\vcredist_x64.exe /install /passive"
    Delete "$INSTDIR\vcredist_x64.exe"

    # Local directory containing files
    !cd "..\build\UI\Release"

    # Put files in install directory
    File keyBoardHook.dll
    File uiMain.exe
    # File words.yaml (Created by exe)

    CreateDirectory "$INSTDIR\res"
    File /oname=res\icon.ico res\icon.ico

    # CreateDirectory "$INSTDIR\log" (Created by exe)

    # Registry entries
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext" "DisplayName" "Autotext"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext" "DisplayIcon" "$INSTDIR\uiMain.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext" "UninstallString" "$INSTDIR\uninstallAutotext.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext" "InstallLocation" "$INSTDIR"

    # Possibility to start with Windows
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "Autotext" "$INSTDIR\uiMain.exe"
    # By default we set registry to disable startup with Windows
    WriteRegBin HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\StartupApproved\Run" "Autotext" 030000000000000000000000
SectionEnd

Section "Startup Menu Icon"
    CreateShortCut "$SMPROGRAMS\Autotext.lnk" "$INSTDIR\uiMain.exe"
SectionEnd

# Seems this section gets flagged by Windows defender, maybe I should do it on user only
Section "Start Autotext with system"
    WriteRegBin HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\StartupApproved\Run" "Autotext" 020000000000000000000000
SectionEnd

Section "Uninstall"
    # Remove only files of Autotext
    Delete "$INSTDIR\keyBoardHook.dll"
    Delete "$INSTDIR\uiMain.exe"
    # Delete "$INSTDIR\words.yaml"

    Delete "$INSTDIR\res\icon.ico"
    RMDir "$INSTDIR\res"

    RMDir /r "$APPDATA\Autotext\log"

    Delete "$INSTDIR\uninstallAutotext.exe"

    # This removes the directory only if empty
    RMDir "$INSTDIR"

    # Remove possible start menu shortcut
    Delete "$SMPROGRAMS\Autotext.lnk"

    # Delete registry entries
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Autotext"
    # Next line raises error flag if value doesn't exist, not a problem since we don't catch these flags
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "Autotext"
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\StartupApproved\Run" "Autotext"
SectionEnd
