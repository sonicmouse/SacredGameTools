// SacredSugarDaddy.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SacredSugarDaddy.h"
#include "SSDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSDApp

BEGIN_MESSAGE_MAP(CSSDApp, CWinApp)
	//{{AFX_MSG_MAP(CSSDApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDApp construction

CSSDApp::CSSDApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSSDApp object

CSSDApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSSDApp initialization

BOOL CSSDApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if(!GetSHLib()->IsFullyLoaded()){
		MessageBox(NULL,"Could not successfully load the \"SHLIB.DLL\"\nMake sure it exists on this computer.\n\nAborting...","Error...",MB_ICONHAND|MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}

	CSSDDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
