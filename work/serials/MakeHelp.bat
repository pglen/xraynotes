@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by SERIALS.HPJ. >"hlp\serials.hm"
echo. >>"hlp\serials.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\serials.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\serials.hm"
echo. >>"hlp\serials.hm"
echo // Prompts (IDP_*) >>"hlp\serials.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\serials.hm"
echo. >>"hlp\serials.hm"
echo // Resources (IDR_*) >>"hlp\serials.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\serials.hm"
echo. >>"hlp\serials.hm"
echo // Dialogs (IDD_*) >>"hlp\serials.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\serials.hm"
echo. >>"hlp\serials.hm"
echo // Frame Controls (IDW_*) >>"hlp\serials.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\serials.hm"
REM -- Make help for Project SERIALS


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\serials.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\serials.hlp" goto :Error
if not exist "hlp\serials.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\serials.hlp" Debug
if exist Debug\nul copy "hlp\serials.cnt" Debug
if exist Release\nul copy "hlp\serials.hlp" Release
if exist Release\nul copy "hlp\serials.cnt" Release
echo.
goto :done

:Error
echo hlp\serials.hpj(1) : error: Problem encountered creating help file

:done
echo.
