/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	FileFlcDlg.cpp
	CREATED:	27.8.2003
	VERSION:	2.1

	AUTHOR:		Dreyk

	DESCRIPTION: implementation file
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FileFlcDlg.h"

#include "FlcImage.h"

#include <Mmsystem.h>
#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REG_FLC_DLG		"Dlg Flc"
#define REG_PREVIEW		"Preview"

/////////////////////////////////////////////////////////////////////////////
// CFileFlcDlg dialog


IMPLEMENT_DYNAMIC(CFileFlcDlg, CFileDialogX)

CFileFlcDlg::CFileFlcDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialogX(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	CWinApp* pApp = AfxGetApp();
	m_bPreview	= pApp->GetProfileInt(REG_FLC_DLG, REG_PREVIEW, 1);
	m_bFlc		= FALSE;
}


BEGIN_MESSAGE_MAP(CFileFlcDlg, CFileDialogX)
	//{{AFX_MSG_MAP(CFileFlcDlg)
	ON_BN_CLICKED(IDC_PREVIEW_CHECK, OnPreview)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFileFlcDlg::OnInitDialog() 
{
	GetDlgItem(IDC_PREVIEW_CHECK)->SendMessage(BM_SETCHECK, (m_bPreview) ? 1 : 0);

	return CFileDialogX::OnInitDialog();
}

void CFileFlcDlg::OnSize(UINT nType, int cx, int cy) 
{
	CFileDialogX::OnSize(nType, cx, cy);

	Invalidate();
}

void CFileFlcDlg::OnDestroy() 
{
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(REG_FLC_DLG, REG_PREVIEW, m_bPreview);

	m_img.CloseFile();
	
	CFileDialogX::OnDestroy();
}


void CFileFlcDlg::OnFileNameChange()
{
	CFileDialogX::OnFileNameChange();
	
	LoadFileForPreview();

	Invalidate();
}

void CFileFlcDlg::OnFolderChange()
{
	CFileDialogX::OnFolderChange();

	m_img.CloseFile();
	Invalidate();
}

void CFileFlcDlg::OnPreview() 
{
	m_bPreview = !m_bPreview;

	if (m_bPreview && !m_img.IsLoaded())
		LoadFileForPreview();

	Invalidate();
}

void CFileFlcDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CWnd *pWnd = GetDlgItem(IDC_PREVIEW);

	if (pWnd->GetSafeHwnd())
	{
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		dc.Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
		
		if (m_bPreview && m_img.IsLoaded())
		{
			rc.DeflateRect(3, 3);
			m_img.Render(&dc, rc, CGraphXImage::PaintProportion2);
		}
	}
}

BOOL CFileFlcDlg::OnFileNameOK()
{
	if (m_bFlc && m_bPreview)
	{
		if (!m_img.IsLoaded())
		{
			AfxMessageBox(uFormatString(IDS_IMG_UNKNOWN_FORMAT, GetPathName()));
			return TRUE;
		}
		
		FILEINFO* pFi = m_img.GetImgFileInfo();
		
		BOOL bChanged = FALSE;
		
		if (pFi->Format != FILE_FLC && 
		   (pFi->Format != FILE_BMP && pFi->Format != FILE_PCX)) 
		{
			AfxMessageBox(uFormatString(IDS_IMG_UNKNOWN_FORMAT, pFi->Name), MB_ICONERROR);
			return TRUE;
		}
	}

	return CFileDialogX::OnFileNameOK();
}

void CFileFlcDlg::LoadFileForPreview()
{
	try
	{
		CString sPath = GetPathName();
		CString sExt;
		
		if (!sPath.IsEmpty())
		{
			sExt = uGetFileExt(sPath);
			if (sExt.CompareNoCase(FLC_EXT) == 0)
			{
				if (m_bPreview)
				{
					CWaitCursor wait;
					m_img.LoadFile(sPath);
					m_bFlc = m_img.IsLoaded();
				}
				else
					m_bFlc = TRUE;
			}
			else if (sExt.CompareNoCase(C3F_EXT) == 0)
			{
				m_img.CloseFile();
				if (m_bPreview)
				{
					CWaitCursor wait;
					C3FSTRUCT c3f;
					if (!ReadC3F(sPath, &c3f))
					{
						AfxMessageBox(IDS_ERR_READ_C3F);
						return;
					}
					
					sPath.Format("%s%s%s", CFileX::GetFolder(sPath), c3f.Name, c3f.FileType == FILE_PCX ? ".pcx" : ".bmp");
					
					m_img.LoadFile(sPath);
					m_bFlc = m_img.IsLoaded();
					GRAPHX::TrimBitmap(&m_img.m_hbmp, 1, 1, c3f.FrmWidth, c3f.FrmHeight);
				}
			}
			else
				m_bFlc = FALSE;
		}
	}
	catch (CGraphXException *e)
	{
		//AfxMessageBox(e->GetErrorMessage(e->GetLastError()));
		GetDlgItem(IDC_PREVIEW)->SetWindowText(e->GetErrorMessage(e->GetLastError()));
		e->Delete();
	}
	catch (...)
	{
		AfxMessageBox(uFormatString(IDS_UNKNOWN_ERR, "FileFlcDlg::LoadFileForPreview"), MB_ICONERROR);
	}
}
