#if !defined(MRU_FILE_MANAGER_H__INCLUDED_)
#define MRU_FILE_MANAGER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxadv.h> // recent file list

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////////////////
// CMruFileManager - class to manage multiple recent file lists in MFC

// Function to test whether file belongs to a particular MRU file list
typedef BOOL(CALLBACK* MRUFILEFN)(LPCTSTR);

///////////////////////////////////////////////////////////////////
// Modified CRecentFileList adds ID range and fixes a bug in MFC.
//
class CRecentFileList2 : public CRecentFileList
{
  public:
    UINT m_nBaseID;     // base command ID
    MRUFILEFN m_pMruFn; // function to test filename

    CRecentFileList2(UINT nBaseID,
                     LPCTSTR lpszSection,
                     LPCTSTR lpszEntryFormat,
                     MRUFILEFN pMruFn,
                     int nMaxMRU     = 4,
                     int nMaxDispLen = AFX_ABBREV_FILENAME_LEN,
                     UINT nStart     = 0);
    ~CRecentFileList2();

    virtual void UpdateMenu(CCmdUI* pCmdUI);
    virtual BOOL IsMyKindOfFile(LPCTSTR lpszPathName);
};

class MFCAIDS_API CMruFileManager : public CCmdTarget
{
  public:
    void RemoveMRUFileLists();
    CMruFileManager(CWinApp* pApp);
    virtual ~CMruFileManager();

    // call this from your app's AddToRecentFileList, return if TRUE
    BOOL AddToRecentFileList(LPCTSTR lpszPathName);

    // call from InitInstance to add each MRU file type (eg, file/project)
    DWORD Add(UINT nBaseID,
              LPCTSTR lpszSection,
              LPCTSTR lpszEntryFormat,
              MRUFILEFN pMruFn,
              UINT nMaxMRU    = 4,
              BOOL bNoLoad    = FALSE,
              int nMaxDispLen = AFX_ABBREV_FILENAME_LEN,
              UINT nStart     = 0);
    BOOL Remove(DWORD dwRfl);

  protected:
    CPtrList m_listRfl; // list of CRecentFileList2's
    CWinApp* m_pApp;    // back ptr to app

    CRecentFileList2* FindRFL(UINT nID);

    DECLARE_DYNAMIC(CMruFileManager)
    DECLARE_MESSAGE_MAP()
    afx_msg void OnUpdateRecentFileMenu(CCmdUI* pCmdUI);
    afx_msg BOOL OnOpenRecentFile(UINT nID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(MRU_FILE_MANAGER_H__INCLUDED_)
