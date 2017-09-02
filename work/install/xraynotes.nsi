;///////////////////////////////////////////////////////////////////////////
;// The XRAYNOTES install file
; Make sure to uncomment DICOM series before distribution

;--------------------------------
;Include Modern UI

	!include "MUI2.nsh"

	!insertmacro MUI_DEFAULT MUI_UNWELCOMEFINISHPAGE_BITMAP "xnt.bmp"
	!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "xnt.bmp"
	
;--------------------------------
;Name and file

  Name "XrayNotes"

;--------------------------------
;Build target selection

	; Define this for no samples build
#	!define NODICOM 1

    !echo "------------------------"
    !echo "Build XrayNotes Original Version"

    OutFile ".\bin\XrayNotes.exe"
    !define BANNER  "logo.jpg"

    ;Default installation folder
    InstallDir "$PROGRAMFILES\XrayNotes"

    BrandingText " XrayNotes Medical Image Annotation and Data Processing Software "

    ;Get installation folder from registry if available
    InstallDirRegKey HKCU "Software\XrayNotes" ""

;--------------------------------
;Interface Settings
	
	!define	MUI_HEADERIMAGE
	!define MUI_HEADERIMAGE_BITMAP xntheader.bmp
	!define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_COMPONENTS

  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
 
  !define 	 	MUI_FINISHPAGE_RUN $PROGRAMFILES\XrayNotes\XrayNotes.exe
  !insertmacro 	MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

    !insertmacro MUI_LANGUAGE "English"
	
    icon XrayNotes.ico
    uninstallicon XrayNotes.ico
	
; -------------------------------------------------------------------------
;Installer Sections

Section "-Install XrayNotes Code" SecCode

    SetShellVarContext all

    SetOutPath "$INSTDIR"

	;MessageBox MB_OK $EXEDIR
	
    ; Add your own files here...
    File  /oname=XrayNotes.exe ..\app\Release\xraynotes.exe

    ; Copy logo to target
    ;File  /oname=logo.jpg ${BANNER}

    ; Copy OEM FILE to target
    ;File /oname=${XBANNER} oem.txt

	File  x-ray.bmp
    File  Readme.txt
    File  License.txt
    File  /oname=default.ges.org default.gesture
	
	;// Python output
	File  /r /x ".svn" /x "*.obj" /x "*.pch" /x "*.pdb" /x ".ilk" /x "*.idb" /x "*.exp" /x "*.ncb" /x "*.opt" /x "*.plg" ..\app\python
	
	;// Image processing plugins
	File  /r /x "*.svn" /x "*.obj" /x "*.pch" /x "*.pdb" /x "*.ilk" /x "*.idb" /x "*.exp" /x "*.ncb" /x "*.opt" /x "*.plg" "..\app\plugins"
	
	;// Plugin source code
	File  /r /x "*.svn" /x "*.obj" /x "*.pch" /x "*.pdb" /x "*.ilk" /x "*.idb" /x "*.exp" /x "*.ncb" /x "*.opt" /x "*.plg" "..\plugins"
	
    ; User manual
    File  /r /x ".svn" /x "*.cpt"  /x "images" .\umanual

    SetOutPath "$SYSDIR"

    File  "..\app\sqlite3.dll"

    SetOverwrite off
    File  /nonfatal .\dll\MFC42D.DLL
    File  /nonfatal .\dll\MSVCRTD.DLL
    SetOverwrite on

    ; ---------------------------------------------------------------------
    SetOutPath $INSTDIR

    CreateDirectory $SMPROGRAMS\XrayNotes

    CreateShortCut "$SMPROGRAMS\XrayNotes\XrayNotes.lnk" "$INSTDIR\XrayNotes.exe" "" \
                    "$INSTDIR\XrayNotes.exe" 0  SW_SHOWNORMAL  "ALT|CONTROL|X"

    CreateShortCut "$SMPROGRAMS\XrayNotes\XrayNotes Reset.lnk" "$INSTDIR\XrayNotes.exe" "/reset" \
                    "$INSTDIR\XrayNotes.exe" 0  SW_SHOWNORMAL  ""

    CreateShortCut "$QUICKLAUNCH\XrayNotes.lnk" "$INSTDIR\XrayNotes.exe"
    CreateShortCut "$SENDTO\XrayNotes.lnk" "$INSTDIR\XrayNotes.exe"

    CreateShortCut "$SMPROGRAMS\XrayNotes\XrayNotes Web Site.lnk" "http:\\www.XrayNotes.com" "" \
                    "" 0  SW_SHOWNORMAL  "ALT|CONTROL|W"

    CreateDirectory $SMPROGRAMS\XrayNotes\Documents
    CreateShortCut "$SMPROGRAMS\XrayNotes\Documents\Readme.lnk" "$INSTDIR\Readme.txt"
    CreateShortCut "$SMPROGRAMS\XrayNotes\Documents\HTML Documentation.lnk" "$INSTDIR\umanual\index.html"

    ;CreateShortCut "$SMPROGRAMS\XrayNotes\Documents\Quick Start.lnk" "$INSTDIR\Quick_Start.doc"

    CreateDirectory $SMPROGRAMS\XrayNotes\Tools
    CreateShortCut "$SMPROGRAMS\XrayNotes\Tools\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    CreateShortCut "$SMPROGRAMS\XrayNotes\Tools\License.lnk" "$INSTDIR\License.txt"
    CreateShortCut "$DESKTOP\XrayNotes.lnk" "$INSTDIR\XrayNotes.exe"
	
    ;Store installation folder
    WriteRegStr HKCU "Software\XrayNotes" "" $INSTDIR

    ; ---------------------------------------------------------------------
    ;Register XrayNotes file as .XrayNotes handler

    WriteRegStr HKCR ".xnt" "" "XrayNotes.FileHandler"
    WriteRegStr HKCR "XrayNotes.FileHandler" "" ""
    WriteRegStr HKCR "XrayNotes.FileHandler\shell" "" ""
    WriteRegStr HKCR "XrayNotes.FileHandler\DefaultIcon" "" "$INSTDIR\XrayNotes.exe,0"
    WriteRegStr HKCR "XrayNotes.FileHandler\shell\Open\" "" "Open XrayNotes File"
    WriteRegStr HKCR "XrayNotes.FileHandler\shell\Open\command" "" '$INSTDIR\XrayNotes.exe "%1"'
    WriteRegStr HKCR "XrayNotes.FileHandler\shell\Edit" "" "Edit XrayNotes File"
    WriteRegStr HKCR "XrayNotes.FileHandler\shell\Edit\command" "" '$INSTDIR\XrayNotes.exe "%1"'
	WriteRegStr HKCR "XrayNotes.FileHandler\shell\Unpack" "" "Unpack Component Files"
    WriteRegStr HKCR "XrayNotes.FileHandler\shell\Unpack\command" "" '$INSTDIR\XrayNotes.exe "/unpack %1"'

   WriteRegStr HKCR ".gesture" "" "XrayNotes.GestureHandler"
 
    ;Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ; Create control panel unistaller
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XrayNotes"  "DisplayName" "XrayNotes Annotation Program"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XrayNotes"  "UninstallString" $INSTDIR\Uninstall.exe
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XrayNotes"  "DisplayIcon" $INSTDIR\XrayNotes.exe,0"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XrayNotes"  "DisplayVersion" "Version 1.0"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XrayNotes"  "HelpLink" "http://www.XrayNotes.com"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XrayNotes"  "NoModify" 1
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XrayNotes"  "NoRepair" 1

SectionEnd

Section "-XrayNotes Data"  SecData
   
	File spell.txt
	File medspell.txt
	
    CreateDirectory "C:\XrayNotes\"

    SetOutPath "C:\XrayNotes\"

    CreateDirectory "C:\XrayNotes\samples"
    CreateDirectory "C:\XrayNotes\samples\dicom"
    CreateDirectory "C:\XrayNotes\samples\dicom_ser"
    CreateDirectory "C:\XrayNotes\samples\images"
    CreateDirectory "C:\XrayNotes\samples\notes"
    
	;CreateDirectory "C:\XrayNotes\images\"
	CreateDirectory "C:\XrayNotes\notes\"
	CreateDirectory "C:\XrayNotes\db\"
	CreateDirectory "C:\XrayNotes\data"
	
	File default.gesture
	
    File  /r /x ".svn" "data" 
	File  /r /x ".svn" "db"
	File  /r /x ".svn" "stock"
	
SectionEnd

Section  "Image Samples" SecImg

    SetOutPath "C:\XrayNotes\samples\"
	
	;File  /r /x ".svn" "samples\images"    
	DetailPrint "Copying image samples ..."
	CopyFiles /SILENT "$EXEDIR\samples\images" "C:\XrayNotes\samples"

SectionEnd

Section "DICOM Single Frame Samples"  SecSamp

    SetOutPath "C:\XrayNotes\samples\"
	
	;File  /r /x ".svn" "samples\dicom"
	DetailPrint "Copying DICOM samples ... "
	CopyFiles /SILENT "$EXEDIR\samples\dicom" "C:\XrayNotes\samples"

	
SectionEnd

Section  "DICOM Multi Frame Samples" SecDICOM

    SetOutPath "C:\XrayNotes\samples\"
	
    ;File /r /x ".svn" 	"samples\dicom_ser"
	DetailPrint "Copying DICOM series samples ... "
	CopyFiles /SILENT "$EXEDIR\samples\dicom_ser" "C:\XrayNotes\samples"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

    SetShellVarContext all

    Delete "$INSTDIR\Uninstall.exe"
    Delete "$INSTDIR\XrayNotes.exe"

    RMDir /r "$INSTDIR"

    Delete "$SMPROGRAMS\XrayNotes\XrayNotes.lnk"
    Delete "$SMPROGRAMS\XrayNotes\Readme.lnk"
    Delete "$SMPROGRAMS\XrayNotes\License.lnk"

    Delete "$SMPROGRAMS\XrayNotes\Guide_inside.pdf"
    Delete "$SMPROGRAMS\XrayNotes\Guide_outside.pdf"
    Delete "$SMPROGRAMS\XrayNotes\manual.pdf"

    Delete "$QUICKLAUNCH\XrayNotes.lnk"
    Delete "$DESKTOP\XrayNotes.lnk"
    Delete "$SENDTO\XrayNotes.lnk"

    RMDir /r "$SMPROGRAMS\XrayNotes\Tools\"
    RMDir /r "$SMPROGRAMS\XrayNotes\"

    DeleteRegKey HKCU "Software\XrayNotes"
    DeleteRegKey HKCR ".XrayNotes"
    DeleteRegKey HKCR "XrayNotes.FileHandler"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\XrayNotes"

SectionEnd

  LangString DESC_SecCode ${LANG_ENGLISH} "Program Code.$\r$\n$\r$\n\
  UnCheck (unselect) this if you do not want the installation to modify or update the XrayNotes executable."

  LangString DESC_SecData ${LANG_ENGLISH} "Initial Data.$\r$\n$\r$\n\
  UnCheck (unselect) this if you do not want the installation to modify or update any of your sample images."

  LangString DESC_SecGal ${LANG_ENGLISH} "Galleries and Modules.$\r$\n$\r$\n\
  UnCheck (unselect) this if you do not want the installation to modify or update any of your gallery items."

  LangString DESC_SecSamp ${LANG_ENGLISH} "Sample images/data.$\r$\n$\r$\n\
  UnCheck (unselect) this if you do not want the installation to modify or update any of your sample patient items. (safe to leave it checked)"

  LangString DESC_SecDicom ${LANG_ENGLISH} "Sample DICOM images/data.$\r$\n$\r$\n\
  UnCheck (unselect) this if you do not want the installation to modify or update any of your DICOM sample items. (safe to leave it checked)"

  LangString DESC_Img ${LANG_ENGLISH} "Sample images.$\r$\n$\r$\n\
  UnCheck (unselect) this if you do not want the installation to modify or update any of your image sample items. (safe to leave it checked)"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCode} $(DESC_SecCode)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecData} $(DESC_SecData)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDICOM} $(DESC_SecDicom)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSamp} $(DESC_SecSamp)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

