#if !defined(PANELANIMATION_H__CIV3FLCEDIT__INCLUDED_)
#define PANELANIMATION_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelAnimation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelAnimation dialog
class CAnimationView;
class CDlgBar;

class CPanelAnimation : public CDialog
{
	friend class CAnimationView;

// Construction
public:
	CPanelAnimation(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPanelAnimation)
	enum { IDD = IDD_PANEL_ANIMATION };
	CStatic			m_staticFrmTotal;
	CStatic			m_staticFrm;
	CStatic			m_staticFrmFirst;
	CStatic			m_staticFrmCur;
	CButtonX		m_btnForward;
	CButtonX		m_btnBack;
	CButtonX		m_btnPlay;
	CButtonX		m_btnPause;
	CButtonX		m_btnStop;
	CButtonX		m_btnFirst;
	CButtonX		m_btnPrevious;
	CButtonX		m_btnNext;
	CButtonX		m_btnLast;
	CString			m_sCurFrame;
	CString			m_sTotalFrame;
	BOOL			m_bBack;
	BOOL			m_bForward;
	CString	m_sFirstFrame;
	//}}AFX_DATA

	CSliderCtrl	m_wndFrameSlider;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelAnimation)
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
	//{{AFX_MSG(CPanelAnimation)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureFramesSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnFirst();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	afx_msg void OnLast();
	afx_msg void OnBack();
	afx_msg void OnForward();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(PANELANIMATION_H__CIV3FLCEDIT__INCLUDED_)
