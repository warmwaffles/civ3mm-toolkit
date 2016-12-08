// ChildFrm.h : interface of the CChildFrm class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(CHILDFRM_H__CIV3FLCEDIT__INCLUDED_)
#define CHILDFRM_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChildFrm : public CNewMDIChildWnd
{
    DECLARE_DYNCREATE(CChildFrm)
  public:
    CChildFrm();

    // Attributes
  public:
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChildFrm)
  public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void ActivateFrame(int nCmdShow = -1);

  protected:
    virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
    virtual void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd*);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CChildFrm();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
  protected:
    //{{AFX_MSG(CChildFrm)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(CHILDFRM_H__CIV3FLCEDIT__INCLUDED_)
