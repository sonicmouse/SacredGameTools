// ImportItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "ImportItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportItemDlg dialog


CImportItemDlg::CImportItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pHero = NULL;
}

void CImportItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportItemDlg)
	DDX_Control(pDX, IDC_COMBO_IMPORTTO, m_ComboTo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImportItemDlg, CDialog)
	//{{AFX_MSG_MAP(CImportItemDlg)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_BROWSE, OnButtonImportBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportItemDlg message handlers

BOOL CImportItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_ComboTo.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportItemDlg::OnButtonImportBrowse(){
	CSITFileDlg fileDlg(TRUE,
						"",
						"",
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						"Sacred Items (*.SIT)|*.SIT||",
						this);
	if(fileDlg.DoModal()==IDCANCEL){ return; }

	CSITFile sit;
	if(!sit.SetSITFile(fileDlg.GetPathName())){
		MessageBox("This is not a valid SIT file!", "Error...", MB_ICONHAND|MB_OK);
		return;
	}

	SetDlgItemText(IDC_EDIT_IMPORTITEM,fileDlg.GetPathName());
}

void CImportItemDlg::OnOK(){
	
	CString txt;
	GetDlgItemText(IDC_EDIT_IMPORTITEM,txt);
	
	CSITFile sit;
	if(!txt.GetLength() || !sit.SetSITFile(txt)){
		MessageBox("This is not a valid SIT file!", "Error...", MB_ICONHAND|MB_OK);
		return;
	}

	if(sit.GetItemInvSize().cx == 0 || sit.GetItemInvSize().cy == 0){
		sit.SetItemInvSize(CSize(1,1));
	}

	CSize sz = CSize(1,1);

	if(!IsDlgButtonChecked(IDC_CHECK_IMPORT_SHRINK)){
		sz = sit.GetItemInvSize();
	}

	UINT rc = m_pHero->ImportItem(sit.GetItem(),sz,(m_ComboTo.GetCurSel()==0?SECTION_PLACEMENT_BP:SECTION_PLACEMENT_CH));

	if(rc == 0){
		MessageBox("There was an error in trying to import this item.\n\nMaybe the selected inventory is full.", "Error...", MB_ICONHAND|MB_OK);
		return;
	}

	CDialog::OnOK();
}
