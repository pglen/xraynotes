@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by GEST.HPJ. >"hlp\gest.hm"
echo. >>"hlp\gest.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\gest.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\gest.hm"
echo. >>"hlp\gest.hm"
echo // Prompts (IDP_*) >>"hlp\gest.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\gest.hm"
echo. >>"hlp\gest.hm"
echo // Resources (IDR_*) >>"hlp\gest.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\gest.hm"
echo. >>"hlp\gest.hm"
echo // Dialogs (IDD_*) >>"hlp\gest.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\gest.hm"
echo. >>"hlp\gest.hm"
echo // Frame Controls (IDW_*) >>"hlp\gest.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\gest.hm"
REM -- Make help for Project GEST


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\gest.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\gest.hlp" goto :Error
if not exist "hlp\gest.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\gest.hlp" Debug
if exist Debug\nul copy "hlp\gest.cnt" Debug
if exist Release\nul copy "hlp\gest.hlp" Release
if exist Release\nul copy "hlp\gest.cnt" Release
echo.
goto :done

:Error
echo hlp\gest.hpj(1) : error: Problem encountered creating help file

:done
echo.
