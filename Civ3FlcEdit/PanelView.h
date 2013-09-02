#if !defined(PANELVIEW_H__CIV3FLCEDIT__INCLUDED_)
#define PANELVIEW_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelView.h : header file
//

#include "ComboBoxCivColor.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelView dialog

class CAnimationView;
class CDlgBar;

class CPanelView : public CDialog
{
	friend class CAnimationView;
	
// Construction
public:
	CPanelView(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPanelView)
	enum { IDD = IDD_PANEL_VIEW };
	CStatic				m_wndStaticCivColor;
	CButton				m_btnAlphaBlend;
	int					m_nPalette;
	CComboBoxCivColor	m_wndPalettes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelView)
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
	//{{AFX_MSG(CPanelView)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokComboPalettes();
	afx_msg void OnUseAlphaBlending();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(PANELVIEW_H__CIV3FLCEDIT__INCLUDED_)
