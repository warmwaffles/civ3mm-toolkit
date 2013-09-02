// FlcView.h : interface of the CFlcView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FLC_VIEW_H__CIV3FLCEDIT__INCLUDED_)
#define FLC_VIEW_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnimationView.h"

class CAnimationView;

class CFlcView : public CAnimationView
{
	friend class CFlcInfoDlg;
	friend class CExportFlcDlg;
	
protected: // create from serialization only
	CFlcView();
	DECLARE_DYNCREATE(CFlcView)

// Operations
public:
	FlcHeader		m_flcHeader;
	FlicAnimHeader	m_civ3Header;
	CArray<FRAME, FRAME&> m_arFrames;	// information about frames	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlcView)
	public:
	virtual void OnFileSave();
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlcView();
	virtual void ShowAnimInfo();
	virtual void Refresh();
	
	virtual void ClearSFrames();
	virtual BOOL LoadAnimation(LPCTSTR lpszPathName, BOOL bColorTable = TRUE);	
//	virtual BOOL SavePalette(LPCTSTR lpszPathName);
	BOOL SaveFLC(LPCTSTR lpszPathName);

	UINT WriteCOLOR_256(FILE* fp);
	static UINT ReadCOLOR_256(BYTE *pBuffer, FILE* fp, RGBQUADPAL& rgbPal);
	UINT ReadDELTA_FLC(BYTE *pBuffer, FILE* fp);
	UINT ReadBLACK(BYTE *pBuffer, FILE* fp);
	UINT ReadBYTE_RUN(BYTE *pBuffer, FILE* fp);
	UINT ReadFLI_COPY(BYTE *pBuffer, FILE* fp);
	
// Generated message map functions
protected:
	ARRAY_SURF m_arSByteRun;
	CDWordArray	m_arColorChunkPos;	// positions of the color chunks
	
	//{{AFX_MSG(CFlcView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnExport();
	afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
	afx_msg void OnOffset();
	afx_msg void OnUpdateOffset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShrinkFrmCount(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FLC_VIEW_H__CIV3FLCEDIT__INCLUDED_)
