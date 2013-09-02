// Civ3FlcEdit.h : main header file for the CIV3FLCEDIT application
//

#if !defined(AFX_CIV3FLCEDIT_H__796E9D7E_F471_4E60_9E2B_BA70409C58A7__INCLUDED_)
#define AFX_CIV3FLCEDIT_H__796E9D7E_F471_4E60_9E2B_BA70409C58A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "FlcDoc.h"
#include "Definitions.h"

#include "..\MFCaids\MruMgr.h"

#define MAIN_APP ((CCiv3FlcEditApp *)AfxGetApp())

class CDocManagerEx;
class CAnimationView;

/////////////////////////////////////////////////////////////////////////////
// CCiv3FlcEditApp:
// See Civ3FlcEdit.cpp for the implementation of this class
//

class CCiv3FlcEditApp : public CWinApp
{
public:
	CAnimationView* GetAnimationView();
	CDocManagerEx* GetDocManager();
	EPalType GetPaletteType() { return m_palType; }
	BOOL IsCivRegistered() { return m_bCivRegistered; }
	CString GetCivInstallPath() { return m_sCivInstallPath; }
	BOOL IsC3CRegistered() { return m_bC3CRegistered; }
	CString GetC3CInstallPath() { return m_sC3CInstallPath; }
	BOOL IsPtwRegistered() { return m_bPtwRegistered; }
	CString GetPtwInstallPath() { return m_sPtwInstallPath; }

	CString GetPaletteDirectory() { return m_sPalDir; }
	CCiv3FlcEditApp();
	
	CDocument* GetDoc();
	void SetFileTypeAssociations(BOOL bRegister = TRUE);
	BOOL IsFileTypeRegistered(CDocTemplate *pDocTemplate);
	void SetAppParameters();
	INT GetMaxMRU() { return m_nMaxMRU; }
	BOOL IsLoadLastDoc() { return m_bLoadLastDoc; }
	BOOL IsStartup() { return m_bStartup; }
	BOOL IsShowStaticFrm() { return m_bShowStaticFrm; }
	BOOL IsExpOpen() { return m_bExpOpen; }
	BOOL IsExpAsk() { return m_bExpAsk; }
	CStringArray* GetSearchFolders() {return &m_arSearch; }
	
protected:	
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
	BOOL OpenWithExisting(CCommandLineInfo &cmdInfo);

	BOOL			m_bCivRegistered;	// is Vanilla Civilization III registered
	BOOL			m_bPtwRegistered;	// is Play the World registered
	BOOL			m_bC3CRegistered;	// is Conquests registered
	
	CString			m_sCivInstallPath;	// the Vanilla Civilization III install path
	CString			m_sPtwInstallPath;	// the Play the World install path
	CString			m_sC3CInstallPath;	// the Conquests install path
	
	CStringArray	m_arSearch; // scenario Search folders
	
	CMruFileManager m_mruFileMgr;
	int             m_nMaxMRU; // maximal count of stored MRU files
	BOOL            m_bLoadLastDoc; // load last used document at startup
	BOOL            m_bStartup; // is it startup
	BOOL            m_bCheckFileType; // check file associations
	BOOL			m_bShowStaticFrm; // show static frame or play animation when doc is loaded
	BOOL			m_bExpOpen;
	BOOL			m_bExpAsk;
	EPalType		m_palType;
	CString			m_sPalDir; // predefined palette directory
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCiv3FlcEditApp)
	public:
	virtual BOOL InitInstance();
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCiv3FlcEditApp)
	afx_msg void OnAppAbout();
	afx_msg void OnWindowCloseall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIV3FLCEDIT_H__796E9D7E_F471_4E60_9E2B_BA70409C58A7__INCLUDED_)
