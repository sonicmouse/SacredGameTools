// SacredMSB.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SacredMSB.h"
#include "SMSBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSBApp

BEGIN_MESSAGE_MAP(CSMSBApp, CWinApp)
	//{{AFX_MSG_MAP(CSMSBApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSBApp construction

CSMSBApp::CSMSBApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSMSBApp object

CSMSBApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSMSBApp initialization

BOOL CSMSBApp::InitInstance(){

	if(!GetSHLib()->IsFullyLoaded()){
		MessageBox(NULL,"Could not successfully load the \"SHLIB.DLL\"\nMake sure it exists on this computer.\n\nAborting...","Error...",MB_ICONHAND|MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSMSBDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
