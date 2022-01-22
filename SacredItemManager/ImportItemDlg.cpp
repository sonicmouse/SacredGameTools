// ImportItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "ImportItemDlg.h"
#include "SIMDlg.h"
#include "SITFileDlg.h"
#include "SITFile.h"

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
}


void CImportItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportItemDlg)
	DDX_Control(pDX, IDC_COMBO_IMPORTITEM_TO, m_ComboTo);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
	DDX_Control(pDX, IDC_BUTTON_BROWSEIMPORTITEM, m_Browse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportItemDlg, CDialog)
	//{{AFX_MSG_MAP(CImportItemDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEIMPORTITEM, OnButtonBrowseimportitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportItemDlg message handlers

BOOL CImportItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSIMDlg* pDlg = (CSIMDlg*)AfxGetMainWnd();

	GetDlgItem(IDC_STATIC_ITIFR)->SetFont(pDlg->GetFontBold());
	GetDlgItem(IDOK)->SetFont(pDlg->GetFontBold());

	m_ComboTo.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportItemDlg::OnButtonBrowseimportitem(){
	CSITFileDlg fileDlg(TRUE,
						"",
						"",
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						"Sacred Items (*.SIT)|*.SIT||",
						this);
	if(fileDlg.DoModal()==IDCANCEL){ return; }
	SetDlgItemText(IDC_EDIT_ITEM_TO_IMPORT,fileDlg.GetPathName());
}

void CImportItemDlg::OnOK() 
{
	CString target;
	GetDlgItemText(IDC_EDIT_ITEM_TO_IMPORT,target);

	CSITFile sit;
	if(!sit.DecompileSitFile(target)){
		MessageBox("Unable to load this item!", "Error...", MB_ICONHAND|MB_OK);
		return;
	}

	m_Item = *sit.GetItem();
	*m_Item.GetItemInvStorage() = (m_ComboTo.GetCurSel()==0?SECTION_PLACEMENT_BP:SECTION_PLACEMENT_CH);

	if( (m_Item.GetItemInvSize()->cx <= 0) ||
		(m_Item.GetItemInvSize()->cy <= 0) ||
		IsDlgButtonChecked(IDC_CHECK_SHRINK_IMPORT) ){
		*m_Item.GetItemInvSize() = CSize(1,1);
	}
	
	CDialog::OnOK();
}
