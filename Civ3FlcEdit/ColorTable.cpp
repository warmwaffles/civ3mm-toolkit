/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ColorTable.cpp
	CREATED:	03.06.2004
	VERSION:	2.4

	AUTHOR:		Dreyk

	DESCRIPTION: implementation of the CColorTable class - a palette control
	Based on CColourPopupXP class by Zorglab (mailto:zorglab@wanadoo.be)
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "ColorTable.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorTable
IMPLEMENT_DYNAMIC(CColorTable, CWnd)

CColorTable::CColorTable()
{
    Initialise();
}

CColorTable::CColorTable(CPoint p, RGBQUADPAL* pPal, CWnd* pParentWnd, UINT nID /*=0*/)
{
    Initialise();

    Create(p, pPal, pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(CColorTable, CWnd)
//{{AFX_MSG_MAP(CColorTable)
ON_WM_NCDESTROY()
ON_WM_LBUTTONUP()
ON_WM_PAINT()
ON_WM_MOUSEMOVE()
ON_WM_KEYDOWN()
ON_WM_QUERYNEWPALETTE()
ON_WM_PALETTECHANGED()
ON_WM_ACTIVATEAPP()
ON_WM_RBUTTONUP()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void
CColorTable::Initialise()
{
    m_nBoxSize  = 16;
    m_nMargin   = ::GetSystemMetrics(SM_CXEDGE);
    m_nCurrent  = 0;
    m_nSelected = 0;
    m_pParent   = NULL;
    m_crCurrent = m_crSelected = RGB(0, 0, 0);
    m_nID                      = 0;
    m_pToolTip                 = NULL;

    // Get all the colors we need
    int nAlpha         = 48;
    m_clrBackground    = ::GetSysColor(COLOR_MENU);
    m_clrHiLightBorder = ::GetSysColor(COLOR_HIGHLIGHT);
    m_clrHiLight       = m_clrHiLightBorder;
    m_clrLoLight       = RGB((GetRValue(m_clrBackground) * (255 - nAlpha) + GetRValue(m_clrHiLightBorder) * nAlpha) >> 8,
                       (GetGValue(m_clrBackground) * (255 - nAlpha) + GetGValue(m_clrHiLightBorder) * nAlpha) >> 8,
                       (GetBValue(m_clrBackground) * (255 - nAlpha) + GetBValue(m_clrHiLightBorder) * nAlpha) >> 8);

    // Idiot check: Make sure the colour square is at least 5 x 5;
    if (m_nBoxSize - 2 * m_nMargin - 2 < 5)
        m_nBoxSize = 5 + 2 * m_nMargin + 2;
}

CColorTable::~CColorTable()
{
    SAFE_DELETE(m_pToolTip);
}

BOOL
CColorTable::Create(CPoint p, RGBQUADPAL* pPal, CWnd* pParentWnd, UINT nID /*=0*/)
{
    ASSERT(pParentWnd && ::IsWindow(pParentWnd->GetSafeHwnd()));

    m_pParent = pParentWnd;
    m_nID     = nID;

    //	m_hCursor = AfxGetApp()->LoadCursor(IDC_COLOR_PICKER);

    // Get the class name and create the window
    UINT nClassStyle = CS_CLASSDC | CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW;

    CString szClassName = AfxRegisterWndClass(nClassStyle, 0, (HBRUSH)(COLOR_MENU + 1), 0);

    // Calculate the window size and position
    m_WindowRect.SetRect(p.x, p.y, p.x + NUM_COL * m_nBoxSize + 3 * m_nMargin, p.y + NUM_ROW * m_nBoxSize + 3 * m_nMargin);

    if (!CWnd::CreateEx(WS_EX_CLIENTEDGE,
                        szClassName,
                        _T(""),
                        WS_VISIBLE | WS_CHILD,
                        m_WindowRect.left,
                        m_WindowRect.top,
                        m_WindowRect.Width(),
                        m_WindowRect.Height(),
                        pParentWnd->GetSafeHwnd(),
                        0,
                        NULL))
        return FALSE;

    GetClientRect(m_WindowRect);

    // Initialize the color box rectangle
    m_BoxesRect = CRect(CPoint(0, 0), CSize(NUM_COL * m_nBoxSize, NUM_ROW * m_nBoxSize));

    if (!SetPalette(pPal))
        return FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CColorTable message handlers

BOOL
CColorTable::PreTranslateMessage(MSG* pMsg)
{
    m_pToolTip->RelayEvent(pMsg);

    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
            case VK_DOWN:
            case VK_UP:
            case VK_RIGHT:
            case VK_LEFT:
            case VK_RETURN:
                OnKeyDown((UINT)pMsg->wParam, 1, 0);
                return TRUE;
        }
    }

    return CWnd::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// If an arrow key is pressed, then move the selection
//
void
CColorTable::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    int row = GetRow(m_nCurrent), col = GetColumn(m_nCurrent);

    if (nChar == VK_DOWN) {
        row++;
        if (GetIndex(row, col) < 0)
            row = col = 0;

        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_UP) {
        if (row > 0)
            row--;
        else /* row == 0 */
        {
            row = GetRow(COLOR8 - 1);
            col = GetColumn(COLOR8 - 1);
        }

        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_RIGHT) {
        if (col < NUM_COL - 1)
            col++;
        else {
            col = 0;
            row++;
        }

        if (GetIndex(row, col) == INVALID_COLOUR) {
            row = col = 0;
        }

        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_LEFT) {
        if (col > 0)
            col--;
        else /* col == 0 */
        {
            if (row > 0) {
                row--;
                col = NUM_COL - 1;
            } else {
                row = GetRow(COLOR8 - 1);
                col = GetColumn(COLOR8 - 1);
            }
        }

        ChangeSelection(GetIndex(row, col));
    }

    if (nChar == VK_RETURN) {
        if (m_nSelected != m_nCurrent) {
            m_nSelected  = m_nCurrent;
            m_crSelected = m_crCurrent;
            m_pParent->SendMessage(CPN_SELENDOK, (WPARAM)m_crSelected, m_nID);
            Invalidate();
        } else {
            COLORREF color = m_crSelected;
            if (ShowColorDialog(color, this)) {
                ChangeSelectedColor(color);
                m_pParent->SendMessage(CPN_SELENDOK, (WPARAM)m_crSelected, m_nID);
            }
        }
        return;
    }

    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// Auto-deletion
//
void
CColorTable::OnNcDestroy()
{
    CWnd::OnNcDestroy();
}

void
CColorTable::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // Draw colour cells
    for (int i = 0; i < COLOR8; i++)
        DrawCell(&dc, i);
}

void
CColorTable::OnMouseMove(UINT nFlags, CPoint point)
{
    int nNewSelection = INVALID_COLOUR;

    // Translate points to be relative raised window edge
    point.x -= m_nMargin;
    point.y -= m_nMargin;

    if (!m_BoxesRect.PtInRect(point))
        nNewSelection = INVALID_COLOUR;
    else {
        // Get the row and column
        nNewSelection = GetIndex(point.y / m_nBoxSize, point.x / m_nBoxSize);

        // In range? If not, default and exit
        if (nNewSelection < 0 || nNewSelection >= COLOR8) {
            //CWnd::OnMouseMove(nFlags, point);
            //return;
            nNewSelection = INVALID_COLOUR;
        }
    }

    // OK - we have the row and column of the current selection (may be CUSTOM_BOX_VALUE)
    // Has the row/col selection changed? If yes, then redraw old and new cells.
    if (nNewSelection != m_nCurrent)
        ChangeSelection(nNewSelection);

    CWnd::OnMouseMove(nFlags, point);
}

void
CColorTable::OnRButtonUp(UINT nFlags, CPoint point)
{
    if (m_WindowRect.PtInRect(point)) {
        OnLButtonUp(nFlags, point);

        COLORREF color = m_crSelected;
        if (ShowColorDialog(color, this)) {
            ChangeSelectedColor(color);
            m_pParent->SendMessage(CPN_SELENDOK, (WPARAM)m_crSelected, m_nID);
        }
        return;
    }

    CWnd::OnRButtonUp(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// End selection on LButtonUp
//
void
CColorTable::OnLButtonUp(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonUp(nFlags, point);

    if (m_WindowRect.PtInRect(point)) {
        int nNewSelection = INVALID_COLOUR;

        // Translate points to be relative raised window edge
        point.x -= m_nMargin;
        point.y -= m_nMargin;

        if (!m_BoxesRect.PtInRect(point))
            nNewSelection = INVALID_COLOUR;
        else {
            // Get the row and column
            nNewSelection = GetIndex(point.y / m_nBoxSize, point.x / m_nBoxSize);

            // In range? If not, default and exit
            if (nNewSelection < 0 || nNewSelection >= COLOR8) {
                //CWnd::OnMouseMove(nFlags, point);
                //return;
                nNewSelection = INVALID_COLOUR;
            }
        }

        // OK - we have the row and column of the current selection (may be CUSTOM_BOX_VALUE)
        // Has the row/col selection changed? If yes, then redraw old and new cells.
        if (nNewSelection != m_nSelected) {
            ChangeSelection(nNewSelection);
            m_nSelected  = m_nCurrent;  // change selected pos
            m_crSelected = m_crCurrent; // change selected color
            Invalidate();
            m_pParent->SendMessage(CPN_SELENDOK, (WPARAM)m_crSelected, m_nID);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CColorTable implementation

int
CColorTable::GetIndex(int row, int col) const
{
    if (row < 0 || col < 0 || row >= NUM_ROW || col >= NUM_COL)
        return INVALID_COLOUR;
    else {
        if (row * NUM_COL + col >= COLOR8)
            return INVALID_COLOUR;

        return row * NUM_COL + col;
    }
}

int
CColorTable::GetRow(int nIndex) const
{
    if (nIndex < 0 || nIndex >= COLOR8)
        return INVALID_COLOUR;

    return nIndex / NUM_COL;
}

int
CColorTable::GetColumn(int nIndex) const
{
    if (nIndex < 0 || nIndex >= COLOR8)
        return INVALID_COLOUR;

    return nIndex % NUM_COL;
}

/////////////////////////////////////////////////////////////////////////////
// Gets the dimensions of the colour cell given by (row,col)
//
BOOL
CColorTable::GetCellRect(int nIndex, const LPRECT& rect)
{
    if (nIndex < 0 || nIndex >= COLOR8)
        return FALSE;

    rect->left   = GetColumn(nIndex) * m_nBoxSize + m_nMargin;
    rect->top    = GetRow(nIndex) * m_nBoxSize + m_nMargin;
    rect->right  = rect->left + m_nBoxSize;
    rect->bottom = rect->top + m_nBoxSize;

    return TRUE;
}

void
CColorTable::ChangeSelection(int nIndex)
{
    CClientDC dc(this); // device context for drawing

    if (!dc.m_hDC) {
        ASSERT(FALSE);
        return;
    }

    if (nIndex > 255)
        nIndex = 0;

    if (m_nCurrent >= 0 && m_nCurrent < COLOR8) {
        // Set Current selection as invalid and redraw old selection (this way
        // the old selection will be drawn unselected)
        int OldSel = m_nCurrent;
        m_nCurrent = INVALID_COLOUR;
        DrawCell(&dc, OldSel);
    } else
        m_nCurrent = 0;

    // Set the current selection as row/col and draw (it will be drawn selected)
    m_nCurrent = nIndex;
    DrawCell(&dc, m_nCurrent);

    // Store the current colour
    if (m_nCurrent == INVALID_COLOUR) {
        m_crCurrent = INVALID_COLOUR;
        m_pParent->SendMessage(CPN_SELCHANGE, (WPARAM)m_crSelected, m_nID);
    } else {
        m_crCurrent = GetColour(m_nCurrent);
        m_pParent->SendMessage(CPN_SELCHANGE, (WPARAM)m_crCurrent, m_nID);
    }
}

void
CColorTable::DrawCell(CDC* pDC, int nIndex)
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

    if (m_nCurrent == nIndex) {
        bSelected  = TRUE;
        clrHiLight = m_clrHiLight;
    } else if (m_nSelected == nIndex) {
        bSelected  = TRUE;
        clrHiLight = m_clrLoLight;
    } else {
        bSelected  = FALSE;
        clrHiLight = m_clrLoLight;
    }

    CPen* pOldPen     = NULL;
    CBrush* pOldBrush = NULL;

    // If we are currently selected
    if (bSelected) {
        // If we have a background margin, then draw that
        if (sizeMargin.cx > 0 || sizeMargin.cy > 0) {
            pDC->FillSolidRect(&rect, m_clrBackground);
            rect.DeflateRect(sizeMargin.cx, sizeMargin.cy);
        }

        // Draw the selection rectagle
        CPen pen1(PS_SOLID, 1, m_clrHiLightBorder);
        CBrush brush1(clrHiLight);

        pOldPen   = pDC->SelectObject(&pen1);
        pOldBrush = pDC->SelectObject(&brush1);

        pDC->Rectangle(&rect);

        rect.DeflateRect(sizeHiBorder.cx - 1, sizeHiBorder.cy - 1);

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);

        DeleteObject(pen1);
        DeleteObject(brush1);
    }

    // Otherwise, we are not selected
    else {
        // Draw the background
        pDC->FillSolidRect(&rect, m_clrBackground);
        rect.DeflateRect(sizeMargin.cx + sizeHiBorder.cx, sizeMargin.cy + sizeHiBorder.cy);
    }

    // Draw color
    CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
    CBrush brush(RGB(GetRValue(GetColour(nIndex)), GetGValue(GetColour(nIndex)), GetBValue(GetColour(nIndex))));

    pOldPen   = pDC->SelectObject(&pen);
    pOldBrush = pDC->SelectObject(&brush);

    // Draw the cell colour
    rect.DeflateRect(1, 1);
    pDC->Rectangle(&rect);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    DeleteObject(pen);
    DeleteObject(brush);
}

BOOL
CColorTable::OnQueryNewPalette()
{
    Invalidate(FALSE);
    return CWnd::OnQueryNewPalette();
}

void
CColorTable::OnPaletteChanged(CWnd* pFocusWnd)
{
    CWnd::OnPaletteChanged(pFocusWnd);

    if (pFocusWnd->GetSafeHwnd() != GetSafeHwnd())
        Invalidate(FALSE);
}

// KillFocus problem fix suggested by Paul Wilkerson.
void
CColorTable::OnActivateApp(BOOL bActive, HTASK hTask)
{
    CWnd::OnActivateApp(bActive, hTask);

    // If Deactivating App, cancel this selection
    if (!bActive) {
        m_crCurrent = m_crSelected;
        m_pParent->SendMessage(CPN_SELENDCANCEL, (WPARAM)m_crCurrent, m_nID);
    }
}

void
CColorTable::ChangeSelectedColor(COLORREF color)
{
    m_crSelected = m_crColours[m_nSelected].crColour = color;

    CString sRGB;
    sRGB.Format("RGB: %d, %d, %d; Idx: %d", GetRValue(m_crSelected), GetGValue(m_crSelected), GetBValue(m_crSelected), m_nSelected);

    memcpy(m_crColours[m_nSelected].szName, sRGB, sRGB.GetLength());
    m_crColours[m_nSelected].szName[sRGB.GetLength()] = 0;

    CString str;
    m_pToolTip->GetText(str, this, m_nSelected + 1);

    m_pToolTip->UpdateTipText(GetColourName(m_nSelected), this, m_nSelected + 1);
    Invalidate();
}

BOOL
CColorTable::SetPalette(RGBQUADPAL* pPal)
{
    ASSERT(pPal);
    if (!pPal)
        return FALSE;

    m_pPal = pPal;

    // Create the tooltips
    SAFE_DELETE(m_pToolTip);

    m_pToolTip = new CToolTipCtrl;

    // Create the tool tip
    if (!m_pToolTip->Create(this))
        return FALSE;

    CString sRGB;
    CRect rect;

    for (int i = 0; i < COLOR8; i++) {
        m_crColours[i].crColour = RGB(pPal->pal[i].rgbRed, pPal->pal[i].rgbGreen, pPal->pal[i].rgbBlue);

        sRGB.Format("RGB: %d, %d, %d; Idx: %d", pPal->pal[i].rgbRed, pPal->pal[i].rgbGreen, pPal->pal[i].rgbBlue, i);
        memcpy(m_crColours[i].szName, sRGB, sRGB.GetLength());
        m_crColours[i].szName[sRGB.GetLength()] = 0;

        if (!GetCellRect(i, rect))
            continue;

        // Set tooltip
        m_pToolTip->AddTool(this, m_crColours[i].szName, rect, i + 1);
    }

    m_nCurrent   = 0;
    m_nSelected  = 0;
    m_crSelected = m_crCurrent = m_crColours[0].crColour;

    m_pToolTip->SetDelayTime(TTDT_INITIAL, 1000);
    m_pToolTip->SetDelayTime(TTDT_RESHOW, 500);

    Invalidate();
    return TRUE;
}

void
CColorTable::ChangeSelectedIdx(int nIndex)
{
    CClientDC dc(this); // device context for drawing

    if (!dc.m_hDC) {
        ASSERT(FALSE);
        return;
    }

    if (nIndex > 255)
        nIndex = 0;

    if (m_nSelected >= 0 && m_nSelected < COLOR8) {
        // Set Selected selection as invalid and redraw old selection (this way
        // the old selection will be drawn unselected)
        int OldSel  = m_nSelected;
        m_nSelected = INVALID_COLOUR;
        DrawCell(&dc, OldSel);
    }

    // Set the Selected selection as row/col and draw (it will be drawn selected)
    m_nSelected = nIndex;
    DrawCell(&dc, m_nSelected);

    // Store the Selected colour
    if (m_nSelected == INVALID_COLOUR) {
        m_crSelected = INVALID_COLOUR;
        m_pParent->SendMessage(CPN_SELENDOK, (WPARAM)m_crSelected, m_nID);
    } else {
        m_crSelected = GetColour(m_nSelected);
        m_pParent->SendMessage(CPN_SELENDOK, (WPARAM)m_crSelected, m_nID);
    }
}
