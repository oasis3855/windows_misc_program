; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgNetStock
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "infobar00.h"
LastPage=0

ClassCount=8
Class1=CDlgNetConf
Class2=CDlgSysConf
Class3=CInfoBar00App
Class4=CInfoBar00Dlg
Class5=CInstDlg

ResourceCount=14
Resource1=IDD_DLG_NETCONF
Resource2=IDD_DLG_SYSCONF (âpåÍ (±“ÿ∂))
Resource3=IDD_DLG_SYSCONF
Resource4=IDD_DLG_INST (âpåÍ (±“ÿ∂))
Resource5=IDD_DLG_NETCONF (âpåÍ (±“ÿ∂))
Resource6=IDD_DLG_ABOUT
Resource7=IDD_INFOBAR00_DIALOG
Resource8=IDD_INFOBAR00_DIALOG (âpåÍ (±“ÿ∂))
Resource9=IDD_DLG_INST
Resource10=IDR_MENU_POPUP
Resource11=IDD_DLG_NET_STOCK
Class6=CDlgAbout
Resource12=IDD_DLG_ABOUT (âpåÍ (±“ÿ∂))
Class7=CDlgNetStock
Resource13=IDD_DLG_NET_STOCK (âpåÍ (±“ÿ∂))
Class8=CNetAccessThread
Resource14=IDR_MENU_POPUP (âpåÍ (±“ÿ∂))

[CLS:CDlgNetConf]
Type=0
BaseClass=CPropertyPage
HeaderFile=DlgNetConf.h
ImplementationFile=DlgNetConf.cpp
LastObject=IDC_EDIT_PORT
Filter=D
VirtualFilter=idWC

[CLS:CDlgSysConf]
Type=0
BaseClass=CPropertyPage
HeaderFile=DlgSysConf.h
ImplementationFile=DlgSysConf.cpp
LastObject=CDlgSysConf
Filter=D
VirtualFilter=idWC

[CLS:CInfoBar00App]
Type=0
BaseClass=CWinApp
HeaderFile=InfoBar00.h
ImplementationFile=InfoBar00.cpp

[CLS:CInfoBar00Dlg]
Type=0
BaseClass=CDialog
HeaderFile=InfoBar00Dlg.h
ImplementationFile=InfoBar00Dlg.cpp
LastObject=CInfoBar00Dlg
Filter=D
VirtualFilter=dWC

[CLS:CInstDlg]
Type=0
BaseClass=CDialog
HeaderFile=InstDlg.h
ImplementationFile=InstDlg.cpp

[DLG:IDD_DLG_NETCONF]
Type=1
Class=CDlgNetConf
ControlCount=22
Control1=IDC_STATIC,button,1342177287
Control2=IDC_COMBO_QLIST,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_EDIT_ADDR,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_EDIT_PORT,combobox,1344340226
Control7=IDC_STATIC,static,1342308864
Control8=IDC_EDIT_PROXY,edit,1350631552
Control9=IDC_STATIC,static,1342308864
Control10=IDC_STATIC,static,1342308864
Control11=IDC_EDIT_TITLE,edit,1350631552
Control12=IDC_STATIC,static,1342308864
Control13=IDC_EDIT_HEADER,edit,1350631552
Control14=IDC_STATIC,static,1342308864
Control15=IDC_EDIT_SKIP,edit,1350631552
Control16=IDC_CHK_DELSPACE,button,1342242819
Control17=IDC_CHK_CR2SPC,button,1342242819
Control18=IDC_STATIC,static,1342308864
Control19=IDC_EDIT_GETNUM,edit,1350631552
Control20=IDC_STATIC,static,1342308864
Control21=IDC_EDIT_LIMIT,edit,1484849280
Control22=IDC_BTN_DETAIL,button,1342242816

[DLG:IDD_DLG_SYSCONF]
Type=1
Class=CDlgSysConf
ControlCount=20
Control1=IDC_STATIC,static,1342308864
Control2=IDC_EDIT_TIMER,edit,1350631552
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_EDIT_SPEED,edit,1350631552
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342308864
Control8=IDC_AREA_BACK_COLOR,static,1342177298
Control9=IDC_BTN_BACK_COLOR,button,1342242816
Control10=IDC_STATIC,static,1342308864
Control11=IDC_AREA_FORE_COLOR,static,1342177298
Control12=IDC_BTN_FORE_COLOR,button,1342242816
Control13=IDC_STATIC,static,1342308864
Control14=IDC_EDIT_REFRESH,edit,1350631552
Control15=IDC_STATIC,static,1342308864
Control16=IDC_CHK_TOPMOST,button,1342242819
Control17=IDC_TXT_FONT,static,1342312960
Control18=IDC_BTN_FONT,button,1342242816
Control19=IDC_STATIC,static,1342308864
Control20=IDC_CHK_DEBUG,button,1342242819

[DLG:IDD_INFOBAR00_DIALOG]
Type=1
Class=CInfoBar00Dlg
ControlCount=0

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
Control14=IDC_BTN_HELP,button,1342242816

[DLG:IDD_DLG_ABOUT]
Type=1
Class=CDlgAbout
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_BTN_MAKEUNIN,button,1342242816
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_INFOBAR00_DIALOG (âpåÍ (±“ÿ∂))]
Type=1
Class=CInfoBar00Dlg
ControlCount=0

[DLG:IDD_DLG_SYSCONF (âpåÍ (±“ÿ∂))]
Type=1
Class=CDlgSysConf
ControlCount=19
Control1=IDC_STATIC,static,1342308864
Control2=IDC_EDIT_TIMER,edit,1350631552
Control3=IDC_STATIC,static,1342308864
Control4=IDC_STATIC,static,1342308864
Control5=IDC_EDIT_SPEED,edit,1350631552
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342308864
Control8=IDC_AREA_BACK_COLOR,static,1342177298
Control9=IDC_BTN_BACK_COLOR,button,1342242816
Control10=IDC_STATIC,static,1342308864
Control11=IDC_AREA_FORE_COLOR,static,1342177298
Control12=IDC_BTN_FORE_COLOR,button,1342242816
Control13=IDC_STATIC,static,1342308864
Control14=IDC_EDIT_REFRESH,edit,1350631552
Control15=IDC_STATIC,static,1342308864
Control16=IDC_CHK_TOPMOST,button,1342242819
Control17=IDC_TXT_FONT,static,1342312960
Control18=IDC_BTN_FONT,button,1342242816
Control19=IDC_CHK_DEBUG,button,1342242819

[DLG:IDD_DLG_NETCONF (âpåÍ (±“ÿ∂))]
Type=1
Class=CDlgNetConf
ControlCount=22
Control1=IDC_STATIC,button,1342177287
Control2=IDC_COMBO_QLIST,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_EDIT_ADDR,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_EDIT_PORT,combobox,1344340226
Control7=IDC_STATIC,static,1342308864
Control8=IDC_EDIT_PROXY,edit,1350631552
Control9=IDC_STATIC,static,1342308864
Control10=IDC_STATIC,static,1342308864
Control11=IDC_EDIT_TITLE,edit,1350631552
Control12=IDC_STATIC,static,1342308864
Control13=IDC_EDIT_HEADER,edit,1350631552
Control14=IDC_STATIC,static,1342308864
Control15=IDC_EDIT_SKIP,edit,1350631552
Control16=IDC_CHK_DELSPACE,button,1342242819
Control17=IDC_CHK_CR2SPC,button,1342242819
Control18=IDC_STATIC,static,1342308864
Control19=IDC_EDIT_GETNUM,edit,1350631552
Control20=IDC_STATIC,static,1342308864
Control21=IDC_EDIT_LIMIT,edit,1484849280
Control22=IDC_BTN_DETAIL,button,1342242816

[DLG:IDD_DLG_INST (âpåÍ (±“ÿ∂))]
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

[DLG:IDD_DLG_ABOUT (âpåÍ (±“ÿ∂))]
Type=1
Class=CDlgAbout
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342177283
Control3=IDC_BTN_MAKEUNIN,button,1342242816
Control4=IDC_STATIC,static,1342308864

[MNU:IDR_MENU_POPUP]
Type=1
Class=?
Command1=ID_MENU_CONFIG
Command2=ID_MENU_HELP
Command3=ID_MENU_EXIT
CommandCount=3

[MNU:IDR_MENU_POPUP (âpåÍ (±“ÿ∂))]
Type=1
Class=?
Command1=ID_MENU_CONFIG
Command2=ID_MENU_HELP
Command3=ID_MENU_EXIT
CommandCount=3

[CLS:CDlgAbout]
Type=0
HeaderFile=DlgAbout.h
ImplementationFile=DlgAbout.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_BTN_MAKEUNIN
VirtualFilter=idWC

[DLG:IDD_DLG_NET_STOCK]
Type=1
Class=CDlgNetStock
ControlCount=16
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CMB_PRESET,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_EDIT_ADDR,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342308864
Control8=IDC_STATIC,static,1342308864
Control9=IDC_EDIT_PH,edit,1350631552
Control10=IDC_EDIT_PHB,edit,1350631552
Control11=IDC_EDIT_PHC,edit,1350631552
Control12=IDC_STATIC,static,1342308864
Control13=IDC_EDIT_ITEMS,edit,1352732804
Control14=IDC_BTN_SMPL,button,1342242816
Control15=IDOK,button,1342242817
Control16=IDCANCEL,button,1342242816

[CLS:CDlgNetStock]
Type=0
HeaderFile=DlgNetStock.h
ImplementationFile=DlgNetStock.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BTN_SMPL

[DLG:IDD_DLG_NET_STOCK (âpåÍ (±“ÿ∂))]
Type=1
Class=CDlgNetStock
ControlCount=16
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CMB_PRESET,combobox,1344339971
Control3=IDC_STATIC,static,1342308864
Control4=IDC_EDIT_ADDR,edit,1350631552
Control5=IDC_STATIC,static,1342308864
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342308864
Control8=IDC_STATIC,static,1342308864
Control9=IDC_EDIT_PH,edit,1350631552
Control10=IDC_EDIT_PHB,edit,1350631552
Control11=IDC_EDIT_PHC,edit,1350631552
Control12=IDC_STATIC,static,1342308864
Control13=IDC_EDIT_ITEMS,edit,1352732804
Control14=IDC_BTN_SMPL,button,1342242816
Control15=IDOK,button,1342242817
Control16=IDCANCEL,button,1342242816

[CLS:CNetAccessThread]
Type=0
HeaderFile=NetAccessThread.h
ImplementationFile=NetAccessThread.cpp
BaseClass=CWinThread
Filter=N
LastObject=CNetAccessThread

