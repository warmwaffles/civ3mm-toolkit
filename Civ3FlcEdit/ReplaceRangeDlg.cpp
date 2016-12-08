/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ReplaceRangeDlg.cpp
	CREATED:	04.11.2004
	VERSION:	1.1

	AUTHOR:		Dreyk
	MODIFIED:	

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ReplaceRangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReplaceRangeDlg dialog

CReplaceRangeDlg::CReplaceRangeDlg(RGBQUADPAL* pPalD, RGBQUADPAL* pPalS, CWnd* pParent /*=NULL*/)
  : CDialog(CReplaceRangeDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CReplaceRangeDlg)
    m_nFrom   = 0;
    m_nTo     = 0;
    m_nNumber = 0;
    //}}AFX_DATA_INIT

    m_bAllPalette = FALSE;

    ASSERT(pPalD);
    m_pPalExtD = pPalD;

    ASSERT(pPalS);
    m_pPalExtS = pPalS;
}

void
CReplaceRangeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CReplaceRangeDlg)
    DDX_Control(pDX, IDC_REFRESH, m_btnRefresh);
    DDX_Control(pDX, IDC_APPLY, m_btnApply);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_MOVE, m_btnMove);
    DDX_Control(pDX, IDC_FIRST_SRC, m_wndTo);
    DDX_Control(pDX, IDC_FIRST_DEST, m_wndFrom);
    DDX_Control(pDX, IDC_RANGE, m_wndNumber);
    DDX_Text(pDX, IDC_FIRST_DEST, m_nFrom);
    DDX_Text(pDX, IDC_FIRST_SRC, m_nTo);
    DDX_Text(pDX, IDC_RANGE, m_nNumber);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReplaceRangeDlg, CDialog)
//{{AFX_MSG_MAP(CReplaceRangeDlg)
ON_WM_DESTROY()
ON_EN_CHANGE(IDC_RANGE, OnChangeRange)
ON_BN_CLICKED(IDC_MOVE, OnMove)
ON_EN_CHANGE(IDC_FIRST_SRC, OnChangeFirstSrc)
ON_EN_CHANGE(IDC_FIRST_DEST, OnChangeFirstDest)
ON_WM_PAINT()
ON_BN_CLICKED(IDC_APPLY, OnApply)
ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
//}}AFX_MSG_MAP
ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplaceRangeDlg message handlers

BOOL
CReplaceRangeDlg::OnInitDialog()
{
    if (!m_pPalExtD || !m_pPalExtS) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    // Inicialize a palettes
    memcpy(&m_rgbPalD, m_pPalExtD, RGBQUADPAL_SIZE);
    memcpy(&m_rgbPalS, m_pPalExtS, RGBQUADPAL_SIZE);

    CDialog::OnInitDialog();

    LoadPosition(this);

    m_wndFrom.LimitText(3);
    m_wndTo.LimitText(3);
    m_wndNumber.LimitText(3);

    m_btnOK.SetIcon(IDI_OK);
    m_btnCancel.SetIcon(IDI_CANCEL);
    m_btnApply.SetIcon(IDI_APPLY);
    m_btnRefresh.SetIcon(IDI_REFRESH);
    m_btnMove.SetIcon(IDI_COLOR_RANGE);

    uAppendCtrlDots(this, IDC_STATIC_1);
    uAppendCtrlDots(this, IDC_STATIC_2);
    uAppendCtrlDots(this, IDC_STATIC_3);

    CWnd* pWnd = NULL;
    CRect rect;

    pWnd = GetDlgItem(IDC_DEST_PALETTE_POS);
    pWnd->GetWindowRect(&rect);
    ScreenToClient(&rect);

    if (!m_wndColorTableD.Create(rect.TopLeft(), &m_rgbPalD, this, IDC_COLOR_TABLE)) {
        TRACE("Cannot create destination ColorTable window\r\n");
        EndDialog(IDCANCEL);
        return FALSE;
    }

    m_wndColorTableD.SetupVariables(m_nFrom, m_nTo, m_nNumber, m_bAllPalette, PrevRangeD);

    pWnd = GetDlgItem(IDC_SRC_PALETTE_POS);
    pWnd->GetWindowRect(&rect);
    ScreenToClient(&rect);

    if (!m_wndColorTableS.Create(rect.TopLeft(), &m_rgbPalS, this, IDC_COLOR_TABLE_S)) {
        TRACE("Cannot create destination ColorTable window\r\n");
        EndDialog(IDCANCEL);
        return FALSE;
    }

    m_wndColorTableS.SetupVariables(m_nFrom, m_nTo, m_nNumber, m_bAllPalette, PrevRangeS);

    UpdateData(FALSE);
    UpdateControlsState();
    return TRUE;
}

void
CReplaceRangeDlg::OnDestroy()
{
    SavePosition(this);
    CDialog::OnDestroy();
}

void
CReplaceRangeDlg::OnOK()
{
    OnApply();

    CDialog::OnOK();
}

void
CReplaceRangeDlg::OnCancel()
{
    CDialog::OnCancel();
}

void
CReplaceRangeDlg::OnChangeRange()
{
    UpdateData();

    UINT num = (IDX_MAX + 1) - max(m_nFrom, m_nTo);
    if (m_nNumber > num) {
        m_nNumber = num;
        UpdateData(FALSE);
    }

    ChangeVariables();
}

void
CReplaceRangeDlg::OnMove()
{
    if (m_nNumber == 0)
        return;

    UINT num = (IDX_MAX + 1) - max(m_nFrom, m_nTo);
    if (m_nNumber > num) {
        m_nNumber = num;
        UpdateData(FALSE);
    }

    try {
        memcpy(m_rgbPalD.pal + m_nTo, m_rgbPalS.pal + m_nFrom, sizeof(RGBQUAD) * m_nNumber);
    } catch (...) {
        ASSERT(FALSE);
        AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CReplaceRangeDlg::OnMove"), MB_ICONERROR);
        return;
    }

    m_wndColorTableD.SetPalette(&m_rgbPalD);
}

void
CReplaceRangeDlg::OnChangeFirstSrc()
{
    UpdateData();
    if (m_nTo > IDX_MAX) {
        m_nTo = IDX_MAX;
        UpdateData(FALSE);
    }

    ChangeVariables();
}

void
CReplaceRangeDlg::OnChangeFirstDest()
{
    UpdateData();
    if (m_nFrom > IDX_MAX) {
        m_nFrom = IDX_MAX;
        UpdateData(FALSE);
    }

    ChangeVariables();
}

void
CReplaceRangeDlg::UpdateControlsState()
{
    UpdateData();
}

void
CReplaceRangeDlg::ChangeVariables()
{
    UpdateData();

    m_wndColorTableD.Invalidate();
    m_wndColorTableS.Invalidate();
}

void
CReplaceRangeDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    if (m_wndNumber.GetSafeHwnd()) {
        CRect rc;

        m_wndFrom.GetWindowRect(rc);
        ScreenToClient(rc);
        rc.InflateRect(2, 2);
        dc.Draw3dRect(&rc, RGB(255, 0, 0), RGB(255, 0, 0));

        m_wndTo.GetWindowRect(rc);
        ScreenToClient(rc);
        rc.InflateRect(2, 2);
        dc.Draw3dRect(&rc, RGB(0, 0, 255), RGB(0, 0, 255));
    }
}

void
CReplaceRangeDlg::OnApply()
{
    // Apply palette changes
    memcpy(m_pPalExtD, &m_rgbPalD, RGBQUADPAL_SIZE);
}

void
CReplaceRangeDlg::OnRefresh()
{
    // Discard palette changes
    memcpy(&m_rgbPalD, m_pPalExtD, RGBQUADPAL_SIZE);
    m_wndColorTableD.SetPalette(&m_rgbPalD);
}

BOOL
CReplaceRangeDlg::PreTranslateMessage(MSG* pMsg)
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

    return CDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// Method:	CColorTableDlg::OnSelEndOK()
//
LONG
CReplaceRangeDlg::OnSelEndOK(UINT lParam, LONG wParam)
{
    if (wParam == IDC_COLOR_TABLE)
        m_nTo = m_wndColorTableD.m_nSelected;
    else
        m_nFrom = m_wndColorTableS.m_nSelected;

    UpdateData(FALSE);
    ChangeVariables();
    return TRUE;
}