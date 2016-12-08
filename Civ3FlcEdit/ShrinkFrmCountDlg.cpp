/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 ShrinkFrmCountDlg.cpp
	CREATED:	 20.4.2005
	VERSION:	 1.0

	AUTHOR:		 Dreyk

	DESCRIPTION: implementation of the CShrinkFrmCountDlg class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "civ3flcedit.h"
#include "ShrinkFrmCountDlg.h"
#include "C3fView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShrinkFrmCountDlg dialog

CShrinkFrmCountDlg::CShrinkFrmCountDlg(CC3fView* pC3fView, CWnd* pParent /*=NULL*/)
  : CDialog(CShrinkFrmCountDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CShrinkFrmCountDlg)
    m_nFrmCount    = 0;
    m_sFrmCountCur = _T("");
    //}}AFX_DATA_INIT

    m_pC3fView = pC3fView;
}

void
CShrinkFrmCountDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CShrinkFrmCountDlg)
    DDX_Control(pDX, IDC_FRM_COUNT, m_wndFrmCount);
    DDX_Control(pDX, IDC_FRM_COUNT_SPIN, m_wndFrmCountSpin);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Text(pDX, IDC_FRM_COUNT, m_nFrmCount);
    DDX_Text(pDX, IDC_STATIC_FRM_COUNT_CUR, m_sFrmCountCur);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShrinkFrmCountDlg, CDialog)
//{{AFX_MSG_MAP(CShrinkFrmCountDlg)
ON_WM_DESTROY()
ON_EN_CHANGE(IDC_FRM_COUNT, OnChangeFrmCount)
ON_NOTIFY(UDN_DELTAPOS, IDC_FRM_COUNT_SPIN, OnDeltaposFrmCountSpin)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShrinkFrmCountDlg message handlers

BOOL
CShrinkFrmCountDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if (!m_pC3fView) {
        EndDialog(IDCANCEL);
        return FALSE;
    }

    LoadPosition(this);

    m_btnOK.SetIcon(IDI_OK);
    m_btnCancel.SetIcon(IDI_CANCEL);

    m_nFrmCount = m_nCurFrmCount = m_pC3fView->m_c3f.FrmCount;
    m_sFrmCountCur               = uInt2Str(m_nCurFrmCount);

    m_wndFrmCount.LimitText(4);
    m_wndFrmCountSpin.SetRange(1, m_nCurFrmCount);

    UpdateData(FALSE);
    return TRUE;
}

void
CShrinkFrmCountDlg::OnDestroy()
{
    SavePosition(this);
    CDialog::OnDestroy();
}

void
CShrinkFrmCountDlg::OnOK()
{
    if (!ShrinkFrmCount())
        return;

    CDialog::OnOK();
}

void
CShrinkFrmCountDlg::OnChangeFrmCount()
{
    if (m_wndFrmCount.GetSafeHwnd()) {
        UpdateData();
        if (m_nFrmCount > m_nCurFrmCount) {
            m_nFrmCount = m_nCurFrmCount;
            UpdateData(FALSE);
        } else if (m_nFrmCount < 1) {
            m_nFrmCount = 1;
            UpdateData(FALSE);
        }
    }
}

void
CShrinkFrmCountDlg::OnDeltaposFrmCountSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    OnChangeFrmCount();
    *pResult = 0;
}

BOOL
CShrinkFrmCountDlg::ShrinkFrmCount()
{
    if (AfxMessageBox(uFormatString(IDS_SHRINK_FRM_COUNT, m_nCurFrmCount, m_nFrmCount), MB_ICONQUESTION | MB_YESNO) != IDYES)
        return FALSE;

    C3FSTRUCT c3f;

    CString sName = CFileX::GetShortFileName(m_pC3fView->m_sPathName);
    memcpy(c3f.Name, sName, sName.GetLength());
    c3f.Name[sName.GetLength()] = 0;

    c3f.FrmWidth   = m_pC3fView->m_c3f.FrmWidth;
    c3f.FrmHeight  = m_pC3fView->m_c3f.FrmHeight;
    c3f.DirCount   = m_pC3fView->m_c3f.DirCount;
    c3f.FileType   = m_pC3fView->m_c3f.FileType;
    c3f.Delay      = m_pC3fView->m_c3f.Delay;
    c3f.XOffset    = m_pC3fView->m_c3f.XOffset;
    c3f.YOffset    = m_pC3fView->m_c3f.YOffset;
    c3f.FileType   = m_pC3fView->m_c3f.FileType;
    c3f.PalType    = m_pC3fView->m_c3f.PalType;
    c3f.LeaderHead = m_pC3fView->m_c3f.LeaderHead;

    c3f.Width    = (c3f.FrmWidth + 1) * m_nFrmCount + 1;
    c3f.Height   = (c3f.FrmWidth + 1) * c3f.DirCount + 1;
    c3f.FrmCount = m_nFrmCount;

    m_sPath.Format("%s%s%s", CFileX::GetFolder(m_pC3fView->m_sPathName), c3f.Name, c3f.FileType == FILE_PCX ? ".pcx" : ".bmp");

    if (!uIsFileExist(m_sPath)) {
        AfxMessageBox(uFormatString(IDS_ERR_SB_NOTEXIST, m_sPath), MB_ICONERROR);
        return FALSE;
    }

    CWaitCursor wait;

    try {
        GRAPHX::FileInfo(m_sPath, &m_fi, FILEINFO_TOTALPAGES, NULL);

        if (GRAPHX::LoadFile((LPTSTR)(LPCTSTR)m_sPath,
                             &m_hbmp,
                             m_fi.BitsPerPixel /*m_constBits*/,
                             ORDER_BGR,
                             LOADFILE_ALLOCATE | LOADFILE_STORE,
                             NULL,
                             NULL,
                             NULL,
                             NULL) != SUCCESS) {
            AfxMessageBox(uFormatString(IDS_ERR_SB_LOAD, m_sPath), MB_ICONERROR);
            return FALSE;
        }

        //		GRAPHX::SizeBitmap(&m_hbmp, c3f.Width, c3f.Height);
        GRAPHX::TrimBitmap(&m_hbmp, 0, 0, c3f.Width, c3f.Height);

        GRAPHX::SaveBitmap(m_sPath, &m_hbmp, c3f.FileType == 0 ? FILE_PCX : FILE_BMP, m_fi.BitsPerPixel);

        /////////////////////////////////////////////////
        // Make INI file
        if (!WriteC3F(m_pC3fView->m_sPathName, c3f)) {
            AfxMessageBox(IDS_ERR_WRITE_C3F, MB_ICONERROR);
            return FALSE;
        }
    } catch (CGraphXException* e) {
        AfxMessageBox(e->GetErrorMessage(e->GetLastError()), MB_ICONERROR);
        e->Delete();
        return FALSE;
    } catch (CMemoryException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
        return FALSE;
    } catch (...) {
        ASSERT(FALSE);
        AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "CShrinkFrmCountDlg::ShrinkFrmCount"), MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}
