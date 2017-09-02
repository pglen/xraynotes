; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUnicodeDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "unicode.h"
ODLFile=unicode.odl

ClassCount=4
Class1=CUnicodeApp
Class2=CUnicodeDlg
Class3=CAboutDlg
Class4=CUnicodeDlgAutoProxy

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_UNICODE_DIALOG

[CLS:CUnicodeApp]
Type=0
HeaderFile=unicode.h
ImplementationFile=unicode.cpp
Filter=N

[CLS:CUnicodeDlg]
Type=0
HeaderFile=unicodeDlg.h
ImplementationFile=unicodeDlg.cpp
Filter=D

[CLS:CAboutDlg]
Type=0
HeaderFile=unicodeDlg.h
ImplementationFile=unicodeDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CUnicodeDlgAutoProxy]
Type=0
HeaderFile=DlgProxy.h
ImplementationFile=DlgProxy.cpp
BaseClass=CCmdTarget
Filter=N

[DLG:IDD_UNICODE_DIALOG]
Type=1
Class=CUnicodeDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=ID_HELP,button,1342242816
Control4=IDC_STATIC,static,1342308352

