// ViewDumpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "ViewDumpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDumpDlg dialog


CViewDumpDlg::CViewDumpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewDumpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewDumpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pHexCtrl = NULL;
	m_pItem = NULL;
}


void CViewDumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDumpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDumpDlg, CDialog)
	//{{AFX_MSG_MAP(CViewDumpDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDumpDlg message handlers

BOOL CViewDumpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(m_pItem);

	// get placement of old window
	CRect rc;
	CWnd* pOldWnd = GetDlgItem(IDC_HEXEDIT_VIEW);
	pOldWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	pOldWnd->DestroyWindow();

	// create our control
	m_pHexCtrl = new CPPDumpCtrl();
	m_pHexCtrl->Create(rc, this, IDC_HEXEDIT_VIEW);
	m_pHexCtrl->SetReadOnly(TRUE);
	m_pHexCtrl->ModifyStyles(PPDUMP_SEPARATOR_LINES|PPDUMP_NAMED_FIELDS|PPDUMP_FIELD_HEX,PPDUMP_FIELD_DEC|/*PPDUMP_BAR_ADDRESS|*/PPDUMP_BAR_HEX|/*PPDUMP_BAR_DEC|PPDUMP_BAR_BIN|*/PPDUMP_BAR_OCT|PPDUMP_BAR_ASCII);
	m_pHexCtrl->SetSpecialCharView('.');
	
	m_pHexCtrl->SetPointerData(m_pItem->GetItemData()->size,m_pItem->GetItemData()->buf);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewDumpDlg::OnDestroy(){
	if(m_pHexCtrl){
		m_pHexCtrl->DestroyWindow();
		SAFE_DELETE(m_pHexCtrl);
	}
	CDialog::OnDestroy();	
}
