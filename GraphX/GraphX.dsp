# Microsoft Developer Studio Project File - Name="GraphX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GraphX - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GraphX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GraphX.mak" CFG="GraphX - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GraphX - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GraphX - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/fd2/GraphX", BWIBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GraphX - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\build\GraphX\Release"
# PROP Intermediate_Dir "..\build\GraphX\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GRAPH_EXPORTS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\build\Release/GraphX.dll"

!ELSEIF  "$(CFG)" == "GraphX - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\build\GraphX\Debug"
# PROP Intermediate_Dir "..\build\GraphX\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "GRAPH_EXPORTS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\build\Debug/GraphX.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GraphX - Win32 Release"
# Name "GraphX - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GifCoder.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphX.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphX.def
# End Source File
# Begin Source File

SOURCE=.\GraphX.rc
# End Source File
# Begin Source File

SOURCE=.\GraphXExp.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "lt12"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LT12\l_bitmap.h
# End Source File
# Begin Source File

SOURCE=.\LT12\l_error.h
# End Source File
# Begin Source File

SOURCE=.\LT12\L_FOX.H
# End Source File
# Begin Source File

SOURCE=.\LT12\L_OCROCX.H
# End Source File
# Begin Source File

SOURCE=.\LT12\L_OCX40.H
# End Source File
# Begin Source File

SOURCE=.\LT12\L_OcxErr.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Ldkrn.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Ltann.h
# End Source File
# Begin Source File

SOURCE=.\LT12\ltcap.h
# End Source File
# Begin Source File

SOURCE=.\LT12\ltcapocx.h
# End Source File
# Begin Source File

SOURCE=.\LT12\LTDIC.H
# End Source File
# Begin Source File

SOURCE=.\LT12\Ltdis.h
# End Source File
# Begin Source File

SOURCE=.\LT12\LTDLG.H
# End Source File
# Begin Source File

SOURCE=.\LT12\LTDLGOCX.H
# End Source File
# Begin Source File

SOURCE=.\LT12\LTEFX.H
# End Source File
# Begin Source File

SOURCE=.\LT12\Lterr.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Ltfil.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Ltfpx.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Ltimg.h
# End Source File
# Begin Source File

SOURCE=.\LT12\LTISI.H
# End Source File
# Begin Source File

SOURCE=.\LT12\LTISIOCX.H
# End Source File
# Begin Source File

SOURCE=.\LT12\ltkey.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Ltkrn.h
# End Source File
# Begin Source File

SOURCE=.\LT12\LTLCK.H
# End Source File
# Begin Source File

SOURCE=.\LT12\ltlck_.h
# End Source File
# Begin Source File

SOURCE=.\LT12\ltlst.h
# End Source File
# Begin Source File

SOURCE=.\LT12\ltnet.h
# End Source File
# Begin Source File

SOURCE=.\LT12\ltnetocx.h
# End Source File
# Begin Source File

SOURCE=.\LT12\LTPCK.H
# End Source File
# Begin Source File

SOURCE=.\LT12\LTSCR.H
# End Source File
# Begin Source File

SOURCE=.\LT12\ltscrocx.h
# End Source File
# Begin Source File

SOURCE=.\LT12\ltsnd.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Ltsys.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Lttwn.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Lttyp.h
# End Source File
# Begin Source File

SOURCE=.\LT12\ltvid.h
# End Source File
# Begin Source File

SOURCE=.\LT12\ltvidocx.h
# End Source File
# Begin Source File

SOURCE=.\LT12\Ltwnd.h
# End Source File
# Begin Source File

SOURCE=.\LT12\lvdlg.h
# End Source File
# Begin Source File

SOURCE=.\LT12\LVKRN.H
# End Source File
# End Group
# Begin Source File

SOURCE=.\GifCoder.h
# End Source File
# Begin Source File

SOURCE=.\GraphX.h
# End Source File
# Begin Source File

SOURCE=.\GraphXExp.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GraphX.rc2
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib12\LTDIS_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib12\ltdlg_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib12\ltefx_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib12\ltfil_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib12\ltimg_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib12\ltkrn_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib12\ltlst_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib12\lttwn_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib12\ltscr_n.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section GraphX : {20000009-0005-000C-AE00-B40002540100}
# 	1:12:IDR_GRAPH_X:1007
# End Section
