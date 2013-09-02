/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ComboBoxCivColor.cpp
	CREATED:	21.06.2004
	VERSION:	1.2

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CComboBoxCivColor class - a owner draw
	civilizations' colored combo box
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "ComboBoxCivColor.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
CivColors CComboBoxCivColor::m_crColours[] = 
{
	{ RGB( 57, 100, 252), _T("Default")		},
	{ RGB(233, 233, 233), _T("Civ Color 1")	},
	{ RGB(231,  12,  12), _T("Civ Color 2")	},
	{ RGB(255, 142,   5), _T("Civ Color 3")	},
	{ RGB(235, 235,   5), _T("Civ Color 4")	},
	{ RGB( 11, 120,  10), _T("Civ Color 5")	},
	{ RGB( 31, 219, 219), _T("Civ Color 6")	},
	{ RGB( 57, 100, 252), _T("Civ Color 7")	},
	{ RGB(255,  79, 242), _T("Civ Color 8")	},
	{ RGB(125,   1, 156), _T("Civ Color 9")	},
	{ RGB(144, 107,  11), _T("Civ Color 10")},
	{ RGB(121, 225,   3), _T("Civ Color 11")},
	{ RGB(152,  11,  10), _T("Civ Color 12")},
	{ RGB(151, 151, 151), _T("Civ Color 13")},
	{ RGB(  3,  41, 148), _T("Civ Color 14")},
	{ RGB( 18, 137, 154), _T("Civ Color 15")},
	{ RGB(248, 197, 255), _T("Civ Color 16")},
	{ RGB( 64,  64,  64), _T("Civ Color 17")},
	{ RGB(195,   3, 255), _T("Civ Color 18")},
	{ RGB(210, 183,   0), _T("Civ Color 19")},
	{ RGB(131,   0, 255), _T("Civ Color 20")},
	{ RGB(175, 219, 237), _T("Civ Color 21")},
	{ RGB(118, 137,  57), _T("Civ Color 22")},
	{ RGB(255, 195,   0), _T("Civ Color 23")},
	{ RGB(255, 112,   0), _T("Civ Color 24")},
	{ RGB(198, 215, 153), _T("Civ Color 25")},
	{ RGB(119, 255, 176), _T("Civ Color 26")},
	{ RGB(194, 201,  72), _T("Civ Color 27")},
	{ RGB(178, 137,  82), _T("Civ Color 28")},
	{ RGB(111,  86,  10), _T("Civ Color 29")},
	{ RGB( 73,  93,  48), _T("Civ Color 30")},
	{ RGB(255, 200, 255), _T("Civ Color 31")},
	{ RGB(199, 172, 237), _T("Civ Color 32")}
};
*/
CComboBoxCivColor::CComboBoxCivColor()
{
	m_crColours[ 0] = RGB( 57, 100, 252);
	m_crColours[ 1] = RGB(233, 233, 233);
	m_crColours[ 2] = RGB(231,  12,  12);
	m_crColours[ 3] = RGB(255, 142,   5);
	m_crColours[ 4] = RGB(235, 235,   5);
	m_crColours[ 5] = RGB( 11, 120,  10);
	m_crColours[ 6] = RGB( 31, 219, 219);
	m_crColours[ 7] = RGB( 57, 100, 252);
	m_crColours[ 8] = RGB(255,  79, 242);
	m_crColours[ 9] = RGB(125,   1, 156);
	m_crColours[10] = RGB(144, 107,  11);
	m_crColours[11] = RGB(121, 225,   3);
	m_crColours[12] = RGB(152,  11,  10);
	m_crColours[13] = RGB(151, 151, 151);
	m_crColours[14] = RGB(  3,  41, 148);
	m_crColours[15] = RGB( 18, 137, 154);
	m_crColours[16] = RGB(248, 197, 255);
	m_crColours[17] = RGB( 64,  64,  64);
	m_crColours[18] = RGB(195,   3, 255);
	m_crColours[19] = RGB(210, 183,   0);
	m_crColours[20] = RGB(131,   0, 255);
	m_crColours[21] = RGB(175, 219, 237);
	m_crColours[22] = RGB(118, 137,  57);
	m_crColours[23] = RGB(255, 195,   0);
	m_crColours[24] = RGB(255, 112,   0);
	m_crColours[25] = RGB(198, 215, 153);
	m_crColours[26] = RGB(119, 255, 176);
	m_crColours[27] = RGB(194, 201,  72);
	m_crColours[28] = RGB(178, 137,  82);
	m_crColours[29] = RGB(111,  86,  10);
	m_crColours[30] = RGB( 73,  93,  48);
	m_crColours[31] = RGB(255, 200, 255);
	m_crColours[32] = RGB(199, 172, 237);
}

CComboBoxCivColor::~CComboBoxCivColor()
{
}


BEGIN_MESSAGE_MAP(CComboBoxCivColor, CComboBox)
	//{{AFX_MSG_MAP(CComboBoxCivColor)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CComboBoxCivColor::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (GetCount() == 0 || lpDrawItemStruct->itemID > (UINT)GetCount())
		return;

	CString str;
	GetLBText(lpDrawItemStruct->itemID, str);

	UINT	state	= lpDrawItemStruct->itemState;
	CDC		dc;
	dc.Attach(lpDrawItemStruct->hDC);

	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor	= dc.GetBkColor();

	// If this item is selected, set the background color and the text color to appropriate 
	// values. Erase the rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(state & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
	}

	CRect rect(lpDrawItemStruct->rcItem);
	rect.DeflateRect(1, 0);

	if ((state & ODS_DISABLED) == 0)
	{
		CRect rcColor(rect);
		rcColor.DeflateRect(5, 2);
		rcColor.right = rcColor.left + 50;
		
		dc.FillSolidRect(&rcColor, m_crColours[lpDrawItemStruct->itemID]);
		
		::FrameRect(dc.m_hDC, &rcColor,
			::GetSysColorBrush(COLOR_WINDOWTEXT));
		
		rect.DeflateRect(rcColor.Width() + 10, -1, 0, -1);
		
		CFont *pOldFont = dc.SelectObject(GetFont());
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(str, &rect, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
		dc.SelectObject(pOldFont);
	}
	else 
	{
		// Disabled text
		dc.SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		dc.DrawText(str, -1, &rect, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
	}

	// Reset the background color and the text color back to their original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach();
}

void CComboBoxCivColor::SetItemColor(int nIndex, COLORREF rcColor)
{
	if (nIndex < 0 || nIndex > MAX_COLORS_NUMBER)
		return;

	m_crColours[nIndex] = rcColor;
	Invalidate();
}

COLORREF CComboBoxCivColor::GetItemColor(int nIndex)
{
	if (nIndex < 0 || nIndex > MAX_COLORS_NUMBER)
		return m_crColours[0];

	return m_crColours[nIndex];
}

void CComboBoxCivColor::Initialize()
{
	if (!GetSafeHwnd())
	{
		ASSERT(FALSE);
		return;
	}

	AddString(uLoadString(IDS_DEF_COLOR));

	for (int i = 1; i < MAX_COLORS_NUMBER; i++)
	{
		AddString(uFormatString(IDS_CIV_COLOR, i));
	}

	SetCurSel(0);
}

void CComboBoxCivColor::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	ASSERT(lpMeasureItemStruct->CtlType == ODT_COMBOBOX);
	
	if (lpMeasureItemStruct->itemID != (UINT) -1)
	{
		LPCTSTR lpszText = (LPCTSTR) lpMeasureItemStruct->itemData;
		ASSERT(lpszText != NULL);
		CSize   sz;
		CDC*    pDC = GetDC();
		
		sz = pDC->GetTextExtent(lpszText);
		
		ReleaseDC(pDC);
		
		lpMeasureItemStruct->itemHeight = sz.cy;
	}
}
