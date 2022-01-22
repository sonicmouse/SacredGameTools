# Microsoft Developer Studio Project File - Name="SacredFilter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SacredFilter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SacredFilter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SacredFilter.mak" CFG="SacredFilter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SacredFilter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SacredFilter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SacredFilter - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SacredFilter - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SacredFilter - Win32 Release"
# Name "SacredFilter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FLGBox.cpp
# End Source File
# Begin Source File

SOURCE=.\LobbyManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SacredFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\SacredFilterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SacredSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FLGBox.h
# End Source File
# Begin Source File

SOURCE=.\LobbyManager.h
# End Source File
# Begin Source File

SOURCE=.\PluginsDlg.h
# End Source File
# Begin Source File

SOURCE=.\SacredCRC32.h
# End Source File
# Begin Source File

SOURCE=.\SacredFilter.h
# End Source File
# Begin Source File

SOURCE=.\SacredFilterDlg.h
# End Source File
# Begin Source File

SOURCE=.\SacredSocket.h
# End Source File
# Begin Source File

SOURCE=.\ScriptEngine.h
# End Source File
# Begin Source File

SOURCE=.\ScriptManager.h
# End Source File
# Begin Source File

SOURCE=.\ScriptObject.h
# End Source File
# Begin Source File

SOURCE=.\Server.h
# End Source File
# Begin Source File

SOURCE=.\ServerManager.h
# End Source File
# Begin Source File

SOURCE=.\StaticInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\SacredFilter.ico
# End Source File
# Begin Source File

SOURCE=.\SacredFilter.rc
# End Source File
# Begin Source File

SOURCE=.\res\SacredFilter.rc2
# End Source File
# End Group
# Begin Group "StdAfx"

# PROP Default_Filter ""
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
# Section SacredFilter : {70841C70-067D-11D0-95D8-00A02463AB28}
# 	2:5:Class:CScriptModule
# 	2:10:HeaderFile:scriptmodule.h
# 	2:8:ImplFile:scriptmodule.cpp
# End Section
# Section SacredFilter : {0E59F1D5-1FBE-11D0-8FF2-00A0D10038BC}
# 	2:21:DefaultSinkHeaderFile:scriptcontrol.h
# 	2:16:DefaultSinkClass:CScriptControl
# End Section
# Section SacredFilter : {70841C78-067D-11D0-95D8-00A02463AB28}
# 	2:5:Class:CScriptError
# 	2:10:HeaderFile:scripterror.h
# 	2:8:ImplFile:scripterror.cpp
# End Section
# Section SacredFilter : {70841C73-067D-11D0-95D8-00A02463AB28}
# 	2:5:Class:CScriptProcedure
# 	2:10:HeaderFile:scriptprocedure.h
# 	2:8:ImplFile:scriptprocedure.cpp
# End Section
# Section SacredFilter : {70841C6F-067D-11D0-95D8-00A02463AB28}
# 	2:5:Class:CScriptModuleCollection
# 	2:10:HeaderFile:scriptmodulecollection.h
# 	2:8:ImplFile:scriptmodulecollection.cpp
# End Section
# Section SacredFilter : {0E59F1D3-1FBE-11D0-8FF2-00A0D10038BC}
# 	2:5:Class:CScriptControl
# 	2:10:HeaderFile:scriptcontrol.h
# 	2:8:ImplFile:scriptcontrol.cpp
# End Section
# Section SacredFilter : {70841C71-067D-11D0-95D8-00A02463AB28}
# 	2:5:Class:CScriptProcedureCollection
# 	2:10:HeaderFile:scriptprocedurecollection.h
# 	2:8:ImplFile:scriptprocedurecollection.cpp
# End Section
