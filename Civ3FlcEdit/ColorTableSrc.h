#if !defined(COLOUR_TABLE_SRC_H__CIV3FLCEDIT__INCLUDED_)
#define COLOUR_TABLE_SRC_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorTable.h"

/////////////////////////////////////////////////////////////////////////////
// CColorTableSrc window

class CColorTableSrc : public CColorTable
{
    friend class CColorReplaceDlg;

  public:
    DECLARE_DYNCREATE(CColorTableSrc);
    // Construction
  public:
    CColorTableSrc();
    CColorTableSrc(CPoint p, RGBQUADPAL* pPal, CWnd* pParentWnd, UINT nID = 0);

    // Attributes
  public:
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorTableSrc)
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CColorTableSrc();

  protected:
    // public attributes
  public:
    // protected attributes
  protected:
    // Generated message map functions
  protected:
    //{{AFX_MSG(CColorTableSrc)
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

#endif // !defined(COLOUR_TABLE_SRC_H__CIV3FLCEDIT__INCLUDED_)
