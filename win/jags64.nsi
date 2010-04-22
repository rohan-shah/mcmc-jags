!define APP_NAME "JAGS"
!define PUBLISHER "JAGS"

!define MULTIUSER_MUI
!define MULTIUSER_EXECUTIONLEVEL Highest
!define MULTIUSER_INSTALLMODE_COMMANDLINE

!define INSTDIR_REG_ROOT "SHELL_CONTEXT"
!define INSTDIR_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}-${VERSION}"

!define MULTIUSER_INSTALLMODE_INSTDIR "${PUBLISHER}\${APP_NAME}-${VERSION}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "SOFTWARE\${PUBLISHER}\${APP_NAME}-${VERSION}"
!define MULTIUSER_INSTDIR_REGISTRY_VALUENAME "InstallDir"

!addincludedir ${JAGSINC}
!include AdvUninstLog.nsh
!include MultiUser64.nsh
!include "MUI2.nsh"

Name "${APP_NAME} ${VERSION}"
OutFile "${APP_NAME}-${VERSION}-win64.exe"

Var SM_FOLDER

!define APP_REG_KEY "Software\${PUBLISHER}\${APP_NAME}-${VERSION}"
!define PUB_REG_KEY "Software\${PUBLISHER}"

;Start Menu Folder Page Configuration
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${APP_NAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${INSTDIR_REG_ROOT}" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${APP_REG_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenuFolder"

; Installer pages

!insertmacro UNATTENDED_UNINSTALL

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ${LICENSE}
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $SM_FOLDER
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

Section # Default section

   SetOutPath "$INSTDIR"
   !insertmacro UNINSTALL.LOG_OPEN_INSTALL
   File /r inst\bin
   !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

   SetOutPath "$INSTDIR\bin"
   !insertmacro UNINSTALL.LOG_OPEN_INSTALL
   File inst\libexec\jags-terminal.exe
   !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

   SetOutPath "$INSTDIR\lib"
   !insertmacro UNINSTALL.LOG_OPEN_INSTALL
   File inst\lib\*.dll.a
   File inst\lib\*.la
   !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

   SetOutPath "$INSTDIR\include"
   !insertmacro UNINSTALL.LOG_OPEN_INSTALL
   File inst\include\JAGS\*.h
   File /r inst\include\JAGS\*
   !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

   SetOutPath "$INSTDIR\modules"
   !insertmacro UNINSTALL.LOG_OPEN_INSTALL
   File /r inst\lib\JAGS\modules-${VERSION}\*
   !insertmacro UNINSTALL.LOG_CLOSE_INSTALL

   Push @JAGS_HOME@               #text to be replaced
   Push $INSTDIR                  #replace with
   Push all                       #replace all occurrences
   Push all                       #replace all occurrences
   Push $INSTDIR\bin\jags.bat     #file to replace in
   Call AdvReplaceInFile

   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "InstallDir" "$INSTDIR"
   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "DisplayName" "JAGS ${VERSION}"
   WriteRegStr ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}" "UninstallString" "${UNINST_EXE}"
   WriteRegStr ${INSTDIR_REG_ROOT} "${APP_REG_KEY}"     "InstallDir" "$INSTDIR"

   !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
      CreateDirectory "$SMPROGRAMS\$SM_FOLDER"
      # The CreateShortCut function takes the current output path to be
      # the working directory for the shortcut
      SetOutPath "%USERPROFILE%"
      CreateShortCut "$SMPROGRAMS\$SM_FOLDER\JAGS ${VERSION}.lnk" "$INSTDIR\bin\jags.bat"
      SetOutPath ""
      ;create shortcut for uninstaller always use ${UNINST_EXE} instead of uninstall.exe
      CreateShortCut "$SMPROGRAMS\$SM_FOLDER\Uninstall JAGS ${VERSION}.lnk" "${UNINST_EXE}"
   !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd #End of default section

Function .onInit
   SetRegView 64
   !insertmacro MULTIUSER_INIT
   !insertmacro UNINSTALL.LOG_PREPARE_INSTALL
FunctionEnd

Function .onInstSuccess
   ;create/update log always within .onInstSuccess function
   !insertmacro UNINSTALL.LOG_UPDATE_INSTALL
FunctionEnd

Section "Uninstall"

   ;uninstall from path, must be repeated for every install logged path individual
   !insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR"
   !insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR\bin"
   !insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR\lib"
   !insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR\include"
   !insertmacro UNINSTALL.LOG_UNINSTALL "$INSTDIR\modules"
   !insertmacro UNINSTALL.LOG_END_UNINSTALL

   RMDir "$INSTDIR"
  
   !insertmacro MUI_STARTMENU_GETFOLDER Application $SM_FOLDER
    
   Delete "$SMPROGRAMS\$SM_FOLDER\JAGS ${VERSION}.lnk"
   Delete "$SMPROGRAMS\$SM_FOLDER\Uninstall JAGS ${VERSION}.lnk"
  
   ;Delete empty start menu parent diretories
   StrCpy $SM_FOLDER "$SMPROGRAMS\$SM_FOLDER"
 
   startMenuDeleteLoop:
      ClearErrors
      RMDir $SM_FOLDER
   GetFullPathName $SM_FOLDER "$SM_FOLDER\.."
    
   IfErrors startMenuDeleteLoopDone
  
   StrCmp $SM_FOLDER $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
   startMenuDeleteLoopDone:

   DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${INSTDIR_REG_KEY}"
   DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${APP_REG_KEY}"
   DeleteRegKey /ifempty ${INSTDIR_REG_ROOT} "${PUB_REG_KEY}"

SectionEnd # end of uninstall section

Function un.onInit
   !insertmacro MULTIUSER_UNINIT
   !insertmacro UNINSTALL.LOG_BEGIN_UNINSTALL
FunctionEnd

#This is a function taken from the NSIS Wiki to replace one text string
#with another.  It doesn't work properly if there is more than one instance
#of the replacement string on a line

Function AdvReplaceInFile
         Exch $0 ;file to replace in
         Exch
         Exch $1 ;number to replace after
         Exch
         Exch 2
         Exch $2 ;replace and onwards
         Exch 2
         Exch 3
         Exch $3 ;replace with
         Exch 3
         Exch 4
         Exch $4 ;to replace
         Exch 4
         Push $5 ;minus count
         Push $6 ;universal
         Push $7 ;end string
         Push $8 ;left string
         Push $9 ;right string
         Push $R0 ;file1
         Push $R1 ;file2
         Push $R2 ;read
         Push $R3 ;universal
         Push $R4 ;count (onwards)
         Push $R5 ;count (after)
         Push $R6 ;temp file name
         GetTempFileName $R6
         FileOpen $R1 $0 r ;file to search in
         FileOpen $R0 $R6 w ;temp file
                  StrLen $R3 $4
                  StrCpy $R4 -1
                  StrCpy $R5 -1
        loop_read:
         ClearErrors
         FileRead $R1 $R2 ;read line
         IfErrors exit
         StrCpy $5 0
         StrCpy $7 $R2
 
        loop_filter:
         IntOp $5 $5 - 1
         StrCpy $6 $7 $R3 $5 ;search
         StrCmp $6 "" file_write2
         StrCmp $6 $4 0 loop_filter
 
         StrCpy $8 $7 $5 ;left part
         IntOp $6 $5 + $R3
         StrCpy $9 $7 "" $6 ;right part
         StrCpy $7 $8$3$9 ;re-join
 
         IntOp $R4 $R4 + 1
         StrCmp $2 all file_write1
         StrCmp $R4 $2 0 file_write2
         IntOp $R4 $R4 - 1
 
         IntOp $R5 $R5 + 1
         StrCmp $1 all file_write1
         StrCmp $R5 $1 0 file_write1
         IntOp $R5 $R5 - 1
         Goto file_write2
 
        file_write1:
         FileWrite $R0 $7 ;write modified line
         Goto loop_read
 
        file_write2:
         FileWrite $R0 $R2 ;write unmodified line
         Goto loop_read
 
        exit:
         FileClose $R0
         FileClose $R1
 
         SetDetailsPrint none
         Delete $0
         Rename $R6 $0
         Delete $R6
         SetDetailsPrint both
 
         Pop $R6
         Pop $R5
         Pop $R4
         Pop $R3
         Pop $R2
         Pop $R1
         Pop $R0
         Pop $9
         Pop $8
         Pop $7
         Pop $6
         Pop $5
         Pop $4
         Pop $3
         Pop $2
         Pop $1
         Pop $0
FunctionEnd
