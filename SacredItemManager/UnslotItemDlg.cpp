// UnslotItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "UnslotItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnslotItemDlg dialog


CUnslotItemDlg::CUnslotItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnslotItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnslotItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bUnslotToBackpack = TRUE;
}


void CUnslotItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnslotItemDlg)
	DDX_Control(pDX, IDC_COMBO_UNSLOTTO, m_ComboTo);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUnslotItemDlg, CDialog)
	//{{AFX_MSG_MAP(CUnslotItemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnslotItemDlg message handlers

BOOL CUnslotItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ComboTo.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUnslotItemDlg::OnOK(){
	m_bUnslotToBackpack = (m_ComboTo.GetCurSel() == 0);
	CDialog::OnOK();
}

