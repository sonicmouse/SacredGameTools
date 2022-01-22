; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEditItemDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SacredItemEdit.h"

ClassCount=16
Class1=CSIEApp
Class2=CSIEDlg

ResourceCount=13
Resource1=IDR_MAINFRAME
Class3=CHexEdit
Resource2=IDD_IMPORTTOSLOT_DIALOG
Class4=CEditItemDlg
Resource3=IDD_ABOUT_DIALOG
Resource4=IDD_DIALOG_SITBROWSE
Class5=CExportItemDlg
Class6=CImageViewer
Class7=CTreeCtrlEx
Class8=CSITFileDlg
Resource5=IDD_SACREDITEMEDIT_DIALOG
Resource6=IDD_EDITITEM_DIALOG
Class9=CDumpViewerDlg
Resource7=IDD_PLACEMENT_DIALOG
Class10=CAboutDlg
Resource8=IDD_EXPORTITEM_DIALOG
Class11=CPlacementDlg
Class12=CPlacementDisplay
Class13=CPicFileDlg
Resource9=IDD_DUPITEM_DIALOG
Resource10=IDD_DUMPVIEWER_DIALOG
Class14=CDupItemDlg
Resource11=IDD_DIALOG_PICBROWSE
Class15=CImportItemDlg
Resource12=IDD_IMPORTITEM_DIALOG
Class16=CImportToSlotDlg
Resource13=IDR_EDIT_COPY

[CLS:CSIEApp]
Type=0
HeaderFile=SacredItemEdit.h
ImplementationFile=SacredItemEdit.cpp
Filter=N
LastObject=CSIEApp

[CLS:CSIEDlg]
Type=0
HeaderFile=SIEDlg.h
ImplementationFile=SIEDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_TREE_ITEMS



[DLG:IDD_SACREDITEMEDIT_DIALOG]
Type=1
Class=CSIEDlg
ControlCount=20
Control1=IDC_BUTTON_BROWSE,button,1342242817
Control2=IDC_BUTTON_CLOSE,button,1342242816
Control3=IDC_BUTTON_REFRESH,button,1342242816
Control4=IDC_BUTTON_EXPORT,button,1342242816
Control5=IDC_BUTTON_IMPORT,button,1342242816
Control6=IDC_BUTTON_DELETE,button,1342242816
Control7=IDC_BUTTON_GRID,button,1342242816
Control8=IDC_BUTTON_DUP,button,1342242816
Control9=IDC_BUTTON_EDIT,button,1342242816
Control10=IDC_BUTTON_UNSLOT,button,1342242816
Control11=IDC_BUTTON_COMPRESS,button,1342242816
Control12=IDC_BUTTON_UPDATE,button,1342242816
Control13=IDC_BUTTON_ABOUT,button,1342242816
Control14=IDCANCEL,button,1342242816
Control15=IDC_TREE_ITEMS,SysTreeView32,1342246951
Control16=IDC_EDIT_HERO,edit,1350633600
Control17=IDC_STATIC_TOTALITEMS,static,1342312449
Control18=IDC_STATIC_CHARFILE,button,1342178055
Control19=IDC_STATIC_ACT_FRAME,button,1342178055
Control20=IDC_BUTTON_IMPORTSLOT,button,1342242816

[CLS:CHexEdit]
Type=0
HeaderFile=HexEdit.h
ImplementationFile=HexEdit.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=CHexEdit

[DLG:IDD_EDITITEM_DIALOG]
Type=1
Class=CEditItemDlg
ControlCount=15
Control1=IDC_HEX_EDIT_CTRL,static,1342312448
Control2=IDC_STATIC,static,1342308354
Control3=IDC_COMBO_BYTEORDER,combobox,1344339971
Control4=IDC_LIST_CONVERT,SysListView32,1342292101
Control5=IDC_CHECK_DEC,button,1342242819
Control6=IDC_COMBO_VIEW,combobox,1344339971
Control7=IDCANCEL,button,1342242816
Control8=IDOK,button,1342242816
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,button,1342177287
Control11=IDC_BUTTON_EX_HEADER,button,1342242816
Control12=IDC_BUTTON_RESET,button,1342242816
Control13=IDC_BUTTON_EX_DATA,button,1342242816
Control14=IDC_BUTTON_IM_HEADER,button,1342242816
Control15=IDC_BUTTON_IM_DATA,button,1342242816

[CLS:CEditItemDlg]
Type=0
HeaderFile=EditItemDlg.h
ImplementationFile=EditItemDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST_CONVERT

[MNU:IDR_EDIT_COPY]
Type=1
Class=CEditItemDlg
Command1=IDM_COPY
CommandCount=1

[DLG:IDD_EXPORTITEM_DIALOG]
Type=1
Class=CExportItemDlg
ControlCount=13
Control1=IDC_STATIC,static,1342308354
Control2=IDC_EDIT_NAME,edit,1342242944
Control3=IDC_STATIC,static,1342308354
Control4=IDC_EDIT_DESC,edit,1344344132
Control5=IDC_STATIC,static,1342308354
Control6=IDC_EDIT_AUTHOR,edit,1342242944
Control7=IDC_STATIC,static,1342308354
Control8=IDC_STATIC_PICTURE,static,1342308608
Control9=IDOK,button,1342242816
Control10=IDCANCEL,button,1342242816
Control11=IDC_STATIC,static,1342308354
Control12=IDC_EDIT_ID,edit,1342253185
Control13=IDC_STATIC,static,1342308353

[CLS:CExportItemDlg]
Type=0
HeaderFile=ExportItemDlg.h
ImplementationFile=ExportItemDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CExportItemDlg

[CLS:CImageViewer]
Type=0
HeaderFile=ImageViewer.h
ImplementationFile=ImageViewer.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CImageViewer

[CLS:CTreeCtrlEx]
Type=0
HeaderFile=TreeCtrlEx.h
ImplementationFile=TreeCtrlEx.cpp
BaseClass=CTreeCtrl
Filter=W

[CLS:CSITFileDlg]
Type=0
HeaderFile=SITFileDlg.h
ImplementationFile=SITFileDlg.cpp
BaseClass=CFileDialog
Filter=D
LastObject=CSITFileDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_SITBROWSE]
Type=1
Class=?
ControlCount=11
Control1=IDC_STATIC_IMG,static,1342308608
Control2=IDC_STATIC,static,1342308354
Control3=IDC_STATIC,static,1342308354
Control4=IDC_STATIC,static,1342308354
Control5=IDC_BUTTON_VIEWDUMP,button,1342242816
Control6=IDC_EDIT_NAMEZ,edit,1342244992
Control7=IDC_EDIT_DESCR,edit,1344342084
Control8=IDC_EDIT_AUTHZ,edit,1342244992
Control9=IDC_EDIT_IDZ,edit,1342244993
Control10=IDC_STATIC,static,1342308354
Control11=IDC_STATIC_INVSIZEZ,static,1342308353

[DLG:IDD_DUMPVIEWER_DIALOG]
Type=1
Class=CDumpViewerDlg
ControlCount=1
Control1=IDC_STATIC_DUMPER,static,1342312448

[CLS:CDumpViewerDlg]
Type=0
HeaderFile=DumpViewerDlg.h
ImplementationFile=DumpViewerDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDumpViewerDlg

[DLG:IDD_ABOUT_DIALOG]
Type=1
Class=CAboutDlg
ControlCount=2
Control1=IDC_STATIC_ABOUT,static,1342308353
Control2=IDOK,button,1342242816

[CLS:CAboutDlg]
Type=0
HeaderFile=AboutDlg.h
ImplementationFile=AboutDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_PLACEMENT_DIALOG]
Type=1
Class=CPlacementDlg
ControlCount=2
Control1=IDC_STATIC_CH,static,1342308608
Control2=IDC_STATIC_BP,static,1342308608

[CLS:CPlacementDlg]
Type=0
HeaderFile=PlacementDlg.h
ImplementationFile=PlacementDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPlacementDlg

[CLS:CPlacementDisplay]
Type=0
HeaderFile=PlacementDisplay.h
ImplementationFile=PlacementDisplay.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CPlacementDisplay

[CLS:CPicFileDlg]
Type=0
HeaderFile=PicFileDlg.h
ImplementationFile=PicFileDlg.cpp
BaseClass=CFileDialog
Filter=D
LastObject=CPicFileDlg

[DLG:IDD_DIALOG_PICBROWSE]
Type=1
Class=?
ControlCount=2
Control1=IDC_STATIC_IMGV,static,1342308352
Control2=IDC_CHECK_SHOW,button,1342242819

[DLG:IDD_DUPITEM_DIALOG]
Type=1
Class=CDupItemDlg
ControlCount=7
Control1=IDC_EDIT_TIMES,edit,1350639745
Control2=IDC_STATIC,button,1342178055
Control3=IDC_CHECK_SHRINK,button,1342242819
Control4=IDCANCEL,button,1342242816
Control5=IDOK,button,1342242816
Control6=IDC_COMBO_TO,combobox,1344339971
Control7=IDC_SPIN_DUPER,msctls_updown32,1342177575

[CLS:CDupItemDlg]
Type=0
HeaderFile=DupItemDlg.h
ImplementationFile=DupItemDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_TO

[DLG:IDD_IMPORTITEM_DIALOG]
Type=1
Class=CImportItemDlg
ControlCount=7
Control1=IDC_EDIT_IMPORTITEM,edit,1342244992
Control2=IDC_BUTTON_IMPORT_BROWSE,button,1342242816
Control3=IDC_CHECK_IMPORT_SHRINK,button,1342242819
Control4=IDC_COMBO_IMPORTTO,combobox,1344339971
Control5=IDCANCEL,button,1342242816
Control6=IDOK,button,1342242816
Control7=IDC_STATIC,button,1342177287

[CLS:CImportItemDlg]
Type=0
HeaderFile=ImportItemDlg.h
ImplementationFile=ImportItemDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_IMPORTTO

[DLG:IDD_IMPORTTOSLOT_DIALOG]
Type=1
Class=CImportToSlotDlg
ControlCount=11
Control1=IDC_STATIC,button,1342177287
Control2=IDC_EDIT_ITEMSLOT,edit,1342244992
Control3=IDC_BUTTON_BROWSE_ITOS,button,1342242816
Control4=IDC_RADIO_BRONZE,button,1342308361
Control5=IDC_RADIO_SILVER,button,1342177289
Control6=IDC_RADIO_GOLD,button,1342177289
Control7=IDC_RADIO_GREEN,button,1342177289
Control8=IDC_RADIO_PLATINUM,button,1342177289
Control9=IDC_STATIC,button,1342177287
Control10=IDOK,button,1342242816
Control11=IDCANCEL,button,1342242816

[CLS:CImportToSlotDlg]
Type=0
HeaderFile=ImportToSlotDlg.h
ImplementationFile=ImportToSlotDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CImportToSlotDlg

