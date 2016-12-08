/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 ColorTableDlg.cpp
	CREATED:	 03.06.2004

	AUTHOR:		 Dreyk
	MODIFIED:	

	DESCRIPTION: implementation of the CColorTableDlg class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ColorTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_SETFOCUS 321

/////////////////////////////////////////////////////////////////////////////
// CColorTableDlg dialog

CColorTableDlg::CColorTableDlg(RGBQUADPAL* pPal, CWnd* pParent /*=NULL*/)
  : CDialogX(CColorTableDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CColorTableDlg)
    m_sRed    = _T("");
    m_sIndex  = _T("");
    m_sGreen  = _T("");
    m_sBlue   = _T("");
    m_sIndexS = _T("");
    m_nBlueS  = 0;
    m_nGreenS = 0;
    m_nRedS   = 0;
    //}}AFX_DATA_INIT

    m_bDrawGripper = FALSE;
    m_pPal         = pPal;

    //	m_brCurrent		= CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
    //	m_brSelected	= CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
}

void
CColorTableDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CColorTableDlg)
    DDX_Control(pDX, IDC_SELECT_COLOR, m_btnSelect);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_STATIC_SELECTED, m_wndSelColor);
    DDX_Control(pDX, IDC_STATIC_CURRENT, m_wndCurColor);
    DDX_Control(pDX, IDC_COLOR_RED_S, m_wndRedS);
    DDX_Control(pDX, IDC_COLOR_GREEN_S, m_wndGreenS);
    DDX_Control(pDX, IDC_COLOR_BLUE_S, m_wndBlueS);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Text(pDX, IDC_COLOR_RED, m_sRed);
    DDX_Text(pDX, IDC_COLOR_INDEX, m_sIndex);
    DDX_Text(pDX, IDC_COLOR_GREEN, m_sGreen);
    DDX_Text(pDX, IDC_COLOR_BLUE, m_sBlue);
    DDX_Text(pDX, IDC_COLOR_INDEX_S, m_sIndexS);
    DDX_Text(pDX, IDC_COLOR_BLUE_S, m_nBlueS);
    DDX_Text(pDX, IDC_COLOR_GREEN_S, m_nGreenS);
    DDX_Text(pDX, IDC_COLOR_RED_S, m_nRedS);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CColorTableDlg, CDialogX)
//{{AFX_MSG_MAP(CColorTableDlg)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_SELECT_COLOR, OnSelectColor)
ON_EN_CHANGE(IDC_COLOR_BLUE_S, OnChangeColorBlueS)
ON_EN_CHANGE(IDC_COLOR_GREEN_S, OnChangeColorGreenS)
ON_EN_CHANGE(IDC_COLOR_RED_S, OnChangeColorRedS)
ON_WM_CTLCOLOR()
ON_WM_TIMER()
//}}AFX_MSG_MAP
ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
ON_MESSAGE(CPN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorTableDlg message handlers

BOOL
CColorTableDlg::OnInitDialog()
{
    ASSERT(m_pPal);
    if (!m_pPal) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    CDialogX::OnInitDialog();

    LoadPosition(this);

    // Setup buttons
    m_btnOK.SetIcon(IDI_OK);
    m_btnCancel.SetIcon(IDI_CANCEL);
    m_btnSelect.SetIcon(IDI_SELECT_COLOR);

    // Set icon to the dialog caption
    SetIcon(AfxGetApp()->LoadIcon(IDI_PALETTE), FALSE);

    CWnd* pWnd = NULL;
    CRect rect;

    pWnd = GetDlgItem(IDC_PALETTE_POS);
    pWnd->GetWindowRect(&rect);
    ScreenToClient(&rect);

    // IDC_COLOR_TABLE
    if (!m_wndColorTable.Create(rect.TopLeft(), m_pPal, this, IDC_COLOR_TABLE)) {
        TRACE("Cannot create ColorTable window\r\n");
        EndDialog(IDCANCEL);
        return FALSE;
    }

    m_crCurrent = m_crSelected = m_wndColorTable.GetColour(0);

    m_brCurrent  = CreateSolidBrush(m_crCurrent);
    m_brSelected = CreateSolidBrush(m_crSelected);

    m_wndRedS.LimitText(3);
    m_wndGreenS.LimitText(3);
    m_wndBlueS.LimitText(3);

    UpdateColorInfo();

    SetTimer(TIMER_SETFOCUS, 100, NULL);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorTableDlg::OnSelEndOK()
//
LONG
CColorTableDlg::OnSelEndOK(UINT lParam, LONG /*wParam*/)
{
    m_crSelected = (COLORREF)lParam;

    if (m_brSelected)
        DeleteObject(m_brSelected);

    m_brSelected = CreateSolidBrush(m_crSelected);

    UpdateColorInfo();
    m_wndSelColor.Invalidate();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorTableDlg::OnSelEndCancel()
//
LONG
CColorTableDlg::OnSelEndCancel(UINT lParam, LONG /*wParam*/)
{
    OnCancel();
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorTableDlg::OnSelChange()
//
LONG
CColorTableDlg::OnSelChange(UINT lParam, LONG /*wParam*/)
{
    m_crCurrent = (COLORREF)lParam;

    if (m_brCurrent)
        DeleteObject(m_brCurrent);

    m_brCurrent = CreateSolidBrush(m_crCurrent);

    UpdateColorInfo();
    m_wndCurColor.Invalidate();

    return TRUE;
}

void
CColorTableDlg::UpdateColorInfo()
{
    // Current color
    m_sRed   = uInt2Str(GetRValue(m_crCurrent));
    m_sGreen = uInt2Str(GetGValue(m_crCurrent));
    m_sBlue  = uInt2Str(GetBValue(m_crCurrent));

    int nIndex = m_wndColorTable.m_nCurrent;
    if (nIndex != -1)
        m_sIndex = uInt2Str(nIndex);
    else
        m_sIndex.Empty();

    // Selected color
    m_nRedS   = GetRValue(m_crSelected);
    m_nGreenS = GetGValue(m_crSelected);
    m_nBlueS  = GetBValue(m_crSelected);

    nIndex = m_wndColorTable.m_nSelected;
    if (nIndex != -1)
        m_sIndexS = uInt2Str(nIndex);
    else
        m_sIndexS.Empty();

    UpdateData(FALSE);
}

void
CColorTableDlg::OnDestroy()
{
    CDialogX::OnDestroy();

    if (m_brCurrent)
        DeleteObject(m_brCurrent);

    if (m_brSelected)
        DeleteObject(m_brSelected);

    SavePosition(this);
}

void
CColorTableDlg::OnSelectColor()
{
    COLORREF color = m_crSelected;
    if (ShowColorDialog(color, this)) {
        m_crSelected = color;
        ChangeSelectedRGB();
    }
}

void
CColorTableDlg::OnOK()
{
    // applay palette changes
    for (int i = 0; i < COLOR8; i++) {
        //		COLORREF color1 = RGB(GetRValue(m_wndColorTable.m_crColours[i].crColour), GetGValue(m_wndColorTable.m_crColours[i].crColour), GetBValue(m_wndColorTable.m_crColours[i].crColour));
        //		COLORREF color2 = RGB(m_pPal->pal[i].rgbRed, m_pPal->pal[i].rgbGreen, m_pPal->pal[i].rgbBlue);

        m_pPal->pal[i].rgbRed   = GetRValue(m_wndColorTable.m_crColours[i].crColour);
        m_pPal->pal[i].rgbGreen = GetGValue(m_wndColorTable.m_crColours[i].crColour);
        m_pPal->pal[i].rgbBlue  = GetBValue(m_wndColorTable.m_crColours[i].crColour);

        //		COLORREF color3 = RGB(m_pPal->pal[i].rgbRed, m_pPal->pal[i].rgbGreen, m_pPal->pal[i].rgbBlue);
    }

    CDialogX::OnOK();
}

void
CColorTableDlg::OnCancel()
{
    // Nothing changed...
    CDialogX::OnCancel();
}

void
CColorTableDlg::OnChangeColorRedS()
{
    CDataExchange dx(this, TRUE);

    CString sValue;
    m_wndRedS.GetWindowText(sValue);

    if (!sValue.IsEmpty()) {
        DDX_Text(&dx, IDC_COLOR_RED_S, m_nRedS);

        if (m_nRedS < 0) {
            m_nRedS               = 0;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_RED_S, m_nRedS);
        } else if (m_nRedS > 255) {
            m_nRedS               = 255;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_RED_S, m_nRedS);
        }
    } else {
        m_nRedS               = 0;
        dx.m_bSaveAndValidate = FALSE;
        DDX_Text(&dx, IDC_COLOR_RED_S, m_nRedS);
    }

    m_crSelected = RGB(m_nRedS, m_nGreenS, m_nBlueS);
    ChangeSelectedRGB();
}

void
CColorTableDlg::OnChangeColorGreenS()
{
    CDataExchange dx(this, TRUE);

    CString sValue;
    m_wndGreenS.GetWindowText(sValue);

    if (!sValue.IsEmpty()) {
        DDX_Text(&dx, IDC_COLOR_GREEN_S, m_nGreenS);

        if (m_nGreenS < 0) {
            m_nGreenS             = 0;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_GREEN_S, m_nGreenS);
        } else if (m_nGreenS > 255) {
            m_nGreenS             = 255;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_GREEN_S, m_nGreenS);
        }
    } else {
        m_nGreenS             = 0;
        dx.m_bSaveAndValidate = FALSE;
        DDX_Text(&dx, IDC_COLOR_GREEN_S, m_nGreenS);
    }

    m_crSelected = RGB(m_nRedS, m_nGreenS, m_nBlueS);
    ChangeSelectedRGB();
}

void
CColorTableDlg::OnChangeColorBlueS()
{
    CDataExchange dx(this, TRUE);

    CString sValue;
    m_wndBlueS.GetWindowText(sValue);

    if (!sValue.IsEmpty()) {
        DDX_Text(&dx, IDC_COLOR_BLUE_S, m_nBlueS);

        if (m_nBlueS < 0) {
            m_nBlueS              = 0;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_BLUE_S, m_nBlueS);
        } else if (m_nBlueS > 255) {
            m_nBlueS              = 255;
            dx.m_bSaveAndValidate = FALSE;
            DDX_Text(&dx, IDC_COLOR_BLUE_S, m_nBlueS);
        }
    } else {
        m_nBlueS              = 0;
        dx.m_bSaveAndValidate = FALSE;
        DDX_Text(&dx, IDC_COLOR_BLUE_S, m_nBlueS);
    }

    m_crSelected = RGB(m_nRedS, m_nGreenS, m_nBlueS);
    ChangeSelectedRGB();
}

//////////////////
// Got WM_CTLCOLOR: return handle of background brush to use.
//
HBRUSH
CColorTableDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC) {
        // Return handle to our CBrush object
        switch (pWnd->GetDlgCtrlID()) {
            case IDC_STATIC_CURRENT:
                return m_brCurrent;

            case IDC_STATIC_SELECTED:
                return m_brSelected;
        }
    }

    // Normal processing: don't do anything
    return CDialogX::OnCtlColor(pDC, pWnd, nCtlColor);
}

void
CColorTableDlg::ChangeSelectedRGB()
{
    m_wndColorTable.ChangeSelectedColor(m_crSelected);

    if (m_brSelected)
        DeleteObject(m_brSelected);

    m_brSelected = CreateSolidBrush(m_crSelected);

    UpdateColorInfo();
    m_wndSelColor.Invalidate();
}

BOOL
CColorTableDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
            case VK_RETURN:
            case VK_ESCAPE:
                ::TranslateMessage(pMsg);
                ::DispatchMessage(pMsg);
                return TRUE;
        }
    }

    return CDialogX::PreTranslateMessage(pMsg);
}

void
CColorTableDlg::OnTimer(UINT nIDEvent)
{
    if (nIDEvent == TIMER_SETFOCUS) {
        KillTimer(TIMER_SETFOCUS);
        m_wndColorTable.SetFocus();
        return;
    }

    CDialogX::OnTimer(nIDEvent);
}
