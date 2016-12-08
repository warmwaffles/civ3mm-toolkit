#if !defined(FORM_VIEW_X_H__MFCAIDS__INCLUDED_)
#define FORM_VIEW_X_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// FormViewX.h : header file
/////////////////////////////////////////////////////////////////////////////
// CFormViewX form view

#include <afxtempl.h>

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MFCdefs.h"

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

class CFormCtrlInfo;
class MFCAIDS_API CFormViewX : public CFormView
{
  protected:
    CFormViewX(UINT nIDTemplate);
    DECLARE_DYNCREATE(CFormViewX)

    // Form Data
  public:
    //{{AFX_DATA(CFormViewX)
    enum
    {
        IDD = 0xBEDA
    };
    //}}AFX_DATA

    // Attributes
  public:
    // Operations
  public:
    void SetControlPos(UINT CtrlId, CRect& pos);
    BOOL GetControlPos(UINT CtrlId, CRect& pos);
    void SetControlInfo(UINT CtrlId,
                        WORD Anchor,
                        WORD xDivider   = 1,
                        WORD xDivPos    = 1,
                        WORD yDivider   = 1,
                        WORD yDivPos    = 1,
                        WORD xDivWidth  = 1,
                        WORD yDivHeight = 1);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFormViewX)
  protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
                                                     //}}AFX_VIRTUAL

    // Implementation
  protected:
    int FindCtrl(UINT CtrlId);
    // Set m_bRememberSize to FALSE if you don't want the dialog box
    // to remember its size between sessions.
    virtual ~CFormViewX();

    int m_xFormSize;
    int m_yFormSize;

#ifdef _DEBUG
#endif

    // Generated message map functions
    //{{AFX_MSG(CFormViewX)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

  private:
    BOOL m_bFirstSize;
    CArray<CFormCtrlInfo, CFormCtrlInfo&> m_CtrlInfo;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FORM_VIEW_X_H__MFCAIDS__INCLUDED_)
