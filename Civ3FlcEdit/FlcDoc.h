// FlcDoc.h : interface of the CFlcDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FLC_DOC_H__CIV3FLCEDIT__INCLUDED_)
#define FLC_DOC_H__CIV3FLCEDIT__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFlcDoc : public CDocument
{
protected: // create from serialization only
	CFlcDoc();
	DECLARE_DYNCREATE(CFlcDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFlcDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FLC_DOC_H__CIV3FLCEDIT__INCLUDED_)
