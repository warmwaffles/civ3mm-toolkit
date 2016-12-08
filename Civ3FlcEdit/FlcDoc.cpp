/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 FlcDoc.cpp
	CREATED:	 26.08.2004

	AUTHOR:		 Dreyk
	MODIFIED:	

	DESCRIPTION: implementation of the CFlcDoc class
\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "FlcDoc.h"

#include "FileFlcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlcDoc

IMPLEMENT_DYNCREATE(CFlcDoc, CDocument)

BEGIN_MESSAGE_MAP(CFlcDoc, CDocument)
//{{AFX_MSG_MAP(CFlcDoc)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlcDoc construction/destruction

CFlcDoc::CFlcDoc()
{
}

CFlcDoc::~CFlcDoc()
{
}

BOOL
CFlcDoc::OnNewDocument()
{
    BOOL bRes = FALSE;

    if (MAIN_APP->IsStartup() && MAIN_APP->IsLoadLastDoc()) {
        // get the tapget path of the last opend document
        CString sPath = AfxGetApp()->GetProfileString(REG_MRU_FLC, uFormatString(IDS_REG_FILE_ENTRY, 1), "");

        if (uIsFileExist(sPath)) {
            if (OnOpenDocument(sPath)) {
                bRes = TRUE;
                SetPathName(sPath, FALSE);
            }
        }
    }

    if (!bRes) {
        return FALSE;
        //		if (CDocument::OnNewDocument())
        //		{
        //			bRes = TRUE;
        //			SetTitle(uLoadString(IDS_NEW_DOCUMENT_CAPTION));
        //			m_strPathName.Empty();
        //		}
    }

    SetModifiedFlag(FALSE);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFlcDoc diagnostics

#ifdef _DEBUG
void
CFlcDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void
CFlcDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFlcDoc commands

BOOL
CFlcDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    CWaitCursor wait;

    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;
    /*
	if (MAIN_APP->IsStartup())
	{
		if (OnNewDocument())
		{
			SetTitle(uLoadString(IDS_NEW_DOCUMENT_CAPTION));
			lpszPathName = "";	
			return TRUE;
		}
	}
*/
    // Check file extension
    if (uGetFileExt(lpszPathName).CompareNoCase(FLC_EXT) != 0) {
        AfxMessageBox(uFormatString(IDS_ERR_INVALID_FLC, lpszPathName), MB_ICONERROR);
        // OnNewDocument();
        return FALSE;
    }

    BOOL bRes = FALSE;
    try {
        FILE* fp;

        fp = fopen(lpszPathName, "rb");

        if (fp != NULL) {
            if (fseek(fp, 4, SEEK_SET) == 0) {
                WORD type[1];
                fread(type, sizeof(WORD), 1, fp);

                // Get the file type from header
                // The type field must contain 0xAF12
                // For standard FLC files (with an 8-bit colour depth).
                if (type[0] == FLC_TYPE)
                    bRes = TRUE;
            }
        }

        if (fp)
            fclose(fp);

        if (!bRes) {
            AfxMessageBox(uFormatString(IDS_ERR_INVALID_FLC, lpszPathName), MB_ICONERROR);
            //OnNewDocument();
            return FALSE;
        }
    } catch (CFileException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
        //OnNewDocument();
        return FALSE;
    } catch (CMemoryException* e) {
        TCHAR szCause[1024];
        e->GetErrorMessage(szCause, 1024);
        AfxMessageBox(szCause, MB_ICONERROR);
        e->Delete();
        //OnNewDocument();
        return FALSE;
    } catch (...) {
        AfxMessageBox(uFormatString(IDS_ERR_INVALID_FLC, lpszPathName), MB_ICONERROR);
        //OnNewDocument();
        return FALSE;
    }

    return TRUE;
}

BOOL
CFlcDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    return TRUE;
    //	return CDocument::OnSaveDocument(lpszPathName);
}
