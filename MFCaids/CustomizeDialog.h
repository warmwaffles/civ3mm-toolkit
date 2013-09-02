/////////////////////////////////////////////////////////////////////////////
// CustomizeDialog.h: interface for the CCustomizeDialog class.
#if !defined(__CUSTOMIZE_DIALOG_H__INCLUDED_)
#define __CUSTOMIZE_DIALOG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

#include "MFCdefs.h"
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// Options

enum ETextOptions
{
    toTextLabels    =  0,
    toTextOnRight   =  1,
    toNoTextLabels  =  2,
    toNone          = -1,
};

enum EIconOptions
{
    ioSmallIcons    =  0,
    ioLargeIcons    =  1,
    ioNone          = -1,
};

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog

class CCustomizeDialog;

class MFCAIDS_API COptionsDialog : public CDialog
{
// Construction
public:
    COptionsDialog(ETextOptions eTextOptions,
                    EIconOptions eIconOptions);
	BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);

// Dialog Data
    //{{AFX_DATA(COptionsDialog)
    enum { IDD = IDD_DLG_CUSTOMIZE };
    CComboBox m_cbTextOptions;
    CComboBox m_cbIconOptions;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COptionsDialog)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Operations
public:
    BOOL SelectTextOption(ETextOptions eTextOptions);
    BOOL SelectIconOption(EIconOptions eIconOptions);

// Implementation
protected:
    CCustomizeDialog* GetCustomizeDialog() const;

// Implementation data
protected:
    ETextOptions m_eTextOptions;
    EIconOptions m_eIconOptions;

// Generated message map functions
protected:
    //{{AFX_MSG(COptionsDialog)
    virtual BOOL OnInitDialog();
    afx_msg void OnTextOptions();
    afx_msg void OnIconOptions();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCustomizeDialog dialog

class CToolBarX;

class MFCAIDS_API CCustomizeDialog : public CWnd
{
    DECLARE_DYNAMIC(CCustomizeDialog);

// Construction
public:
    CCustomizeDialog(CToolBarX* pToolBar);

// Operations
public:
    void SetTextOptions(ETextOptions eTextOptions, BOOL bInDialog);
    void SetIconOptions(EIconOptions eIconOptions, BOOL bInDialog);

    void AddTextOption(CComboBox& cbTextOptions, ETextOptions eTextOptions, UINT nStringID);
    void AddIconOption(CComboBox& cbIconOptions, EIconOptions eIconOptions, UINT nStringID);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCustomizeDialog)
    protected:
    virtual void PostNcDestroy();
    //}}AFX_VIRTUAL

// Implementation
protected:
    CSize GetButtonSize() const;

// Implementation data
protected:
    COptionsDialog  m_dlgOptions;
    CToolBarX*     m_pToolBar;

// Generated message map functions
protected:
    //{{AFX_MSG(CCustomizeDialog)
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    //}}AFX_MSG
    LRESULT OnInitDialog(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif  // !defined(__CUSTOMIZE_DIALOG_H__INCLUDED_)
