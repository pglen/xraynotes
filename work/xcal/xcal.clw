; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPopup
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "xcal.h"

ClassCount=8
Class1=CXcalApp
Class2=CXcalDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CvSplit
Class5=CxWnd
Class6=CxCalen
Resource3=IDD_CALDET_DIALOG1
Class7=CAlaDlg
Resource4=IDD_XCAL_DIALOG
Class8=CPopup
Resource5=IDD_DIALOG12

[CLS:CXcalApp]
Type=0
HeaderFile=xcal.h
ImplementationFile=xcal.cpp
Filter=N

[CLS:CXcalDlg]
Type=0
HeaderFile=xcalDlg.h
ImplementationFile=xcalDlg.cpp
Filter=W
LastObject=CXcalDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=xcalDlg.h
ImplementationFile=xcalDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_XCAL_DIALOG]
Type=1
Class=CXcalDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SPLIT,static,1342377984
Control4=IDC_XC_BUTTON1,button,1342242816
Control5=IDC_XC_BUTTON2,button,1342242816
Control6=IDC_XC_BUTTON3,button,1342242816
Control7=IDC_XC_BUTTON4,button,1342242816
Control8=IDC_XC_BUTTON5,button,1342242816
Control9=IDC_XC_BUTTON6,button,1342242816
Control10=IDC_LAB,static,1342308352

[CLS:CvSplit]
Type=0
HeaderFile=vSplit.h
ImplementationFile=vSplit.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC

[CLS:CxWnd]
Type=0
HeaderFile=xWnd.h
ImplementationFile=xWnd.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC

[CLS:CxCalen]
Type=0
HeaderFile=xCalen.h
ImplementationFile=xCalen.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC

[CLS:CAlaDlg]
Type=0
HeaderFile=AlaDlg.h
ImplementationFile=AlaDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_TREE1
VirtualFilter=dWC

[DLG:IDD_DIALOG12]
Type=1
Class=CPopup
ControlCount=0

[CLS:CPopup]
Type=0
HeaderFile=Popup.h
ImplementationFile=Popup.cpp
BaseClass=CDialog

[DLG:IDD_CALDET_DIALOG1]
Type=1
Class=CAlaDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CD_EDIT1,edit,1350631552
Control4=IDC_CD_EDIT2,edit,1350631552
Control5=IDC_CD_EDIT3,edit,1350631552
Control6=IDC_CD_EDIT4,edit,1350631552
Control7=IDC_CD_EDIT5,edit,1350631552
Control8=IDC_CD_TREE1,SysTreeView32,1350631424

