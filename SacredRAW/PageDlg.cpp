// PageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredRAW.h"
#include "PageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDlg dialog


CPageDlg::CPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBuffer = NULL;
	m_nSize = 0;

	m_pHexCtrl = NULL;

	m_pOldBuffer = NULL;
	m_nOldSize = 0;
}

void CPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDlg, CDialog)
	//{{AFX_MSG_MAP(CPageDlg)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDlg message handlers

void CPageDlg::OnOK(){}
void CPageDlg::OnCancel(){}

void CPageDlg::OnDestroy(){
	if(m_pHexCtrl){
		m_pHexCtrl->DestroyWindow();
		delete m_pHexCtrl;
	}
	if(m_pBuffer){
		delete m_pBuffer;
	}
	if(m_pOldBuffer){
		delete m_pOldBuffer;
	}
	CDialog::OnDestroy();	
}

void CPageDlg::SetPageData(const UCHAR* pData, int nSize){
	if(m_pBuffer){
		delete m_pBuffer;
	}
	m_pBuffer = new UCHAR[nSize];
	memcpy(m_pBuffer,pData,nSize);
	m_nSize = nSize;

	if(m_pOldBuffer){
		delete m_pOldBuffer;
	}
	m_pOldBuffer = new UCHAR[nSize];
	memcpy(m_pOldBuffer,pData,nSize);
	m_nOldSize = nSize;

}

BOOL CPageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// get placement of old window
	CRect rc;
	CWnd* pOldWnd = GetDlgItem(IDC_HEX_EDIT_CTRL);
	pOldWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	pOldWnd->DestroyWindow();

	// create our control
	m_pHexCtrl = new CPPDumpCtrl();
	m_pHexCtrl->Create(rc, this, IDC_HEX_EDIT_CTRL);
	m_pHexCtrl->SetNotify();
	m_pHexCtrl->EnableSelect();
	m_pHexCtrl->SetReadOnly(FALSE);
	m_pHexCtrl->ModifyStyles(PPDUMP_SEPARATOR_LINES|PPDUMP_NAMED_FIELDS|PPDUMP_FIELD_HEX,PPDUMP_FIELD_DEC|PPDUMP_READ_ONLY|/*PPDUMP_BAR_ADDRESS|*/PPDUMP_BAR_HEX|/*PPDUMP_BAR_DEC|PPDUMP_BAR_BIN|*/PPDUMP_BAR_OCT|PPDUMP_BAR_ASCII);
	m_pHexCtrl->SetSpecialCharView('.');
	
	// set up hex ctrl
	m_pHexCtrl->SetPointerData(m_nSize,m_pBuffer,m_pOldBuffer);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(!m_pHexCtrl || !::IsWindow(m_pHexCtrl->GetSafeHwnd())){ return; }
	m_pHexCtrl->SetWindowPos(NULL,5,5,cx-10,cy-10,SWP_NOACTIVATE|SWP_NOZORDER);
}
