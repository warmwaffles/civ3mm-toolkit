// FramePosition.h: interface for the CFramePosition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(FRAMEPOSITION_H__MFCAIDS__INCLUDED_)
#define FRAMEPOSITION_H__MFCAIDS__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

class MFCAIDS_API CFramePosition  
{
public:
	CFramePosition();
	virtual ~CFramePosition();
	void SavePosition(CWnd* pWnd);
	void LoadPosition(CWnd* pWnd, BOOL bMax = FALSE);
	
protected:
	virtual LPCTSTR RegKey();
};

class MFCAIDS_API CNoSizePosition  
{
public:
	CNoSizePosition();
	virtual ~CNoSizePosition();
	void SavePosition(CWnd* pWnd);
	void LoadPosition(CWnd* pWnd);
	
protected:
	virtual LPCTSTR RegKey();
};

#endif // !defined(FRAMEPOSITION_H__MFCAIDS__INCLUDED_)
