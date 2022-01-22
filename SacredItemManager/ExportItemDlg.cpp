// ExportItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "ExportItemDlg.h"
#include "SITFile.h"
#include "SITFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportItemDlg dialog


CExportItemDlg::CExportItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pItem = NULL;
}


void CExportItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportItemDlg)
	DDX_Control(pDX, IDC_STATIC_EX_ITEMPIC, m_Picture);
	DDX_Control(pDX, IDOK, m_ButtonOk);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportItemDlg, CDialog)
	//{{AFX_MSG_MAP(CExportItemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportItemDlg message handlers

BOOL CExportItemDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	ASSERT(m_pItem);

	SetDlgItemInt(IDC_EDIT_EX_ITEMID,*m_pItem->GetItemDescId(),FALSE);
	if(m_pItem->GetSlottedArray()->GetSize()>0){
		CheckDlgButton(IDC_CHECK_EX_INCSLOT,1);
	}else{
		
	}
	GetDlgItem(IDC_CHECK_EX_INCSLOT)->EnableWindow(FALSE);

	m_Picture.SetWindowPos(NULL,0,0,ITEM_IMAGE_SIZE_X+6,ITEM_IMAGE_SIZE_Y+6,SWP_NOMOVE);

	return TRUE;
}

void CExportItemDlg::OnOK(){
	
	CString itemName;
	CString itemDesc;
	CString itemAuthor;
	BOOL bIncSlotted;

	GetDlgItemText(IDC_EDIT_EX_ITEMNAME,itemName);
	GetDlgItemText(IDC_EDIT_EX_ITEMDESC,itemDesc);
	GetDlgItemText(IDC_EDIT_EX_ITEMAUTH,itemAuthor);
	bIncSlotted = IsDlgButtonChecked(IDC_CHECK_EX_INCSLOT);

	CSITFile sit;
	sit.SetItem(m_pItem);
	*sit.GetItemName() = itemName;
	*sit.GetItemDesc() = itemDesc;
	*sit.GetItemAuthor() = itemAuthor;
	if(m_Picture.GetImage()->IsValid()){
		sit.GetItemPicture()->Copy(*m_Picture.GetImage());
	}

	// make a decent file name...
	char fileTitle[MAX_PATH];
	if(itemName.GetLength()){
		CString tempName;
		for(int i = 0; i < itemName.GetLength(); i++){
			tempName += itemName.GetAt(i);
			switch(tempName.GetAt(i)){
				case '\\':
				case '/':
				case ':':
				case '*':
				case '?':
				case '\"':
				case '<':
				case '>':
				case '|':
				case ' ':
					tempName.SetAt(i,'_');
					break;
			}
		}
		sprintf(fileTitle,"%s.sit", tempName);
	}else{
		sprintf(fileTitle,"%u.sit", *m_pItem->GetItemDescId());
	}

	CSITFileDlg fileDlg(FALSE,
						"sit",
						fileTitle,
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						"Sacred Items (*.SIT)|*.SIT||",
						this);

	if(fileDlg.DoModal()==IDCANCEL){ return; }

	if(sit.CompileSitFile(fileDlg.GetPathName(),bIncSlotted)){
		MessageBox("Successfully exported item!", "Success...", MB_ICONINFORMATION|MB_OK);
	}else{
		MessageBox("Failed to export item properly!", "Error...", MB_ICONHAND|MB_OK);
		return;
	}

	CDialog::OnOK();
}
