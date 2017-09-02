# Microsoft Developer Studio Project File - Name="ftext" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ftext - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ftext.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ftext.mak" CFG="ftext - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ftext - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ftext - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ftext - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ftext - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib user32.lib msimg32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ftext - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib user32.lib msimg32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "ftext - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ftext___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ftext___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ftext___Win32_Unicode_Debug"
# PROP Intermediate_Dir "ftext___Win32_Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib user32.lib msimg32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib user32.lib msimg32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ftext - Win32 Release"
# Name "ftext - Win32 Debug"
# Name "ftext - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\common\BaseWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcRep.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Flag.cpp
# End Source File
# Begin Source File

SOURCE=.\ftext.cpp
# End Source File
# Begin Source File

SOURCE=.\ftextDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\common\GrayDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\common\label.cpp
# End Source File
# Begin Source File

SOURCE=..\common\LevenStein.cpp
# End Source File
# Begin Source File

SOURCE=..\common\memdc.cpp
# End Source File
# Begin Source File

SOURCE=..\common\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\common\mxpad.cpp
# End Source File
# Begin Source File

SOURCE=..\common\PaintDCSave.cpp
# End Source File
# Begin Source File

SOURCE=.\ParseStr.cpp
# End Source File
# Begin Source File

SOURCE=..\common\PersistDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\common\PGedit.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Popup.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\common\Support.cpp
# End Source File
# Begin Source File

SOURCE=.\toolstrip.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Version.cpp
# End Source File
# Begin Source File

SOURCE=.\xFedit.cpp
# End Source File
# Begin Source File

SOURCE=..\common\xPtrArr.cpp
# End Source File
# Begin Source File

SOURCE=..\common\xRect.cpp
# End Source File
# Begin Source File

SOURCE=..\common\xScrollBar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\common\BaseWnd.h
# End Source File
# Begin Source File

SOURCE=.\ClipBoard.h
# End Source File
# Begin Source File

SOURCE=.\ExcRep.h
# End Source File
# Begin Source File

SOURCE=..\common\Flag.h
# End Source File
# Begin Source File

SOURCE=.\ftext.h
# End Source File
# Begin Source File

SOURCE=.\ftextDlg.h
# End Source File
# Begin Source File

SOURCE=.\ftitems.h
# End Source File
# Begin Source File

SOURCE=..\common\GrayDlg.h
# End Source File
# Begin Source File

SOURCE=..\common\label.h
# End Source File
# Begin Source File

SOURCE=..\common\LevenStein.h
# End Source File
# Begin Source File

SOURCE=..\common\memdc.h
# End Source File
# Begin Source File

SOURCE=..\common\misc.h
# End Source File
# Begin Source File

SOURCE=..\common\mxpad.h
# End Source File
# Begin Source File

SOURCE=..\common\PaintDCSave.h
# End Source File
# Begin Source File

SOURCE=.\ParseStr.h
# End Source File
# Begin Source File

SOURCE=..\common\PersistDlg.h
# End Source File
# Begin Source File

SOURCE=..\common\PGedit.h
# End Source File
# Begin Source File

SOURCE=..\common\Popup.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\common\Support.h
# End Source File
# Begin Source File

SOURCE=.\toolstrip.h
# End Source File
# Begin Source File

SOURCE=..\common\Version.h
# End Source File
# Begin Source File

SOURCE=.\xFedit.h
# End Source File
# Begin Source File

SOURCE=..\common\xPtrArr.h
# End Source File
# Begin Source File

SOURCE=.\xRect.h
# End Source File
# Begin Source File

SOURCE=.\xScrollBar.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\editbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ftext.ico
# End Source File
# Begin Source File

SOURCE=.\ftext.rc
# End Source File
# Begin Source File

SOURCE=.\res\ftext.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ftext : {486CD11A-D4FD-4E19-9F65-D331CF88691C}
# 	2:7:Popup.h:Popup.h
# 	2:13:CLASS: CPopup:CPopup
# 	2:19:Application Include:ftext.h
# 	2:9:Popup.cpp:Popup.cpp
# End Section
# Section ftext : {CAB82EC9-8D96-46F9-9230-F4873B908CAA}
# 	2:17:CLASS: PersistDlg:PersistDlg
# 	2:12:PersistDlg.h:PersistDlg.h
# 	2:10:ENUM: enum:enum
# 	2:14:PersistDlg.cpp:PersistDlg.cpp
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {4344BFD6-0221-4AB3-A0FC-16C0C89475DF}
# 	2:9:label.cpp:label.cpp
# 	2:13:CLASS: CLabel:CLabel
# 	2:7:label.h:label.h
# 	2:15:ENUM: FlashType:FlashType
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {9BF215B2-C0B4-4E89-83D3-FCEA1F805064}
# 	2:12:CLASS: MxPad:MxPad
# 	2:7:mxpad.h:mxpad.h
# 	2:9:mxpad.cpp:mxpad.cpp
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {70701787-1A76-4044-8007-EF3663D47261}
# 	2:15:CLASS: CxPtrArr:CxPtrArr
# 	2:11:xPtrArr.cpp:xPtrArr.cpp
# 	2:9:xPtrArr.h:xPtrArr.h
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {A58ED7EC-1548-4EAF-B048-C3EC56F89A27}
# 	2:15:CLASS: CBaseWnd:CBaseWnd
# 	2:19:Application Include:ftext.h
# 	2:11:BaseWnd.cpp:BaseWnd.cpp
# 	2:9:BaseWnd.h:BaseWnd.h
# End Section
# Section ftext : {A4BDCBFA-E07D-4B1E-8CF0-491B36E8E8F8}
# 	2:6:Flag.h:Flag1.h
# 	2:12:CLASS: CFlag:CFlag
# 	2:19:Application Include:ftext.h
# 	2:8:Flag.cpp:Flag.cpp
# End Section
# Section ftext : {02CF5036-7E27-4502-8728-DB9B1CC60EF2}
# 	2:9:GrayDlg.h:GrayDlg.h
# 	2:10:ENUM: enum:enum
# 	2:11:GrayDlg.cpp:GrayDlg.cpp
# 	2:15:CLASS: CGrayDlg:CGrayDlg
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {5BE83457-7166-4876-A1A5-FABB68E8002E}
# 	2:9:xRect.cpp:xRect.cpp
# 	2:7:xRect.h:xRect.h
# 	2:13:CLASS: CxRect:CxRect
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {59BB733B-053C-4C24-8744-677637E258D3}
# 	2:17:CLASS: LevenStein:LevenStein
# 	2:14:LevenStein.cpp:LevenStein.cpp
# 	2:12:LevenStein.h:LevenStein.h
# 	2:15:CLASS: Distance:Distance
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {1C76C37E-1A09-417D-84BD-ECF6ECB05C15}
# 	2:6:misc.h:misc.h
# 	2:12:CLASS: CMisc:CMisc
# 	2:8:misc.cpp:misc.cpp
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {9610DE0E-5442-40B4-97B6-856C7D29FC6A}
# 	2:9:GrayDlg.h:GrayDlg.h
# 	2:10:ENUM: enum:enum
# 	2:11:GrayDlg.cpp:GrayDlg.cpp
# 	2:15:CLASS: CGrayDlg:CGrayDlg
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {17B3FB65-37DB-4063-BFF8-96879600E22A}
# 	2:13:CLASS: CMemDC:CMemDC
# 	2:7:memdc.h:memdc.h
# 	2:14:CLASS: CSaveDC:CSaveDC
# 	2:9:memdc.cpp:memdc.cpp
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {D2248C11-0753-48EA-A48F-51DD0E637B6D}
# 	2:17:CLASS: PersistDlg:PersistDlg
# 	2:12:PersistDlg.h:PersistDlg.h
# 	2:10:ENUM: enum:enum
# 	2:14:PersistDlg.cpp:PersistDlg.cpp
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {5F1818CD-A0B0-45E7-9B16-B42F278FC70A}
# 	2:10:PGedit.cpp:PGedit.cpp
# 	2:8:PGedit.h:PGedit.h
# 	2:14:CLASS: CPGedit:CPGedit
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {A8838F97-9AAF-45C2-AC20-88804FD1F9D4}
# 	1:11:IDD_DIALOG2:104
# 	2:16:Resource Include:resource.h
# 	2:8:Flyout.h:Flyout.h
# 	2:14:CLASS: CFlyout:CFlyout
# 	2:10:Flyout.cpp:Flyout.cpp
# 	2:10:ENUM: enum:enum
# 	2:11:IDD_DIALOG2:IDD_DIALOG2
# 	2:19:Application Include:ftext.h
# End Section
# Section ftext : {47CB8676-6ADD-44B0-8ACF-9855265EE60E}
# 	2:6:Flag.h:Flag.h
# 	2:12:CLASS: CFlag:CFlag
# 	2:19:Application Include:ftext.h
# 	2:8:Flag.cpp:Flag.cpp
# End Section
# Section ftext : {477B5E63-0DA1-4A31-B69F-928F72475F01}
# 	2:9:Support.h:Support.h
# 	2:15:CLASS: CSupport:CSupport
# 	2:39:TYPEDEF: lpfnSetLayeredWindowAttributes:lpfnSetLayeredWindowAttributes
# 	2:11:Support.cpp:Support.cpp
# 	2:19:Application Include:ftext.h
# End Section
