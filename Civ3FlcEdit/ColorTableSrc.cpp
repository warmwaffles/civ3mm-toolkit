/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ColorTableSrc.cpp
	CREATED:	04.11.2004
	VERSION:	1.0

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CColorTableSrc class - a palette control
	Based on CColourPopupXP class by Zorglab (mailto:zorglab@wanadoo.be)
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "ColorTableSrc.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorTableSrc
IMPLEMENT_DYNAMIC(CColorTableSrc, CColorTable)

CColorTableSrc::CColorTableSrc()
{
    Initialise();
}

CColorTableSrc::CColorTableSrc(CPoint p, RGBQUADPAL* pPal, CWnd* pParentWnd, UINT nID /*=0*/)
{
    Initialise();

    Create(p, pPal, pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(CColorTableSrc, CWnd)
//{{AFX_MSG_MAP(CColorTableSrc)
ON_WM_RBUTTONUP()
ON_WM_PAINT()
ON_WM_NCDESTROY()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_KEYDOWN()
ON_WM_QUERYNEWPALETTE()
ON_WM_PALETTECHANGED()
ON_WM_ACTIVATEAPP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CColorTableSrc::~CColorTableSrc()
{
    CColorTable::~CColorTable();
}

void
CColorTableSrc::OnRButtonUp(UINT nFlags, CPoint point)
{
}

void
CColorTableSrc::OnPaint()
{
    //	CPaintDC dc(this); // device context for painting

    CColorTable::OnPaint();
}

void
CColorTableSrc::OnNcDestroy()
{
    CColorTable::OnNcDestroy();
}

void
CColorTableSrc::OnMouseMove(UINT nFlags, CPoint point)
{
    CColorTable::OnMouseMove(nFlags, point);
}

void
CColorTableSrc::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CColorTable::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL
CColorTableSrc::OnQueryNewPalette()
{
    return CColorTable::OnQueryNewPalette();
}

void
CColorTableSrc::OnPaletteChanged(CWnd* pFocusWnd)
{
    CColorTable::OnPaletteChanged(pFocusWnd);
}

void
CColorTableSrc::OnActivateApp(BOOL bActive, HTASK hTask)
{
    CColorTable::OnActivateApp(bActive, hTask);
}

void
CColorTableSrc::OnLButtonUp(UINT nFlags, CPoint point)
{
    CColorTable::OnLButtonUp(nFlags, point);
}