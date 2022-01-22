// SacredItemManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "SIMDlg.h"

#include "cgfiltyp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIMApp

BEGIN_MESSAGE_MAP(CSIMApp, CWinApp)
	//{{AFX_MSG_MAP(CSIMApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIMApp construction

CSIMApp::CSIMApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSIMApp object

CSIMApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSIMApp initialization

BOOL CSIMApp::InitInstance(){

	if(!m_libSh.IsFullyLoaded()){
		MessageBox(NULL,"Could not successfully load the \"SHLIB.DLL\"\nMake sure it exists on this computer.\n\nAborting...","Error...",MB_ICONHAND|MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}

	//SetRegistryKey("mouseindustries");

	// register "sit" files with this program
	CGCFileTypeAccess TheFTA;
	TCHAR szProgPath[MAX_PATH * 2];
	GetModuleFileName(NULL, szProgPath, sizeof(szProgPath)/sizeof(TCHAR));
	CString csTempText;
	TheFTA.SetExtension("SIT");
	// just pass file path in quotes on command line
	csTempText  = szProgPath;
	csTempText += " \"%1\"";
	TheFTA.SetShellOpenCommand(csTempText);
	TheFTA.SetDocumentShellOpenCommand(csTempText);
	TheFTA.SetDocumentClassName("SacredItem.Document");
	// use first icon in program
	csTempText  = szProgPath;
	csTempText += ",1";
	TheFTA.SetDocumentDefaultIcon(csTempText);
	TheFTA.RegSetAllInfo();
	//////////////////////////////////////////////////////

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	srand(GetTickCount());

	CSIMDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
