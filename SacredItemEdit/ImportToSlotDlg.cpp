// ImportToSlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "ImportToSlotDlg.h"
#include "SITFileDlg.h"
#include "SITFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportToSlotDlg dialog


CImportToSlotDlg::CImportToSlotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportToSlotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportToSlotDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pHero = NULL;
}


void CImportToSlotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportToSlotDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportToSlotDlg, CDialog)
	//{{AFX_MSG_MAP(CImportToSlotDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_ITOS, OnButtonBrowseItos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportToSlotDlg message handlers

BOOL CImportToSlotDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(m_pHero != NULL);
	
	CheckDlgButton(IDC_RADIO_GREEN,1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportToSlotDlg::OnButtonBrowseItos() 
{
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

	SetDlgItemText(IDC_EDIT_ITEMSLOT,fileDlg.GetPathName());	
}

void CImportToSlotDlg::OnOK(){
	
	CString txt;
	GetDlgItemText(IDC_EDIT_ITEMSLOT,txt);
	
	CSITFile sit;
	if(!txt.GetLength() || !sit.SetSITFile(txt)){
		MessageBox("This is not a valid SIT file!", "Error...", MB_ICONHAND|MB_OK);
		return;
	}

	m_pHero->ImportItemToNextAvalibleSlot(m_ulItemId,sit.GetItem(),GetSelectedColor());

	CDialog::OnOK();
}

int CImportToSlotDlg::GetSelectedColor(){
	if(IsDlgButtonChecked(IDC_RADIO_BRONZE)){
		return SLOT_COLOR_BRONZE;
	}else if(IsDlgButtonChecked(IDC_RADIO_SILVER)){
		return SLOT_COLOR_SILVER;
	}else if(IsDlgButtonChecked(IDC_RADIO_GOLD)){
		return SLOT_COLOR_GOLD;
	}else if(IsDlgButtonChecked(IDC_RADIO_GREEN)){
		return SLOT_COLOR_GREEN;
	}else if(IsDlgButtonChecked(IDC_RADIO_PLATINUM)){
		return SLOT_COLOR_PLATINUM;
	}
	return SLOT_COLOR_GREEN;
}
