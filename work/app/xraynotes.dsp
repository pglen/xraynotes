# Microsoft Developer Studio Project File - Name="xraynotes" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=xraynotes - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xraynotes.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xraynotes.mak" CFG="xraynotes - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xraynotes - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "xraynotes - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xraynotes - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\common\work" /I "..\common" /I "include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../png/Release/png.lib ../jpeg/Release/jpeg.lib ../zlib/Release/zlib.lib ../tiff/Release/tiff.lib ../j2k/Release/j2k.lib ../jbig/Release/jbig.lib ../jasper/Release/jasper.lib ../cximage/Release/cximage.lib wininet.lib msimg32.lib rpcrt4.lib sqlite3.lib winmm.lib user32.lib python25.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "xraynotes - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\common\work" /I "..\common" /I "include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../png/Debug/png.lib ../jpeg/Debug/jpeg.lib ../zlib/Debug/zlib.lib ../tiff/Debug/tiff.lib ../j2k/Debug/j2k.lib ../jbig/Debug/jbig.lib ../jasper/Debug/jasper.lib ../cximage/Debug/cximage.lib wininet.lib msimg32.lib rpcrt4.lib sqlite3.lib winmm.lib user32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "xraynotes - Win32 Release"
# Name "xraynotes - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\2dCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\AddrEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\AddrEntry.h
# End Source File
# Begin Source File

SOURCE=.\AlaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Alpha.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpButt.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpDC.cpp
# End Source File
# Begin Source File

SOURCE=.\BriCon.cpp
# End Source File
# Begin Source File

SOURCE=.\CalCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\calen.cpp
# End Source File
# Begin Source File

SOURCE=.\calen.h
# End Source File
# Begin Source File

SOURCE=.\Chevron.cpp
# End Source File
# Begin Source File

SOURCE=.\CLayImg.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeEd.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeEd.h
# End Source File
# Begin Source File

SOURCE=.\ColDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Curve.cpp
# End Source File
# Begin Source File

SOURCE=.\CurveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DateSel.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagList.cpp
# End Source File
# Begin Source File

SOURCE=.\DicomFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DicomWin.cpp
# End Source File
# Begin Source File

SOURCE=.\DiMore.cpp
# End Source File
# Begin Source File

SOURCE=.\do3dp.cpp
# End Source File
# Begin Source File

SOURCE=.\DocList.cpp
# End Source File
# Begin Source File

SOURCE=.\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\EventDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDialogST.cpp
# End Source File
# Begin Source File

SOURCE=.\Flag.cpp
# End Source File
# Begin Source File

SOURCE=.\Flyout.cpp
# End Source File
# Begin Source File

SOURCE=.\GdiRot.cpp
# End Source File
# Begin Source File

SOURCE=.\gestureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GetText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\GrayDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\GrayDlg.h
# End Source File
# Begin Source File

SOURCE=.\HistoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HShield.cpp
# End Source File
# Begin Source File

SOURCE=.\INeuNet.cpp
# End Source File
# Begin Source File

SOURCE=.\INeuron.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\label.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\label.h
# End Source File
# Begin Source File

SOURCE=.\LayCirc.cpp
# End Source File
# Begin Source File

SOURCE=.\Layer.cpp
# End Source File
# Begin Source File

SOURCE=.\LayLines.cpp
# End Source File
# Begin Source File

SOURCE=.\LayRect.cpp
# End Source File
# Begin Source File

SOURCE=.\LaySline.cpp
# End Source File
# Begin Source File

SOURCE=.\LayText.cpp
# End Source File
# Begin Source File

SOURCE=.\lcd7.cpp
# End Source File
# Begin Source File

SOURCE=.\LevenStein.cpp
# End Source File
# Begin Source File

SOURCE=.\Light.cpp
# End Source File
# Begin Source File

SOURCE=.\lineProp.cpp
# End Source File
# Begin Source File

SOURCE=.\lines.cpp
# End Source File
# Begin Source File

SOURCE=.\MagDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MagWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MedReg.cpp
# End Source File
# Begin Source File

SOURCE=.\memdc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\misc.h
# End Source File
# Begin Source File

SOURCE=.\MsgBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\multimon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\mxpad.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\mxpad.h
# End Source File
# Begin Source File

SOURCE=.\parse.cpp
# End Source File
# Begin Source File

SOURCE=.\PatDmg.cpp
# End Source File
# Begin Source File

SOURCE=.\pcSelection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\PersistDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\PersistDlg.h
# End Source File
# Begin Source File

SOURCE=.\PGedit.cpp
# End Source File
# Begin Source File

SOURCE=.\PGtree.cpp
# End Source File
# Begin Source File

SOURCE=.\PicList.cpp
# End Source File
# Begin Source File

SOURCE=.\PicSel.cpp
# End Source File
# Begin Source File

SOURCE=.\Plot.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\Popup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\Popup.h
# End Source File
# Begin Source File

SOURCE=.\prl.cpp
# End Source File
# Begin Source File

SOURCE=.\prl.h
# End Source File
# Begin Source File

SOURCE=.\ProcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PRocList.cpp
# End Source File
# Begin Source File

SOURCE=.\pybind.cpp
# End Source File
# Begin Source File

SOURCE=.\PyGlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrKbd.cpp
# End Source File
# Begin Source File

SOURCE=.\SelHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\sellist.cpp
# End Source File
# Begin Source File

SOURCE=.\shades.cpp
# End Source File
# Begin Source File

SOURCE=.\shadesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadesFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadesHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadesSel.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeProp.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\Status.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Support.cpp
# End Source File
# Begin Source File

SOURCE=.\TestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TextMode.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProp.cpp
# End Source File
# Begin Source File

SOURCE=.\tools.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolShow.cpp
# End Source File
# Begin Source File

SOURCE=.\Transcript.cpp
# End Source File
# Begin Source File

SOURCE=.\transprog.cpp
# End Source File
# Begin Source File

SOURCE=.\TransSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\TransStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Triangle.cpp
# End Source File
# Begin Source File

SOURCE=.\UniqID.cpp
# End Source File
# Begin Source File

SOURCE=.\xcalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\xCalen.cpp
# End Source File
# Begin Source File

SOURCE=.\xConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\xFile.cpp
# End Source File
# Begin Source File

SOURCE=.\xImg.cpp
# End Source File
# Begin Source File

SOURCE=.\xntmask.cpp
# End Source File
# Begin Source File

SOURCE=.\xPtrArr.cpp
# End Source File
# Begin Source File

SOURCE=.\XrayDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\XrayM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\XrayM.h
# End Source File
# Begin Source File

SOURCE=.\hlp\xraynotes.hpj

!IF  "$(CFG)" == "xraynotes - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help file...
OutDir=.\Release
InputPath=.\hlp\xraynotes.hpj
InputName=xraynotes

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "xraynotes - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help file...
OutDir=.\Debug
InputPath=.\hlp\xraynotes.hpj
InputName=xraynotes

"$(OutDir)\$(InputName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	start /wait hcw /C /E /M "hlp\$(InputName).hpj" 
	if errorlevel 1 goto :Error 
	if not exist "hlp\$(InputName).hlp" goto :Error 
	copy "hlp\$(InputName).hlp" $(OutDir) 
	goto :done 
	:Error 
	echo hlp\$(InputName).hpj(1) : error: 
	type "hlp\$(InputName).log" 
	:done 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\xraynotes.rc
# End Source File
# Begin Source File

SOURCE=.\xRect.cpp
# End Source File
# Begin Source File

SOURCE=.\xScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\xStr.cpp
# End Source File
# Begin Source File

SOURCE=.\xTargetButton.cpp
# End Source File
# Begin Source File

SOURCE=.\xTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\xyzDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\yScroll.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\2dCtl.h
# End Source File
# Begin Source File

SOURCE=.\about.h
# End Source File
# Begin Source File

SOURCE=.\AddrEntry1.h
# End Source File
# Begin Source File

SOURCE=.\AlaDlg.h
# End Source File
# Begin Source File

SOURCE=.\Alpha.h
# End Source File
# Begin Source File

SOURCE=.\BaseWnd.h
# End Source File
# Begin Source File

SOURCE=.\BmpButt.h
# End Source File
# Begin Source File

SOURCE=.\BmpDC.h
# End Source File
# Begin Source File

SOURCE=.\BriCon.h
# End Source File
# Begin Source File

SOURCE=.\CalCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Chevron.h
# End Source File
# Begin Source File

SOURCE=.\ColDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommLine.h
# End Source File
# Begin Source File

SOURCE=.\CTimeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Curve.h
# End Source File
# Begin Source File

SOURCE=.\CurveDlg.h
# End Source File
# Begin Source File

SOURCE=.\DateSel.h
# End Source File
# Begin Source File

SOURCE=.\dbdef.h
# End Source File
# Begin Source File

SOURCE=.\DBMain.h
# End Source File
# Begin Source File

SOURCE=.\DiagList.h
# End Source File
# Begin Source File

SOURCE=.\DicomFile.h
# End Source File
# Begin Source File

SOURCE=.\DicomWin.h
# End Source File
# Begin Source File

SOURCE=.\DiMore.h
# End Source File
# Begin Source File

SOURCE=.\do3dp.h
# End Source File
# Begin Source File

SOURCE=.\DocList.h
# End Source File
# Begin Source File

SOURCE=.\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\EventDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileDialogST.h
# End Source File
# Begin Source File

SOURCE=.\Flag.h
# End Source File
# Begin Source File

SOURCE=.\Flyout.h
# End Source File
# Begin Source File

SOURCE=.\GdiRot.h
# End Source File
# Begin Source File

SOURCE=.\gestureDlg.h
# End Source File
# Begin Source File

SOURCE=.\GetText.h
# End Source File
# Begin Source File

SOURCE=.\helperdlgs.h
# End Source File
# Begin Source File

SOURCE=.\HistoryDlg.h
# End Source File
# Begin Source File

SOURCE=.\HShield.h
# End Source File
# Begin Source File

SOURCE=.\INeuNet.h
# End Source File
# Begin Source File

SOURCE=.\INeuron.h
# End Source File
# Begin Source File

SOURCE=.\LayCirc.h
# End Source File
# Begin Source File

SOURCE=.\Layer.h
# End Source File
# Begin Source File

SOURCE=.\LayImg.h
# End Source File
# Begin Source File

SOURCE=.\LayLines.h
# End Source File
# Begin Source File

SOURCE=.\LayRect.h
# End Source File
# Begin Source File

SOURCE=.\LaySline.h
# End Source File
# Begin Source File

SOURCE=.\LayText.h
# End Source File
# Begin Source File

SOURCE=.\lcd7.h
# End Source File
# Begin Source File

SOURCE=.\LevenStein.h
# End Source File
# Begin Source File

SOURCE=.\Light.h
# End Source File
# Begin Source File

SOURCE=.\lineProp.h
# End Source File
# Begin Source File

SOURCE=.\lines.h
# End Source File
# Begin Source File

SOURCE=.\MagDlg.h
# End Source File
# Begin Source File

SOURCE=.\MagWnd.h
# End Source File
# Begin Source File

SOURCE=.\MedReg.h
# End Source File
# Begin Source File

SOURCE=.\memdc.h
# End Source File
# Begin Source File

SOURCE=.\MsgBox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\work\multimon.h
# End Source File
# Begin Source File

SOURCE=.\parse.h
# End Source File
# Begin Source File

SOURCE=.\PatDmg.h
# End Source File
# Begin Source File

SOURCE=.\pcSelection.h
# End Source File
# Begin Source File

SOURCE=.\PGedit.h
# End Source File
# Begin Source File

SOURCE=.\PGtree.h
# End Source File
# Begin Source File

SOURCE=.\PicList.h
# End Source File
# Begin Source File

SOURCE=.\PicSel.h
# End Source File
# Begin Source File

SOURCE=.\Plot.h
# End Source File
# Begin Source File

SOURCE=.\PlotDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProcDlg.h
# End Source File
# Begin Source File

SOURCE=.\PRocList.h
# End Source File
# Begin Source File

SOURCE=.\PyGlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "xraynotes - Win32 Release"

!ELSEIF  "$(CFG)" == "xraynotes - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=xraynotes
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ScrKbd.h
# End Source File
# Begin Source File

SOURCE=.\SelHelper.h
# End Source File
# Begin Source File

SOURCE=.\sellist.h
# End Source File
# Begin Source File

SOURCE=.\shades.h
# End Source File
# Begin Source File

SOURCE=.\shadesDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShadesFunc.h
# End Source File
# Begin Source File

SOURCE=.\ShadesHelp.h
# End Source File
# Begin Source File

SOURCE=.\ShadesSel.h
# End Source File
# Begin Source File

SOURCE=.\ShapeProp.h
# End Source File
# Begin Source File

SOURCE=.\SimpleGrid.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\Status.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Support.h
# End Source File
# Begin Source File

SOURCE=.\TestDlg.h
# End Source File
# Begin Source File

SOURCE=.\TextMode.h
# End Source File
# Begin Source File

SOURCE=.\TextProp.h
# End Source File
# Begin Source File

SOURCE=.\tooldef.h
# End Source File
# Begin Source File

SOURCE=.\tools.h
# End Source File
# Begin Source File

SOURCE=.\ToolShow.h
# End Source File
# Begin Source File

SOURCE=.\Transcript.h
# End Source File
# Begin Source File

SOURCE=.\transprog.h
# End Source File
# Begin Source File

SOURCE=.\TransSlider.h
# End Source File
# Begin Source File

SOURCE=.\TransStatic.h
# End Source File
# Begin Source File

SOURCE=.\Triangle.h
# End Source File
# Begin Source File

SOURCE=.\UniqID.h
# End Source File
# Begin Source File

SOURCE=.\worker.h
# End Source File
# Begin Source File

SOURCE=.\xcalDlg.h
# End Source File
# Begin Source File

SOURCE=.\xCalen.h
# End Source File
# Begin Source File

SOURCE=.\xConfig.h
# End Source File
# Begin Source File

SOURCE=.\xFile.h
# End Source File
# Begin Source File

SOURCE=.\xImg.h
# End Source File
# Begin Source File

SOURCE=.\XntMask.h
# End Source File
# Begin Source File

SOURCE=.\xPtrArr.h
# End Source File
# Begin Source File

SOURCE=.\XrayDoc.h
# End Source File
# Begin Source File

SOURCE=.\xraynotes.h
# End Source File
# Begin Source File

SOURCE=.\xRect.h
# End Source File
# Begin Source File

SOURCE=.\xScroll.h
# End Source File
# Begin Source File

SOURCE=.\xStr.h
# End Source File
# Begin Source File

SOURCE=.\xTargetButton.h
# End Source File
# Begin Source File

SOURCE=.\xTracker.h
# End Source File
# Begin Source File

SOURCE=.\xyzDlg.h
# End Source File
# Begin Source File

SOURCE=.\yScroll.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\aestheticzone.bmp
# End Source File
# Begin Source File

SOURCE=.\res\appoint.bmp
# End Source File
# Begin Source File

SOURCE=.\res\assets.bmp
# End Source File
# Begin Source File

SOURCE=.\res\beach.BMP
# End Source File
# Begin Source File

SOURCE=.\res\bg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bg2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bg3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap18.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap20.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap21.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap22.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap23.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap32.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap34.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap35.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor10.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor11.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor12.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor13.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor4.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor5.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor6.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor7.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor8.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor9.cur
# End Source File
# Begin Source File

SOURCE=.\res\Graphic2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Graphic3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Graphic4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graphic5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hippaa2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon7.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon8.ico
# End Source File
# Begin Source File

SOURCE=.\images\logo.bmp
# End Source File
# Begin Source File

SOURCE=..\images\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\images\logo2.bmp
# End Source File
# Begin Source File

SOURCE=..\images\logo2.bmp
# End Source File
# Begin Source File

SOURCE=.\images\logo3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo3.bmp
# End Source File
# Begin Source File

SOURCE=..\images\logo3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mboxbg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\newpat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pay.bmp
# End Source File
# Begin Source File

SOURCE=.\res\reports.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shades.ico
# End Source File
# Begin Source File

SOURCE=.\res\shades.rc2
# End Source File
# Begin Source File

SOURCE=.\res\shades2.ico
# End Source File
# Begin Source File

SOURCE=.\res\stat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tools.bmp
# End Source File
# Begin Source File

SOURCE=.\res\xray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\xray2.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section xraynotes : {3A8A3283-486D-4827-B10F-0247D6E720A9}
# 	2:19:CLASS: CTransStatic:CTransStatic
# 	2:13:TransStatic.h:TransStatic.h
# 	2:15:TransStatic.cpp:TransStatic.cpp
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {7320D7FC-7F9C-436D-8E54-48DE16C935C5}
# 	2:8:lcd7.cpp:lcd7.cpp
# 	2:6:lcd7.h:lcd7.h
# 	2:12:CLASS: Clcd7:Clcd7
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {A4CFC18D-82A7-4ED3-8781-269FE42AEE7F}
# 	1:13:IDD_PROC_LIST:115
# 	2:16:Resource Include:resource.h
# 	2:16:CLASS: CPRocList:CPRocList
# 	2:12:PRocList.cpp:PRocList.cpp
# 	2:13:IDD_PROC_LIST:IDD_PROC_LIST
# 	2:10:ENUM: enum:enum
# 	2:10:PRocList.h:PRocList.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {004E93D7-FCC3-46CA-9EDE-8CD5F1E0A1E4}
# 	1:13:IDD_EVENT_DLG:113
# 	2:16:CLASS: CEventDlg:CEventDlg
# 	2:16:Resource Include:resource.h
# 	2:10:EventDlg.h:EventDlg.h
# 	2:10:ENUM: enum:enum
# 	2:13:IDD_EVENT_DLG:IDD_EVENT_DLG
# 	2:12:EventDlg.cpp:EventDlg.cpp
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {58E9AA19-94B8-4158-A810-231AFCA65996}
# 	2:13:CLASS: BCMenu:BCMenu
# 	2:24:TYPEDEF: BC_MenuDrawMode:BC_MenuDrawMode
# 	2:18:ENUM: BC_Seperator:BC_Seperator
# 	2:18:CLASS: BCMenuMemDC:BCMenuMemDC
# 	2:17:CLASS: BCMenuData:BCMenuData
# 	2:8:BCMenu.h:BCMenu1.h
# 	2:21:TYPEDEF: BC_Seperator:BC_Seperator
# 	2:20:CLASS: CMenuItemInfo:CMenuItemInfo
# 	2:21:ENUM: BC_MenuDrawMode:BC_MenuDrawMode
# 	2:19:Application Include:xraynotes.h
# 	2:10:BCMenu.cpp:BCMenu1.cpp
# End Section
# Section xraynotes : {B76CCA1F-D8D6-478B-99AD-EE4305B90E76}
# 	1:14:IDD_ALA_DIALOG:112
# 	2:16:Resource Include:resource.h
# 	2:8:AlaDlg.h:AlaDlg.h
# 	2:14:CLASS: CAlaDlg:CAlaDlg
# 	2:10:AlaDlg.cpp:AlaDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:14:IDD_ALA_DIALOG:IDD_ALA_DIALOG
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {28F1F68E-C88D-4FF7-B153-9B6F88748CF4}
# 	2:13:CLASS: BCMenu:BCMenu
# 	2:24:TYPEDEF: BC_MenuDrawMode:BC_MenuDrawMode
# 	2:18:ENUM: BC_Seperator:BC_Seperator
# 	2:18:CLASS: BCMenuMemDC:BCMenuMemDC
# 	2:17:CLASS: BCMenuData:BCMenuData
# 	2:8:BCMenu.h:BCMenu.h
# 	2:21:TYPEDEF: BC_Seperator:BC_Seperator
# 	2:20:CLASS: CMenuItemInfo:CMenuItemInfo
# 	2:21:ENUM: BC_MenuDrawMode:BC_MenuDrawMode
# 	2:19:Application Include:xraynotes.h
# 	2:10:BCMenu.cpp:BCMenu.cpp
# End Section
# Section xraynotes : {8B3C46EF-DA5F-4B05-8AA3-260435CCFE73}
# 	2:9:lines.cpp:lines.cpp
# 	2:13:CLASS: Clines:Clines
# 	2:19:Application Include:xraynotes.h
# 	2:7:lines.h:lines.h
# End Section
# Section xraynotes : {9100308A-7529-4F7B-AFEA-B6EB7AB0E5D9}
# 	2:7:Popup.h:Popup.h
# 	2:13:CLASS: CPopup:CPopup
# 	2:19:Application Include:xraynotes.h
# 	2:9:Popup.cpp:Popup.cpp
# End Section
# Section xraynotes : {15D2D826-1DDB-4062-ABD1-E9AB70A7B4A9}
# 	2:14:CLASS: CEditEx:CEditEx
# 	2:10:EditEx.cpp:EditEx.cpp
# 	2:19:Application Include:xraynotes.h
# 	2:8:EditEx.h:EditEx.h
# End Section
# Section shades : {A7E8470F-9B32-4353-B61E-7A35935B4C93}
# 	2:12:CLASS: MxPad:MxPad
# 	2:7:mxpad.h:mxpad.h
# 	2:9:mxpad.cpp:mxpad.cpp
# 	2:19:Application Include:shades.h
# End Section
# Section xraynotes : {F806629A-B549-4794-9B93-A2EF30724CD1}
# 	2:15:CLASS: CBmpButt:CBmpButt
# 	2:11:BmpButt.cpp:BmpButt.cpp
# 	2:9:BmpButt.h:BmpButt.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {64EA5D66-2592-4B7D-B1B2-614BB29FE321}
# 	2:15:CLASS: CCalCtrl:CCalCtrl
# 	2:11:CalCtrl.cpp:CalCtrl.cpp
# 	2:9:CalCtrl.h:CalCtrl.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {772090BA-D142-4DD4-AEFF-B887F76746F9}
# 	1:15:IDD_ADDR_DIALOG:110
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_ADDR_DIALOG:IDD_ADDR_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:13:AddrEntry.cpp:AddrEntry1.cpp
# 	2:17:CLASS: CAddrEntry:CAddrEntry
# 	2:11:AddrEntry.h:AddrEntry1.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section shades : {FB9D5184-B2FE-4A38-84B7-4D0449E2AEA7}
# 	2:21:CLASS: CxTargetButton:CxTargetButton
# 	2:17:xTargetButton.cpp:xTargetButton.cpp
# 	2:19:Application Include:shades.h
# 	2:15:xTargetButton.h:xTargetButton.h
# End Section
# Section shades : {4B6C57BA-0105-4115-8D8E-704712B4CB56}
# 	2:12:CLASS: MxPad:MxPad
# 	2:7:mxpad.h:mxpad1.h
# 	2:9:mxpad.cpp:mxpad1.cpp
# 	2:19:Application Include:shades.h
# End Section
# Section xraynotes : {6FFDFB0A-5F35-4345-AA65-1847F62A2C41}
# 	2:13:CTimeCtrl.cpp:CTimeCtrl.cpp
# 	2:16:CLASS: CTimeCtrl:CTimeCtrl
# 	2:11:CTimeCtrl.h:CTimeCtrl.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {C72FE548-3EE0-4AD6-B443-8EA436AEBC69}
# 	1:10:IDD_BRIGHT:105
# 	2:16:Resource Include:resource.h
# 	2:11:DlgBriCon.h:DlgBriCon.h
# 	2:16:CLASS: DlgBriCon:DlgBriCon
# 	2:10:IDD_BRIGHT:IDD_BRIGHT
# 	2:13:DlgBriCon.cpp:DlgBriCon.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {C41E36A6-4AC6-4AF8-8B2D-D46156EE9116}
# 	2:14:CLASS: INeuron:INeuron
# 	2:11:INeuron.cpp:INeuron.cpp
# 	2:9:INeuron.h:INeuron.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {A50FFBAE-8E3A-4671-92CD-D69B19AF7ECE}
# 	1:16:IDD_XCAL_DIALOG5:117
# 	2:16:Resource Include:resource.h
# 	2:16:IDD_XCAL_DIALOG5:IDD_XCAL_DIALOG5
# 	2:10:ENUM: enum:enum
# 	2:11:xcalDlg.cpp:xcalDlg.cpp
# 	2:9:xcalDlg.h:xcalDlg.h
# 	2:19:Application Include:xraynotes.h
# 	2:15:CLASS: CXcalDlg:CXcalDlg
# End Section
# Section xraynotes : {DBB25AE7-E294-4086-84AC-0525ABD29865}
# 	2:15:CLASS: CBmpButt:CBmpButt
# 	2:11:BmpButt.cpp:BmpButt.cpp
# 	2:9:BmpButt.h:BmpButt.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {B16D5B34-025D-431B-AFFB-B7FF2E9B8403}
# 	1:10:IDD_PICSEL:122
# 	2:16:Resource Include:resource.h
# 	2:10:PicSel.cpp:PicSel.cpp
# 	2:10:ENUM: enum:enum
# 	2:14:CLASS: CPicSel:CPicSel
# 	2:10:IDD_PICSEL:IDD_PICSEL
# 	2:8:PicSel.h:PicSel.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {561D8BE7-F8DE-4B9A-B0EE-51A3D212231E}
# 	1:18:IDD_TRANSCR_DIALOG:120
# 	2:16:Resource Include:resource.h
# 	2:12:Transcript.h:Transcript.h
# 	2:18:IDD_TRANSCR_DIALOG:IDD_TRANSCR_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:14:Transcript.cpp:Transcript.cpp
# 	2:19:Application Include:xraynotes.h
# 	2:18:CLASS: CTranscript:CTranscript
# End Section
# Section xraynotes : {693F8948-8137-4E37-8DCA-89EE7CD426DB}
# 	2:9:label.cpp:label.cpp
# 	2:13:CLASS: CLabel:CLabel
# 	2:7:label.h:label.h
# 	2:15:ENUM: FlashType:FlashType
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {4E76ECC8-5F39-4CC3-887A-D9852E0EA3F2}
# 	2:8:xCalen.h:xCalen.h
# 	2:10:xCalen.cpp:xCalen.cpp
# 	2:14:CLASS: CxCalen:CxCalen
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {1234423D-78DA-4806-BA54-9E7A6E55CB35}
# 	2:9:INeuNet.h:INeuNet.h
# 	2:11:INeuNet.cpp:INeuNet.cpp
# 	2:14:CLASS: INeuNet:INeuNet
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {64AC3BB8-FD68-439B-91C7-E07B439E3075}
# 	1:12:IDD_DIALOG21:108
# 	2:14:CLASS: CDBMain:CDBMain
# 	2:16:Resource Include:resource.h
# 	2:10:DBMain.cpp:DBMain.cpp
# 	2:10:ENUM: enum:enum
# 	2:11:IDD_DIALOG2:IDD_DIALOG21
# 	2:19:Application Include:xraynotes.h
# 	2:8:DBMain.h:DBMain.h
# End Section
# Section xraynotes : {96B3550A-586A-4DBE-9D60-8EDD6F79DA5E}
# 	2:14:CLASS: CPGtree:CPGtree
# 	2:10:PGtree.cpp:PGtree.cpp
# 	2:8:PGtree.h:PGtree.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {39FCFEB0-2F4B-48F9-BDE3-4EF2DC66CECF}
# 	1:15:IDD_HIST_DIALOG:116
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_HIST_DIALOG:IDD_HIST_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:14:HistoryDlg.cpp:HistoryDlg.cpp
# 	2:12:HistoryDlg.h:HistoryDlg.h
# 	2:19:Application Include:xraynotes.h
# 	2:18:CLASS: CHistoryDlg:CHistoryDlg
# End Section
# Section xraynotes : {C5D3FD95-503D-4F31-BE11-3743347A2AF6}
# 	1:14:IDD_REG_DIALOG:114
# 	2:16:Resource Include:resource.h
# 	2:14:CLASS: CMedReg:CMedReg
# 	2:8:MedReg.h:MedReg.h
# 	2:14:IDD_REG_DIALOG:IDD_REG_DIALOG
# 	2:10:MedReg.cpp:MedReg.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {76A84947-82E4-4892-B7E6-F2796873738C}
# 	2:12:SimpleGrid.h:SimpleGrid.h
# 	2:14:SimpleGrid.cpp:SimpleGrid.cpp
# 	2:19:Application Include:xraynotes.h
# 	2:18:CLASS: CSimpleGrid:CSimpleGrid
# End Section
# Section xraynotes : {CE7427A4-C953-45BB-9471-3CF50B5D15A9}
# 	1:27:IDD_ONSCREENKEYBOARD_DIALOG:123
# 	2:16:Resource Include:resource.h
# 	2:27:IDD_ONSCREENKEYBOARD_DIALOG:IDD_ONSCREENKEYBOARD_DIALOG
# 	2:27:CLASS: COnscreenKeyboardDlg:COnscreenKeyboardDlg
# 	2:10:ENUM: enum:enum
# 	2:21:OnscreenKeyboardDlg.h:OnscreenKeyboardDlg.h
# 	2:13:CLASS: KEYDEF:KEYDEF
# 	2:19:Application Include:xraynotes.h
# 	2:23:OnscreenKeyboardDlg.cpp:OnscreenKeyboardDlg.cpp
# End Section
# Section xraynotes : {5A59A1CF-D741-4177-A531-7DADD8B94B2A}
# 	2:19:CLASS: CTransSlider:CTransSlider
# 	2:15:TransSlider.cpp:TransSlider.cpp
# 	2:13:TransSlider.h:TransSlider.h
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {1CA0EEF8-A256-44B4-8A2B-904E010B0542}
# 	1:15:IDD_DIAG_DIALOG:118
# 	2:16:Resource Include:resource.h
# 	2:12:DiagList.cpp:DiagList.cpp
# 	2:10:ENUM: enum:enum
# 	2:15:IDD_DIAG_DIALOG:IDD_DIAG_DIALOG
# 	2:10:DiagList.h:DiagList.h
# 	2:16:CLASS: CDiagList:CDiagList
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {966354CC-2F7D-4C1C-8EBD-9970EAA86FA6}
# 	2:17:CLASS: LevenStein:LevenStein
# 	2:14:LevenStein.cpp:LevenStein.cpp
# 	2:12:LevenStein.h:LevenStein.h
# 	2:15:CLASS: Distance:Distance
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {A4FF3D8B-0A94-4D43-876A-3CD6E4CD1673}
# 	1:9:IDD_HIPAA:109
# 	2:16:Resource Include:resource.h
# 	2:9:HShield.h:HShield.h
# 	2:11:HShield.cpp:HShield.cpp
# 	2:9:IDD_HIPAA:IDD_HIPAA
# 	2:10:ENUM: enum:enum
# 	2:15:CLASS: CHShield:CHShield
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {D180312F-48CF-4D63-BA39-090C7B9829DF}
# 	2:11:sellist.cpp:sellist.cpp
# 	2:9:sellist.h:sellist.h
# 	2:19:Application Include:xraynotes.h
# 	2:14:CLASS: sellist:sellist
# End Section
# Section xraynotes : {237D5865-CE21-40E9-89E7-0C43B66DED0A}
# 	2:14:CLASS: CUniqID:CUniqID
# 	2:8:UniqID.h:UniqID.h
# 	2:10:UniqID.cpp:UniqID.cpp
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {F1681EA9-3C82-4BC2-A69E-E813895876A9}
# 	1:12:IDD_DIALOG31:107
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:13:AddrEntry.cpp:AddrEntry.cpp
# 	2:17:CLASS: CAddrEntry:CAddrEntry
# 	2:11:AddrEntry.h:AddrEntry.h
# 	2:11:IDD_DIALOG3:IDD_DIALOG31
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {2E273282-3995-4424-84ED-C8D08696F4F5}
# 	2:13:CLASS: CLabel:CLabel
# 	2:15:ENUM: FlashType:FlashType
# 	2:8:label1.h:label1.h
# 	2:10:label1.cpp:label1.cpp
# 	2:19:Application Include:shades.h
# End Section
# Section xraynotes : {071C07DC-A983-4BCA-A3AA-9A67FDEB17B2}
# 	1:11:IDD_CODESEL:119
# 	2:16:Resource Include:resource.h
# 	2:13:pcSelection.h:pcSelection.h
# 	2:15:pcSelection.cpp:pcSelection.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:CLASS: CpcSelection:CpcSelection
# 	2:11:IDD_CODESEL:IDD_CODESEL
# 	2:19:Application Include:xraynotes.h
# End Section
# Section xraynotes : {0CDBB7FE-A227-4B19-8BE0-385A027957D2}
# 	1:12:IDD_DIALOG41:111
# 	2:16:Resource Include:resource.h
# 	2:15:CLASS: CDateSel:CDateSel
# 	2:9:DateSel.h:DateSel.h
# 	2:10:ENUM: enum:enum
# 	2:11:IDD_DIALOG4:IDD_DIALOG41
# 	2:19:Application Include:xraynotes.h
# 	2:11:DateSel.cpp:DateSel.cpp
# End Section
# Section xraynotes : {B0FA39A8-CE91-4FE3-B7D3-D922B0E9C25D}
# 	2:10:PGedit.cpp:PGedit.cpp
# 	2:8:PGedit.h:PGedit.h
# 	2:14:CLASS: CPGedit:CPGedit
# 	2:19:Application Include:xraynotes.h
# End Section
# Section shades : {E2124016-3BF4-4A52-BEC7-6BFF2EFE60D8}
# 	2:12:CLASS: MxPad:MxPad
# 	2:7:mxpad.h:mxpad.h
# 	2:9:mxpad.cpp:mxpad1.cpp
# 	2:19:Application Include:shades.h
# End Section
# Section xraynotes : {08056AAB-9A54-4919-9001-EDEED9F859C1}
# 	2:16:FileDialogST.cpp:FileDialogST.cpp
# 	2:26:ENUM: SHVIEW_ListViewModes:SHVIEW_ListViewModes
# 	2:14:FileDialogST.h:FileDialogST.h
# 	2:19:Application Include:shades.h
# 	2:20:CLASS: CFileDialogST:CFileDialogST
# End Section
