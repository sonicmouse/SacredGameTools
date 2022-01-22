; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSacredFilterDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SacredFilter.h"

ClassCount=6
Class1=CSacredFilterApp
Class2=CSacredFilterDlg

ResourceCount=3
Resource1=IDR_MAINFRAME
Class3=CSacredSocket
Class4=CFLGBox
Class5=CStaticInfo
Resource2=IDD_SACREDFILTER_DIALOG
Class6=CPluginsDlg
Resource3=IDD_PLUGINS_DIALOG

[CLS:CSacredFilterApp]
Type=0
HeaderFile=SacredFilter.h
ImplementationFile=SacredFilter.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CSacredFilterApp

[CLS:CSacredFilterDlg]
Type=0
HeaderFile=SacredFilterDlg.h
ImplementationFile=SacredFilterDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_STATIC_FLC



[DLG:IDD_SACREDFILTER_DIALOG]
Type=1
Class=CSacredFilterDlg
ControlCount=14
Control1=IDC_STATIC_FLC,button,1342177287
Control2=IDC_STATIC_FGC,button,1342177287
Control3=IDC_STATIC_INFO_CONNECTED,static,1342308352
Control4=IDC_STATIC_INFO_SENT,static,1342308352
Control5=IDC_STATIC_INFO_RECV,static,1342308352
Control6=IDC_STATIC_INFO_CONNECTED_G,static,1342308352
Control7=IDC_STATIC_INFO_SENT_G,static,1342308352
Control8=IDC_STATIC_INFO_RECV_G,static,1342308352
Control9=IDC_STATIC_FCTRL,button,1342177287
Control10=IDC_BUTTON_PLUGINS,button,1342242816
Control11=IDC_BUTTON_DISCONNECT,button,1342242816
Control12=IDC_BUTTON3,button,1342242816
Control13=IDC_BUTTON4,button,1342242816
Control14=IDCANCEL,button,1342242816

[CLS:CSacredSocket]
Type=0
HeaderFile=SacredSocket.h
ImplementationFile=SacredSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CSacredSocket

[CLS:CFLGBox]
Type=0
HeaderFile=FLGBox.h
ImplementationFile=FLGBox.cpp
BaseClass=CButton
Filter=W

[CLS:CStaticInfo]
Type=0
HeaderFile=StaticInfo.h
ImplementationFile=StaticInfo.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CStaticInfo

[DLG:IDD_PLUGINS_DIALOG]
Type=1
Class=CPluginsDlg
ControlCount=0

[CLS:CPluginsDlg]
Type=0
HeaderFile=PluginsDlg.h
ImplementationFile=PluginsDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPluginsDlg

