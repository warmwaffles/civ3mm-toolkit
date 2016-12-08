// C3fDoc.h : interface of the CC3fDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(C3F_DOC_H__CIV3FLCEDIT__INCLUDED_)
#define C3F_DOC_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FlcImage.h"

class CC3fDoc : public CDocument, CFlcImage
{
    friend class CC3fView;

  protected: // create from serialization only
    CC3fDoc();
    DECLARE_DYNCREATE(CC3fDoc)

    // Attributes
  public:
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CC3fDoc)
  public:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CC3fDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

  protected:
    C3FSTRUCT m_c3f;

    // Generated message map functions
  protected:
    //{{AFX_MSG(CC3fDoc)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(C3F_DOC_H__CIV3FLCEDIT__INCLUDED_)
