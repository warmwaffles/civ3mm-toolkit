/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ColorTablePreview.cpp
	CREATED:	12.11.2004
	VERSION:	1.1

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CColorTablePreview class - a palette control
	Based on CColourPopupXP class by Zorglab (mailto:zorglab@wanadoo.be)
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "ColorTablePreview.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorTablePreview
IMPLEMENT_DYNAMIC(CColorTablePreview, CColorTable)

CColorTablePreview::CColorTablePreview()
{
    Initialise();
}

CColorTablePreview::CColorTablePreview(CPoint p, RGBQUADPAL* pPal, CWnd* pParentWnd, UINT nID /*=0*/)
{
    Initialise();
	
    Create(p, pPal, pParentWnd, nID);
}


BEGIN_MESSAGE_MAP(CColorTablePreview, CWnd)
	//{{AFX_MSG_MAP(CColorTablePreview)
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

CColorTablePreview::~CColorTablePreview()
{
    CColorTable::~CColorTable();
}

void CColorTablePreview::OnRButtonUp(UINT nFlags, CPoint point) 
{
}

void CColorTablePreview::OnPaint() 
{
	CColorTable::OnPaint();
}

void CColorTablePreview::OnNcDestroy() 
{
    CColorTable::OnNcDestroy();
}

void CColorTablePreview::OnMouseMove(UINT nFlags, CPoint point) 
{
	CColorTable::OnMouseMove(nFlags, point);
}

void CColorTablePreview::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CColorTable::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CColorTablePreview::OnQueryNewPalette() 
{
    return CColorTable::OnQueryNewPalette();
}

void CColorTablePreview::OnPaletteChanged(CWnd* pFocusWnd) 
{
    CColorTable::OnPaletteChanged(pFocusWnd);
}

void CColorTablePreview::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CColorTable::OnActivateApp(bActive, hTask);
}

void CColorTablePreview::OnLButtonUp(UINT nFlags, CPoint point)
{
	CColorTable::OnLButtonUp(nFlags, point);
}

void CColorTablePreview::SetupVariables(UINT &nFrom, UINT &nTo, UINT &nNumber, BOOL& bAllPalette, EPreviewType type)
{
	m_pFrom			= &nFrom;
    m_pTo			= &nTo;
	m_pNumber		= &nNumber;
	m_bAllPalette	= &bAllPalette;
	m_PrevType		= type;

	Invalidate();
}

void CColorTablePreview::DrawCell(CDC* pDC, int nIndex)
{
	// Get the drawing rect
	CRect rect;
	if (!GetCellRect(nIndex, &rect)) 
		return;

	// Get the text pointer and colors
	COLORREF clrBox;
	CSize sizeMargin;
	CSize sizeHiBorder;

	clrBox       = GetColour(nIndex);
	sizeMargin   = s_sizeBoxMargin;
	sizeHiBorder = s_sizeBoxHiBorder;

	// Based on the selectons, get our colors
	COLORREF clrHiLight;
	BOOL bSelected = FALSE;

	int nMin, nMax;

	switch (m_PrevType)
	{
	case PrevMove:
		{
			if (*m_bAllPalette)
			{
				nMin = min(*m_pFrom, *m_pTo);
				nMax = max(*m_pFrom, *m_pTo);
			}
			else
			{
				nMin = *m_pFrom;
				nMax = *m_pFrom + *m_pNumber;
			}
			
			if (*m_pFrom == (UINT)nIndex)
			{
				bSelected  = TRUE;
				clrHiLight = m_clrFrom;
			}
			else if (*m_pTo == (UINT)nIndex)
			{
				bSelected  = TRUE;
				clrHiLight = m_clrTo;
			}
			else if (nIndex > nMin && nIndex < nMax)
			{
				bSelected  = TRUE;
				clrHiLight = m_clrRange;
			}
			else
			{
				bSelected  = FALSE;
				clrHiLight = m_clrLoLight;
			}
		}
		break;

	case PrevRangeD:
		{
			nMin = *m_pTo;
			nMax = *m_pTo + *m_pNumber;
			
			if (*m_pTo == (UINT)nIndex)
			{
				bSelected  = TRUE;
				clrHiLight = m_clrTo;
			}
			else if (nIndex > nMin && nIndex < nMax)
			{
				bSelected  = TRUE;
				clrHiLight = m_clrRange;
			}
			else
			{
				bSelected  = FALSE;
				clrHiLight = m_clrLoLight;
			}
		}
		break;
		
	case PrevRangeS:
		{
			nMin = *m_pFrom;
			nMax = *m_pFrom + *m_pNumber;
			
			if (*m_pFrom == (UINT)nIndex)
			{
				bSelected  = TRUE;
				clrHiLight = m_clrFrom;
			}
			else if (nIndex > nMin && nIndex < nMax)
			{
				bSelected  = TRUE;
				clrHiLight = m_clrRange;
			}
			else
			{
				bSelected  = FALSE;
				clrHiLight = m_clrLoLight;
			}
		}
		break;
	}

	CPen *pOldPen = NULL;
	CBrush *pOldBrush = NULL;

	// If we are currently selected
	if (bSelected)
	{
		// If we have a background margin, then draw that
		if (sizeMargin.cx > 0 || sizeMargin.cy > 0)
		{
			pDC->FillSolidRect(&rect, m_clrBackground);
			rect.DeflateRect(sizeMargin.cx, sizeMargin.cy);
		}

		// Draw the selection rectagle
		// Draw the selection rectagle
		CPen pen1(PS_SOLID, 1, m_clrHiLightBorder);
		CBrush brush1(clrHiLight);
		
		pOldPen		= pDC->SelectObject(&pen1);
		pOldBrush	= pDC->SelectObject(&brush1);
		
		pDC->Rectangle(&rect);
		
		rect.DeflateRect(sizeHiBorder.cx - 1, sizeHiBorder.cy - 1);
		
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		
		DeleteObject(pen1);
		DeleteObject(brush1);
	}

	// Otherwise, we are not selected
	else
	{
		// Draw the background
		pDC->FillSolidRect(&rect, m_clrBackground);
		rect.DeflateRect(sizeMargin.cx + sizeHiBorder.cx, sizeMargin.cy + sizeHiBorder.cy);
	}

	// Draw color
	// Draw color
	CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
	CBrush brush(RGB(GetRValue(GetColour(nIndex)), 
		GetGValue(GetColour(nIndex)), 
		GetBValue(GetColour(nIndex))));
	
	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = pDC->SelectObject(&brush);
	
	
	// Draw the cell colour
	rect.DeflateRect(1, 1);
	pDC->Rectangle(&rect);
	
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	
	DeleteObject(pen);
	DeleteObject(brush);
}

void CColorTablePreview::Initialise()
{
	m_pFrom			= NULL;
    m_pTo			= NULL;
	m_pNumber		= NULL;
	m_bAllPalette	= NULL;
	m_PrevType		= PrevMove;
	
	
	m_clrFrom		= RGB(255, 0, 0);
	m_clrTo			= RGB(0, 0, 255);
	m_clrRange		= RGB(0, 255, 0);

	CColorTable::Initialise();
}