// CommandLineInfoX.h: interface for the CCommandLineInfoX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(COMMAND_LINE_INFO_X_H__INCLUDED_)
#define COMMAND_LINE_INFO_X_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MFCAIDS_EXPORTS
#define MFCAIDS_API __declspec(dllexport)
#else
#define MFCAIDS_API __declspec(dllimport)
#endif

class MFCAIDS_API CCommandLineInfoX : public CCommandLineInfo
{
  public:
    CCommandLineInfoX();
    virtual ~CCommandLineInfoX();
    virtual void ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);

    BOOL m_bRegFileType;   // register the application file type key
    BOOL m_bUnregFileType; // unregister the application file type key
    BOOL m_bSilent;        // do not show any messages
    BOOL m_bInstall;       // install the application
    BOOL m_bUninstall;     // uninstall the application
};

#endif // !defined(COMMAND_LINE_INFO_X_H__INCLUDED_)
