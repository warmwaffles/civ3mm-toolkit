#if !defined(COLOUR_TABLE_H__CIV3FLCEDIT__INCLUDED_)
#define COLOUR_TABLE_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Comment this line out if you don't want XP theme support.
#include "Definitions.h"

#include "..\MFCaids\ColourPickerXP.h"
#include "..\MFCaids\MemDC.h"
#include "..\MFCaids\Utilities.h"

#include <math.h>

// Defines and init for ColorTable
#define INVALID_COLOUR -1

#ifndef SPI_GETFLATMENU
#define SPI_GETFLATMENU 0x1022
#endif

#ifndef SPI_GETDROPSHADOW
#define SPI_GETDROPSHADOW 0x1024
#endif

#ifndef CS_DROPSHADOW
#define CS_DROPSHADOW 0x00020000
#endif

#ifndef ODS_HOTLIGHT
#define ODS_HOTLIGHT 0x0040
#endif

#ifndef COLOR_MENUHILIGHT
#define COLOR_MENUHILIGHT 29
#endif

#define NUM_COL 16
#define NUM_ROW 16

/////////////////////////////////////////////////////////////////////////////
// ColourPopupXP                                         //
/////////////////////////////////////////////////////////////////////////////

// Sizing definitions
static const CSize s_sizeBoxHiBorder(2, 2);
static const CSize s_sizeBoxMargin(0, 0);

/////////////////////////////////////////////////////////////////////////////
// CColorTable window

class CColorTable : public CWnd
{
    friend class CColorTableDlg;
    friend class CColorReplaceDlg;
    friend class CReplaceRangeDlg;
    friend class CColorSelectDlg;

  public:
    DECLARE_DYNCREATE(CColorTable);
    // Construction
  public:
    CColorTable();
    CColorTable(CPoint p, RGBQUADPAL* pPal, CWnd* pParentWnd, UINT nID = 0);
    virtual void Initialise();

    // Attributes
  public:
    // Operations
  public:
    BOOL Create(CPoint p, RGBQUADPAL* pPal, CWnd* pParentWnd, UINT nID = 0);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorTable)
  public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CColorTable();

  protected:
    BOOL GetCellRect(int nIndex, const LPRECT& rect);
    virtual void ChangeSelection(int nIndex);
    virtual void DrawCell(CDC* pDC, int nIndex);

    int GetIndex(int row, int col) const;
    int GetRow(int nIndex) const;
    int GetColumn(int nIndex) const;

    // public attributes
  public:
    void ChangeSelectedIdx(int nIndex);
    virtual BOOL SetPalette(RGBQUADPAL* pPal);
    void ChangeSelectedColor(COLORREF color);
    COLORREF GetColour(int nIndex)
    {
        return m_crColours[nIndex].crColour;
    }
    LPCTSTR GetColourName(int nIndex)
    {
        return m_crColours[nIndex].szName;
    }

    ColourTableEntry m_crColours[COLOR8];

    // protected attributes
  protected:
    int m_nBoxSize, m_nMargin;
    int m_nCurrent;        // current color number
    int m_nSelected;       // selected color number
    COLORREF m_crCurrent;  // current color
    COLORREF m_crSelected; // selected color
    CRect m_WindowRect, m_BoxesRect;
    CToolTipCtrl* m_pToolTip;
    CWnd* m_pParent;
    UINT m_nID;
    BOOL m_bChildWindowVisible;

    COLORREF m_clrBackground, m_clrHiLightBorder, m_clrHiLight, m_clrLoLight;

    RGBQUADPAL* m_pPal; // current palette

    // Generated message map functions
  protected:
    //{{AFX_MSG(CColorTable)
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

#endif // !defined(COLOUR_TABLE_H__CIV3FLCEDIT__INCLUDED_)
