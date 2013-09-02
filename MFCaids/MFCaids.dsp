# Microsoft Developer Studio Project File - Name="MFCaids" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MFCaids - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MFCaids.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MFCaids.mak" CFG="MFCaids - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MFCaids - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MFCaids - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MFCaids - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Build\MFCaids\Release"
# PROP Intermediate_Dir "..\Build\MFCaids\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MFCAIDS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MFCAIDS_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D _WIN32_WINNT=0x0401 /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 Version.lib shlwapi.lib Ws2_32.lib "..\Build\GraphX\Release\GraphX.lib" /nologo /dll /machine:I386 /out:"..\Build\Release/MFCaids.dll"

!ELSEIF  "$(CFG)" == "MFCaids - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Build\MFCaids\Debug"
# PROP Intermediate_Dir "..\Build\MFCaids\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MFCAIDS_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MFCAIDS_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /D _WIN32_WINNT=0x0401 /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Version.lib shlwapi.lib Ws2_32.lib "..\Build\GraphX\Debug\GraphX.lib" /nologo /dll /debug /machine:I386 /out:"..\Build\Debug/MFCaids.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MFCaids - Win32 Release"
# Name "MFCaids - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonX.cpp
# End Source File
# Begin Source File

SOURCE=.\CivTextWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPickerXP.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboZoom.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandLineInfoX.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogBarX.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogX.cpp
# End Source File
# Begin Source File

SOURCE=.\DocXML.cpp
# End Source File
# Begin Source File

SOURCE=.\EditX.cpp
# End Source File
# Begin Source File

SOURCE=.\FadeWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDialogX.cpp
# End Source File
# Begin Source File

SOURCE=.\FormViewX.cpp
# End Source File
# Begin Source File

SOURCE=.\FramePosition.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphXImage.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlX.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MFCaids.cpp
# End Source File
# Begin Source File

SOURCE=.\MFCaids.def
# End Source File
# Begin Source File

SOURCE=.\MFCaids.rc
# End Source File
# Begin Source File

SOURCE=.\MruMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\NewMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\ReBarX.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditCtrlX.cpp
# End Source File
# Begin Source File

SOURCE=.\RollupCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusProgressX.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolbarX.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlX.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\XMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\XPTheme.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\ButtonX.h
# End Source File
# Begin Source File

SOURCE=.\CivTextWrapper.h
# End Source File
# Begin Source File

SOURCE=.\ColourPickerXP.h
# End Source File
# Begin Source File

SOURCE=.\ComboZoom.h
# End Source File
# Begin Source File

SOURCE=.\CommandLineInfoX.h
# End Source File
# Begin Source File

SOURCE=.\CustomizeDialog.h
# End Source File
# Begin Source File

SOURCE=.\DialogBarX.h
# End Source File
# Begin Source File

SOURCE=.\DialogX.h
# End Source File
# Begin Source File

SOURCE=.\DocXML.h
# End Source File
# Begin Source File

SOURCE=.\EditX.h
# End Source File
# Begin Source File

SOURCE=.\FadeWnd.h
# End Source File
# Begin Source File

SOURCE=.\FileDialogX.h
# End Source File
# Begin Source File

SOURCE=.\FormViewX.h
# End Source File
# Begin Source File

SOURCE=.\FramePosition.h
# End Source File
# Begin Source File

SOURCE=.\GraphXImage.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlX.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MenuMgr.h
# End Source File
# Begin Source File

SOURCE=.\MFCaids.h
# End Source File
# Begin Source File

SOURCE=.\MFCdefs.h
# End Source File
# Begin Source File

SOURCE=.\MruMgr.h
# End Source File
# Begin Source File

SOURCE=.\NewMenu.h
# End Source File
# Begin Source File

SOURCE=.\ReBarX.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RichEditCtrlX.h
# End Source File
# Begin Source File

SOURCE=.\RollupCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StatusProgressX.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolbarX.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlX.h
# End Source File
# Begin Source File

SOURCE=.\Utilities.h
# End Source File
# Begin Source File

SOURCE=.\XMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\XPTheme.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CursorHand.cur
# End Source File
# Begin Source File

SOURCE=.\res\Customize.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DragMul.ico
# End Source File
# Begin Source File

SOURCE=.\res\DragOne.ico
# End Source File
# Begin Source File

SOURCE=.\res\Edit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hdrdown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hdrup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconOptions.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Info.ico
# End Source File
# Begin Source File

SOURCE=.\res\TextOptions.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\AniProc.avi
# End Source File
# Begin Source File

SOURCE=.\res\AniProcess.avi
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
