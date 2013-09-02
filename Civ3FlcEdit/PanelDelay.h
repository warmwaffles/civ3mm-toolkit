#if !defined(PANELDELAY_H__CIV3FLCEDIT__INCLUDED_)
#define PANELDELAY_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelDelay.h : header file
//

#include "..\MFCaids\EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelDelay dialog

class CAnimationView;
class CDlgBar;

class CPanelDelay : public CDialog
{
	friend class CAnimationView;
	
// Construction
public:
	CPanelDelay(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPanelDelay)
	enum { IDD = IDD_PANEL_DELAY };
	CSpinButtonCtrl	m_wndDelaySpin;
	CEditX			m_wndDelay;
	int				m_nTimerValue;
	CStatic			m_wndDefTimer;
	CString			m_sDefTimer;
	CString			m_sDelay;
	//}}AFX_DATA
	
	CSliderCtrl	m_wndTimerSlider;
	UINT		m_nDelay;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelDelay)
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
	//{{AFX_MSG(CPanelDelay)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureTimerSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioTimerDef();
	afx_msg void OnRadioTimerCustom();
	afx_msg void OnChangeDelay();
	afx_msg void OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(PANELDELAY_H__CIV3FLCEDIT__INCLUDED_)
