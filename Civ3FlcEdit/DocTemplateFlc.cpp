// DocTemplateFlc.cpp: implementation of the CDocTemplateFlc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Civ3FlcEdit.h"
#include "DocTemplateFlc.h"

#include "DocManagerEx.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "FlcView.h"
#include "FlcDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDocTemplateFlc, CNewMultiDocTemplate)

CDocTemplateFlc::CDocTemplateFlc(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass, CDocManagerEx* pDocManager) :
	CNewMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
	m_pDocManager = pDocManager;
	m_bStartup    = TRUE;
}

CDocTemplateFlc::~CDocTemplateFlc()
{

}

CDocument* CDocTemplateFlc::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	CString sLastPath;

	// if this function was called at startup
	if (MAIN_APP->IsStartup() && MAIN_APP->IsLoadLastDoc())
	{
		// get the tapget path of the last opend document
		sLastPath = AfxGetApp()->GetProfileString(REG_MRU_FLC, uFormatString(IDS_REG_FILE_ENTRY, 1), "");
		
		if (uIsFileExist(sLastPath))
			lpszPathName = sLastPath;
	}

	CFlcDoc* pDoc = (CFlcDoc*)CNewMultiDocTemplate::OpenDocumentFile(lpszPathName, bMakeVisible);

	return pDoc;
}

CFlcDoc* CDocTemplateFlc::GetFlcDoc()
{
	POSITION pos = GetFirstDocPosition();
	return (pos == NULL) ? NULL : (CFlcDoc*)GetNextDoc(pos);
}

CDocManagerEx* CDocTemplateFlc::GetDocManager()
{
	return m_pDocManager;
}

CDocTemplate::Confidence CDocTemplateFlc::MatchDocType(LPCTSTR lpszPathName, CDocument *&rpDocMatch)
{
	ASSERT(lpszPathName != NULL);
	rpDocMatch = NULL;

	if (m_pDocManager->IsOpenAsC3f())
		return noAttempt;

	// go through all documents
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		if (lstrcmpi(pDoc->GetPathName(), lpszPathName) == 0)
		{
			// already open
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
		}
	}

	// see if it matches our default suffix
	// see if extension matches
	LPCTSTR lpszDot = _tcsrchr(lpszPathName, '.');
	if (lpszDot != NULL && lstrcmpi(lpszDot, ".flc") == 0)
		return yesAttemptNative; // extension matches, looks like ours

	// otherwise we will guess it may work
	return noAttempt;
}

void CDocTemplateFlc::InitialUpdateFrame(CFrameWnd *pFrame, CDocument *pDoc, BOOL bMakeVisible)
{
	CNewMultiDocTemplate::InitialUpdateFrame(pFrame, pDoc, bMakeVisible);
	
	CFlcView* pView = (CFlcView*)pFrame->GetActiveView();
	if (pView && !pView->IsLoaded())
		GetFlcDoc()->OnCloseDocument();
}

