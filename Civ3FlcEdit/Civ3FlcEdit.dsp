# Microsoft Developer Studio Project File - Name="Civ3FlcEdit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Civ3FlcEdit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Civ3FlcEdit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Civ3FlcEdit.mak" CFG="Civ3FlcEdit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Civ3FlcEdit - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Civ3FlcEdit - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Civ3FlcEdit - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Build\Civ3FlcEdit\Release"
# PROP Intermediate_Dir "..\Build\Civ3FlcEdit\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Winmm.lib Version.lib ddraw.lib gdi32.lib "..\Build\MFCaids\Release\MFCaids.lib" "..\Build\GraphX\Release\GraphX.lib" /nologo /subsystem:windows /machine:I386 /out:"..\Build\Release/Civ3FlcEdit.exe"

!ELSEIF  "$(CFG)" == "Civ3FlcEdit - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Build\Civ3FlcEdit\Debug"
# PROP Intermediate_Dir "..\Build\Civ3FlcEdit\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Winmm.lib Version.lib ddraw.lib gdi32.lib "..\Build\MFCaids\Debug\MFCaids.lib" "..\Build\GraphX\Debug\GraphX.lib" /nologo /subsystem:windows /debug /machine:I386 /out:"..\Build\Debug/Civ3FlcEdit.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Civ3FlcEdit - Win32 Release"
# Name "Civ3FlcEdit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "FLC"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\DocTemplateFlc.cpp
# End Source File
# Begin Source File

SOURCE=.\FlcDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FlcView.cpp
# End Source File
# Begin Source File

SOURCE=.\ShrinkFrmCountDlg.cpp
# End Source File
# End Group
# Begin Group "C3F"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\C3fDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\C3fView.cpp
# End Source File
# Begin Source File

SOURCE=.\DocTemplateC3f.cpp
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileFlcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileImgAllDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileImgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileWavDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportToC3F.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\OffsetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectBgdImgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoomingDlg.cpp
# End Source File
# End Group
# Begin Group "Options"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlgDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlgFlc.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlgView.cpp
# End Source File
# End Group
# Begin Group "Palette"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\ColorBCHSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorMoveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorReplaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorTableDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorTablePreview.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorTableSrc.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxCivColor.cpp
# End Source File
# Begin Source File

SOURCE=.\ReplaceRangeDlg.cpp
# End Source File
# End Group
# Begin Group "Export FLC"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\ExportFlcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportFlcFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportFlcGen.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportFlcOut.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportFlcPal.cpp
# End Source File
# End Group
# Begin Group "New FLC"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\NewFlcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewFlcFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NewFlcGen.cpp
# End Source File
# Begin Source File

SOURCE=.\NewFlcOut.cpp
# End Source File
# Begin Source File

SOURCE=.\NewFlcPal.cpp
# End Source File
# End Group
# Begin Group "FLC Info"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\FlcInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FlcInfoExt.cpp
# End Source File
# Begin Source File

SOURCE=.\FlcInfoGen.cpp
# End Source File
# End Group
# Begin Group "Graphic AIDS"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\AnimationView.cpp
# End Source File
# Begin Source File

SOURCE=.\BgdImg.cpp
# End Source File
# Begin Source File

SOURCE=.\FlcImage.cpp
# End Source File
# Begin Source File

SOURCE=.\Pcx.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen.cpp
# End Source File
# Begin Source File

SOURCE=.\Surface.cpp
# End Source File
# End Group
# Begin Group "DlgBar"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\DialogBarMy.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelDelay.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelDirection.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelExtra.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelSound.cpp
# End Source File
# Begin Source File

SOURCE=.\PanelView.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Civ3FlcEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Civ3FlcEdit.rc
# End Source File
# Begin Source File

SOURCE=.\DocManagerEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "FLC H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\DocTemplateFlc.h
# End Source File
# Begin Source File

SOURCE=.\FlcDoc.h
# End Source File
# Begin Source File

SOURCE=.\FlcView.h
# End Source File
# Begin Source File

SOURCE=.\ShrinkFrmCountDlg.h
# End Source File
# End Group
# Begin Group "C3F H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\C3fDoc.h
# End Source File
# Begin Source File

SOURCE=.\C3fView.h
# End Source File
# Begin Source File

SOURCE=.\DocTemplateC3f.h
# End Source File
# End Group
# Begin Group "Dialogs H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileFlcDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileImgAllDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileImgDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileWavDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImportToC3F.h
# End Source File
# Begin Source File

SOURCE=.\InfoDialog.h
# End Source File
# Begin Source File

SOURCE=.\OffsetDlg.h
# End Source File
# Begin Source File

SOURCE=.\PreviewDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectBgdImgDlg.h
# End Source File
# Begin Source File

SOURCE=.\ZoomingDlg.h
# End Source File
# End Group
# Begin Group "Options H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlgDoc.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlgFlc.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlgView.h
# End Source File
# End Group
# Begin Group "Palette H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\ColorBCHSDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorMoveDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorReplaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorTable.h
# End Source File
# Begin Source File

SOURCE=.\ColorTableDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorTablePreview.h
# End Source File
# Begin Source File

SOURCE=.\ColorTableSrc.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxCivColor.h
# End Source File
# Begin Source File

SOURCE=.\ReplaceRangeDlg.h
# End Source File
# End Group
# Begin Group "Export FLC H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\ExportFlcDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExportFlcFrm.h
# End Source File
# Begin Source File

SOURCE=.\ExportFlcGen.h
# End Source File
# Begin Source File

SOURCE=.\ExportFlcOut.h
# End Source File
# Begin Source File

SOURCE=.\ExportFlcPal.h
# End Source File
# End Group
# Begin Group "New FLC H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\NewFlcDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewFlcFrm.h
# End Source File
# Begin Source File

SOURCE=.\NewFlcGen.h
# End Source File
# Begin Source File

SOURCE=.\NewFlcOut.h
# End Source File
# Begin Source File

SOURCE=.\NewFlcPal.h
# End Source File
# End Group
# Begin Group "FLC Info H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\FlcInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\FlcInfoExt.h
# End Source File
# Begin Source File

SOURCE=.\FlcInfoGen.h
# End Source File
# End Group
# Begin Group "Graphic AIDS H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\AnimationView.h
# End Source File
# Begin Source File

SOURCE=.\BgdImg.h
# End Source File
# Begin Source File

SOURCE=.\FlcImage.h
# End Source File
# Begin Source File

SOURCE=.\GraphicsLib.h
# End Source File
# Begin Source File

SOURCE=.\Pcx.h
# End Source File
# Begin Source File

SOURCE=.\Screen.h
# End Source File
# Begin Source File

SOURCE=.\Surface.h
# End Source File
# End Group
# Begin Group "DlgBar H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\DialogBarMy.h
# End Source File
# Begin Source File

SOURCE=.\PanelAnimation.h
# End Source File
# Begin Source File

SOURCE=.\PanelDelay.h
# End Source File
# Begin Source File

SOURCE=.\PanelDirection.h
# End Source File
# Begin Source File

SOURCE=.\PanelExtra.h
# End Source File
# Begin Source File

SOURCE=.\PanelSound.h
# End Source File
# Begin Source File

SOURCE=.\PanelView.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Civ3FlcEdit.h
# End Source File
# Begin Source File

SOURCE=.\Definitions.h
# End Source File
# Begin Source File

SOURCE=.\DlgBar.h
# End Source File
# Begin Source File

SOURCE=.\DocManagerEx.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
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

SOURCE=.\res\About.ico
# End Source File
# Begin Source File

SOURCE=.\res\Apply.ico
# End Source File
# Begin Source File

SOURCE=.\res\Back.ico
# End Source File
# Begin Source File

SOURCE=.\res\BCHS.ico
# End Source File
# Begin Source File

SOURCE=.\res\BgdGrid.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BgdLand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BgdSea.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Browse.ico
# End Source File
# Begin Source File

SOURCE=.\res\C3fDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\C3fType.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cancel.ico
# End Source File
# Begin Source File

SOURCE=.\res\Centering.ico
# End Source File
# Begin Source File

SOURCE=.\res\Civ3FlcEdit.ico
# End Source File
# Begin Source File

SOURCE=.\res\Civ3FlcEdit.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ColorRange.ico
# End Source File
# Begin Source File

SOURCE=.\res\ColorReplace.ico
# End Source File
# Begin Source File

SOURCE=.\res\ColorSelect.ico
# End Source File
# Begin Source File

SOURCE=.\res\Cross.cur
# End Source File
# Begin Source File

SOURCE=.\res\CursorHand.cur
# End Source File
# Begin Source File

SOURCE=.\res\Default.ico
# End Source File
# Begin Source File

SOURCE=.\res\Delete.ico
# End Source File
# Begin Source File

SOURCE=.\res\DirectAll.ico
# End Source File
# Begin Source File

SOURCE=.\res\DirectE.ico
# End Source File
# Begin Source File

SOURCE=.\res\DirectN.ico
# End Source File
# Begin Source File

SOURCE=.\res\DirectNE.ico
# End Source File
# Begin Source File

SOURCE=.\res\DirectNW.ico
# End Source File
# Begin Source File

SOURCE=.\res\DirectS.ico
# End Source File
# Begin Source File

SOURCE=.\res\DirectSE.ico
# End Source File
# Begin Source File

SOURCE=.\res\DirectSW.ico
# End Source File
# Begin Source File

SOURCE=.\res\DirectW.ico
# End Source File
# Begin Source File

SOURCE=.\res\DocType.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit.ico
# End Source File
# Begin Source File

SOURCE=.\res\First.ico
# End Source File
# Begin Source File

SOURCE=.\res\FlcDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\FlcExp.ico
# End Source File
# Begin Source File

SOURCE=.\res\FlcImp.ico
# End Source File
# Begin Source File

SOURCE=.\res\FlcInfo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FlcNew.ico
# End Source File
# Begin Source File

SOURCE=.\res\FlcType.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Forward.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImportC3F.ico
# End Source File
# Begin Source File

SOURCE=.\res\Info.ico
# End Source File
# Begin Source File

SOURCE=.\res\Last.ico
# End Source File
# Begin Source File

SOURCE=.\res\Lock.ico
# End Source File
# Begin Source File

SOURCE=.\res\MainToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MoveColor.ico
# End Source File
# Begin Source File

SOURCE=.\res\MoveColorL.ico
# End Source File
# Begin Source File

SOURCE=.\res\MoveColorR.ico
# End Source File
# Begin Source File

SOURCE=.\res\Next.ico
# End Source File
# Begin Source File

SOURCE=.\res\Offset.ico
# End Source File
# Begin Source File

SOURCE=.\res\OK.ico
# End Source File
# Begin Source File

SOURCE=.\res\OpenC3F.ico
# End Source File
# Begin Source File

SOURCE=.\res\OpenFLC.ico
# End Source File
# Begin Source File

SOURCE=.\res\Options.ico
# End Source File
# Begin Source File

SOURCE=.\res\OptionsTab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Palette.ico
# End Source File
# Begin Source File

SOURCE=.\res\PanelAnimation.ico
# End Source File
# Begin Source File

SOURCE=.\res\PanelDelay.ico
# End Source File
# Begin Source File

SOURCE=.\res\PanelDirection.ico
# End Source File
# Begin Source File

SOURCE=.\res\PanelExtra.ico
# End Source File
# Begin Source File

SOURCE=.\res\PanelSound.ico
# End Source File
# Begin Source File

SOURCE=.\res\PanelView.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pause.ico
# End Source File
# Begin Source File

SOURCE=.\res\Play.ico
# End Source File
# Begin Source File

SOURCE=.\res\PlayWAV.ico
# End Source File
# Begin Source File

SOURCE=.\res\Previous.ico
# End Source File
# Begin Source File

SOURCE=.\res\Refresh.ico
# End Source File
# Begin Source File

SOURCE=.\res\ReplaceColor.ico
# End Source File
# Begin Source File

SOURCE=.\res\Save.ico
# End Source File
# Begin Source File

SOURCE=.\res\SelectColor.ico
# End Source File
# Begin Source File

SOURCE=.\res\Stop.ico
# End Source File
# Begin Source File

SOURCE=.\res\StopWAV.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Unlock.ico
# End Source File
# Begin Source File

SOURCE=.\res\Zoom.ico
# End Source File
# Begin Source File

SOURCE=.\res\ZoomFit.ico
# End Source File
# Begin Source File

SOURCE=.\res\ZoomIn.ico
# End Source File
# Begin Source File

SOURCE=.\res\ZoomOut.ico
# End Source File
# End Group
# Begin Group "Other Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\Civ3FlcEdit.manifest
# End Source File
# Begin Source File

SOURCE=.\Civ3FlcEdit.reg
# End Source File
# Begin Source File

SOURCE=.\res\CyberLion.avi
# End Source File
# Begin Source File

SOURCE=.\res\LandBgd.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp00.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp01.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp02.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp03.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp04.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp05.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp06.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp07.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp08.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp09.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp10.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp11.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp12.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp13.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp14.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp15.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp16.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp17.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp18.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp19.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp20.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp21.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp22.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp23.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp24.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp25.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp26.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp27.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp28.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp29.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp30.pcx
# End Source File
# Begin Source File

SOURCE=.\res\Palettes\ntp31.pcx
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\SeaBgd.pcx
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\GrigBgd.pcx
# End Source File
# End Target
# End Project
