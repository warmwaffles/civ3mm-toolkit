// MFCaids.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MFCaids.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCaids

BEGIN_MESSAGE_MAP(CMFCaids, CWinApp)
//{{AFX_MSG_MAP(CMFCaids)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCaids construction

CMFCaids::CMFCaids()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMFCaids object

CMFCaids theApp;

// 66f93646-b5b1-4859-8e77-d2c4f748d038
//const GUID CDECL BASED_CODE _tlid = { 0x66F93646, 0xB5B1, 0x4859,
//{ 0x8E, 0x77, 0xD2, 0xC4, 0xF7, 0x48, 0xD0, 0x38 } };

/////////////////////////////////////////////////////////////////////////////
// CMFCaids initialization

BOOL
CMFCaids::InitInstance()
{
    // Register all OLE server (factories) as running.  This enables the
    //  OLE libraries to create objects from other applications.
    //	COleObjectFactory::RegisterAll();

    //	srand(time(NULL));

    return TRUE;
}
/*
/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers
//
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

/////////////////////////////////////////////////////////////////////////////
// By exporting DllRegisterServer, you can use regsvr.exe
//
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}
*/