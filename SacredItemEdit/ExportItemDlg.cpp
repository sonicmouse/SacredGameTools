// ExportItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
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
	m_pBuffer = NULL;
	m_invSize.cx = 1;
	m_invSize.cy = 1;
}

void CExportItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportItemDlg)
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_Picture);
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
	
	ASSERT(m_pBuffer);
	
	SetDlgItemInt(IDC_EDIT_ID,*(ULONG*)m_pBuffer->buf,FALSE);

	m_Picture.SetWindowPos(NULL,0,0,ITEM_IMAGE_SIZE_X+6,ITEM_IMAGE_SIZE_Y+6,SWP_NOMOVE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExportItemDlg::OnOK(){

	CString strName, strDesc, strAuthor;
	GetDlgItemText(IDC_EDIT_NAME,strName);
	GetDlgItemText(IDC_EDIT_DESC,strDesc);
	GetDlgItemText(IDC_EDIT_AUTHOR,strAuthor);

	strName.TrimLeft(); strName.TrimRight();
	strDesc.TrimLeft(); strDesc.TrimRight();
	strAuthor.TrimLeft(); strAuthor.TrimRight();

	char fileTitle[MAX_PATH];
	if(strName.GetLength()){
		CString tempName;
		for(int i = 0; i < strName.GetLength(); i++){
			tempName += strName.GetAt(i);
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
		sprintf(fileTitle,"%u.sit", *(ULONG*)m_pBuffer->buf);
	}

	CSITFileDlg fileDlg(FALSE,
						"sit",
						fileTitle,
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						"Sacred Items (*.SIT)|*.SIT||",
						this);
	if(fileDlg.DoModal()==IDCANCEL){ return; }

	CString rc = fileDlg.GetPathName();

	ST_BUFFER picBuf;
	m_Picture.GetImageData(&picBuf);

	CSITFile sit;

	sit.SetItem(m_pBuffer);
	sit.SetItemAuthor(strAuthor);
	sit.SetItemDescription(strDesc);
	sit.SetItemName(strName);
	sit.SetItemPicture(&picBuf);
	sit.SetItemInvSize(m_invSize);

	CxImage temp; temp.FreeMemory(picBuf.buf);

	if(sit.CompileSITFile(rc)){
		MessageBox("Successfully exported this item!","Success...", MB_ICONINFORMATION|MB_OK);
	}else{
		MessageBox("Failed to export item!", "Error...", MB_ICONHAND|MB_OK);
	}

	CDialog::OnOK();
}

