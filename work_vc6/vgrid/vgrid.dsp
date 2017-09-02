# Microsoft Developer Studio Project File - Name="vgrid" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=vgrid - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vgrid.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vgrid.mak" CFG="vgrid - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vgrid - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "vgrid - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vgrid - Win32 Release"

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
# ADD LINK32 rpcrt4.lib ws2_32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "vgrid - Win32 Debug"

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
# ADD LINK32 rpcrt4.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "vgrid - Win32 Release"
# Name "vgrid - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GrayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\label.cpp
# End Source File
# Begin Source File

SOURCE=.\misc.cpp
# End Source File
# Begin Source File

SOURCE=.\mxpad.cpp
# End Source File
# Begin Source File

SOURCE=.\PersistDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Popup.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Support.cpp
# End Source File
# Begin Source File

SOURCE=.\TextMode.cpp
# End Source File
# Begin Source File

SOURCE=.\TransSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\vgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\vgrid.rc
# End Source File
# Begin Source File

SOURCE=.\vgridDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\xPtrArr.cpp
# End Source File
# Begin Source File

SOURCE=.\XrayM.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GrayDlg.h
# End Source File
# Begin Source File

SOURCE=.\label.h
# End Source File
# Begin Source File

SOURCE=.\misc.h
# End Source File
# Begin Source File

SOURCE=.\mxpad.h
# End Source File
# Begin Source File

SOURCE=.\PersistDlg.h
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

SOURCE=.\Support.h
# End Source File
# Begin Source File

SOURCE=.\TextMode.h
# End Source File
# Begin Source File

SOURCE=.\TransSlider.h
# End Source File
# Begin Source File

SOURCE=.\vgrid.h
# End Source File
# Begin Source File

SOURCE=.\vgridDlg.h
# End Source File
# Begin Source File

SOURCE=.\xPtrArr.h
# End Source File
# Begin Source File

SOURCE=.\XrayM.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\vgrid.ico
# End Source File
# Begin Source File

SOURCE=.\res\vgrid.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section vgrid : {3AF596F2-D8C8-4DF7-9437-246CF3E58205}
# 	2:6:misc.h:misc.h
# 	2:12:CLASS: CMisc:CMisc
# 	2:8:misc.cpp:misc.cpp
# 	2:19:Application Include:vgrid.h
# End Section
# Section vgrid : {2CEAB9C6-76F7-4832-8B36-34759B20B7CF}
# 	2:9:GrayDlg.h:GrayDlg.h
# 	2:10:ENUM: enum:enum
# 	2:11:GrayDlg.cpp:GrayDlg.cpp
# 	2:15:CLASS: CGrayDlg:CGrayDlg
# 	2:19:Application Include:vgrid.h
# End Section
# Section vgrid : {486CD11A-D4FD-4E19-9F65-D331CF88691C}
# 	2:7:Popup.h:Popup.h
# 	2:13:CLASS: CPopup:CPopup
# 	2:19:Application Include:vgrid.h
# 	2:9:Popup.cpp:Popup.cpp
# End Section
# Section vgrid : {4344BFD6-0221-4AB3-A0FC-16C0C89475DF}
# 	2:9:label.cpp:label.cpp
# 	2:13:CLASS: CLabel:CLabel
# 	2:7:label.h:label.h
# 	2:15:ENUM: FlashType:FlashType
# 	2:19:Application Include:vgrid.h
# End Section
# Section vgrid : {AD5D85F8-1E28-425A-B053-C09DC25E5BA9}
# 	2:15:CLASS: CxPtrArr:CxPtrArr
# 	2:11:xPtrArr.cpp:xPtrArr.cpp
# 	2:9:xPtrArr.h:xPtrArr.h
# 	2:19:Application Include:vgrid.h
# End Section
# Section vgrid : {2CF96092-23D7-4B9E-B8E6-44F698595613}
# 	2:12:CLASS: MxPad:MxPad
# 	2:7:mxpad.h:mxpad.h
# 	2:9:mxpad.cpp:mxpad.cpp
# 	2:19:Application Include:vgrid.h
# End Section
# Section vgrid : {D71C1AC9-7116-4AF6-9B55-5585AB528637}
# 	2:16:CLASS: CTextMode:CTextMode
# 	2:10:TextMode.h:TextMode.h
# 	2:19:Application Include:vgrid.h
# 	2:12:TextMode.cpp:TextMode.cpp
# End Section
# Section vgrid : {4333B953-FE48-4E59-BBFB-71A6115BFE54}
# 	1:12:IDD_MENUBASE:103
# 	2:16:Resource Include:resource.h
# 	2:12:IDD_MENUBASE:IDD_MENUBASE
# 	2:17:CLASS: CXrayMitem:CXrayMitem
# 	2:9:XrayM.cpp:XrayM.cpp
# 	2:7:XrayM.h:XrayM.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:vgrid.h
# 	2:13:CLASS: CXrayM:CXrayM
# End Section
# Section vgrid : {B518CCA8-0EB9-4CBF-943C-4644E8140974}
# 	2:17:CLASS: PersistDlg:PersistDlg
# 	2:12:PersistDlg.h:PersistDlg.h
# 	2:10:ENUM: enum:enum
# 	2:14:PersistDlg.cpp:PersistDlg.cpp
# 	2:19:Application Include:vgrid.h
# End Section
# Section vgrid : {9F7A13B3-54E9-4942-B7F7-AA5C5A4F96FC}
# 	2:17:CLASS: CXrayMitem:CXrayMitem
# 	2:9:XrayM.cpp:XrayM1.cpp
# 	2:7:XrayM.h:XrayM1.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:vgrid.h
# 	2:13:CLASS: CXrayM:CXrayM
# End Section
# Section vgrid : {9957E51B-15C9-423F-9B58-1BD9E1FC8111}
# 	2:9:Support.h:Support.h
# 	2:15:CLASS: CSupport:CSupport
# 	2:39:TYPEDEF: lpfnSetLayeredWindowAttributes:lpfnSetLayeredWindowAttributes
# 	2:11:Support.cpp:Support.cpp
# 	2:19:Application Include:vgrid.h
# End Section
# Section vgrid : {4C9D17B9-65A0-44A3-89C5-F7CB2F1E6E20}
# 	2:19:CLASS: CTransSlider:CTransSlider
# 	2:15:TransSlider.cpp:TransSlider.cpp
# 	2:13:TransSlider.h:TransSlider.h
# 	2:19:Application Include:vgrid.h
# End Section
