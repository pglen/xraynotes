; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFlyout
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ftext.h"
LastPage=0

ClassCount=12
Class1=CBaseWnd
Class2=CFtextApp
Class3=CAboutDlg
Class4=CFtextDlg
Class5=CPGedit
Class6=CPopup
Class7=CxFedit

ResourceCount=5
Resource1=IDD_DIALOG2
Class8=PersistDlg
Class9=CxScroll
Class10=CxScrollBar
Resource2=IDD_DIALOG1
Class11=CExcRep
Resource3=IDD_FTEXT_DIALOG
Class12=CFlyout
Resource4=IDD_ABOUTBOX
Resource5=IDD_DIALOG3

[CLS:CBaseWnd]
Type=0
BaseClass=CWnd
HeaderFile=BaseWnd.h
ImplementationFile=BaseWnd.cpp

[CLS:CFtextApp]
Type=0
BaseClass=CWinApp
HeaderFile=ftext.h
ImplementationFile=ftext.cpp
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ftextDlg.cpp
ImplementationFile=ftextDlg.cpp
LastObject=CAboutDlg

[CLS:CFtextDlg]
Type=0
BaseClass=CGrayDlg
HeaderFile=ftextDlg.h
ImplementationFile=ftextDlg.cpp
Filter=W
VirtualFilter=dWC
LastObject=CFtextDlg

[CLS:CPGedit]
Type=0
BaseClass=CEdit
HeaderFile=PGedit.h
ImplementationFile=PGedit.cpp

[CLS:CPopup]
Type=0
BaseClass=CEdit
HeaderFile=Popup.h
ImplementationFile=Popup.cpp

[CLS:CxFedit]
Type=0
BaseClass=CBaseWnd
HeaderFile=xFedit.h
ImplementationFile=xFedit.cpp
Filter=W
VirtualFilter=WC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_FTEXT_DIALOG]
Type=1
Class=CFtextDlg
ControlCount=29
Control1=IDC_ED,static,1350762496
Control2=IDOK,button,1342242817
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_BUTTON2,button,1342242816
Control5=IDC_BUTTON3,button,1342242816
Control6=IDC_BUTTON4,button,1342242816
Control7=IDC_BUTTON5,button,1342242816
Control8=IDC_BUTTON6,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_COMBO1,combobox,1344339970
Control11=IDC_STATIC,static,1342308352
Control12=IDC_COMBO2,combobox,1344339970
Control13=IDC_STATIC,static,1342308352
Control14=IDC_COMBO3,combobox,1344339970
Control15=IDC_BUTTON7,button,1342242816
Control16=IDC_BUTTON8,button,1342242816
Control17=IDCANCEL,button,1073807360
Control18=IDC_BUTTON9,button,1342242816
Control19=IDC_BUTTON10,button,1342242816
Control20=IDC_BUTTON11,button,1342242816
Control21=IDC_BUTTON12,button,1342242816
Control22=IDC_BUTTON13,button,1342242816
Control23=IDC_BUTTON14,button,1342242816
Control24=IDC_BUTTON15,button,1342242816
Control25=IDC_BUTTON16,button,1342242816
Control26=IDC_BUTTON17,button,1342242816
Control27=IDC_STATIC,static,1342308352
Control28=IDC_BUTTON18,button,1342242816
Control29=IDC_EDIT1,edit,1350631552

[CLS:PersistDlg]
Type=0
HeaderFile=PersistDlg.h
ImplementationFile=PersistDlg.cpp
BaseClass=CDialog

[CLS:CxScroll]
Type=0
HeaderFile=xScroll.h
ImplementationFile=xScroll.cpp
BaseClass=CScrollBar
Filter=W

[CLS:CxScrollBar]
Type=0
HeaderFile=xScrollBar.h
ImplementationFile=xScrollBar.cpp
BaseClass=CScrollBar
Filter=W
VirtualFilter=WC
LastObject=CxScrollBar

[DLG:IDD_DIALOG1]
Type=1
Class=CExcRep
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1208025088
Control3=IDC_EXC_TEXT,static,1342308352
Control4=IDC_EXC_HEAD,static,1342308353
Control5=IDOK2,button,1342242816

[CLS:CExcRep]
Type=0
HeaderFile=ExcRep.h
ImplementationFile=ExcRep.cpp
BaseClass=CDialog
Filter=W
LastObject=IDC_EXC_HEAD
VirtualFilter=dWC

[DLG:IDD_DIALOG2]
Type=1
Class=CFlyout
ControlCount=0

[CLS:CFlyout]
Type=0
HeaderFile=Flyout.h
ImplementationFile=Flyout.cpp
BaseClass=CDialog

[DLG:IDD_DIALOG3]
Type=1
Class=?
ControlCount=0

