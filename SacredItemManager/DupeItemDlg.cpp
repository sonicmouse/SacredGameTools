// DupeItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "DupeItemDlg.h"
#include "SIMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDupeItemDlg dialog


CDupeItemDlg::CDupeItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDupeItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDupeItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pItem = NULL;
	m_bShrinkItem = FALSE;
	m_bCarrySlots = TRUE;
	m_bToBackpack = TRUE;
	m_nTotalDupes = 1;
}


void CDupeItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDupeItemDlg)
	DDX_Control(pDX, IDC_COMBO_DUPETO, m_ComboTo);
	DDX_Control(pDX, IDOK, m_ButtonOk);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
	DDX_Control(pDX, IDC_SPIN_DUPETOTAL, m_SpinTotal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDupeItemDlg, CDialog)
	//{{AFX_MSG_MAP(CDupeItemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDupeItemDlg message handlers

BOOL CDupeItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ASSERT(m_pItem != NULL);

	m_ComboTo.SetCurSel(1);

	m_SpinTotal.SetBuddy(GetDlgItem(IDC_EDIT_DUPE_TOTAL));
	m_SpinTotal.SetRange(1,200);
	SetDlgItemText(IDC_EDIT_DUPE_TOTAL,"10");
	
	if(m_pItem->GetSlottedArray()->GetSize()>0){
		CheckDlgButton(IDC_CHECK_INC_SLOTTED_ITEMS,1);
	}else{
		
	}
	GetDlgItem(IDC_CHECK_INC_SLOTTED_ITEMS)->EnableWindow(FALSE);

	if(m_pItem->GetItemInvSize()->cx <= 1 && m_pItem->GetItemInvSize()->cy <= 1){
		GetDlgItem(IDC_CHECK_SHRINK)->EnableWindow(FALSE);
	}

	CSIMDlg* pDlg = (CSIMDlg*)AfxGetMainWnd();
	GetDlgItem(IDC_STATIC_FRAME_HMT)->SetFont(pDlg->GetFontBold());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDupeItemDlg::OnOK(){
	m_bShrinkItem = IsDlgButtonChecked(IDC_CHECK_SHRINK);
	m_bCarrySlots = IsDlgButtonChecked(IDC_CHECK_INC_SLOTTED_ITEMS);
	m_bToBackpack = (m_ComboTo.GetCurSel()==1);
	m_nTotalDupes = GetDlgItemInt(IDC_EDIT_DUPE_TOTAL,NULL,FALSE);
	CDialog::OnOK();
}
