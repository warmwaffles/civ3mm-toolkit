// DocTemplateC3f.h: interface for the CDocTemplateC3f class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(DOC_TEMPLATE_C3F_H__CIV3FLCEDIT__INCLUDED_)
#define DOC_TEMPLATE_C3F_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CC3fDoc;
class CDocManagerEx;

class CDocTemplateC3f : public CNewMultiDocTemplate  
{
	DECLARE_DYNAMIC(CDocTemplateC3f)
public:
	virtual void InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, BOOL bMakeVisible = TRUE);
	virtual Confidence MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch);
	CDocManagerEx* GetDocManager();
	CC3fDoc* GetC3fDoc();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
	CDocTemplateC3f(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass, CDocManagerEx* pDocManager);
	virtual ~CDocTemplateC3f();
	
protected:
	CDocManagerEx* m_pDocManager;
	BOOL m_bStartup;
};

#endif // !defined(DOC_TEMPLATE_C3F_H__CIV3FLCEDIT__INCLUDED_)
