/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 Civ3FlcEdit.cpp
	CREATED:	 26.08.2004

	AUTHOR:		 Dreyk
	MODIFIED:	

	DESCRIPTION: Defines the class behaviors for the application.
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"

#include "MainFrm.h"
#include "DocManagerEx.h"

#include "DocTemplateFlc.h"
#include "ChildFrm.h"
#include "FlcDoc.h"
#include "FlcView.h"

#include "DocTemplateC3f.h"
#include "ChildFrm.h"
#include "C3fDoc.h"
#include "C3fView.h"
#include "AboutDlg.h"

#include "..\MFCaids\CommandLineInfoX.h"
//#include "..\MFCaids\XMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4355)

/////////////////////////////////////////////////////////////////////////////
// Test if filename ends with suffix
//
static BOOL CompareFilenameSuffix(LPCTSTR lpszPathName, LPCTSTR lpszSuffix)
{
	int len = strlen(lpszPathName);
	int suflen = strlen(lpszSuffix);
	
	return (suflen > 0 && len > suflen && 
		strcmpi(&lpszPathName[len - suflen], lpszSuffix) == 0);
}

/////////////////////////////////////////////////////////////////////////////
// Test for FLC (.flc) file
//
static BOOL CALLBACK FlcFileFunc(LPCTSTR lpszPathName)
{
	return CompareFilenameSuffix(lpszPathName, _T(".flc"));
}

/////////////////////////////////////////////////////////////////////////////
// Test for Storyboard (.c3f) file
//
static BOOL CALLBACK C3fFileFunc(LPCTSTR lpszPathName)
{
	return CompareFilenameSuffix(lpszPathName, _T(".c3f"));
}

/////////////////////////////////////////////////////////////////////////////
// CCiv3FlcEditApp

BEGIN_MESSAGE_MAP(CCiv3FlcEditApp, CWinApp)
	//{{AFX_MSG_MAP(CCiv3FlcEditApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_WINDOW_CLOSEALL, OnWindowCloseall)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCiv3FlcEditApp construction

CCiv3FlcEditApp::CCiv3FlcEditApp() : m_mruFileMgr(this)
{
	m_bCivRegistered	= FALSE;
	m_bStartup			= TRUE;
	m_palType			= PalGame;
	m_bShowStaticFrm	= FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCiv3FlcEditApp object

CCiv3FlcEditApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCiv3FlcEditApp initialization

BOOL CCiv3FlcEditApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if (!AfxInitRichEdit())
	{
		AfxMessageBox(IDS_RICHEDIT_ERR, MB_ICONERROR);
		//return FALSE;
	}
	
	SetRegistryKey(_T("Cyber Dreyk"));
	
	///////////////////////////////////////////////////////////////////////////////
	// Init some app parameters and variables
	SetAppParameters();

	m_pDocManager = new CDocManagerEx;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CDocTemplateFlc* pDocTemplateFlc = new CDocTemplateFlc(
		IDR_FLCTYPE,
		RUNTIME_CLASS(CFlcDoc),
		RUNTIME_CLASS(CChildFrm), // custom MDI child frame
		RUNTIME_CLASS(CFlcView),
		GetDocManager());

	AddDocTemplate(pDocTemplateFlc);

	CDocTemplateC3f* pDocTemplateC3f = new CDocTemplateC3f(
		IDR_C3FTYPE,
		RUNTIME_CLASS(CC3fDoc),
		RUNTIME_CLASS(CChildFrm), // custom MDI child frame
		RUNTIME_CLASS(CC3fView),
		GetDocManager());
	
	AddDocTemplate(pDocTemplateC3f);

	// Enable DDE Execute open
	EnableShellOpen();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfoX cmdInfo;
	ParseCommandLine(cmdInfo);

	// We should just registry file type
	if (cmdInfo.m_bRegFileType)
	{
		SetFileTypeAssociations();
	
		if (!cmdInfo.m_bSilent)
			AfxMessageBox(IDS_REGFILETYPE_OK, MB_ICONINFORMATION);
		
		// in case install we need to make something more...
		if (!cmdInfo.m_bInstall)
			return FALSE;
	}
	// We should just unregistry file type
	else if (cmdInfo.m_bUnregFileType)
	{
		SetFileTypeAssociations(FALSE);
		
		if (!cmdInfo.m_bSilent)
			AfxMessageBox(IDS_UNREGFILETYPE_OK, MB_ICONINFORMATION);
		
		return FALSE;
	}
/*	
	// in case install
	if (cmdInfo.m_bInstall)
	{
		return FALSE; // to not use installation utility
		// IN CASE WE WANT TO USE INSTALLATION UTILITY
		
		// get current directory
		CString sCurDir;
		char buf[MAX_PATH];
		if (GetModuleFileName(theApp.m_hInstance, buf, MAX_PATH) == 0)
			return FALSE;
		
		sCurDir = uGetPathName(buf);
		
		// try to remove install.exe and msvcp60.dll
		BOOL bDone = FALSE;
		while (!bDone)
		{
			if (uIsFileExist(sCurDir + "InstallFlcEdit.exe") &&
				!DeleteFile(sCurDir + "InstallFlcEdit.exe"))
				Sleep(300);
			else
			{
				DeleteFile(sCurDir + "msvcp60.dll");
				bDone = TRUE;
			}
		}
		return FALSE;
	}
*/
	// if file is opening - try to open it with existing CImaging instance
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
	{
		if (OpenWithExisting(cmdInfo))
			return FALSE;
		else
			m_bStartup = FALSE;
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
	// NewMenu

	// Load our own toolbar
	pDocTemplateFlc->m_NewMenuShared.LoadToolBar(TbMain, RGB_BACKGROUND);
	
	// Set the color in the bitmaps that is the background transparent color
	pDocTemplateFlc->m_NewMenuShared.SetBitmapBackground(RGB_BACKGROUND);
	
	// Use ModifyODMenu to add a bitmap to a menu options.
	pDocTemplateFlc->m_NewMenuShared.ModifyODMenu(0, uLoadString(IDS_RECENT_FILES_FLC), IDB_FLCTYPE);
	pDocTemplateFlc->m_NewMenuShared.ModifyODMenu(0, uLoadString(IDS_RECENT_FILES_C3F), IDB_C3FTYPE);

	// Load our own toolbar
	pDocTemplateC3f->m_NewMenuShared.LoadToolBar(TbMain, RGB_BACKGROUND);
	
	// Set the color in the bitmaps that is the background transparent color
	pDocTemplateC3f->m_NewMenuShared.SetBitmapBackground(RGB_BACKGROUND);
	
	// Use ModifyODMenu to add a bitmap to a menu options.
	pDocTemplateC3f->m_NewMenuShared.ModifyODMenu(0, uLoadString(IDS_RECENT_FILES_FLC), IDB_FLCTYPE);
	pDocTemplateC3f->m_NewMenuShared.ModifyODMenu(0, uLoadString(IDS_RECENT_FILES_C3F), IDB_C3FTYPE);
	
	// Get menu parameters from registry
	CNewMenu::SetMenuDrawMode(GetProfileInt(REG_MENU, REG_MENU_STYLE, 2));
	CNewMenu::SetSelectDisableMode(GetProfileInt(REG_MENU, REG_MENU_SELECTION, 1));
	CNewMenu::SetXpBlendig(GetProfileInt(REG_MENU, REG_MENU_GLOOM, 1));
	
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
	// ButtonX
	
	// Setup style for all ButtonX in our app
#ifdef BX_USE_SETTINGS
	CButtonX::SetButtonsSettings(GetProfileInt(BX_REG_KEY, BX_REG_SETTINGS, 0));
#endif // BX_USE_SETTINGS
	//////////////////////////////////////////////////////////////////////
	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Check, is File Type registered now, or not...
	// If not, ask user to make it...
	if (m_bCheckFileType)
	{
		if (!IsFileTypeRegistered(GetDocManager()->GetDocTemplateFlc()) ||
			!IsFileTypeRegistered(GetDocManager()->GetDocTemplateC3f()))
		{
			// get current target path
			char buf[MAX_PATH];
			GetModuleFileName(theApp.m_hInstance, buf, MAX_PATH);
			
			if (AfxMessageBox(uFormatString(IDS_CHECK_FILETYPE, buf), MB_ICONQUESTION|MB_YESNO) == IDYES)
				SetFileTypeAssociations();
		}
	}

	////////////////////////////////////////////////////
	// Get Civilization III and addons registry values
	m_bCivRegistered = FALSE;
	m_bPtwRegistered = FALSE;
	m_bC3CRegistered = FALSE;
	
	HKEY    hKey;
	TCHAR	tcPath1[MAX_PATH];
	DWORD	dwRegLen1  = sizeof(tcPath1);
	TCHAR	tcPath2[MAX_PATH];
	DWORD	dwRegLen2  = sizeof(tcPath2);
	TCHAR	tcPath3[MAX_PATH];
	DWORD	dwRegLen3  = sizeof(tcPath3);
	DWORD	dwTypeStr = REG_EXPAND_SZ;
	
	if (::RegCreateKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Infogrames\\Conquests", &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, "Install_Path", NULL, &dwTypeStr, (PUCHAR)&tcPath3, &dwRegLen3) == ERROR_SUCCESS)
		{
			m_sC3CInstallPath = tcPath3;
			m_bC3CRegistered  = TRUE;
			
			m_arSearch.Add(m_sC3CInstallPath);
		}
		
		RegCloseKey(hKey);
	}
	
	if (::RegCreateKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Infogrames\\Civ3PTW", &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, "Install_Path", NULL, &dwTypeStr, (PUCHAR)&tcPath2, &dwRegLen2) == ERROR_SUCCESS)
		{
			m_sPtwInstallPath = tcPath2;
			m_bPtwRegistered  = TRUE;
			
			m_arSearch.Add(m_sPtwInstallPath);
		}
		
		RegCloseKey(hKey);
	}

	if (::RegCreateKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Infogrames Interactive\\Civilization III", &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, "Install_Path", NULL, &dwTypeStr, (PUCHAR)&tcPath1, &dwRegLen1) == ERROR_SUCCESS)
		{
			m_sCivInstallPath = tcPath1;
			m_bCivRegistered  = TRUE;
			
			m_arSearch.Add(m_sCivInstallPath);
		}
		
		RegCloseKey(hKey);
	}
	/////////////////////////////////////////////////////

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_bStartup = FALSE;
	
	// The one and only window has been initialized, so show and update it.
	if (!pMainFrame->IsWindowVisible())
		m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

// App command to run the dialog
void CCiv3FlcEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////
// Get current document
//
CDocument* CCiv3FlcEditApp::GetDoc()
{
	POSITION pos = GetFirstDocTemplatePosition();
	if (pos == NULL)
	{
		TRACE("There is no doc templates registered..\n");
		ASSERT(FALSE);
		return NULL;
	}

	CDocTemplate *pDocTemplate = GetNextDocTemplate(pos);

	if (!pDocTemplate)
	{
		TRACE("The document template is not found...\n");
		ASSERT(FALSE);
		return NULL;
	}
	
	pos = pDocTemplate->GetFirstDocPosition();
	return (pos == NULL) ? NULL : pDocTemplate->GetNextDoc(pos);
}

/////////////////////////////////////////////////////////////////
// When adding file to recent file list, let MRU manager have 1st
// crack, otherwise do default thing
//
void CCiv3FlcEditApp::AddToRecentFileList(LPCTSTR lpszPathName) 
{
	m_mruFileMgr.AddToRecentFileList(lpszPathName);
    
	// Only call if you want standard MFC recent file list too
//	CWinApp::AddToRecentFileList(lpszPathName);
}

// Override message routing to pass to MRU file manager
BOOL CCiv3FlcEditApp::OnCmdMsg(UINT nID, int nCode, void* pExtra,
					  AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_mruFileMgr.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

///////////////////////////////////////////////////////////////////////
// Init some app parameters and variables
//
void CCiv3FlcEditApp::SetAppParameters()
{
	m_nMaxMRU			= GetProfileInt(REG_OPTIONS, REG_MAX_MRU, 4);
	m_bLoadLastDoc		= GetProfileInt(REG_OPTIONS, REG_LOAD_LAST_DOC, 0);
	m_bCheckFileType	= GetProfileInt(REG_OPTIONS, REG_CHECK_FILE_TYPE, 1);
	m_palType			= (EPalType)GetProfileInt(REG_OPTIONS, REG_PAL_TYPE, PalGame);
	m_sPalDir			= GetProfileString(REG_OPTIONS, REG_PAL_DIR, "");
	m_bShowStaticFrm	= GetProfileInt(REG_OPTIONS, REG_ANIM_TYPE, 0);
	m_bExpOpen			= GetProfileInt(REG_OPTIONS, REG_EXP_OPEN, 1);
	m_bExpAsk			= GetProfileInt(REG_OPTIONS, REG_EXP_ASK, 1);
	
	if (m_nMaxMRU < 4 || m_nMaxMRU > 16)
		m_nMaxMRU = 4;

	/////////////////////////////////////////////////////////////////
	// free recent file list
	if (m_pRecentFileList != NULL)
	{
		delete m_pRecentFileList;
		m_pRecentFileList = NULL;
	}

	LoadStdProfileSettings(m_nMaxMRU); // Load INI file options (including MRU)

	if (MAIN_FRAME)
	{
		CMenu* menu  = MAIN_FRAME->GetMenu(); // app menu
		CMenu* mmenu = menu->GetSubMenu(0);   // menu "File"
		
		CString str;
		UINT pos = mmenu->GetMenuItemCount() - 4;
		
		// It is necessary FLC MRU menu option
		CMenu* submenu = mmenu->GetSubMenu(pos);
		int count = submenu->GetMenuItemCount();
		
		for (int i = 0; i < count; i++)
			submenu->RemoveMenu(0, MF_BYPOSITION); // remove all recent files menu options
		
		submenu->AppendMenu(MF_STRING, ID_FILE_MRU_FLC1, uLoadString(IDS_NONE));

		// It is necessary C3F MRU menu option
		pos += 1;
		submenu = mmenu->GetSubMenu(pos);
		count = submenu->GetMenuItemCount();
		
		for (i = 0; i < count; i++)
			submenu->RemoveMenu(0, MF_BYPOSITION); // remove all recent files menu options
		
		submenu->AppendMenu(MF_STRING, ID_FILE_MRU_C3F1, uLoadString(IDS_NONE));
	}

	m_mruFileMgr.RemoveMRUFileLists();

	// Add recent file category for *.flc files
	m_mruFileMgr.Add(ID_FILE_MRU_FLC1,	     // base command ID
		REG_MRU_FLC,		                 // szSection = registry key name 
		CString((LPCSTR)IDS_REG_FILE_ENTRY), // szFileEntry = registry value name
		FlcFileFunc,						 // test for .flc file
		m_nMaxMRU);					         // max number list/menu entries

	// Add recent file category for *.c3f files
	m_mruFileMgr.Add(ID_FILE_MRU_C3F1,		 // base command ID
		REG_MRU_C3F,						 // szSection = registry key name 
		CString((LPCSTR)IDS_REG_FILE_ENTRY), // szFileEntry = registry value name
		C3fFileFunc,						 // test for .c3f file
		m_nMaxMRU);						     // max number list/menu entries
}

/////////////////////////////////////////////////////////////////////
// Check, is File Type registered now, or not...
//
BOOL CCiv3FlcEditApp::IsFileTypeRegistered(CDocTemplate *pDocTemplate)
{
	CString strPathName, strFileTypeId, strOpenCommandLine, strRegKey;
	
	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	if (pDocTemplate->GetDocString(strFileTypeId,
		CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
	{
		// path\shell\open\command = path filename
		strOpenCommandLine = strPathName + _T(" \"%1\"");
		
		// path\shell\open\command = path filename
		strRegKey.Format(_T("%s\\shell\\open\\%s"), (LPCTSTR)strFileTypeId,
			(LPCTSTR)_T("command"));
		
		HKEY    hKey;
		TCHAR	tcPath[MAX_PATH];
		DWORD	dwRegLen  = sizeof(tcPath);
		DWORD	dwTypeStr = REG_EXPAND_SZ;
		
		if (::RegOpenKey(HKEY_CLASSES_ROOT, strRegKey, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hKey, "", NULL, &dwTypeStr, (PUCHAR)&tcPath, &dwRegLen) == ERROR_SUCCESS)
			{
				if (strOpenCommandLine.CompareNoCase(tcPath) == 0)
					return TRUE;
			}
			
			RegCloseKey(hKey);
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////
// Registry or unregistry the File Type associations
//
void CCiv3FlcEditApp::SetFileTypeAssociations(BOOL bRegister /*= TRUE*/)
{
	if (bRegister)
		RegisterShellFileTypes(FALSE);
	else
		UnregisterShellFileTypes();
}

CDocManagerEx* CCiv3FlcEditApp::GetDocManager()
{
	return (CDocManagerEx*)m_pDocManager;
}

void CCiv3FlcEditApp::OnWindowCloseall() 
{
	GetDocManager()->CloseAllDocuments(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// Tries to open file in command line with existing instance
//
BOOL CCiv3FlcEditApp::OpenWithExisting(CCommandLineInfo &cmdInfo)
{
	CWnd* pWnd = CWnd::GetDesktopWindow();
	if (!pWnd)
		return FALSE;

	pWnd = pWnd->GetWindow(GW_CHILD);
	
	// enumerate all top level windows
	TCHAR szBuf[1001];
	BOOL bFound = FALSE;

	while (pWnd && !bFound)
	{
		// determine if the window has "CImg60WndCls%s" class name
		GetClassName(pWnd->m_hWnd, szBuf, 1000);
		if (strstr(szBuf, CIV3FLC_CLASS_NAME) != NULL)
		{
			// send file name using WM_COPYDATA
			COPYDATASTRUCT cds;
			cds.dwData = MSG_DOCUMENT_OPEN;
			cds.cbData = (cmdInfo.m_strFileName.GetLength() + 1) * sizeof(TCHAR);
			cds.lpData = (LPVOID)(LPCTSTR)cmdInfo.m_strFileName;
			
			pWnd->SendMessage(WM_COPYDATA, 0, (LPARAM)&cds);
			bFound = TRUE;
		}
		
		if (!bFound)
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	
	return bFound;
}

BOOL CCiv3FlcEditApp::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
    {
		if (GetKeyState(VK_CONTROL) < 0)
		{
			switch (pMsg->wParam)
			{
			case 'O':
				OnFileOpen();
				return TRUE;

			case 'S':
				{
					CAnimationView* pView = GetAnimationView();
					if (pView && pView->IsKindOf(RUNTIME_CLASS(CFlcView)))
					{	
						((CFlcView*)pView)->OnFileSave();
						return TRUE;
					}
				}
				break;
			}
		}
		else if (pMsg->wParam == VK_PAUSE)
		{
			CAnimationView* pView = GetAnimationView();
			if (pView)
			{
				pView->OnPause();
				return TRUE;
			}
		}
		else if (pMsg->wParam == VK_F5)
		{
			CAnimationView* pView = GetAnimationView();
			if (pView)
			{
				pView->OnRefresh();
				return TRUE;
			}
		}
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

CAnimationView* CCiv3FlcEditApp::GetAnimationView()
{
	CFrameWnd* pFrame = MAIN_FRAME->GetActiveFrame();
	if (!pFrame)
		return NULL;
	
	return (CAnimationView*)pFrame->GetActiveView();
}
