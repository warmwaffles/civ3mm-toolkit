#if !defined(COLOUR_TABLE_PREVIEW_H__CIV3FLCEDIT__INCLUDED_)
#define COLOUR_TABLE_PREVIEW_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorTable.h"

/////////////////////////////////////////////////////////////////////////////
// CColorTablePreview window
enum EPreviewType
{
	PrevMove	= 0,
	PrevRangeD	= 1,
	PrevRangeS	= 2
};

class CColorTablePreview : public CColorTable
{
public:
	DECLARE_DYNCREATE(CColorTablePreview);
// Construction
public:
    CColorTablePreview();
    CColorTablePreview(CPoint p, RGBQUADPAL* pPal, CWnd* pParentWnd, UINT nID = 0);
	virtual void Initialise();

// Attributes
public:
	UINT*			m_pFrom;
    UINT*			m_pTo;
	UINT*			m_pNumber;
	BOOL*			m_bAllPalette;
	EPreviewType	m_PrevType;

// Operations
public:
	COLORREF		m_clrFrom;
	COLORREF		m_clrTo;
	COLORREF		m_clrRange;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorTablePreview)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CColorTablePreview();

protected:
    virtual void DrawCell(CDC* pDC, int nIndex);

// public attributes
public:
	virtual void SetupVariables(UINT& nFrom, UINT&nTo, UINT& nNumber, BOOL& bAllPalette, EPreviewType type);

// protected attributes
protected:

    // Generated message map functions
protected:
    //{{AFX_MSG(CColorTablePreview)
	afx_msg void OnNcDestroy();
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(COLOUR_TABLE_PREVIEW_H__CIV3FLCEDIT__INCLUDED_)
