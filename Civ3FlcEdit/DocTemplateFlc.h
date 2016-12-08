// DocTemplateFlc.h: interface for the CDocTemplateFlc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(DOC_TEMPLATE_FLC_H__CIV3FLCEDIT__INCLUDED_)
#define DOC_TEMPLATE_FLC_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFlcDoc;
class CDocManagerEx;

class CDocTemplateFlc : public CNewMultiDocTemplate
{
    DECLARE_DYNAMIC(CDocTemplateFlc)
  public:
    virtual void InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, BOOL bMakeVisible = TRUE);
    virtual Confidence MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch);
    CDocManagerEx* GetDocManager();
    CFlcDoc* GetFlcDoc();
    virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
    CDocTemplateFlc(UINT nIDResource,
                    CRuntimeClass* pDocClass,
                    CRuntimeClass* pFrameClass,
                    CRuntimeClass* pViewClass,
                    CDocManagerEx* pDocManager);
    virtual ~CDocTemplateFlc();

  protected:
    CDocManagerEx* m_pDocManager;
    BOOL m_bStartup;
};

#endif // !defined(DOC_TEMPLATE_FLC_H__CIV3FLCEDIT__INCLUDED_)
