// CivTextWrapper.h

#if !defined(CIV_TEXT_WRAPPER_H__INCLUDED_)
#define CIV_TEXT_WRAPPER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

CString MFCAIDS_API Txt2Rtf(LPCTSTR sTxt); // Convert plain text to RTF
void    MFCAIDS_API MakeCivFormat(CString& sTxt);

#endif // !defined(CIV_TEXT_WRAPPER_H__INCLUDED_)
