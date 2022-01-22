// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_BUTTON_GOTO, m_ButtonGoto);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_BUTTON_GOTO, OnButtonGoto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString txt;

	txt = "Sacred Item Manager v1.5\n\n";
	txt+= "Author: Andrew Aksel Heinlein\n";
	txt+= "(SonicMouse)\n\n";
	txt+= "Web: http://www.mouseindustries.com\n";
	txt+= "Email: sacred@mouseindustries.com\n";
	txt+= "Build: " + CString(__DATE__) + " (" + __TIME__ + ")\n";
	txt+= "MSVC++ 6.0 SP6\n\n";
	
	SetDlgItemText(IDC_STATIC_ABOUT,txt);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnButtonGoto() 
{
	ShellExecute(GetSafeHwnd(),"open","http://www.mouseindustries.com/sacred/",NULL,NULL,SW_SHOWNORMAL);	
}
