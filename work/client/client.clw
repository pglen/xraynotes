; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CClientDlg
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "client.h"

ClassCount=6
Class1=CClientApp
Class2=CClientDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CMySocket
Class5=CLogWin
Class6=CxTargetButton
Resource3=IDD_CLIENT_DIALOG

[CLS:CClientApp]
Type=0
HeaderFile=client.h
ImplementationFile=client.cpp
Filter=N

[CLS:CClientDlg]
Type=0
HeaderFile=clientDlg.h
ImplementationFile=clientDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT8

[CLS:CAboutDlg]
Type=0
HeaderFile=clientDlg.h
ImplementationFile=clientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CLIENT_DIALOG]
Type=1
Class=CClientDlg
ControlCount=38
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_BUTTON3,button,1342242816
Control3=IDC_EDIT2,edit,1350631552
Control4=IDC_BUTTON13,button,1342242817
Control5=IDC_BUTTON9,button,1342242816
Control6=IDC_BUTTON4,button,1342242816
Control7=IDC_BUTTON10,button,1342242816
Control8=IDC_BUTTON5,button,1342242816
Control9=IDC_BUTTON7,button,1342242816
Control10=IDC_BUTTON8,button,1342242816
Control11=IDC_BUTTON6,button,1342242816
Control12=IDC_BUTTON11,button,1342242816
Control13=IDC_BUTTON2,button,1342242816
Control14=IDC_BUTTON14,button,1342242816
Control15=IDOK,button,1342242816
Control16=IDC_BUTTON12,button,1342242816
Control17=IDC_LOG2,edit,1350631552
Control18=IDC_STATUS,static,1342312448
Control19=IDCANCEL,button,1208025088
Control20=IDC_EDIT1,edit,1350631552
Control21=IDC_BUTTON15,button,1342242816
Control22=IDC_BUTTON16,button,1342242816
Control23=IDC_BUTTON17,button,1342242816
Control24=IDC_EDIT3,edit,1350631584
Control25=IDC_EDIT4,edit,1350631552
Control26=IDC_BUTTON18,button,1342242816
Control27=IDC_BUTTON19,button,1342242816
Control28=IDC_BUTTON20,button,1342242816
Control29=IDC_BUTTON21,button,1342242816
Control30=IDC_EDIT5,edit,1350631552
Control31=IDC_EDIT6,edit,1350631552
Control32=IDC_EDIT7,edit,1350631552
Control33=IDC_STATIC,button,1342177287
Control34=IDC_BUTTON22,button,1342242816
Control35=IDC_EDIT8,edit,1350631552
Control36=IDC_STATIC,static,1342308352
Control37=IDC_STATIC,static,1342308352
Control38=IDC_STATIC,static,1342308352

[CLS:CMySocket]
Type=0
HeaderFile=MySocket.h
ImplementationFile=MySocket.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=uq
LastObject=CMySocket

[CLS:CLogWin]
Type=0
HeaderFile=LogWin.h
ImplementationFile=LogWin.cpp
BaseClass=CWnd

[CLS:CxTargetButton]
Type=0
HeaderFile=xTargetButton.h
ImplementationFile=xTargetButton.cpp
BaseClass=CButton
LastObject=CxTargetButton

