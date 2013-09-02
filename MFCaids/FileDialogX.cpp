//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
// 
// FILE NAME:   FileDialogX.cpp
// AUTHOR:      Paul DiLascia
// DESCRIPTION:	Encapsulate Windows-2000 style open dialog.
//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

#include "stdafx.h"
#include "FileDialogX.h"
#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
// CFileDialogX

IMPLEMENT_DYNAMIC(CFileDialogX, CFileDialog)

CFileDialogX::CFileDialogX(BOOL bOpenFileDialog,
   LPCTSTR lpszDefExt,
   LPCTSTR lpszFileName,
   DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
   CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName,
      dwFlags, lpszFilter, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CFileDialogX, CFileDialog)
   //{{AFX_MSG_MAP(CFileDialogX)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CFileDialogX::DoModal()
{
   ASSERT_VALID(this);
   ASSERT(m_ofn.Flags & OFN_ENABLEHOOK);
   ASSERT(m_ofn.lpfnHook != NULL); // can still be a user hook

   // zero out the file buffer for consistent parsing later
   ASSERT(AfxIsValidAddress(m_ofn.lpstrFile, m_ofn.nMaxFile));
   DWORD nOffset = lstrlen(m_ofn.lpstrFile)+1;
   ASSERT(nOffset <= m_ofn.nMaxFile);
   memset(m_ofn.lpstrFile+nOffset, 0, 
         (m_ofn.nMaxFile-nOffset)*sizeof(TCHAR));

   // WINBUG: This is a special case for the file open/save dialog,
   //  which sometimes pumps while it is coming up but before it has
   //  disabled the main window.
   HWND hWndFocus = ::GetFocus();
   BOOL bEnableParent = FALSE;
   m_ofn.hwndOwner = PreModal();
   AfxUnhookWindowCreate();
   if (m_ofn.hwndOwner != NULL && ::IsWindowEnabled(m_ofn.hwndOwner))
   {
      bEnableParent = TRUE;
      ::EnableWindow(m_ofn.hwndOwner, FALSE);
   }

   _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
   ASSERT(pThreadState->m_pAlternateWndInit == NULL);

   if (m_ofn.Flags & OFN_EXPLORER)
      pThreadState->m_pAlternateWndInit = this;
   else
      AfxHookWindowCreate(this);

   memset(&m_ofnEx, 0, sizeof(m_ofnEx));
   memcpy(&m_ofnEx, &m_ofn, sizeof(m_ofn));
   if (uIsWin2000())
      m_ofnEx.lStructSize = sizeof(m_ofnEx);

   int nResult;
   if (m_bOpenFileDialog)
      nResult = ::GetOpenFileName((OPENFILENAME*)&m_ofnEx);
   else
      nResult = ::GetSaveFileName((OPENFILENAME*)&m_ofnEx);

   memcpy(&m_ofn, &m_ofnEx, sizeof(m_ofn));
   m_ofn.lStructSize = sizeof(m_ofn);

   if (nResult)
      ASSERT(pThreadState->m_pAlternateWndInit == NULL);
   pThreadState->m_pAlternateWndInit = NULL;

   // WINBUG: Second part of special case for file open/save dialog.
   if (bEnableParent)
      ::EnableWindow(m_ofnEx.hwndOwner, TRUE);
   if (::IsWindow(hWndFocus))
      ::SetFocus(hWndFocus);

   PostModal();
   return nResult ? nResult : IDCANCEL;
}

BOOL CFileDialogX::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
   memcpy(&m_ofn, &m_ofnEx, sizeof(m_ofn));
   m_ofn.lStructSize = sizeof(m_ofn);

   return CFileDialog::OnNotify( wParam, lParam, pResult);
}

////////////////////////////////////////////////////////////////
// The following functions are provided just to demonstrate that they
// get called; ie, that MFC's internal dialog proc is in fact hooked
// up. You can delete them if you like.
//
BOOL CFileDialogX::OnFileNameOK()
{
//   TRACE(_T("CFileDialogX::OnFileNameOK\n"));
   return CFileDialog::OnFileNameOK();
}

void CFileDialogX::OnInitDone()
{
//   TRACE(_T("CFileDialogX::OnInitDone\n"));
   CFileDialog::OnInitDone();
}

void CFileDialogX::OnFileNameChange()
{
//   TRACE(_T("CFileDialogX::OnFileNameChange\n"));
   CFileDialog::OnFileNameChange();
}

void CFileDialogX::OnFolderChange()
{
//   TRACE(_T("CFileDialogX::OnFolderChange\n"));
   CFileDialog::OnFolderChange();
}

void CFileDialogX::OnTypeChange()
{
//   TRACE(_T("OnTypeChange(), index = %d\n"), m_ofn.nFilterIndex);
   CFileDialog::OnTypeChange();
}
