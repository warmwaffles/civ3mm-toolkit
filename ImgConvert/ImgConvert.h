// ImgConvert.h : main header file for the IMGCONVERT application
//

#if !defined(AFX_IMGCONVERT_H__D8BC3E72_8EF2_4C1D_8323_DF4271AD055C__INCLUDED_)
#define AFX_IMGCONVERT_H__D8BC3E72_8EF2_4C1D_8323_DF4271AD055C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImgConvertApp:
// See ImgConvert.cpp for the implementation of this class
//

class CImgConvertApp : public CWinApp
{
public:
	CImgConvertApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgConvertApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImgConvertApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGCONVERT_H__D8BC3E72_8EF2_4C1D_8323_DF4271AD055C__INCLUDED_)
