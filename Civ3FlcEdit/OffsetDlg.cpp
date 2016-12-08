/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	OffsetDlg.cpp
	CREATED:	26.10.2004
	VERSION:	1.3

	AUTHOR:		Dreyk
	MODIFIED:	

	DESCRIPTION: implementation of the COffsetDlg class.
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "OffsetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COffsetDlg dialog

COffsetDlg::COffsetDlg(CAnimationView* pAnim, CWnd* pParent /*=NULL*/)
  : CDialog(COffsetDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(COffsetDlg)
    m_nXOffset      = 0;
    m_nYOffset      = 0;
    m_sFrmOffsetCur = _T("");
    //}}AFX_DATA_INIT

    ASSERT(pAnim);
    m_pAnim    = pAnim;
    m_bChanged = TRUE;
}

void
COffsetDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COffsetDlg)
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDC_FRM_TOP_SPIN, m_wndYOffsetSpin);
    DDX_Control(pDX, IDC_FRM_TOP, m_wndYOffset);
    DDX_Control(pDX, IDC_FRM_LEFT_SPIN, m_wndXOffsetSpin);
    DDX_Control(pDX, IDC_FRM_LEFT, m_wndXOffset);
    DDX_Text(pDX, IDC_FRM_LEFT, m_nXOffset);
    DDX_Text(pDX, IDC_FRM_TOP, m_nYOffset);
    DDX_Text(pDX, IDC_STATIC_FRM_OFFSET, m_sFrmOffsetCur);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COffsetDlg, CDialog)
//{{AFX_MSG_MAP(COffsetDlg)
ON_WM_DESTROY()
ON_EN_CHANGE(IDC_FRM_TOP, OnChangeFrmTop)
ON_EN_CHANGE(IDC_FRM_LEFT, OnChangeFrmLeft)
ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_TOP_SPIN, OnDeltaposFrmTopSpin)
ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_LEFT_SPIN, OnDeltaposFrmLeftSpin)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COffsetDlg message handlers

BOOL
COffsetDlg::OnInitDialog()
{
    if ((!m_pAnim || !m_pAnim->IsLoaded()) || m_pAnim->IsLeaderHead()) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    CDialog::OnInitDialog();

    LoadPosition(this);

    SetIcon(AfxGetApp()->LoadIcon(IDI_OFFSET), FALSE);
    m_btnOK.SetIcon(IDI_OK);
    m_btnCancel.SetIcon(IDI_CANCEL);

    m_wndYOffsetSpin.SetRange(0, WND_H - 1);
    m_wndXOffsetSpin.SetRange(0, WND_W - 1);
    m_wndXOffset.LimitText(3);
    m_wndYOffset.LimitText(3);

    m_nXOffset = m_nXOffsetCur = (BYTE)m_pAnim->m_xPos;
    m_nYOffset = m_nYOffsetCur = (BYTE)m_pAnim->m_yPos;
    m_nFrmHeight               = (BYTE)m_pAnim->m_nFrmHeight;
    m_nFrmWidth                = (BYTE)m_pAnim->m_nFrmWidth;
    m_sFrmOffsetCur            = uFormatString(IDS_CUR_OFFSET, m_nXOffset, m_nYOffset);

    UpdateData(FALSE);
    UpdateControlsState();
    return TRUE;
}

void
COffsetDlg::OnDestroy()
{
    SavePosition(this);
    CDialog::OnDestroy();
}

void
COffsetDlg::OnOK()
{
    UpdateData();

    m_pAnim->m_xPos = (BYTE)m_nXOffset;
    m_pAnim->m_yPos = (BYTE)m_nYOffset;

    CDialog::OnOK();
}

void
COffsetDlg::OnChangeFrmTop()
{
    if (m_wndYOffset.GetSafeHwnd()) {
        m_bChanged = TRUE;
        UpdateData();

        UINT nFrmHeight = WND_H - m_nFrmHeight;
        if (m_nYOffset > nFrmHeight) {
            m_nYOffset = nFrmHeight;
            UpdateData(FALSE);
        }

        UpdateControlsState();
    }
}

void
COffsetDlg::OnDeltaposFrmTopSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeFrmTop();
    *pResult = 0;
}

void
COffsetDlg::OnChangeFrmLeft()
{
    if (m_wndXOffset.GetSafeHwnd()) {
        m_bChanged = TRUE;
        UpdateData();

        UINT nFrmWidth = WND_W - m_nFrmWidth;
        if (m_nXOffset > nFrmWidth) {
            m_nXOffset = nFrmWidth;
            UpdateData(FALSE);
        }

        UpdateControlsState();
    }
}

void
COffsetDlg::OnDeltaposFrmLeftSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeFrmLeft();
    *pResult = 0;
}

void
COffsetDlg::UpdateControlsState()
{
    UpdateData();

    m_btnOK.EnableWindow(m_bChanged);

    m_pAnim->m_xPos        = m_nXOffset;
    m_pAnim->m_yPos        = m_nYOffset;
    m_pAnim->m_bPlayStatic = FALSE;
}

void
COffsetDlg::OnCancel()
{
    m_pAnim->m_xPos = m_nXOffsetCur;
    m_pAnim->m_yPos = m_nYOffsetCur;

    CDialog::OnCancel();
}
