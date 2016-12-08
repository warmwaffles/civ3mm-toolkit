// GraphX.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "GraphX.h"
#include "GraphXExp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CGraphXApp

BEGIN_MESSAGE_MAP(CGraphXApp, CWinApp)
//{{AFX_MSG_MAP(CGraphXApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphXApp construction

CGraphXApp::CGraphXApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGraphXApp object

CGraphXApp theApp;

// fa17eb30-0b4b-43d9-a6ce-66ad513e88f3
//const GUID CDECL BASED_CODE _tlid = { 0xFA17Eb30, 0x0B4B, 0x43D9,
//{ 0xA6, 0xCE, 0x66, 0xAD, 0x51, 0x3E, 0x88, 0xF3 } };

/////////////////////////////////////////////////////////////////////////////
// CGraphXApp initialization

BOOL
CGraphXApp::InitInstance()
{
    // Register all OLE server (factories) as running.  This enables the
    //  OLE libraries to create objects from other applications.
    //	COleObjectFactory::RegisterAll();

    //	srand(time(NULL));

    // display mode: scale to gray
    GraphXnamespace::L_SetDisplayMode(DISPLAYMODE_RESETPOSITIONS, DISPLAYMODE_FASTPAINT);

    return TRUE;
}
/*
/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);
	
	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
	{
//		AfxMessageBox("SELFREG_E_CLASS");
		return ResultFromScode(SELFREG_E_CLASS);
	}
	
	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
	{
//		AfxMessageBox("SELFREG_E_TYPELIB");
		return ResultFromScode(SELFREG_E_TYPELIB);
	}

	return NOERROR;
}


STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);
	
	if (!AfxOleUnregisterTypeLib(_tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);
	
	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);
	
	return NOERROR;
}
*/