// C3fView.h : interface of the CC3fView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(C3F_VIEW_H__CIV3FLCEDIT__INCLUDED_)
#define C3F_VIEW_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnimationView.h"

class CDlgBar;

class CC3fView : public CAnimationView
{
	friend class CFlcImage;
	friend class CShrinkFrmCountDlg;

protected: // create from serialization only
	CC3fView();
	DECLARE_DYNCREATE(CC3fView)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC3fView)
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CC3fView();
	virtual void ShowAnimInfo();
	virtual void Refresh();

	BOOL StoryboardToFlc(CString sPathName);
	virtual BOOL LoadAnimation(LPCTSTR lpszPathName, BOOL bColorTable = TRUE);	

// Generated message map functions
protected:
	UINT WriteBYTE_RUN(BYTE *pBuffer, BYTE nDir);
	UINT WriteDELTA_FLC(BYTE *pBuffer, BYTE nDir, WORD nFrm);
	UINT WriteCOLOR_256(BYTE *pBuffer);
	UINT WriteFLI_COPY(BYTE *pBuffer, WORD nFrm);
	BOOL SaveC3F(LPCTSTR lpszPathName);
	
	C3FSTRUCT		m_c3f;
	CFlcImage*		m_pFlcImage;
	
	enum EPacketType
	{
		UnknownPacket	= 0,
		RepeatPacket	= 1,
		LiterallyPacket	= 2
	};

	//{{AFX_MSG(CC3fView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnExport();
	afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
	afx_msg void OnOffset();
	afx_msg void OnUpdateOffset(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnShrinkFrmCount();
	afx_msg void OnUpdateShrinkFrmCount(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(C3F_VIEW_H__CIV3FLCEDIT__INCLUDED_)
