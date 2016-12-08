/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	DlgBar.cpp
	CREATED:	16.09.2004
	VERSION:	5.0

	AUTHOR:		Dreyk

	DESCRIPTION: PanelSound.cpp : implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "PanelSound.h"

#include "DlgBar.h"
#include "MainFrm.h"
#include "AnimationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelSound dialog

CPanelSound::CPanelSound(CWnd* pParent /*=NULL*/)
  : CDialog(CPanelSound::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPanelSound)
    m_bNonStopWAV = TRUE;
    m_bPreviewWAV = TRUE;
    m_sPathWAV    = _T("");
    //}}AFX_DATA_INIT

    m_pView   = NULL;
    m_pParent = (CDlgBar*)pParent;
}

void
CPanelSound::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPanelSound)
    DDX_Control(pDX, IDC_PREVIEW_WAV, m_btnPreviewWAV);
    DDX_Control(pDX, IDC_NON_STOP, m_btnNonStopWAV);
    DDX_Control(pDX, IDC_STOP_WAV, m_btnStopWAV);
    DDX_Control(pDX, IDC_DEL_PATH, m_btnDelPath);
    DDX_Control(pDX, IDC_BROWSE_PATH, m_btnBrowsePath);
    DDX_Control(pDX, IDC_PLAY_WAV, m_btnPlayWAV);
    DDX_Control(pDX, IDC_EDIT_PATH, m_wndEditPath);
    DDX_Check(pDX, IDC_NON_STOP, m_bNonStopWAV);
    DDX_Check(pDX, IDC_PREVIEW_WAV, m_bPreviewWAV);
    DDX_Text(pDX, IDC_EDIT_PATH, m_sPathWAV);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPanelSound, CDialog)
//{{AFX_MSG_MAP(CPanelSound)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BROWSE_PATH, OnBrowsePath)
ON_BN_CLICKED(IDC_DEL_PATH, OnDelPath)
ON_BN_CLICKED(IDC_NON_STOP, OnNonStop)
ON_BN_CLICKED(IDC_PLAY_WAV, OnPlayWav)
ON_BN_CLICKED(IDC_PREVIEW_WAV, OnPreviewWav)
ON_BN_CLICKED(IDC_STOP_WAV, OnStopWav)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelSound message handlers

BOOL
CPanelSound::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_btnBrowsePath.SetIcon(IDI_BROWSE);
    m_btnDelPath.SetIcon(IDI_DELETE);
    m_btnPlayWAV.SetIcon(IDI_PLAY_WAV);
    m_btnStopWAV.SetIcon(IDI_STOP_WAV);

    CWinApp* pApp = AfxGetApp();
    m_bNonStopWAV = pApp->GetProfileInt(REG_DLG_BAR, REG_WAV_NON_STOP, 1);
    m_bPreviewWAV = pApp->GetProfileInt(REG_DLG_BAR, REG_PREVIEW, 1);

    SetView(NULL);
    return TRUE;
}

void
CPanelSound::SetView(CAnimationView* pView)
{
    m_pView = pView;

    if (!m_pView) {
        m_btnStopWAV.EnableWindow(FALSE);
        m_btnDelPath.EnableWindow(FALSE);
        m_btnBrowsePath.EnableWindow(FALSE);
        m_btnPlayWAV.EnableWindow(FALSE);
        m_wndEditPath.EnableWindow(FALSE);
        m_btnPreviewWAV.EnableWindow(FALSE);
        m_btnNonStopWAV.EnableWindow(FALSE);

        UpdateData(FALSE);
    }
}

void
CPanelSound::OnBrowsePath()
{
    if (!IsAttached())
        return;

    m_pView->SelectWavPath();
}

void
CPanelSound::OnDelPath()
{
    if (!IsAttached())
        return;

    m_pView->SetPathWAV("");
    m_sPathWAV = "";
    UpdateData(FALSE);
    m_pView->StopSoundFile();
    m_pView->UpdateControlsState();
}

void
CPanelSound::OnNonStop()
{
    if (!IsAttached())
        return;

    UpdateData();

    if (m_bNonStopWAV) {
        if (m_pView->IsPlaying())
            m_pView->PlaySoundFile();
    } else
        m_pView->StopSoundFile();
}

void
CPanelSound::OnPreviewWav()
{
    if (!IsAttached())
        return;

    UpdateData();
    if (m_bPreviewWAV) {
        if (m_bNonStopWAV && m_pView->IsPlaying())
            m_pView->PlaySoundFile();
    } else
        m_pView->StopSoundFile();
}

void
CPanelSound::OnPlayWav()
{
    if (!IsAttached())
        return;

    m_pView->PlaySoundFile();
}

void
CPanelSound::OnStopWav()
{
    if (!IsAttached())
        return;

    m_pView->StopSoundFile();
}

BOOL
CPanelSound::IsAttached()
{
    return (m_pView != NULL && m_pView->GetSafeHwnd());
}

void
CPanelSound::OnDestroy()
{
    CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt(REG_DLG_BAR, REG_WAV_NON_STOP, m_bNonStopWAV);
    pApp->WriteProfileInt(REG_DLG_BAR, REG_PREVIEW, m_bPreviewWAV);

    CDialog::OnDestroy();
}

void
CPanelSound::UpdateControlsState()
{
    if (m_pView->IsLoaded() && !m_pView->IsLeaderHead()) {
        m_wndEditPath.EnableWindow(TRUE);
        m_btnPreviewWAV.EnableWindow(TRUE);
        m_btnNonStopWAV.EnableWindow(TRUE);
        m_btnBrowsePath.EnableWindow(TRUE);

        BOOL bEnable = !m_sPathWAV.IsEmpty();
        m_btnPlayWAV.EnableWindow(bEnable);
        m_btnStopWAV.EnableWindow(bEnable);
        m_btnDelPath.EnableWindow(bEnable);
    } else {
        m_btnStopWAV.EnableWindow(FALSE);
        m_btnDelPath.EnableWindow(FALSE);
        m_btnBrowsePath.EnableWindow(FALSE);
        m_btnPlayWAV.EnableWindow(FALSE);
        m_wndEditPath.EnableWindow(FALSE);
        m_btnPreviewWAV.EnableWindow(FALSE);
        m_btnNonStopWAV.EnableWindow(FALSE);
    }
}

BOOL
CPanelSound::PreTranslateMessage(MSG* pMsg)
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
