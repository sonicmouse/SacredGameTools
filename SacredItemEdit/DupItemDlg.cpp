// DupItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "DupItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDupItemDlg dialog


CDupItemDlg::CDupItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDupItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDupItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTimes = 0;
	m_bShrink = FALSE;
	m_bToChest = TRUE;
}

void CDupItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDupItemDlg)
	DDX_Control(pDX, IDC_COMBO_TO, m_Combo);
	DDX_Control(pDX, IDC_SPIN_DUPER, m_Spin);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDupItemDlg, CDialog)
	//{{AFX_MSG_MAP(CDupItemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDupItemDlg message handlers

void CDupItemDlg::OnOK(){
	m_nTimes = GetDlgItemInt(IDC_EDIT_TIMES,NULL,FALSE);
	m_bShrink = IsDlgButtonChecked(IDC_CHECK_SHRINK);
	m_bToChest = (m_Combo.GetCurSel()==0);
	CDialog::OnOK();
}

BOOL CDupItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Combo.SetCurSel(0);

	m_Spin.SetBuddy(GetDlgItem(IDC_EDIT_TIMES));
	m_Spin.SetRange32(1,5000);

	SetDlgItemInt(IDC_EDIT_TIMES,10,FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
