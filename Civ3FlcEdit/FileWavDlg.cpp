/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	FileWavDlg.cpp
	CREATED:	11.3.2003
	VERSION:	2.0

	AUTHOR:		Dreyk

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FileWavDlg.h"

#include <Mmsystem.h>
#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileWavDlg dialog

IMPLEMENT_DYNAMIC(CFileWavDlg, CFileDialogX)

CFileWavDlg::CFileWavDlg(BOOL bOpenFileDialog,
                         LPCTSTR lpszDefExt,
                         LPCTSTR lpszFileName,
                         DWORD dwFlags,
                         LPCTSTR lpszFilter,
                         CWnd* pParentWnd)
  : CFileDialogX(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
    CWinApp* pApp = AfxGetApp();
    //{{AFX_DATA_INIT(CFileWavDlg)
    m_bPreview = pApp->GetProfileInt(REG_WAV_DLG, REG_PREVIEW, 1);
    m_bNonStop = pApp->GetProfileInt(REG_WAV_DLG, REG_WAV_NON_STOP, 0);
    //}}AFX_DATA_INIT
}

void
CFileWavDlg::DoDataExchange(CDataExchange* pDX)
{
    CFileDialogX::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFileWavDlg)
    DDX_Control(pDX, IDC_PLAY, m_btnPlay);
    DDX_Control(pDX, IDC_STOP, m_btnStop);
    DDX_Control(pDX, IDC_NON_STOP, m_btnNonStop);
    DDX_Control(pDX, IDC_PREVIEW, m_btnPreview);
    DDX_Check(pDX, IDC_NON_STOP, m_bNonStop);
    DDX_Check(pDX, IDC_PREVIEW, m_bPreview);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileWavDlg, CFileDialogX)
//{{AFX_MSG_MAP(CFileWavDlg)
ON_BN_CLICKED(IDC_PLAY, OnPlay)
ON_BN_CLICKED(IDC_STOP, OnStop)
ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
ON_BN_CLICKED(IDC_NON_STOP, OnNonStop)
ON_WM_SIZE()
ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL
CFileWavDlg::OnInitDialog()
{
    CFileDialogX::OnInitDialog();

    m_btnPlay.EnableWindow(FALSE);
    m_btnStop.EnableWindow(FALSE);

    m_btnPlay.SetIcon(IDI_PLAY_WAV);
    m_btnStop.SetIcon(IDI_STOP_WAV);

    return TRUE;
}

void
CFileWavDlg::OnFileNameChange()
{
    CFileDialogX::OnFileNameChange();

    CString sPath = GetPathName();

    StopSoundFile();

    if (!sPath.IsEmpty() && uIsFileExist(sPath)) {
        if (uGetFileExt(sPath).CompareNoCase("WAV") != 0) {
            m_btnPlay.EnableWindow(FALSE);
            m_btnStop.EnableWindow(FALSE);

            m_sFile.Empty();
        } else {
            m_btnPlay.EnableWindow(TRUE);
            m_btnStop.EnableWindow(TRUE);

            m_sFile = sPath;

            if (m_bPreview)
                PlaySoundFile();
        }
    } else {
        m_btnPlay.EnableWindow(FALSE);
        m_btnStop.EnableWindow(FALSE);
    }
}

void
CFileWavDlg::OnFolderChange()
{
    CFileDialogX::OnFolderChange();

    m_btnPlay.EnableWindow(FALSE);
    m_btnStop.EnableWindow(FALSE);

    StopSoundFile();
}

void
CFileWavDlg::OnPreview()
{
    m_bPreview = !m_bPreview;

    if (m_bPreview)
        PlaySoundFile();
    else
        StopSoundFile();

    m_btnPlay.EnableWindow(m_bPreview);
    m_btnStop.EnableWindow(m_bPreview);
}

void
CFileWavDlg::OnNonStop()
{
    m_bNonStop = !m_bNonStop;

    if (m_bPreview)
        PlaySoundFile();
    else
        StopSoundFile();
}

void
CFileWavDlg::OnSize(UINT nType, int cx, int cy)
{
    CFileDialogX::OnSize(nType, cx, cy);

    Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CFileWavDlg message handlers

void
CFileWavDlg::PlaySoundFile()
{
    sndPlaySound(NULL, SND_SYNC);

    if (m_sFile.IsEmpty())
        return;

    UINT uFlags = SND_ASYNC;

    if (m_bNonStop)
        uFlags = uFlags | SND_LOOP | SND_NOSTOP;

    sndPlaySound(m_sFile, uFlags);
}

void
CFileWavDlg::StopSoundFile()
{
    sndPlaySound(NULL, SND_SYNC);
}

void
CFileWavDlg::OnStop()
{
    StopSoundFile();
}

void
CFileWavDlg::OnPlay()
{
    PlaySoundFile();
}

void
CFileWavDlg::OnDestroy()
{
    CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt(REG_WAV_DLG, REG_PREVIEW, m_bPreview);
    pApp->WriteProfileInt(REG_WAV_DLG, REG_WAV_NON_STOP, m_bNonStop);
    pApp->WriteProfileInt(REG_WAV_DLG, REG_FILTER, m_ofn.nFilterIndex);

    StopSoundFile();

    CFileDialogX::OnDestroy();
}
