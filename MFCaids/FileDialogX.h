#if !defined FILE_DIALOG_X_H_MFCAIDS__INCLUDED_
#define FILE_DIALOG_X_H_MFCAIDS__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// FileDialogX.h : header file
//

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

// Windows 2000 version of OPENFILENAME.
// The new version has three extra members.
// This is copied from commdlg.h
struct OPENFILENAMEEX : public OPENFILENAME 
{ 
  void *        pvReserved;
  DWORD         dwReserved;
  DWORD         FlagsEx;
};

///////////////////////////////////////////////////////////////////////////
// CFileDialogX: Encapsulate Windows-2000 style open dialog.
class MFCAIDS_API CFileDialogX : public CFileDialog 
{
      DECLARE_DYNAMIC(CFileDialogX)
public: 
      CFileDialogX(BOOL bOpenFileDialog, // TRUE for open, 
                                          // FALSE for FileSaveAs
      LPCTSTR lpszDefExt = NULL,
      LPCTSTR lpszFileName = NULL,
      DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      LPCTSTR lpszFilter = NULL,
      CWnd* pParentWnd = NULL);

   // override
   virtual int DoModal();

protected:
   OPENFILENAMEEX m_ofnEx; // new Windows 2000 version of OPENFILENAME

   virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

   // virtual fns that handle various notifications
   virtual BOOL OnFileNameOK();
   virtual void OnInitDone();
   virtual void OnFileNameChange();
   virtual void OnFolderChange();
   virtual void OnTypeChange();

   DECLARE_MESSAGE_MAP()
   //{{AFX_MSG(CFileDialogX)
   //}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(FILE_DIALOG_X_H_MFCAIDS__INCLUDED_)