!include "MUI.nsh"

Name "JAGS"
OutFile "jags-${VERSION}-setup.exe"
InstallDir "$PROGRAMFILES\JAGS\JAGS-${VERSION}"
InstallDirRegKey HKEY_LOCAL_MACHINE "SOFTWARE\JAGS-${VERSION}" ""
RequestExecutionLevel admin

Var SM_FOLDER
Var MUI_TEMP

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "../COPYING"
!insertmacro MUI_PAGE_DIRECTORY

;Start Menu Folder Page Configuration
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "SOFTWARE\JAGS-${VERSION}" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

!insertmacro MUI_PAGE_STARTMENU Application $SM_FOLDER
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

Section # Default section

   SetOutPath "$INSTDIR"
   File /r inst\bin

   SetOutPath "$INSTDIR\bin"
   File inst\libexec\jags-terminal.exe

   SetOutPath "$INSTDIR\lib"
   File inst\lib\*.dll.a
   File inst\lib\*.la

   SetOutPath "$INSTDIR\include"
   File inst\include\JAGS\*.h
   File /r inst\include\JAGS\*

   SetOutPath "$INSTDIR\modules"
   File inst\lib\JAGS\modules\basemod.dll
   File inst\lib\JAGS\modules\basemod.dll.a
   File inst\lib\JAGS\modules\basemod.la
   File inst\lib\JAGS\modules\bugs.dll
   File inst\lib\JAGS\modules\bugs.dll.a
   File inst\lib\JAGS\modules\bugs.la
   File inst\lib\JAGS\modules\mix.dll
   File inst\lib\JAGS\modules\mix.dll.a
   File inst\lib\JAGS\modules\mix.la
   File inst\lib\JAGS\modules\msm.dll
   File inst\lib\JAGS\modules\msm.dll.a
   File inst\lib\JAGS\modules\msm.la
   File inst\lib\JAGS\modules\dic.dll
   File inst\lib\JAGS\modules\dic.dll.a
   File inst\lib\JAGS\modules\dic.la

   Push @JAGS_HOME@               #text to be replaced
   Push $INSTDIR                  #replace with
   Push all                       #replace all occurrences
   Push all                       #replace all occurrences
   Push $INSTDIR\bin\jags.bat     #file to replace in
   Call AdvReplaceInFile

   WriteRegStr HKLM "SOFTWARE\JAGS-${VERSION}" "Install_Dir" "$INSTDIR"
   WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\JAGS-${VERSION}" "DisplayName" "JAGS ${VERSION}"
   WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\JAGS-${VERSION}" "UninstallString" '"$INSTDIR\uninst.exe"'
   WriteUninstaller "$INSTDIR\uninst.exe"

   !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
      CreateDirectory "$SMPROGRAMS\$SM_FOLDER"
      # The CreateShortCut function takes the current output path to be
      # the working directory for the shortcut
      SetOutPath "%USERPROFILE%"
      CreateShortCut "$SMPROGRAMS\$SM_FOLDER\JAGS ${VERSION}.lnk" "$INSTDIR\bin\jags.bat"
      SetOutPath ""
      CreateShortCut "$SMPROGRAMS\$SM_FOLDER\Uninstall JAGS ${VERSION}.lnk" "$INSTDIR\uninst.exe"
   !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd #End of default section

Section "Uninstall"

  #Hopefully the user didn't put anything in these directories!
  RMDir /r "$INSTDIR\bin"
  RMDir /r "$INSTDIR\lib"
  RMDir /r "$INSTDIR\include"
  #Modules have to be handled more carefully, in case there are 
  #custom user-installed modules
  Delete "$INSTDIR\modules\basemod.dll"
  Delete "$INSTDIR\modules\basemod.dll.a"
  Delete "$INSTDIR\modules\basemod.la"
  Delete "$INSTDIR\modules\bugs.dll"
  Delete "$INSTDIR\modules\bugs.dll.a"
  Delete "$INSTDIR\modules\bugs.la"
  Delete "$INSTDIR\modules\mix.dll"
  Delete "$INSTDIR\modules\mix.dll.a"
  Delete "$INSTDIR\modules\mix.la"
  Delete "$INSTDIR\modules\msm.dll"
  Delete "$INSTDIR\modules\msm.dll.a"
  Delete "$INSTDIR\modules\msm.la"
  Delete "$INSTDIR\modules\dic.dll"
  Delete "$INSTDIR\modules\dic.dll.a"
  Delete "$INSTDIR\modules\dic.la"
  RMDir $INSTDIR\modules"
  #Uninstaller
  Delete "$INSTDIR\uninst.exe"

  RMDir "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
    
  Delete "$SMPROGRAMS\$MUI_TEMP\JAGS ${VERSION}.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall JAGS ${VERSION}.lnk"
  
  ;Delete empty start menu parent diretories
  StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
 
  startMenuDeleteLoop:
	ClearErrors
    RMDir $MUI_TEMP
    GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
    
    IfErrors startMenuDeleteLoopDone
  
    StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
  startMenuDeleteLoopDone:

  DeleteRegKey HKLM "SOFTWARE\JAGS-${VERSION}"
  DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\JAGS-${VERSION}"
  DeleteRegKey /ifempty HKCU "SOFTWARE\JAGS-${VERSION}"

SectionEnd # end of uninstall section

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
