# Microsoft Developer Studio Project File - Name="dbtest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=dbtest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dbtest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dbtest.mak" CFG="dbtest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dbtest - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "dbtest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dbtest - Win32 Release"

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

!ELSEIF  "$(CFG)" == "dbtest - Win32 Debug"

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

# Name "dbtest - Win32 Release"
# Name "dbtest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddrEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\AlaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpButt.cpp
# End Source File
# Begin Source File

SOURCE=.\CalCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\calen.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeEd.cpp
# End Source File
# Begin Source File

SOURCE=.\CtimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DateSel.cpp
# End Source File
# Begin Source File

SOURCE=.\DBMain.cpp
# End Source File
# Begin Source File

SOURCE=.\dbtest.cpp
# End Source File
# Begin Source File

SOURCE=.\dbtest.rc
# End Source File
# Begin Source File

SOURCE=.\dbtestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DiagList.cpp
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

SOURCE=.\GrayWin.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HShield.cpp
# End Source File
# Begin Source File

SOURCE=.\label.cpp
# End Source File
# Begin Source File

SOURCE=.\lcd7.cpp
# End Source File
# Begin Source File

SOURCE=.\LevenStein.cpp
# End Source File
# Begin Source File

SOURCE=.\MedReg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\mxpad.cpp
# End Source File
# Begin Source File

SOURCE=.\parse.cpp
# End Source File
# Begin Source File

SOURCE=.\pcSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\PersistDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PGedit.cpp
# End Source File
# Begin Source File

SOURCE=.\PGtree.cpp
# End Source File
# Begin Source File

SOURCE=.\Popup.cpp
# End Source File
# Begin Source File

SOURCE=.\prl.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcedureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PRocList.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Support.cpp
# End Source File
# Begin Source File

SOURCE=.\Transcript.cpp
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

SOURCE=.\xTargetButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddrEntry.h
# End Source File
# Begin Source File

SOURCE=.\AlaDlg.h
# End Source File
# Begin Source File

SOURCE=.\AlaDlg1.h
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BmpButt.h
# End Source File
# Begin Source File

SOURCE=.\CalCtrl.h
# End Source File
# Begin Source File

SOURCE=.\calen.h
# End Source File
# Begin Source File

SOURCE=.\CodeEd.h
# End Source File
# Begin Source File

SOURCE=.\CTimeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DateSel.h
# End Source File
# Begin Source File

SOURCE=.\DBMain.h
# End Source File
# Begin Source File

SOURCE=.\dbtest.h
# End Source File
# Begin Source File

SOURCE=.\dbtestDlg.h
# End Source File
# Begin Source File

SOURCE=.\DiagList.h
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

SOURCE=.\GrayWin.h
# End Source File
# Begin Source File

SOURCE=.\HistoryDlg.h
# End Source File
# Begin Source File

SOURCE=.\HShield.h
# End Source File
# Begin Source File

SOURCE=.\label.h
# End Source File
# Begin Source File

SOURCE=.\lcd7.h
# End Source File
# Begin Source File

SOURCE=.\LevenStein.h
# End Source File
# Begin Source File

SOURCE=.\MedReg.h
# End Source File
# Begin Source File

SOURCE=.\MsgBox.h
# End Source File
# Begin Source File

SOURCE=.\mxpad.h
# End Source File
# Begin Source File

SOURCE=.\parse.h
# End Source File
# Begin Source File

SOURCE=.\pcSelection.h
# End Source File
# Begin Source File

SOURCE=.\PersistDlg.h
# End Source File
# Begin Source File

SOURCE=.\PGedit.h
# End Source File
# Begin Source File

SOURCE=.\PGtree.h
# End Source File
# Begin Source File

SOURCE=.\Popup.h
# End Source File
# Begin Source File

SOURCE=.\prl.h
# End Source File
# Begin Source File

SOURCE=.\ProcedureDlg.h
# End Source File
# Begin Source File

SOURCE=.\PRocList.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SimpleGrid.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Support.h
# End Source File
# Begin Source File

SOURCE=.\Transcript.h
# End Source File
# Begin Source File

SOURCE=.\UniqID.h
# End Source File
# Begin Source File

SOURCE=.\xcalDlg.h
# End Source File
# Begin Source File

SOURCE=.\xCalen.h
# End Source File
# Begin Source File

SOURCE=.\xTargetButton.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\images\appoint.bmp
# End Source File
# Begin Source File

SOURCE=.\images\assets.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\images\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dbtest.ico
# End Source File
# Begin Source File

SOURCE=.\res\dbtest.rc2
# End Source File
# Begin Source File

SOURCE=.\images\hippaa2.bmp
# End Source File
# Begin Source File

SOURCE=.\images\newpat.bmp
# End Source File
# Begin Source File

SOURCE=.\images\pay.bmp
# End Source File
# Begin Source File

SOURCE=.\images\reports.bmp
# End Source File
# Begin Source File

SOURCE=.\images\stat.bmp
# End Source File
# Begin Source File

SOURCE=.\images\tools.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section dbtest : {E4F927B1-057D-47A3-A22B-55498A08F6C2}
# 	2:8:xCalen.h:xCalen.h
# 	2:10:xCalen.cpp:xCalen.cpp
# 	2:14:CLASS: CxCalen:CxCalen
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {3A50F487-104F-433A-8F6A-87F757BB9EF7}
# 	2:8:xCalen.h:xCalen1.h
# 	2:10:xCalen.cpp:xCalen.cpp
# 	2:14:CLASS: CxCalen:CxCalen
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {F7B8DB30-B0CD-40C3-89F8-4DF099FEBADC}
# 	2:21:CLASS: CxTargetButton:CxTargetButton
# 	2:17:xTargetButton.cpp:xTargetButton.cpp
# 	2:19:Application Include:dbtest.h
# 	2:15:xTargetButton.h:xTargetButton.h
# End Section
# Section dbtest : {30494BAC-473B-4EB5-A341-65DD36421A86}
# 	1:12:IDD_DIALOG12:105
# 	2:16:Resource Include:resource.h
# 	2:7:Popup.h:Popup.h
# 	2:10:ENUM: enum:enum
# 	2:12:IDD_DIALOG12:IDD_DIALOG12
# 	2:13:CLASS: CPopup:CPopup
# 	2:19:Application Include:dbtest.h
# 	2:9:Popup.cpp:Popup.cpp
# End Section
# Section dbtest : {2478AD58-9832-484A-A626-FBDCE5687B85}
# 	1:12:IDD_DIALOG11:116
# 	2:16:Resource Include:resource.h
# 	2:15:TYPEDEF: fields:fields
# 	2:8:AlaDlg.h:AlaDlg1.h
# 	2:14:CLASS: CAlaDlg:CAlaDlg
# 	2:14:CLASS: _fields:_fields
# 	2:10:AlaDlg.cpp:AlaDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:11:IDD_DIALOG1:IDD_DIALOG11
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {0DF0048C-4262-4CCA-AE6F-46EAC372D851}
# 	2:8:xCalen.h:xCalen.h
# 	2:10:xCalen.cpp:xCalen.cpp
# 	2:14:CLASS: CxCalen:CxCalen
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {739AFBE3-1805-4183-AEDC-981231D12616}
# 	1:12:IDD_DIALOG11:108
# 	2:16:Resource Include:resource.h
# 	2:15:TYPEDEF: fields:fields
# 	2:8:AlaDlg.h:AlaDlg.h
# 	2:14:CLASS: CAlaDlg:CAlaDlg
# 	2:14:CLASS: _fields:_fields
# 	2:10:AlaDlg.cpp:AlaDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:11:IDD_DIALOG1:IDD_DIALOG11
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {28F1F68E-C88D-4FF7-B153-9B6F88748CF4}
# 	2:13:CLASS: BCMenu:BCMenu
# 	2:24:TYPEDEF: BC_MenuDrawMode:BC_MenuDrawMode
# 	2:18:ENUM: BC_Seperator:BC_Seperator
# 	2:18:CLASS: BCMenuMemDC:BCMenuMemDC
# 	2:17:CLASS: BCMenuData:BCMenuData
# 	2:8:BCMenu.h:BCMenu.h
# 	2:21:TYPEDEF: BC_Seperator:BC_Seperator
# 	2:20:CLASS: CMenuItemInfo:CMenuItemInfo
# 	2:21:ENUM: BC_MenuDrawMode:BC_MenuDrawMode
# 	2:19:Application Include:dbtest.h
# 	2:10:BCMenu.cpp:BCMenu.cpp
# End Section
# Section dbtest : {AEDECC20-5555-4EAC-B41E-A74E888B653C}
# 	1:16:IDD_XCAL_DIALOG1:110
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_XCAL_DIALOG:IDD_XCAL_DIALOG1
# 	2:10:ENUM: enum:enum
# 	2:11:xcalDlg.cpp:xcalDlg1.cpp
# 	2:9:xcalDlg.h:xcalDlg1.h
# 	2:19:Application Include:dbtest.h
# 	2:15:CLASS: CXcalDlg:CXcalDlg
# End Section
# Section dbtest : {9C771CFD-1DAD-4C45-B06B-0AA9814B46CD}
# 	1:16:IDD_XCAL_DIALOG5:115
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_XCAL_DIALOG:IDD_XCAL_DIALOG5
# 	2:10:ENUM: enum:enum
# 	2:11:xcalDlg.cpp:xcalDlg.cpp
# 	2:9:xcalDlg.h:xcalDlg.h
# 	2:19:Application Include:dbtest.h
# 	2:15:CLASS: CXcalDlg:CXcalDlg
# End Section
# Section dbtest : {8B3EF6C5-05F6-49A2-AF27-32364451ECFC}
# 	2:12:CLASS: MxPad:MxPad
# 	2:7:mxpad.h:mxpad.h
# 	2:9:mxpad.cpp:mxpad.cpp
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {C1D9F1C0-1CAB-4733-BAC8-38E81F36D67E}
# 	2:17:CLASS: PersistDlg:PersistDlg
# 	2:12:PersistDlg.h:PersistDlg.h
# 	2:10:ENUM: enum:enum
# 	2:14:PersistDlg.cpp:PersistDlg.cpp
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {F25D5D98-6A42-4DE7-8806-B5F437AB6053}
# 	2:8:xCalen.h:xCalen.h
# 	2:10:xCalen.cpp:xCalen.cpp
# 	2:14:CLASS: CxCalen:CxCalen
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {8B53F110-8655-4B9D-B93D-1901DDA6EA92}
# 	2:16:FileDialogST.cpp:FileDialogST.cpp
# 	2:26:ENUM: SHVIEW_ListViewModes:SHVIEW_ListViewModes
# 	2:14:FileDialogST.h:FileDialogST.h
# 	2:19:Application Include:dbtest.h
# 	2:20:CLASS: CFileDialogST:CFileDialogST
# End Section
# Section dbtest : {522F149B-27B8-4C21-9755-2B51185A190D}
# 	1:16:IDD_XCAL_DIALOG4:114
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_XCAL_DIALOG:IDD_XCAL_DIALOG4
# 	2:10:ENUM: enum:enum
# 	2:11:xcalDlg.cpp:xcalDlg.cpp
# 	2:9:xcalDlg.h:xcalDlg1.h
# 	2:19:Application Include:dbtest.h
# 	2:15:CLASS: CXcalDlg:CXcalDlg
# End Section
# Section dbtest : {E9C2A0D4-2BED-4BA3-B9B7-87DA465177A0}
# 	1:11:IDD_DIALOG3:104
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:13:AddrEntry.cpp:AddrEntry.cpp
# 	2:17:CLASS: CAddrEntry:CAddrEntry
# 	2:11:AddrEntry.h:AddrEntry.h
# 	2:11:IDD_DIALOG3:IDD_DIALOG3
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {FF16B7C1-EDD6-4F4E-B935-2D1CEC07D65F}
# 	2:17:CLASS: LevenStein:LevenStein
# 	2:14:LevenStein.cpp:LevenStein.cpp
# 	2:12:LevenStein.h:LevenStein.h
# 	2:15:CLASS: Distance:Distance
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {4454B8E9-1495-4009-9CD4-6E2942480B13}
# 	2:10:PGedit.cpp:PGedit.cpp
# 	2:8:PGedit.h:PGedit.h
# 	2:14:CLASS: CPGedit:CPGedit
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {DEF85792-964A-4B6B-BFF5-D81E1ECC7472}
# 	1:16:IDD_XCAL_DIALOG3:113
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_XCAL_DIALOG:IDD_XCAL_DIALOG3
# 	2:10:ENUM: enum:enum
# 	2:11:xcalDlg.cpp:xcalDlg.cpp
# 	2:9:xcalDlg.h:xcalDlg.h
# 	2:19:Application Include:dbtest.h
# 	2:15:CLASS: CXcalDlg:CXcalDlg
# End Section
# Section dbtest : {99567A57-EFC5-4EEC-A63D-5C7742497005}
# 	2:9:GrayWin.h:GrayWin.h
# 	2:10:ENUM: enum:enum
# 	2:15:CLASS: CGrayDlg:CGrayDlg
# 	2:19:Application Include:dbtest.h
# 	2:11:GrayWin.cpp:GrayWin.cpp
# End Section
# Section dbtest : {1748EE25-1557-42A5-9E96-C243289BD345}
# 	1:8:IDD_MBOX:112
# 	2:16:Resource Include:resource.h
# 	2:8:MsgBox.h:MsgBox.h
# 	2:14:CLASS: CMsgBox:CMsgBox
# 	2:8:IDD_MBOX:IDD_MBOX
# 	2:10:MsgBox.cpp:MsgBox.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:dbtest.h
# End Section
# Section dbtest : {44C40F95-641E-49A5-9739-77B09C3B056A}
# 	1:16:IDD_XCAL_DIALOG2:111
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_XCAL_DIALOG:IDD_XCAL_DIALOG2
# 	2:10:ENUM: enum:enum
# 	2:11:xcalDlg.cpp:xcalDlg2.cpp
# 	2:9:xcalDlg.h:xcalDlg2.h
# 	2:19:Application Include:dbtest.h
# 	2:15:CLASS: CXcalDlg:CXcalDlg
# End Section
# Section dbtest : {FD99C5EF-A0CB-4CF0-8D9D-26F3922B2A29}
# 	1:15:IDD_XCAL_DIALOG:109
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_XCAL_DIALOG:IDD_XCAL_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:11:xcalDlg.cpp:xcalDlg.cpp
# 	2:9:xcalDlg.h:xcalDlg.h
# 	2:19:Application Include:dbtest.h
# 	2:15:CLASS: CXcalDlg:CXcalDlg
# End Section
