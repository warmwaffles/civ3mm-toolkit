/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	DlgBar.cpp
	CREATED:	16.09.2004
	VERSION:	5.0

	AUTHOR:		Dreyk

	DESCRIPTION: PanelDelay.cpp : implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "PanelDelay.h"

#include "DlgBar.h"
#include "MainFrm.h"
#include "AnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelDelay dialog

CPanelDelay::CPanelDelay(CWnd* pParent /*=NULL*/)
  : CDialog(CPanelDelay::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPanelDelay)
    m_nTimerValue = 0;
    m_sDefTimer   = _T("");
    m_sDelay      = _T("");
    //}}AFX_DATA_INIT

    m_pView   = NULL;
    m_pParent = (CDlgBar*)pParent;
}

void
CPanelDelay::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPanelDelay)
    DDX_Control(pDX, IDC_SPIN_DELAY, m_wndDelaySpin);
    DDX_Control(pDX, IDC_DELAY, m_wndDelay);
    DDX_Radio(pDX, IDC_RADIO_TIMER_DEF, m_nTimerValue);
    DDX_Control(pDX, IDC_STATIC_DEF_TIMER, m_wndDefTimer);
    DDX_Text(pDX, IDC_STATIC_DEF_TIMER, m_sDefTimer);
    DDX_Text(pDX, IDC_DELAY, m_sDelay);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPanelDelay, CDialog)
//{{AFX_MSG_MAP(CPanelDelay)
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TIMER_SLIDER, OnReleasedcaptureTimerSlider)
ON_BN_CLICKED(IDC_RADIO_TIMER_DEF, OnRadioTimerDef)
ON_BN_CLICKED(IDC_RADIO_TIMER_CUSTOM, OnRadioTimerCustom)
ON_EN_CHANGE(IDC_DELAY, OnChangeDelay)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELAY, OnDeltaposSpinDelay)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelDelay message handlers

BOOL
CPanelDelay::OnInitDialog()
{
    CDialog::OnInitDialog();

    CWnd* pWnd = NULL;
    CRect rect;

    // Initially create timer slider control
    pWnd = GetDlgItem(IDC_TIMER_SLIDER_POS);
    pWnd->GetWindowRect(&rect);
    ScreenToClient(&rect);

    // Initialise controls
    if (!m_wndTimerSlider.Create(
          WS_VISIBLE | WS_CHILD | TBS_HORZ | TBS_BOTTOM | TBS_AUTOTICKS | TBS_TOOLTIPS, rect, this, IDC_TIMER_SLIDER)) {
        ASSERT(FALSE);
    }

    m_wndTimerSlider.SetRangeMin(FRM_DELAY_MIN);
    m_wndTimerSlider.SetRangeMax(FRM_DELAY_MAX);
    m_wndTimerSlider.SetTicFreq(100);
    m_wndDelay.SetLimitText(4);
    m_wndDelaySpin.SetRange(FRM_DELAY_MIN, FRM_DELAY_MAX);

    SetView(NULL);
    return TRUE;
}

void
CPanelDelay::SetView(CAnimationView* pView)
{
    m_pView = pView;

    if (!m_pView) {
        m_wndDefTimer.EnableWindow(FALSE);
        m_wndTimerSlider.EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_TIMER_DEF)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_TIMER_CUSTOM)->EnableWindow(FALSE);
        m_wndDelay.EnableWindow(FALSE);
        m_wndDelaySpin.EnableWindow(FALSE);
        m_sDefTimer.Empty();
        m_sDelay.Empty();
        UpdateData(FALSE);
    }
}

void
CPanelDelay::OnReleasedcaptureTimerSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (IsAttached()) {
        m_pView->SetTimerValue(m_wndTimerSlider.GetPos());

        m_sDelay = uInt2Str(m_pView->m_uElapse);
        CDataExchange dx(this, FALSE);
        DDX_Text(&dx, IDC_DELAY, m_sDelay);
    }

    *pResult = 0;
}

void
CPanelDelay::OnRadioTimerDef()
{
    if (!IsAttached())
        return;

    UpdateData();
    m_pView->OnRadioTimerDef();
}

void
CPanelDelay::OnRadioTimerCustom()
{
    if (!IsAttached())
        return;

    UpdateData();
    m_pView->OnRadioTimerCustom();
}

BOOL
CPanelDelay::IsAttached()
{
    return (m_pView != NULL && m_pView->GetSafeHwnd());
}

void
CPanelDelay::UpdateControlsState()
{
    if (m_pView->IsLoaded()) {
        BOOL bEnable = !m_pView->IsDefaultTimer();
        m_wndTimerSlider.EnableWindow(bEnable);
        m_wndDelay.EnableWindow(bEnable);
        m_wndDelaySpin.EnableWindow(bEnable);

        bEnable = m_pView->IsPlaying();
        m_wndDefTimer.EnableWindow(bEnable);
        GetDlgItem(IDC_RADIO_TIMER_DEF)->EnableWindow(bEnable);
        GetDlgItem(IDC_RADIO_TIMER_CUSTOM)->EnableWindow(bEnable);

        m_wndTimerSlider.SetPos(m_pView->m_uElapse);
        m_sDelay    = uInt2Str(m_pView->m_uElapse);
        m_sDefTimer = uFormatString(IDS_DEF_TIMER, m_pView->m_uSpeed);
        UpdateData(FALSE);
    } else {
        m_wndTimerSlider.EnableWindow(FALSE);
        m_wndDelay.EnableWindow(FALSE);
        m_wndDelaySpin.EnableWindow(FALSE);

        GetDlgItem(IDC_RADIO_TIMER_DEF)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_TIMER_CUSTOM)->EnableWindow(FALSE);
        m_wndDefTimer.EnableWindow(FALSE);

        m_sDefTimer.Empty();
        m_sDelay.Empty();
        UpdateData(FALSE);
    }
}

void
CPanelDelay::OnChangeDelay()
{
    if (m_wndDelay.GetSafeHwnd() && IsAttached()) {
        UpdateData();

        m_sDelay.Remove(' ');
        m_nDelay = atoi(m_sDelay);
        if (m_nDelay < FRM_DELAY_MIN) {
            m_nDelay = FRM_DELAY_MIN;
            m_sDelay = uInt2Str(m_nDelay);
        } else if (m_nDelay > FRM_DELAY_MAX) {
            m_nDelay = FRM_DELAY_MAX;
            m_sDelay = uInt2Str(m_nDelay);
        }

        CDataExchange dx(this, FALSE);
        DDX_Text(&dx, IDC_DELAY, m_sDelay);

        m_pView->SetTimerValue(m_nDelay);
        m_wndTimerSlider.SetPos(m_nDelay);
    }
}

void
CPanelDelay::OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeDelay();
    *pResult = 0;
}

BOOL
CPanelDelay::PreTranslateMessage(MSG* pMsg)
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
