; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CInfoBar00Dlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "infobar00.h"
LastPage=0

ClassCount=7
Class1=CDlgAbout
Class2=CDlgNetConf
Class3=CDlgSysConf
Class4=CInfoBar00App
Class5=CAboutDlg
Class6=CInfoBar00Dlg

ResourceCount=12
Resource1=IDD_DLG_ABOUT (âpåÍ (±“ÿ∂))
Resource2=IDD_DLG_SYSCONF
Resource3=IDD_INFOBAR00_DIALOG
Resource4=IDD_DLG_INST
Resource5=IDD_DLG_NETCONF (âpåÍ (±“ÿ∂))
Class7=CInstDlg
Resource6=IDD_INFOBAR00_DIALOG (âpåÍ (±“ÿ∂))
Resource7=IDD_DLG_NETCONF
Resource8=IDR_MENU_POPUP
Resource9=IDD_DLG_SYSCONF (âpåÍ (±“ÿ∂))
Resource10=IDD_DLG_ABOUT
Resource11=IDD_DLG_INST (âpåÍ (±“ÿ∂))
Resource12=IDR_MENU_POPUP (âpåÍ (±“ÿ∂))

[CLS:CDlgAbout]
Type=0
BaseClass=CPropertyPage
HeaderFile=DlgAbout.h
ImplementationFile=DlgAbout.cpp

[CLS:CDlgNetConf]
Type=0
BaseClass=CPropertyPage
HeaderFile=DlgNetConf.h
ImplementationFile=DlgNetConf.cpp
Filter=D
VirtualFilter=idWC

[CLS:CDlgSysConf]
Type=0
BaseClass=CPropertyPage
HeaderFile=DlgSysConf.h
ImplementationFile=DlgSysConf.cpp
Filter=D
VirtualFilter=idWC
LastObject=IDC_EDIT_REFRESH

[CLS:CInfoBar00App]
Type=0
BaseClass=CWinApp
HeaderFile=InfoBar00.h
ImplementationFile=InfoBar00.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=InfoBar00Dlg.cpp
ImplementationFile=InfoBar00Dlg.cpp

[CLS:CInfoBar00Dlg]
Type=0
BaseClass=CDialog
HeaderFile=InfoBar00Dlg.h
ImplementationFile=InfoBar00Dlg.cpp
LastObject=CInfoBar00Dlg
Filter=D
VirtualFilter=dWC

[DLG:IDD_DLG_NETCONF]
Type=1
Class=CDlgNetConf
ControlCount=19
Control1=IDC_STATIC,button,1342177287
Control2=IDC_COMBO_QLIST,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_EDIT_ADDR,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_EDIT_PORT,edit,1350631552
Control7=IDC_STATIC,static,1342308864
Control8=IDC_EDIT_PROXY,edit,1350631552
Control9=IDC_STATIC,static,1342308864
Control10=IDC_EDIT_TITLE,edit,1350631552
Control11=IDC_STATIC,static,1342308864
Control12=IDC_EDIT_HEADER,edit,1350631552
Control13=IDC_STATIC,static,1342308864
Control14=IDC_EDIT_SKIP,edit,1350631552
Control15=IDC_STATIC,static,1342308864
Control16=IDC_EDIT_GETNUM,edit,1350631552
Control17=IDC_STATIC,static,1342308864
Control18=IDC_EDIT_LIMIT,edit,1484849280
Control19=IDC_STATIC,static,1342308864

[DLG:IDD_DLG_SYSCONF]
Type=1
Class=CDlgSysConf
ControlCount=16
Control1=IDC_STATIC,static,1342308864
Control2=IDC_EDIT_TIMER,edit,1350631552
Control3=IDC_STATIC,static,1342308864
Control4=IDC_EDIT_SPEED,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342308864
Control8=IDC_STATIC,static,1342308864
Control9=IDC_AREA_BACK_COLOR,static,1342177298
Control10=IDC_AREA_FORE_COLOR,static,1342177298
Control11=IDC_BTN_BACK_COLOR,button,1342242816
Control12=IDC_BTN_FORE_COLOR,button,1342242816
Control13=IDC_STATIC,static,1342308864
Control14=IDC_EDIT_REFRESH,edit,1350631552
Control15=IDC_STATIC,static,1342308864
Control16=IDC_CHK_TOPMOST,button,1342242819

[DLG:IDD_INFOBAR00_DIALOG]
Type=1
Class=CInfoBar00Dlg
ControlCount=1
Control1=IDC_INFO_AREA,static,1342312448

[DLG:IDD_DLG_INST]
Type=1
Class=CInstDlg
ControlCount=14
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308864
Control3=IDC_STATIC,static,1342177296
Control4=IDC_STATIC,button,1342177287
Control5=IDC_R_AUTOEXEC,button,1342373897
Control6=IDC_R_AUTOEXEC2,button,1342177289
Control7=IDC_R_AUTOEXEC3,button,1476395017
Control8=IDC_R_AUTOEXEC4,button,1476395017
Control9=IDC_STATIC,button,1342177287
Control10=IDC_CHECK_UNINST_CPL,button,1476460547
Control11=IDC_CHECK_UNINST_START,button,1342242819
Control12=IDOK,button,1342242816
Control13=IDCANCEL,button,1342242816
Control14=IDC_BTN_HELP,button,1476460544

[MNU:IDR_MENU_POPUP]
Type=1
Class=?
Command1=ID_MENU_CONFIG
Command2=ID_MENU_EXIT
CommandCount=2

[CLS:CInstDlg]
Type=0
HeaderFile=InstDlg.h
ImplementationFile=InstDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_DLG_ABOUT]
Type=1
Class=CDlgAbout
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342312448

[DLG:IDD_INFOBAR00_DIALOG (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=1
Control1=IDC_INFO_AREA,static,1342312448

[DLG:IDD_DLG_SYSCONF (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=16
Control1=IDC_STATIC,static,1342308864
Control2=IDC_EDIT_TIMER,edit,1350631552
Control3=IDC_STATIC,static,1342308864
Control4=IDC_EDIT_SPEED,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342308864
Control8=IDC_STATIC,static,1342308864
Control9=IDC_AREA_BACK_COLOR,static,1342177298
Control10=IDC_AREA_FORE_COLOR,static,1342177298
Control11=IDC_BTN_BACK_COLOR,button,1342242816
Control12=IDC_BTN_FORE_COLOR,button,1342242816
Control13=IDC_STATIC,static,1342308864
Control14=IDC_EDIT_REFRESH,edit,1350631552
Control15=IDC_STATIC,static,1342308864
Control16=IDC_CHK_TOPMOST,button,1342242819

[DLG:IDD_DLG_NETCONF (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=19
Control1=IDC_STATIC,button,1342177287
Control2=IDC_COMBO_QLIST,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_EDIT_ADDR,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_EDIT_PORT,edit,1350631552
Control7=IDC_STATIC,static,1342308864
Control8=IDC_EDIT_PROXY,edit,1350631552
Control9=IDC_STATIC,static,1342308864
Control10=IDC_EDIT_TITLE,edit,1350631552
Control11=IDC_STATIC,static,1342308864
Control12=IDC_EDIT_HEADER,edit,1350631552
Control13=IDC_STATIC,static,1342308864
Control14=IDC_EDIT_SKIP,edit,1350631552
Control15=IDC_STATIC,static,1342308864
Control16=IDC_EDIT_GETNUM,edit,1350631552
Control17=IDC_STATIC,static,1342308864
Control18=IDC_EDIT_LIMIT,edit,1484849280
Control19=IDC_STATIC,static,1342308864

[DLG:IDD_DLG_INST (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=14
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308864
Control3=IDC_STATIC,static,1342177296
Control4=IDC_STATIC,button,1342177287
Control5=IDC_R_AUTOEXEC,button,1342373897
Control6=IDC_R_AUTOEXEC2,button,1342177289
Control7=IDC_R_AUTOEXEC3,button,1476395017
Control8=IDC_R_AUTOEXEC4,button,1476395017
Control9=IDC_STATIC,button,1342177287
Control10=IDC_CHECK_UNINST_CPL,button,1476460547
Control11=IDC_CHECK_UNINST_START,button,1342242819
Control12=IDOK,button,1342242816
Control13=IDCANCEL,button,1342242816
Control14=IDC_BTN_HELP,button,1476460544

[DLG:IDD_DLG_ABOUT (âpåÍ (±“ÿ∂))]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342312448

[MNU:IDR_MENU_POPUP (âpåÍ (±“ÿ∂))]
Type=1
Class=?
Command1=ID_MENU_CONFIG
Command2=ID_MENU_EXIT
CommandCount=2

