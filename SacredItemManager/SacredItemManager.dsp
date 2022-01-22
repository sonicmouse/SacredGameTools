# Microsoft Developer Studio Project File - Name="SacredItemManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SacredItemManager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SacredItemManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SacredItemManager.mak" CFG="SacredItemManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SacredItemManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SacredItemManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SacredItemManager - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "./hexeditctrl\\" /I "./hexeditctrl/" /I "./CxImage/" /I "../shlib/exp/" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
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

!ELSEIF  "$(CFG)" == "SacredItemManager - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./hexeditctrl/" /I "./CxImage/" /I "../shlib/exp/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
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

# Name "SacredItemManager - Win32 Release"
# Name "SacredItemManager - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Hex Edit Control (.cpp)"

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

SOURCE=.\DupeItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditIDDlg.cpp
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

SOURCE=.\HoverButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Item.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemEditHelperDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemPicDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemPlacementDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemPlacementDispDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PicFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SacredItemManager.cpp
# End Source File
# Begin Source File

SOURCE=..\shlib\exp\SHLib.cpp
# End Source File
# Begin Source File

SOURCE=.\SIMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SITFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SITFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TempFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\UnslotItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewDumpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewItemDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Hex Edit Control (.h)"

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

SOURCE=.\DupeItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditIDDlg.h
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

SOURCE=.\HoverButton.h
# End Source File
# Begin Source File

SOURCE=.\ImportItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\ItemEditHelperDlg.h
# End Source File
# Begin Source File

SOURCE=.\ItemManager.h
# End Source File
# Begin Source File

SOURCE=.\ItemPicDisp.h
# End Source File
# Begin Source File

SOURCE=.\ItemPlacementDisp.h
# End Source File
# Begin Source File

SOURCE=.\ItemPlacementDispDlg.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\PicFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\SacredItemManager.h
# End Source File
# Begin Source File

SOURCE=..\shlib\exp\SHLib.h
# End Source File
# Begin Source File

SOURCE=.\SIMDlg.h
# End Source File
# Begin Source File

SOURCE=.\SITFile.h
# End Source File
# Begin Source File

SOURCE=.\SITFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\TempFile.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\UnslotItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\ViewDumpDlg.h
# End Source File
# Begin Source File

SOURCE=.\ViewItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\CxImage\ximage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\SacredItemManager.ico
# End Source File
# Begin Source File

SOURCE=.\SacredItemManager.rc
# End Source File
# Begin Source File

SOURCE=.\res\SacredItemManager.rc2
# End Source File
# End Group
# Begin Group "StdAfx"

# PROP Default_Filter "*.h;*.cpp"
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
