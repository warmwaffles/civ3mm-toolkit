// DocTemplateC3f.cpp: implementation of the CDocTemplateC3f class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "DocTemplateC3f.h"

#include "DocManagerEx.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "C3fView.h"
#include "C3fDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDocTemplateC3f, CNewMultiDocTemplate)

CDocTemplateC3f::CDocTemplateC3f(UINT nIDResource,
                                 CRuntimeClass* pDocClass,
                                 CRuntimeClass* pFrameClass,
                                 CRuntimeClass* pViewClass,
                                 CDocManagerEx* pDocManager)
  : CNewMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
    m_pDocManager = pDocManager;
    m_bStartup    = TRUE;
}

CDocTemplateC3f::~CDocTemplateC3f()
{
}

CDocument*
CDocTemplateC3f::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
    CString sLastPath;

    // if this function was called at startup
    if (MAIN_APP->IsStartup() && MAIN_APP->IsLoadLastDoc()) {
        // get the tapget path of the last opend document
        sLastPath = AfxGetApp()->GetProfileString(REG_MRU_C3F, uFormatString(IDS_REG_FILE_ENTRY, 1), "");

        if (uIsFileExist(sLastPath))
            lpszPathName = sLastPath;
    }

    CC3fDoc* pDoc = (CC3fDoc*)CNewMultiDocTemplate::OpenDocumentFile(lpszPathName, bMakeVisible);

    return (CDocument*)pDoc;
}

CC3fDoc*
CDocTemplateC3f::GetC3fDoc()
{
    POSITION pos = GetFirstDocPosition();
    return (pos == NULL) ? NULL : (CC3fDoc*)GetNextDoc(pos);
}

CDocManagerEx*
CDocTemplateC3f::GetDocManager()
{
    return m_pDocManager;
}

CDocTemplate::Confidence
CDocTemplateC3f::MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch)
{
    ASSERT(lpszPathName != NULL);
    rpDocMatch = NULL;

    if (m_pDocManager->IsOpenAsC3f())
        return noAttempt;

    // go through all documents
    POSITION pos = GetFirstDocPosition();
    while (pos != NULL) {
        CDocument* pDoc = GetNextDoc(pos);
        if (lstrcmpi(pDoc->GetPathName(), lpszPathName) == 0) {
            // already open
            rpDocMatch = pDoc;
            return yesAlreadyOpen;
        }
    }

    // see if it matches our default suffix
    // see if extension matches
    LPCTSTR lpszDot = _tcsrchr(lpszPathName, '.');
    if (lpszDot != NULL && lstrcmpi(lpszDot, ".c3f") == 0)
        return yesAttemptNative; // extension matches, looks like ours

    // otherwise we will guess it may work
    return noAttempt;
}

void
CDocTemplateC3f::InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, BOOL bMakeVisible)
{
    CNewMultiDocTemplate::InitialUpdateFrame(pFrame, pDoc, bMakeVisible);

    CC3fView* pView = (CC3fView*)pFrame->GetActiveView();
    if (pView && !pView->IsLoaded())
        GetC3fDoc()->OnCloseDocument();
}
