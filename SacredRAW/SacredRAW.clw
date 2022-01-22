; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSRDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SacredRAW.h"

ClassCount=5
Class1=CSRApp
Class2=CSRDlg

ResourceCount=4
Resource1=IDR_MAINFRAME
Resource2=IDD_NUMERICCONV_DIALOG
Class3=CNumericConvDlg
Class4=CTabPages
Resource3=IDD_SACREDRAW_DIALOG
Class5=CPageDlg
Resource4=IDD_PAGE_DIALOG

[CLS:CSRApp]
Type=0
HeaderFile=SacredRAW.h
ImplementationFile=SacredRAW.cpp
Filter=N

[CLS:CSRDlg]
Type=0
HeaderFile=SRDlg.h
ImplementationFile=SRDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CSRDlg



[DLG:IDD_SACREDRAW_DIALOG]
Type=1
Class=CSRDlg
ControlCount=3
Control1=IDC_TAB_MAIN,SysTabControl32,1342227010
Control2=IDC_EDIT_HERO,edit,1350633600
Control3=IDC_BUTTON_BROWSE,button,1342242816

[DLG:IDD_NUMERICCONV_DIALOG]
Type=1
Class=CNumericConvDlg
ControlCount=4
Control1=IDC_STATIC,button,1342210055
Control2=IDC_COMBO1,combobox,1344339971
Control3=IDC_STATIC,static,1342308354
Control4=IDC_LIST1,SysListView32,1342275589

[CLS:CNumericConvDlg]
Type=0
HeaderFile=NumericConvDlg.h
ImplementationFile=NumericConvDlg.cpp
BaseClass=CDialog
Filter=D

[CLS:CTabPages]
Type=0
HeaderFile=TabPages.h
ImplementationFile=TabPages.cpp
BaseClass=CTabCtrl
Filter=W
LastObject=CTabPages
VirtualFilter=UWC

[DLG:IDD_PAGE_DIALOG]
Type=1
Class=CPageDlg
ControlCount=1
Control1=IDC_HEX_EDIT_CTRL,static,1342312448

[CLS:CPageDlg]
Type=0
HeaderFile=PageDlg.h
ImplementationFile=PageDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPageDlg

