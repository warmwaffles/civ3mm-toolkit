/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 C3fDoc.cpp
	CREATED:	 02.09.2004

	AUTHOR:		 Dreyk
	MODIFIED:	

	DESCRIPTION: implementation of the CC3fDoc class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "C3fDoc.h"
#include "C3fView.h"

#include "..\MFCaids\FileDialogX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CC3fDoc

IMPLEMENT_DYNCREATE(CC3fDoc, CDocument)

BEGIN_MESSAGE_MAP(CC3fDoc, CDocument)
//{{AFX_MSG_MAP(CC3fDoc)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC3fDoc construction/destruction

CC3fDoc::CC3fDoc()
{
}

CC3fDoc::~CC3fDoc()
{
}

BOOL
CC3fDoc::OnNewDocument()
{
    BOOL bRes = FALSE;

    if (MAIN_APP->IsStartup() && MAIN_APP->IsLoadLastDoc()) {
        // get the tapget path of the last opend document
        CString sPath = AfxGetApp()->GetProfileString(REG_MRU_C3F, uFormatString(IDS_REG_FILE_ENTRY, 1), "");

        if (uIsFileExist(sPath)) {
            if (OnOpenDocument(sPath)) {
                bRes = TRUE;
                SetPathName(sPath, FALSE);
            }
        }
    }

    if (!bRes) {
        return FALSE;
    }

    SetModifiedFlag(FALSE);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CC3fDoc diagnostics

#ifdef _DEBUG
void
CC3fDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void
CC3fDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CC3fDoc commands

BOOL
CC3fDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    CWaitCursor wait;

    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    return OpenStoryboard(lpszPathName, m_c3f);
}

BOOL
CC3fDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    return TRUE;
    //	return CDocument::OnSaveDocument(lpszPathName);
}
