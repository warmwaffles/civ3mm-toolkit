#if !defined(PANELEXTRA_H__CIV3FLCEDIT__INCLUDED_)
#define PANELEXTRA_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelExtra.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelExtra dialog

class CAnimationView;
class CDlgBar;

class CPanelExtra : public CDialog
{
	friend class CAnimationView;
	
// Construction
public:
	CPanelExtra(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPanelExtra)
	enum { IDD = IDD_PANEL_EXTRA };
	CButtonX		m_btnInfo;
	CButtonX		m_btnColorTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelExtra)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
public:
	BOOL IsAttached();
	void SetView(CAnimationView* pView);
	void UpdateControlsState();
	
	CAnimationView*	m_pView;
	CDlgBar*		m_pParent;
		
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelExtra)
	virtual BOOL OnInitDialog();
	afx_msg void OnInfo();
	afx_msg void OnColorTable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(PANELEXTRA_H__CIV3FLCEDIT__INCLUDED_)
