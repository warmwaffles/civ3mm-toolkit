#if !defined(DIALOG_BAR_H__CIV3FLCEDIT__INCLUDED_)
#define DIALOG_BAR_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBar.h : header file
//

#include "PanelAnimation.h"
#include "PanelDelay.h"
#include "PanelDirection.h"
#include "PanelExtra.h"
#include "PanelSound.h"
#include "PanelView.h"

#define REG_DLG_BAR			"DlgBar"

/////////////////////////////////////////////////////////////////////////////
// CDlgBar dialog

#include "..\MFCaids\DialogBarX.h"
#include "..\MFCaids\RollupCtrl.h"

class CAnimationView;

class CPanelAnimation;
class CPanelDelay;
class CPanelDirection;
class CPanelExtra;
class CPanelSound;
class CPanelView;

class CDlgBar : public CDialog
{
	friend class CAnimationView;

// Construction
public:
	void UpdateControlsState();
	BOOL IsAttached();
	void SetView(CAnimationView* pView);
	CAnimationView* GetView();
	CDocument* GetDoc();
	CDlgBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBar)
	enum { IDD = IDD_DIALOG_BAR };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CAnimationView*		m_pView;
	HACCEL				m_hAccelTable;
	CRollupCtrl			m_wndRollup;

	CPanelAnimation*	m_pPanelAnimation;
	CPanelDelay*		m_pPanelDelay;
	CPanelDirection*	m_pPanelDirection;
	CPanelExtra*		m_pPanelExtra;
	CPanelSound*		m_pPanelSound;
	CPanelView*			m_pPanelView;

	// Generated message map functions
	//{{AFX_MSG(CDlgBar)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(DIALOG_BAR_H__CIV3FLCEDIT__INCLUDED_)
