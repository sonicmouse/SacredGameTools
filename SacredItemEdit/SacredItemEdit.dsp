# Microsoft Developer Studio Project File - Name="SacredItemEdit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SacredItemEdit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SacredItemEdit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SacredItemEdit.mak" CFG="SacredItemEdit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SacredItemEdit - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SacredItemEdit - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SacredItemEdit - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "./hexeditctrl/" /I "./CxImage/" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SacredItemEdit - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./hexeditctrl/" /I "./CxImage/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SacredItemEdit - Win32 Release"
# Name "SacredItemEdit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "HexEdit Control (cpp)"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\hexeditctrl\PPDumpCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\hexeditctrl\PPNumEdit.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cgfiltyp.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\DumpViewerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DupItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Hero.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportToSlotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PicFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlacementDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\PlacementDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SacredItemEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Section.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionPlacement.cpp
# End Source File
# Begin Source File

SOURCE=.\SIEDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SITFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SITFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ZlibWrapper.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "HexEdit Control (h)"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\hexeditctrl\PPDumpCtrl.h
# End Source File
# Begin Source File

SOURCE=.\hexeditctrl\PPNumEdit.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\cgfiltyp.h
# End Source File
# Begin Source File

SOURCE=.\CoolScroll.h
# End Source File
# Begin Source File

SOURCE=.\DumpViewerDlg.h
# End Source File
# Begin Source File

SOURCE=.\DupItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExportItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileDlg.h
# End Source File
# Begin Source File

SOURCE=.\Hero.h
# End Source File
# Begin Source File

SOURCE=.\ImageViewer.h
# End Source File
# Begin Source File

SOURCE=.\ImportItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImportToSlotDlg.h
# End Source File
# Begin Source File

SOURCE=.\hexeditctrl\memdc.h
# End Source File
# Begin Source File

SOURCE=.\PicFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlacementDisplay.h
# End Source File
# Begin Source File

SOURCE=.\PlacementDlg.h
# End Source File
# Begin Source File

SOURCE=.\SacredItemEdit.h
# End Source File
# Begin Source File

SOURCE=.\Section.h
# End Source File
# Begin Source File

SOURCE=.\SectionInventory.h
# End Source File
# Begin Source File

SOURCE=.\SectionPlacement.h
# End Source File
# Begin Source File

SOURCE=.\SIEDlg.h
# End Source File
# Begin Source File

SOURCE=.\SITFile.h
# End Source File
# Begin Source File

SOURCE=.\SITFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\CxImage\ximage.h
# End Source File
# Begin Source File

SOURCE=.\ZlibWrapper.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\Sacred.ico
# End Source File
# Begin Source File

SOURCE=.\res\sacred_200.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SacredItemEdit.ico
# End Source File
# Begin Source File

SOURCE=.\SacredItemEdit.rc
# End Source File
# Begin Source File

SOURCE=.\res\SacredItemEdit.rc2
# End Source File
# End Group
# Begin Group "StdAfx"

# PROP Default_Filter "*.cpp; *.h"
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
