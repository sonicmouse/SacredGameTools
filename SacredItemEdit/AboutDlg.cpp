// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
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
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString txt;
	//txt.Format("Sacred Item Manager v1.0a\nAuthor: Andrew Aksel Heinlein [SonicMouse]\nWeb: http://www.mouseindustries.com/sacred/\nEMail: sacred@mouseindustries.com\nCompiled: %s (%s) [MSVC++6.0]\nOnly tested on: %s",__DATE__,__TIME__,SACRED_VERSION);

	txt = "Sacred Item Manager v1.0a\n\n";
	txt+= "Author: Andrew Aksel Heinlein\n";
	txt+= "(SonicMouse)\n\n";
	txt+= "Web: www.mouseindustries.com\n";
	txt+= "Email: sacred@mouseindustries.com\n";
	txt+= "Build: " + CString(__DATE__) + " (" + __TIME__ + ")\n";
	txt+= "MSVC++6.0 SP5\n\n";
	txt+= "Thanks to:\n";
	txt+= "Me\nMe\nMe\nMaybe you.";

	m_scroller.Create(this,IDC_STATIC_ABOUT);
	m_scroller.SetScrollText(txt);
	m_scroller.SetBackgroundColor(RGB(255,255,255));
	m_scroller.SetScrollTextColor(RGB(127,0,0));
	m_scroller.SetShadowDrop(2);
	m_scroller.SetTextShadow(RGB(180,180,180));
	m_scroller.DrawFrame(TRUE);
	//m_scroller.SetBackgroundBitmap(IDB_BITMAP_ABOUT);
	m_scroller.DoScroll(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
