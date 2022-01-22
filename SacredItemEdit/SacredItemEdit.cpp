// SacredItemEdit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "SIEDlg.h"
#include "cgfiltyp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIEApp

BEGIN_MESSAGE_MAP(CSIEApp, CWinApp)
	//{{AFX_MSG_MAP(CSIEApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIEApp construction

CSIEApp::CSIEApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSIEApp object

CSIEApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSIEApp initialization

BOOL CSIEApp::InitInstance(){

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	srand(GetTickCount());

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

	CSIEDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
