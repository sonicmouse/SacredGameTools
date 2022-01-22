// SacredRAW.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SacredRAW.h"
#include "SRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSRApp

BEGIN_MESSAGE_MAP(CSRApp, CWinApp)
	//{{AFX_MSG_MAP(CSRApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRApp construction

CSRApp::CSRApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSRApp object

CSRApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSRApp initialization

BOOL CSRApp::InitInstance()
{
	// Standard initialization

	if(!GetSHLib()->IsFullyLoaded()){
		MessageBox(NULL,"Could not successfully load the \"SHLIB.DLL\"\nMake sure it exists on this computer.\n\nAborting...","Error...",MB_ICONHAND|MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSRDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
