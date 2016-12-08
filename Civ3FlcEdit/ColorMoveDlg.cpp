/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	ColorMoveDlg.cpp
	CREATED:	04.11.2004
	VERSION:	1.2

	AUTHOR:		Dreyk
	MODIFIED:	

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "ColorMoveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REG_ALL_PALETTE "MoveAll"

/////////////////////////////////////////////////////////////////////////////
// CColorMoveDlg dialog

CColorMoveDlg::CColorMoveDlg(RGBQUADPAL* pPal, CWnd* pParent /*=NULL*/)
  : CDialog(CColorMoveDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CColorMoveDlg)
    m_bAllPalette = FALSE;
    m_nFrom       = 0;
    m_nTo         = 0;
    m_nNumber     = 0;
    //}}AFX_DATA_INIT

    ASSERT(pPal);
    m_pPalExtD = pPal;
}

void
CColorMoveDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CColorMoveDlg)
    DDX_Control(pDX, IDC_REFRESH, m_btnRefresh);
    DDX_Control(pDX, IDC_APPLY, m_btnApply);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_MOVE_RIGHT, m_btnRight);
    DDX_Control(pDX, IDC_MOVE_LEFT, m_btnLeft);
    DDX_Control(pDX, IDC_MOVE, m_btnMove);
    DDX_Control(pDX, IDC_FIRST_SRC, m_wndTo);
    DDX_Control(pDX, IDC_FIRST_DEST, m_wndFrom);
    DDX_Control(pDX, IDC_RANGE, m_wndNumber);
    DDX_Check(pDX, IDC_ALL_PALETTE, m_bAllPalette);
    DDX_Text(pDX, IDC_FIRST_DEST, m_nFrom);
    DDX_Text(pDX, IDC_FIRST_SRC, m_nTo);
    DDX_Text(pDX, IDC_RANGE, m_nNumber);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CColorMoveDlg, CDialog)
//{{AFX_MSG_MAP(CColorMoveDlg)
ON_WM_DESTROY()
ON_EN_CHANGE(IDC_RANGE, OnChangeRange)
ON_BN_CLICKED(IDC_MOVE_RIGHT, OnMoveRight)
ON_BN_CLICKED(IDC_MOVE_LEFT, OnMoveLeft)
ON_BN_CLICKED(IDC_MOVE, OnMove)
ON_EN_CHANGE(IDC_FIRST_SRC, OnChangeFirstSrc)
ON_EN_CHANGE(IDC_FIRST_DEST, OnChangeFirstDest)
ON_BN_CLICKED(IDC_ALL_PALETTE, OnAllPalette)
ON_WM_PAINT()
ON_BN_CLICKED(IDC_APPLY, OnApply)
ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorMoveDlg message handlers

BOOL
CColorMoveDlg::OnInitDialog()
{
    if (!m_pPalExtD) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    // Inicialize a palette
    memcpy(&m_rgbPalD, m_pPalExtD, RGBQUADPAL_SIZE);

    CDialog::OnInitDialog();

    LoadPosition(this);

    m_wndFrom.LimitText(3);
    m_wndTo.LimitText(3);
    m_wndNumber.LimitText(3);

    m_btnOK.SetIcon(IDI_OK);
    m_btnCancel.SetIcon(IDI_CANCEL);
    m_btnApply.SetIcon(IDI_APPLY);
    m_btnRefresh.SetIcon(IDI_REFRESH);
    m_btnRight.SetIcon(IDI_MOVE_RIGHT);
    m_btnLeft.SetIcon(IDI_MOVE_LEFT);
    m_btnMove.SetIcon(IDI_MOVE_COLOR);

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

    m_bAllPalette = AfxGetApp()->GetProfileInt(RegKey(), REG_ALL_PALETTE, 0);
    if (m_bAllPalette) {
        m_nFrom = 0;
        m_nTo   = IDX_MAX;
    }

    m_wndColorTableD.SetupVariables(m_nFrom, m_nTo, m_nNumber, m_bAllPalette, PrevMove);

    UpdateData(FALSE);
    UpdateControlsState();
    return TRUE;
}

void
CColorMoveDlg::OnDestroy()
{
    AfxGetApp()->WriteProfileInt(RegKey(), REG_ALL_PALETTE, m_bAllPalette);

    SavePosition(this);
    CDialog::OnDestroy();
}

void
CColorMoveDlg::OnOK()
{
    OnApply();

    CDialog::OnOK();
}

void
CColorMoveDlg::OnCancel()
{
    CDialog::OnCancel();
}

void
CColorMoveDlg::OnChangeRange()
{
    UpdateData();

    if (m_bAllPalette) {
        if (m_nNumber > IDX_MAX) {
            m_nNumber = IDX_MAX;
            UpdateData(FALSE);
        }
    } else {
        UINT num = IDX_MAX - max(m_nFrom, m_nTo);
        if (m_nNumber > num && m_nNumber != 1) {
            m_nNumber = num;
            UpdateData(FALSE);
        }
    }

    ChangeVariables();
}

void
CColorMoveDlg::OnMoveRight()
{
    MoveTo(1, 0, m_nNumber);
    /*
	memcpy(&m_rgbPalTemp, &m_rgbPalD, RGBQUADPAL_SIZE);

	memcpy(m_rgbPalD.pal + 1, m_rgbPalTemp.pal, SIZE_255);

	m_rgbPalD.pal[0].rgbRed		= 255;
	m_rgbPalD.pal[0].rgbGreen	= 0;
	m_rgbPalD.pal[0].rgbBlue		= 0;

	m_wndColorTableD.SetPalette(&m_rgbPalD);
*/
}

void
CColorMoveDlg::OnMoveLeft()
{
    MoveTo(0, 1, m_nNumber);
    /*	memcpy(&m_rgbPalTemp, &m_rgbPalD, RGBQUADPAL_SIZE);
	
	memcpy(m_rgbPalD.pal, m_rgbPalTemp.pal + 1, SIZE_255);
	
	m_rgbPalD.pal[255].rgbRed	= 255;
	m_rgbPalD.pal[255].rgbGreen	= 0;
	m_rgbPalD.pal[255].rgbBlue	= 0;
	
	m_wndColorTableD.SetPalette(&m_rgbPalD);
*/
}

void
CColorMoveDlg::OnMove()
{
    MoveTo(m_nFrom, m_nTo, m_nNumber);
}

void
CColorMoveDlg::OnChangeFirstSrc()
{
    UpdateData();
    if (m_nTo > IDX_MAX) {
        m_nTo = IDX_MAX;
        UpdateData(FALSE);
    }

    ChangeVariables();
}

void
CColorMoveDlg::OnChangeFirstDest()
{
    UpdateData();
    if (m_nFrom > IDX_MAX) {
        m_nFrom = IDX_MAX;
        UpdateData(FALSE);
    }

    ChangeVariables();
}

void
CColorMoveDlg::OnAllPalette()
{
    UpdateControlsState();

    m_nFrom = 0;
    m_nTo   = IDX_MAX;

    UpdateData(FALSE);

    ChangeVariables();
}

void
CColorMoveDlg::UpdateControlsState()
{
    UpdateData();

    m_btnLeft.EnableWindow(m_bAllPalette);
    m_btnRight.EnableWindow(m_bAllPalette);

    m_btnMove.EnableWindow(!m_bAllPalette);
    m_wndTo.EnableWindow(!m_bAllPalette);
    m_wndFrom.EnableWindow(!m_bAllPalette);
}

void
CColorMoveDlg::ChangeVariables()
{
    UpdateData();

    m_wndColorTableD.Invalidate();
}

void
CColorMoveDlg::OnPaint()
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
CColorMoveDlg::MoveTo(UINT nFrom, UINT nTo, UINT nNumber)
{
    if (nNumber == 0)
        return;

    try {
        memcpy(&m_rgbPalTemp, &m_rgbPalD, RGBQUADPAL_SIZE);

        RGBQUAD pal;
        pal.rgbRed   = 255;
        pal.rgbGreen = 0;
        pal.rgbBlue  = 0;

        if (m_bAllPalette) {
            // Move Right
            if (nFrom == 1) {
                memcpy(m_rgbPalD.pal + nNumber, m_rgbPalTemp.pal, RGBQUADPAL_SIZE - nNumber * sizeof(RGBQUAD));

                for (UINT i          = 0; i < nNumber; i++)
                    m_rgbPalD.pal[i] = pal;
            } else // Move Left
            {
                memcpy(m_rgbPalD.pal, m_rgbPalTemp.pal + nNumber, RGBQUADPAL_SIZE - nNumber * sizeof(RGBQUAD));

                for (UINT i          = COLOR8 - nNumber; i < COLOR8; i++)
                    m_rgbPalD.pal[i] = pal;
            }
        } //if (m_bAllPalette)
        else {
            if (nFrom == nTo || nNumber == 0)
                return;

            // Move Right
            if (nFrom < nTo) {
                memcpy(m_rgbPalD.pal + nTo, m_rgbPalTemp.pal + nFrom, sizeof(RGBQUAD) * nNumber);

                for (UINT i          = nFrom; i < nFrom + nNumber; i++)
                    m_rgbPalD.pal[i] = pal;
            } else // Move Left
            {
                memcpy(m_rgbPalD.pal + nTo, m_rgbPalTemp.pal + nFrom, sizeof(RGBQUAD) * nNumber);

                for (UINT i          = nFrom; i < nFrom + nNumber; i++)
                    m_rgbPalD.pal[i] = pal;
            }
        }
    }
    /*	catch (CMemoryException* e)
	{
		TCHAR szCause[1024];
		e->GetErrorMessage(szCause, 1024);
		AfxMessageBox(szCause, MB_ICONERROR);
		e->Delete();
		return;
	}
*/ catch (...) {
        ASSERT(FALSE);
        AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CColorMoveDlg::MoveTo"), MB_ICONERROR);
        return;
    }

    m_wndColorTableD.SetPalette(&m_rgbPalD);
}

void
CColorMoveDlg::OnApply()
{
    // Apply palette changes
    memcpy(m_pPalExtD, &m_rgbPalD, RGBQUADPAL_SIZE);
}

void
CColorMoveDlg::OnRefresh()
{
    // Discard palette changes
    memcpy(&m_rgbPalD, m_pPalExtD, RGBQUADPAL_SIZE);
    m_wndColorTableD.SetPalette(&m_rgbPalD);
}

BOOL
CColorMoveDlg::PreTranslateMessage(MSG* pMsg)
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
