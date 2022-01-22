// EditIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "EditIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditIDDlg dialog


CEditIDDlg::CEditIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditIDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pID = NULL;
}


void CEditIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditIDDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditIDDlg, CDialog)
	//{{AFX_MSG_MAP(CEditIDDlg)
	ON_BN_CLICKED(IDC_BUTTON_WTFCID, OnButtonWtfcid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditIDDlg message handlers

void CEditIDDlg::OnOK(){

	DWORD dwId = GetDlgItemInt(IDC_EDIT_NEWID);
	if(!dwId){ MessageBox("The value of ZERO or NULL is not an acceptable item ID!","Error...", MB_ICONHAND|MB_OK); return; }


	*m_pID = dwId;
	
	CDialog::OnOK();
}

BOOL CEditIDDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ASSERT(m_pID != NULL);

	SetDlgItemInt(IDC_EDIT_OLDID,*m_pID);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditIDDlg::OnButtonWtfcid() 
{
	CString txt;
	txt = "This will change an item's identifier number.\n";
	txt+= "Why is this useful?  You know how you can't equip certain items on certain heros?\n";
	txt+= "(Such as orge weapons and leg greaves on a non-leg greaves hero.)\n\n";
	txt+= "For example:\n";
	txt+= "Equip your hero with a crappy sword (NOTE: make sure you know the swords item ID).\n";
	txt+= "Next, find the item ID in this program in the item list to the left.\n";
	txt+= "Finally, get the item ID of the orge weapon and enter it in the box above.\n";
	txt+= "Start up Sacred and whallah... you will now be holding an orge weapon!\n\n";
	txt+= "Sounds complicated?  It is!\n\nGood luck!";
	MessageBox(txt, "Info...", MB_OK|MB_ICONINFORMATION);
}
