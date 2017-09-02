; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVgridDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "vgrid.h"

ClassCount=7
Class1=CVgridApp
Class2=CVgridDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_VGRID_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_MENUBASE
Class5=CTransSlider
Class6=PersistDlg
Class7=CPopup
Resource4=IDD_ABOUTBOX
Class4=CXrayM
Resource5=IDR_MENU1

[CLS:CVgridApp]
Type=0
HeaderFile=vgrid.h
ImplementationFile=vgrid.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CVgridDlg]
Type=0
HeaderFile=vgridDlg.h
ImplementationFile=vgridDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_NEW_SHOWCOARSEGRID

[CLS:CAboutDlg]
Type=0
HeaderFile=vgridDlg.h
ImplementationFile=vgridDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VGRID_DIALOG]
Type=1
Class=CVgridDlg
ControlCount=0

[DLG:IDD_MENUBASE]
Type=1
Class=CXrayM
ControlCount=2
Control1=IDOK,button,1208025089
Control2=IDCANCEL,button,1073807360

[CLS:CXrayM]
Type=0
HeaderFile=XrayM.h
ImplementationFile=XrayM.cpp
BaseClass=CGrayDlg
Filter=W
VirtualFilter=dWC

[CLS:CTransSlider]
Type=0
HeaderFile=TransSlider.h
ImplementationFile=TransSlider.cpp
BaseClass=CSliderCtrl

[CLS:PersistDlg]
Type=0
HeaderFile=PersistDlg.h
ImplementationFile=PersistDlg.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC

[CLS:CPopup]
Type=0
HeaderFile=Popup.h
ImplementationFile=Popup.cpp
BaseClass=CEdit
LastObject=ID_NEW_SHOWGRIDDEFAULT

[MNU:IDR_MENU1]
Type=1
Class=CVgridDlg
Command1=ID_NEW_SHOWGRIDDEFAULT
Command2=ID_NEW_SHOWFINEGRID
Command3=ID_NEW_SHOWCOARSEGRID
Command4=ID_NEW_SHOWCOLORPATETTE
Command5=ID_NEW_SHOWBLAHBLAH
Command6=ID_NEW_SHOWREDGRADIENT
Command7=ID_NEW_COLORGRADIENTS_SHOWGREENGRADIENT
Command8=ID_NEW_COLORGRADIENTS_SHOWBLUEGRADIENT
Command9=ID_NEW_SHOWBLEH
CommandCount=9

