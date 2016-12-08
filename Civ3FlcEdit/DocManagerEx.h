// DocManagerEx.h: interface for the CDocManagerEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(DOCMANAGEREX_H__CIV3FLCEDIT__INCLUDED_)
#define DOCMANAGEREX_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFlcDoc;
class CC3fDoc;

class CDocManagerEx : public CDocManager
{
  public:
    void OpenFromFolder();
    void OpenFromINI();
    virtual void RegisterShellFileTypes(BOOL bCompat);
    virtual BOOL OnFileSaveAs();
    virtual void OnFileOpen();
    virtual void OnOpenC3f();
    CDocTemplate* GetDocTemplateC3f();
    CDocTemplate* GetDocTemplateFlc();
    CFlcDoc* GetFlcDoc();
    CC3fDoc* GetC3fDoc();
    BOOL CheckDocUnique();
    virtual void OnFileNew();
    CDocManagerEx();
    virtual ~CDocManagerEx();
    BOOL IsOpenAsC3f()
    {
        return m_bOpenAsC3f;
    };

  protected:
    BOOL m_bOpenAsC3f;
};

#endif // !defined(DOCMANAGEREX_H__CIV3FLCEDIT__INCLUDED_)
