/*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\
	FILE NAME:	 CommandLineInfoX.cpp
	CREATED:	 24.10.2003

	AUTHOR:		 Dreyk
	MODIFIED:	

	DESCRIPTION: Modified CCommandLineInfo class.
	The CCommandLineInfoX class aids in parsing the command line at application startup.
 
  ______________________________________________________
  Command-line argument                Command executed 
  ______________________________________________________
  app                                  New file. 
  app filename                         Open file. 
  app /p filename                      Print file to default printer. 
  app /pt filename printer driver port Print file to the specified printer. 
  app /dde                             Start up and await DDE command. 
  app /Automation                      Start up as an OLE automation server. 
  app /Embedding                       Start up to edit an embedded OLE item. 
  app /r                               Registry file type.
  app /x                               Unregistry file type.
  app /i                               Install application.
  app /u                               Uninstall application.
  app /s                               Silent mode - without any messages.

\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/

#include "stdafx.h"
#include "CommandLineInfoX.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCommandLineInfoX::CCommandLineInfoX() 
{ 
	m_bInstall = m_bUninstall = m_bSilent = m_bRegFileType = m_bUnregFileType = FALSE; 
}

CCommandLineInfoX::~CCommandLineInfoX()
{
}

void CCommandLineInfoX::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	if (bFlag)
	{
		CString sParam(lpszParam);
		sParam.MakeUpper();

		if (!m_bInstall)
			m_bInstall = (sParam.Compare("I") == NULL);

		if (!m_bUninstall)
		{
			m_bUninstall = (sParam.Compare("U") == NULL);
			if (m_bUninstall)
			{
				m_nShellCommand  = CCommandLineInfo::AppUnregister;
				m_bUnregFileType = TRUE;
			}
		}

		if (!m_bSilent)
			m_bSilent = (sParam.Compare("S") == NULL);

		if (!m_bRegFileType)
			m_bRegFileType = (sParam.Compare("R") == NULL);

		if (!m_bRegFileType && !m_bUninstall)
		{
			if (!m_bUnregFileType)
				m_bUnregFileType = (sParam.Compare("X") == NULL);
		}
	}

	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
}
