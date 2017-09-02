; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDCMConverterDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DCMConverter.h"

ClassCount=3
Class1=CDCMConverterApp
Class2=CDCMConverterDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DCMCONVERTER_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_DCMCONVERTER_DIALOG (English (U.S.))

[CLS:CDCMConverterApp]
Type=0
HeaderFile=DCMConverter.h
ImplementationFile=DCMConverter.cpp
Filter=N

[CLS:CDCMConverterDlg]
Type=0
HeaderFile=DCMConverterDlg.h
ImplementationFile=DCMConverterDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_DCM_SOURCE_FILE

[CLS:CAboutDlg]
Type=0
HeaderFile=DCMConverterDlg.h
ImplementationFile=DCMConverterDlg.cpp
Filter=D
LastObject=IDOK

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_DCMCONVERTER_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CDCMConverterDlg

[DLG:IDD_DCMCONVERTER_DIALOG (English (U.S.))]
Type=1
Class=CDCMConverterDlg
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BUTTON_CONVERT,button,1476460544
Control5=IDC_IMAGE_CONTAINER,static,1476395271
Control6=IDC_BUTTON_CONVERT2DCM,button,1476460544
Control7=IDC_STATIC,button,1342177287
Control8=IDC_EDIT_DCM_SOURCE_FILE,edit,1350633600
Control9=IDC_EDIT_DEST_BMP_FILE,edit,1350631552
Control10=IDC_BUTTON_OPEN_DCM,button,1342242816
Control11=IDC_STATIC,button,1342177287
Control12=IDC_EDIT_BMP_SOURCE_FILE,edit,1350633600
Control13=IDC_EDIT_DCM_DEST_FILE,edit,1350631552
Control14=IDC_BUTTON_OPEN_BMP,button,1342242816

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

