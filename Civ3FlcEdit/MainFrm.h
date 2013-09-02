// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8474F6D4_DF51_4A69_AEB5_D3DE67CF57C6__INCLUDED_)
#define AFX_MAINFRM_H__8474F6D4_DF51_4A69_AEB5_D3DE67CF57C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DlgBar.h"
#include "DialogBarMy.h"

#include "..\MFCaids\FramePosition.h"
#include "..\MFCaids\ToolBarX.h"
#include "..\MFCaids\ReBarX.h"

#define CIV3FLC_CLASS_NAME	"Civ3FlcEditV2"
#define MSG_DOCUMENT_OPEN ((DWORD)-73)

#define MAIN_FRAME ((CMainFrame *)AfxGetMainWnd())

// Main Toolbar
static WORD TbMain[] = 
{
	IDB_MAINFRAME,		// Resource id of the bitmap-icons
		16, 16,			// Dimension of each icon
		ID_FILE_OPEN,
		ID_FILE_CLOSE,
		ID_OPEN_INI,
		ID_OPEN_MULTI,
		ID_FILE_SAVE,
		ID_FILE_SAVE_AS,
		ID_FLC_OPEN,
		ID_C3F_OPEN,
		ID_C3F_NEW,
		ID_FLC_EXPORT,
		ID_C3F_EXPORT,
		ID_REFRESH,
		ID_APP_EXIT,
		ID_PLAY,
		ID_PAUSE,
		ID_STOP,
		ID_FIRST,
		ID_BACK,
		ID_PREVIOUS,
		ID_NEXT,
		ID_FORWARD,
		ID_LAST,
		ID_BGD_SELECT,
		ID_COLOR_TABLE,
		ID_IMPORT_PALETTE,
		ID_EXPORT_PALETTE,
		ID_INFO,
		ID_COPY_TO_CLIPBOARD,
		ID_COPY_TO_FILE,
		ID_ZOOM,
		ID_WINDOW_NEW,
		ID_WINDOW_CASCADE,
		ID_WINDOW_TILE_HORZ,
		ID_WINDOW_ARRANGE,
		ID_WINDOW_CLOSEALL,
		ID_OPTIONS,
		ID_APP_ABOUT,
		ID_OFFSET,
		ID_IMPORT_C3F,
		ID_SAVE_FRAME,
		ID_DIRECTION_SW,
		ID_DIRECTION_W,
		ID_DIRECTION_NW,
		ID_DIRECTION_N,
		ID_DIRECTION_NE,
		ID_DIRECTION_E,
		ID_DIRECTION_SE,
		ID_DIRECTION_S,
		ID_DIRECTION_ALL,
		ID_COLOR_REPLACE,
		ID_SELECT_IMG,
		ID_SHRINK_FRM_COUNT,
		NULL
};       // Marker for end of table

class CMainFrame : public CNewMDIFrameWnd, CFramePosition
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:
	COLORREF		m_crBgd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	CAnimationView* GetAnimationView();
	CToolBarX* GetToolBar();
	void ApplyOptions();
	void UpdateViewOptions();
	
	CDlgBar* GetDlgBar();
	void ShowDialogBar(BOOL bShow = TRUE);
	

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar			m_wndStatusBar;
	CToolBarX			m_wndToolBar;
	CReBarX				m_wndReBar;

	CDlgBar				m_wndDlgBar;
	CDialogBarMy		m_wndDialogBar;

	BOOL				m_bShowToolBar;
	BOOL				m_bShowStatusBar;
	BOOL				m_bShowDialogBar;

	HWND				m_hWndTopMost;

// Generated message map functions
protected:
	void ShowToolBars();
	void LoadFromRegistry();
	void SaveToRegistry();
	virtual LPCTSTR RegKey() { return "MainFrame"; }

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnOptions();
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewStatusBar();
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnViewDialogBar();
	afx_msg void OnUpdateViewDialogBar(CCmdUI* pCmdUI);
	afx_msg void OnAlwaysontop();
	afx_msg void OnUpdateAlwaysontop(CCmdUI* pCmdUI);
	afx_msg void OnNewStoryboard();
	afx_msg void OnUpdateFALSE(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFALSEandUNCHECK(CCmdUI* pCmdUI);
	afx_msg void OnOpenINI();
	afx_msg void OnOpenMulti();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnImporC3f();
	afx_msg void OnColorReplace();
	//}}AFX_MSG
	afx_msg void OnDrawColorButton(NMHDR * pNotifyStruct, LRESULT * result);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8474F6D4_DF51_4A69_AEB5_D3DE67CF57C6__INCLUDED_)
