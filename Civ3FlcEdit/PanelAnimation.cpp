/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	DlgBar.cpp
	CREATED:	16.09.2004
	VERSION:	1.1

	AUTHOR:		Dreyk

	DESCRIPTION: PanelAnimation.cpp : implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "PanelAnimation.h"

#include "DlgBar.h"
#include "MainFrm.h"
#include "AnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelAnimation dialog


CPanelAnimation::CPanelAnimation(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelAnimation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelAnimation)
	m_sCurFrame = _T("0");
	m_sTotalFrame = _T("0");
	m_bBack = FALSE;
	m_bForward = FALSE;
	m_sFirstFrame = _T("0");
	//}}AFX_DATA_INIT

	m_pView		= NULL;
	m_pParent	= (CDlgBar*)pParent;
}


void CPanelAnimation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelAnimation)
	DDX_Control(pDX, IDC_STATIC_TOTAL_FRAME, m_staticFrmTotal);
	DDX_Control(pDX, IDC_STATIC_FRAME, m_staticFrm);
	DDX_Control(pDX, IDC_STATIC_FIRST_FRAME, m_staticFrmFirst);
	DDX_Control(pDX, IDC_STATIC_CUR_FRAME, m_staticFrmCur);
	DDX_Control(pDX, IDC_FORWARD, m_btnForward);
	DDX_Control(pDX, IDC_BACK, m_btnBack);
	DDX_Control(pDX, IDC_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_PAUSE, m_btnPause);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
	DDX_Control(pDX, IDC_FIRST, m_btnFirst);
	DDX_Control(pDX, IDC_PREVIOUS, m_btnPrevious);
	DDX_Control(pDX, IDC_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_LAST, m_btnLast);
	DDX_Text(pDX, IDC_STATIC_CUR_FRAME, m_sCurFrame);
	DDX_Text(pDX, IDC_STATIC_TOTAL_FRAME, m_sTotalFrame);
	DDX_Check(pDX, IDC_BACK, m_bBack);
	DDX_Check(pDX, IDC_FORWARD, m_bForward);
	DDX_Text(pDX, IDC_STATIC_FIRST_FRAME, m_sFirstFrame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelAnimation, CDialog)
	//{{AFX_MSG_MAP(CPanelAnimation)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_FRAMES_SLIDER, OnReleasedcaptureFramesSlider)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_FIRST, OnFirst)
	ON_BN_CLICKED(IDC_PREVIOUS, OnPrevious)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_LAST, OnLast)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_FORWARD, OnForward)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelAnimation message handlers

BOOL CPanelAnimation::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWnd* pWnd = NULL;
	CRect rect;
	
	// Initially create frames slider control
	pWnd = GetDlgItem(IDC_FRAMES_SLIDER_POS);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	// Initialise controls
	if (!m_wndFrameSlider.Create(WS_VISIBLE | WS_CHILD | TBS_HORZ | TBS_BOTTOM/*TBS_BOTH | TBS_NOTICKS | TBS_AUTOTICKS*/ | TBS_TOOLTIPS,
		rect, this, IDC_FRAMES_SLIDER))
	{
		ASSERT(FALSE);
	}
	
	m_btnFirst.SetShowIconOnly();
	m_btnBack.SetShowIconOnly();
	m_btnPrevious.SetShowIconOnly();
	m_btnNext.SetShowIconOnly();
	m_btnForward.SetShowIconOnly();
	m_btnLast.SetShowIconOnly();

	m_btnPlay.SetIcon(IDI_PLAY);
	m_btnPause.SetIcon(IDI_PAUSE);
	m_btnStop.SetIcon(IDI_STOP);
	m_btnFirst.SetIcon(IDI_FIRST);
	m_btnBack.SetIcon(IDI_BACK);
	m_btnPrevious.SetIcon(IDI_PREVIOUS);
	m_btnNext.SetIcon(IDI_NEXT);
	m_btnForward.SetIcon(IDI_FORWARD);
	m_btnLast.SetIcon(IDI_LAST);

	m_wndFrameSlider.EnableWindow(FALSE);

	m_sCurFrame		= "0";
	m_sTotalFrame	= "0";
	m_sFirstFrame	= "0";
	
	m_staticFrm.EnableWindow(FALSE);
	m_staticFrmFirst.EnableWindow(FALSE);
	m_staticFrmCur.EnableWindow(FALSE);
	m_staticFrmTotal.EnableWindow(FALSE);

	SetView(NULL);
	return TRUE;
}

void CPanelAnimation::OnReleasedcaptureFramesSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (IsAttached())
		m_pView->ReleasedcaptureFramesSlider(m_wndFrameSlider.GetPos());
	*pResult = 0;
}

void CPanelAnimation::OnPlay() 
{
	if (IsAttached())
		m_pView->OnPlay();
}

void CPanelAnimation::OnPause() 
{
	if (IsAttached())
		m_pView->OnPause();
}

void CPanelAnimation::OnStop() 
{
	if (IsAttached())
		m_pView->OnStop();
}

void CPanelAnimation::OnBack() 
{
	if (IsAttached())
		m_pView->OnBack();
}

void CPanelAnimation::OnForward() 
{
	if (IsAttached())
		m_pView->OnForward();
}

void CPanelAnimation::OnFirst() 
{
	if (IsAttached())
		m_pView->OnFirst();
}

void CPanelAnimation::OnPrevious() 
{
	if (IsAttached())
		m_pView->OnPrevious();
}

void CPanelAnimation::OnNext() 
{
	if (IsAttached())
		m_pView->OnNext();
}

void CPanelAnimation::OnLast() 
{
	if (IsAttached())
		m_pView->OnLast();
}

BOOL CPanelAnimation::IsAttached()
{
	return (m_pView != NULL && m_pView->GetSafeHwnd());
}

void CPanelAnimation::SetView(CAnimationView *pView)
{
	m_pView = pView;
	
	if (!m_pView)
	{
/*		m_btnStop.EnableWindow(FALSE);
		m_btnPause.EnableWindow(FALSE);
		m_btnPause.SetCheck(0);
		m_btnPlay.EnableWindow(FALSE);
		
		m_btnFirst.EnableWindow(FALSE);
		m_btnBack.EnableWindow(FALSE);
		m_btnPrevious.EnableWindow(FALSE);
		m_btnNext.EnableWindow(FALSE);
		m_btnForward.EnableWindow(FALSE);
		m_btnLast.EnableWindow(FALSE);
*/
		//////////////////////////////////////////////////////////////////////////
		m_btnStop.EnableWindow(FALSE);
		m_btnPause.EnableWindow(FALSE);
		m_btnPause.SetCheck(0);
		m_btnPlay.EnableWindow(FALSE);
		
		m_btnFirst.EnableWindow(FALSE);
		m_btnBack.EnableWindow(FALSE);
		m_btnPrevious.EnableWindow(FALSE);
		m_btnNext.EnableWindow(FALSE);
		m_btnForward.EnableWindow(FALSE);
		m_btnLast.EnableWindow(FALSE);
		
		m_wndFrameSlider.EnableWindow(FALSE);
		
		m_staticFrm.EnableWindow(FALSE);
		m_staticFrmFirst.EnableWindow(FALSE);
		m_staticFrmCur.EnableWindow(FALSE);
		m_staticFrmTotal.EnableWindow(FALSE);

		m_sCurFrame		= "0";
		m_sTotalFrame	= "0";
		m_sFirstFrame	= "0";

		UpdateData(FALSE);
	}
}

void CPanelAnimation::UpdateControlsState()
{
	if (m_pView->IsLoaded())
	{
		BOOL bPlaying = m_pView->IsPlaying();
		m_btnStop.EnableWindow(bPlaying);
		m_btnPause.EnableWindow(bPlaying);
		m_btnPlay.EnableWindow(!bPlaying);

		m_btnFirst.EnableWindow(!bPlaying);
		m_btnBack.EnableWindow(!bPlaying);
		m_btnPrevious.EnableWindow(!bPlaying);
		m_btnNext.EnableWindow(!bPlaying);
		m_btnForward.EnableWindow(!bPlaying);
		m_btnLast.EnableWindow(!bPlaying);

		m_wndFrameSlider.EnableWindow(!bPlaying);

		m_staticFrm.EnableWindow(TRUE);
		m_staticFrmFirst.EnableWindow(TRUE);
		m_staticFrmCur.EnableWindow(TRUE);
		m_staticFrmTotal.EnableWindow(TRUE);
	}
	else
	{
		m_btnStop.EnableWindow(FALSE);
		m_btnPause.EnableWindow(FALSE);
		m_btnPause.SetCheck(0);
		m_btnPlay.EnableWindow(FALSE);
		
		m_btnFirst.EnableWindow(FALSE);
		m_btnBack.EnableWindow(FALSE);
		m_btnPrevious.EnableWindow(FALSE);
		m_btnNext.EnableWindow(FALSE);
		m_btnForward.EnableWindow(FALSE);
		m_btnLast.EnableWindow(FALSE);
		
		m_wndFrameSlider.EnableWindow(FALSE);
		
		m_staticFrm.EnableWindow(FALSE);
		m_staticFrmFirst.EnableWindow(FALSE);
		m_staticFrmCur.EnableWindow(FALSE);
		m_staticFrmTotal.EnableWindow(FALSE);
	}
}

BOOL CPanelAnimation::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
    {
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}
