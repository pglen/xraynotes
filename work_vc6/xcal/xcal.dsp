# Microsoft Developer Studio Project File - Name="xcal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=xcal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xcal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xcal.mak" CFG="xcal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xcal - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "xcal - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xcal - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 sqlite3.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "xcal - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 sqlite3.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "xcal - Win32 Release"
# Name "xcal - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\About.cpp
# End Source File
# Begin Source File

SOURCE=.\AlaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mxpad.cpp
# End Source File
# Begin Source File

SOURCE=.\Popup.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\support.cpp
# End Source File
# Begin Source File

SOURCE=.\vSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\xcal.cpp
# End Source File
# Begin Source File

SOURCE=.\xcal.rc
# End Source File
# Begin Source File

SOURCE=.\xcalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\xCalen.cpp
# End Source File
# Begin Source File

SOURCE=.\xWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlaDlg.h
# End Source File
# Begin Source File

SOURCE=.\mxpad.h
# End Source File
# Begin Source File

SOURCE=.\Popup.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\support.h
# End Source File
# Begin Source File

SOURCE=.\vSplit.h
# End Source File
# Begin Source File

SOURCE=.\xcal.h
# End Source File
# Begin Source File

SOURCE=.\xcalDlg.h
# End Source File
# Begin Source File

SOURCE=.\xCalen.h
# End Source File
# Begin Source File

SOURCE=.\xWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\xcal.ico
# End Source File
# Begin Source File

SOURCE=.\res\xcal.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section xcal : {A53D0938-B6D2-4097-B444-F2542BF788D4}
# 	2:11:support.cpp:support.cpp
# 	2:15:CLASS: CSupport:CSupport
# 	2:39:TYPEDEF: lpfnSetLayeredWindowAttributes:lpfnSetLayeredWindowAttributes
# 	2:9:support.h:support.h
# 	2:19:Application Include:xcal.h
# End Section
# Section xcal : {2D153FE8-4AA9-444F-9D73-A39C3AC22A8C}
# 	1:12:IDD_DIALOG12:103
# 	2:16:Resource Include:resource.h
# 	2:7:Popup.h:Popup.h
# 	2:10:ENUM: enum:enum
# 	2:12:IDD_DIALOG12:IDD_DIALOG12
# 	2:13:CLASS: CPopup:CPopup
# 	2:19:Application Include:xcal.h
# 	2:9:Popup.cpp:Popup.cpp
# End Section
# Section xcal : {A17C24D7-AA2D-4C49-AB5C-9F74AF3CC1A5}
# 	2:7:Popup.h:Popup.h
# 	2:13:CLASS: CPopup:CPopup
# 	2:19:Application Include:xcal.h
# 	2:9:Popup.cpp:Popup.cpp
# End Section
