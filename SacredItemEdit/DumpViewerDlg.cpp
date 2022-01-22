// DumpViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "DumpViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDumpViewerDlg dialog


CDumpViewerDlg::CDumpViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDumpViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDumpViewerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBuffer = NULL;
	m_pHexCtrl = NULL;
}


void CDumpViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDumpViewerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDumpViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CDumpViewerDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDumpViewerDlg message handlers

BOOL CDumpViewerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(m_pBuffer);

	// get placement of old window
	CRect rc;
	CWnd* pOldWnd = GetDlgItem(IDC_STATIC_DUMPER);
	pOldWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	pOldWnd->DestroyWindow();

	// create our control
	m_pHexCtrl = new CPPDumpCtrl();
	m_pHexCtrl->Create(rc, this, IDC_STATIC_DUMPER);
	m_pHexCtrl->SetReadOnly(TRUE);
	m_pHexCtrl->ModifyStyles(PPDUMP_SEPARATOR_LINES|PPDUMP_NAMED_FIELDS|PPDUMP_FIELD_HEX,PPDUMP_FIELD_DEC|/*PPDUMP_BAR_ADDRESS|*/PPDUMP_BAR_HEX|/*PPDUMP_BAR_DEC|PPDUMP_BAR_BIN|*/PPDUMP_BAR_OCT|PPDUMP_BAR_ASCII);
	m_pHexCtrl->SetSpecialCharView('.');
	
	m_pHexCtrl->SetPointerData(m_pBuffer->size,m_pBuffer->buf);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDumpViewerDlg::OnDestroy() 
{
	if(m_pHexCtrl){
		m_pHexCtrl->DestroyWindow();
		SAFE_DELETE(m_pHexCtrl);
	}
	CDialog::OnDestroy();	
}
