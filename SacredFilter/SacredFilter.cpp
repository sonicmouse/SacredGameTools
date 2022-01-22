// SacredFilter.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SacredFilter.h"
#include "SacredFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSacredFilterApp

BEGIN_MESSAGE_MAP(CSacredFilterApp, CWinApp)
	//{{AFX_MSG_MAP(CSacredFilterApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSacredFilterApp construction

CSacredFilterApp::CSacredFilterApp()
{
	SetLastError(0);
	CreateMutex(NULL,TRUE,"CSacredFilterApp::CSacredFilterApp()");
	m_bAlreadyOpen = (GetLastError()==ERROR_ALREADY_EXISTS);
	SetLastError(0);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSacredFilterApp object

CSacredFilterApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSacredFilterApp initialization

BOOL CSacredFilterApp::InitInstance(){

	// Initialize COM
	CoInitialize(NULL);

	if(m_bAlreadyOpen){
		MessageBox(NULL,"Looks like there is a copy of Sacred Filter already running!\nAborting...","Error...",MB_ICONHAND|MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}

	if (!AfxSocketInit()){
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer(); // allow ActiveX controls

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	try {
		CSacredFilterDlg dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();
	}catch (_com_error& e){
		CString strMsg = _T("Apparently you don't have the script object [MSSCRIPT.OCX] installed.\nDownload it here at: http://www.microsoft.com/scripting/");
		AfxMessageBox( strMsg, MB_ICONHAND|MB_OK|MB_SYSTEMMODAL);
		::OutputDebugString(e.ErrorMessage()); 
	}

	return FALSE;
}

int CSacredFilterApp::ExitInstance(){
	// shut down COM
	CoUninitialize();
	return CWinApp::ExitInstance();
}
