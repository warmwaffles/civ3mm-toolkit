/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
 
   FILE NAME:   MruMgr.cpp
   AUTHOR:      Paul DiLascia (Microsoft Systems Journal)
   MODIFIED:    Dreyk
   DESCRIPTION:	CMruFileManager - class to manage multiple recent file
   lists in MFC

   Manager class manages multiple recent file lists. To use:

	* instantiate one of these in your app;
	* override your app's OnCmdMsg to pass to mru file manager;
	* override your app's AddToRecentFileList to call mru manager;
	* in your InitInstance, call Add to add each type of file.

\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

/*  
// To use, add the code below to your main app class:

///////////////////
// MyApp.h

virtual void AddToRecentFileList(LPCTSTR lpszPathName);  // add to MRU
virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
					  AFX_CMDHANDLERINFO* pHandlerInfo);	

CMruFileManager m_mruFileMgr;

///////////////////
// MyApp.cpp
#pragma warning(disable: 4355)

static BOOL CALLBACK FirstTypeFileFunc(LPCTSTR lpszPathName)
{
	return CompareFilenameSuffix(lpszPathName, _T(".ex1"));
}

static BOOL CALLBACK SecondTypeFileFunc(LPCTSTR lpszPathName)
{
	if (CompareFilenameSuffix(lpszPathName, _T(".ex2")) ||
		CompareFilenameSuffix(lpszPathName, _T(".ex3")) ||
		CompareFilenameSuffix(lpszPathName, _T(".ex4")) )
		return TRUE;
	
	return FALSE;
}

CMyApp::CMyApp() : m_mruFileMgr(this)
{
}

// Override message routing to pass to MRU file manager
BOOL CMyApp::OnCmdMsg(UINT nID, int nCode, void* pExtra,
					  AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_mruFileMgr.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// When adding file to recent file list, let MRU manager have 1st
// crack, otherwise do default thing
void CMyApp::AddToRecentFileList(LPCTSTR lpszPathName)
{
	m_mruFileMgr.AddToRecentFileList(lpszPathName);
    
	// Only call if you want standard MFC recent file list too
//	CWinApp::AddToRecentFileList(lpszPathName);
}

BOOL CMyApp::InitInstance()
{
	// some code...

	m_mruFileMgr.RemoveMRUFileLists();

	// Add recent file category for first type (.ex1) files
	m_mruFileMgr.Add(ID_FILE_MRU_FIRSTTYPE1, // base command ID
		REG_MRU_FIRSTTYPE,		        // szSection = registry key name 
		"File%d",                       // szFileEntry = registry value name
		FirstTypeFileFunc,			    // test for .ex1 file
		m_nMaxMRU1);					// max number list/menu entries

	// Add recent file category for second type (.ex2, .ex3 and .ex4) files
	m_mruFileMgr.Add(ID_FILE_MRU_SECONDTYPE1, // base command ID
		REG_MRU_SECONDTYPE,		        // szSection = registry key name 
		"File%d",                       // szFileEntry = registry value name
		SecondTypeFileFunc,				// test for .ex2, .ex3 or .ex4 file
		m_nMaxMRU2);					// max number list/menu entries

	// some code......
}
*/

#include "StdAfx.h"
#include "MruMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMruFileManager, CCmdTarget)

BEGIN_MESSAGE_MAP(CMruFileManager, CCmdTarget)
	// MRU - most recently used file menu
	ON_UPDATE_COMMAND_UI_RANGE(0, 0xFFFF, OnUpdateRecentFileMenu)
	ON_COMMAND_EX_RANGE(0, 0xFFFF, OnOpenRecentFile)
//	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnUpdateRecentFileMenu)
//	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
END_MESSAGE_MAP()

CMruFileManager::CMruFileManager(CWinApp* pApp) : m_pApp(pApp)
{
}

///////////////////////////////////////////////////////////////////
// Cleanup: destroy all the recent file lists
//
CMruFileManager::~CMruFileManager()
{
	RemoveMRUFileLists();
}

///////////////////////////////////////////////////////////////////
// Add a recent file list to the MRU manager
//
DWORD CMruFileManager::Add(UINT nBaseID,	// base command ID
	LPCTSTR lpszSection,					// registry section (keyname)
	LPCTSTR lpszEntryFormat,				// registry value name
	MRUFILEFN pMruFn,						// filter function
	UINT nMaxMRU,							// max num menu entries
	BOOL bNoLoad,							// don't load from reg (rarely used)
	int nMaxDispLen,						// display length
	UINT nStart)							// size to start
{
	CRecentFileList2* prfl = new CRecentFileList2(nBaseID,
		lpszSection, lpszEntryFormat, pMruFn, 
		nMaxMRU, nMaxDispLen, nStart);
	
	if (!bNoLoad)
		prfl->ReadList();					// load from registry

	m_listRfl.AddTail(prfl);				// add to my list
	
	return (DWORD)prfl;	
}

///////////////////////////////////////////////////////////////////
// Remove a recent file list.
//
BOOL CMruFileManager::Remove(DWORD dwRfl)
{
	POSITION pos = m_listRfl.Find((void*)dwRfl);
	if (pos) 
	{
		m_listRfl.RemoveAt(pos);
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////
// Update recent file menu; that is, convert menuitem to
// sequence of items with each file name.
//
void CMruFileManager::OnUpdateRecentFileMenu(CCmdUI* pCmdUI)
{
	CRecentFileList2* prfl = FindRFL(pCmdUI->m_nID);
	if (prfl)
	{
		pCmdUI->Enable(prfl->GetSize()>0);
		prfl->UpdateMenu(pCmdUI);
	} 
	else 
	{
		pCmdUI->ContinueRouting();
	}
}

///////////////////////////////////////////////////////////////////
// Open one of the recent files
//
BOOL CMruFileManager::OnOpenRecentFile(UINT nID)
{
	CRecentFileList2* prfl = FindRFL(nID);
	if (prfl) 
	{
		int nIndex = nID - prfl->m_nBaseID; // index of file name in list
		ASSERT((*prfl)[nIndex].GetLength() != 0);
		TRACE2("CMruFileManager: open file (%d) '%s'.\n", nIndex + 1,
			(LPCTSTR)(*prfl)[nIndex]);

		// try to open it!
		if (m_pApp->OpenDocumentFile((*prfl)[nIndex]) == NULL)
			prfl->Remove(nIndex);

		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////
// Helper to find the recent file list that goes with a command ID
//
CRecentFileList2* CMruFileManager::FindRFL(UINT nID)
{
	POSITION pos = m_listRfl.GetHeadPosition();
	while (pos) 
	{
		CRecentFileList2* prfl = (CRecentFileList2*)m_listRfl.GetNext(pos);
		if (prfl->m_nBaseID <= nID && nID < prfl->m_nBaseID + prfl->GetSize()) 
		{
			return prfl;
		}
	}
	return NULL;
}

// MFC private fn, but that's no reason not to call it!
extern BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);

///////////////////////////////////////////////////////////////////
// Add a file name to one of the mru lists. 
// Return FALSE if it doesn't belong.
//
BOOL CMruFileManager::AddToRecentFileList(LPCTSTR lpszPathName)
{
	// fully qualify the path name
	TCHAR szTemp[_MAX_PATH];
	AfxFullPath(szTemp, lpszPathName);

	POSITION pos = m_listRfl.GetHeadPosition();

	while (pos) 
	{
		CRecentFileList2* prfl = (CRecentFileList2*)m_listRfl.GetNext(pos);
		if (prfl->IsMyKindOfFile(szTemp))  // does it belong here?
		{
			prfl->Add(szTemp);			   // yup.
			return TRUE;
		}
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////
//
//IMPLEMENT_DYNAMIC(CRecentFileList2, CRecentFileList)

CRecentFileList2::CRecentFileList2(UINT nBaseID, LPCTSTR lpszSection,
	LPCTSTR lpszEntryFormat, MRUFILEFN pMruFn,
	int nMaxMRU, int nMaxDispLen, UINT nStart)
		: CRecentFileList(nStart, lpszSection, lpszEntryFormat, nMaxMRU)
{
		m_nBaseID = nBaseID;
		m_pMruFn  = pMruFn;
}

CRecentFileList2::~CRecentFileList2()
{
}

///////////////////////////////////////////////////////////////////
// Test if file name belongs in my list: call callback fn.
//
BOOL CRecentFileList2::IsMyKindOfFile(LPCTSTR lpszPathName)
{
	return m_pMruFn && (*m_pMruFn)(lpszPathName);
}

///////////////////////////////////////////////////////////////////
// Update the menu
//
void CRecentFileList2::UpdateMenu(CCmdUI* pCmdUI)
{
	// If the menu is really a submenu, MFC can't handle it, so fake out
	// MFC by temporarily setting m_pMenu = m_pSubMenu.
	//
	CMenu* pMenu = pCmdUI->m_pMenu;

	if (pCmdUI->m_pSubMenu)
		pCmdUI->m_pMenu = pCmdUI->m_pSubMenu;

//	CRecentFileList::UpdateMenu(pCmdUI);

	ASSERT(m_arrNames != NULL);

	if (m_strOriginal.IsEmpty() && pMenu != NULL)
		pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);

	if (m_arrNames[0].IsEmpty())
	{
		// no MRU files
		if (!m_strOriginal.IsEmpty())
			pCmdUI->SetText(m_strOriginal);
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pCmdUI->m_pMenu == NULL)
		return;

	for (int iMRU = 0; iMRU < m_nSize; iMRU++)
		pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

	TCHAR szCurDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, szCurDir);
	int nCurDir = lstrlen(szCurDir);
	ASSERT(nCurDir >= 0);
	szCurDir[nCurDir] = '\\';
	szCurDir[++nCurDir] = '\0';

	CString sName;
	CString sTemp;
	for (iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		if (!GetDisplayName(sName, iMRU, szCurDir, nCurDir))
			break;

		// double up any '&' characters so they are not underlined
		LPCTSTR lpszSrc = sName;
		LPTSTR lpszDest = sTemp.GetBuffer(sName.GetLength()*2);
		while (*lpszSrc != 0)
		{
			if (*lpszSrc == '&')
				*lpszDest++ = '&';
			if (_istlead(*lpszSrc))
				*lpszDest++ = *lpszSrc++;
			*lpszDest++ = *lpszSrc++;
		}
		*lpszDest = 0;
		sTemp.ReleaseBuffer();

		// insert mnemonic + the file name
		TCHAR buf[10];
		wsprintf(buf, iMRU < 9 ? _T("&%d ") : _T("%d "), iMRU+1);
		
		pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
			MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++,
			CString(buf) + sTemp);
	}

	// update end menu count
	pCmdUI->m_nIndex--; // point to last menu added
	pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

	pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled

	if (pCmdUI->m_pSubMenu)
		pCmdUI->m_pMenu = pMenu;
}

///////////////////////////////////////////////////////////////////
// Remove a recent file list.
//
void CMruFileManager::RemoveMRUFileLists()
{
	while (!m_listRfl.IsEmpty()) 
	{
		CRecentFileList2* prfl = (CRecentFileList2*)m_listRfl.RemoveHead();
		prfl->WriteList();
		delete prfl;
	}
}
