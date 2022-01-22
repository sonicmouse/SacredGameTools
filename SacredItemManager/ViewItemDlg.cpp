// ViewItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "ViewItemDlg.h"
#include "SIMDlg.h"
#include "ViewDumpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewItemDlg dialog


CViewItemDlg::CViewItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewItemDlg)
	//}}AFX_DATA_INIT
	m_pSit = NULL;
}


void CViewItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewItemDlg)
	DDX_Control(pDX, IDC_LIST_VI_ATTRLIST, m_List);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
	DDX_Control(pDX, IDC_BUTTON_VI_VIEWDUMP, m_ButtonViewDump);
	DDX_Control(pDX, IDC_STATIC_VI_PICTURE, m_Picture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewItemDlg, CDialog)
	//{{AFX_MSG_MAP(CViewItemDlg)
	ON_BN_CLICKED(IDC_BUTTON_VI_VIEWDUMP, OnButtonViViewdump)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_VI_ATTRLIST, OnRclickListViAttrlist)
	ON_COMMAND(ID_POPUP_COPY, OnPopupCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewItemDlg message handlers

BOOL CViewItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ASSERT(m_pSit);
	
	SetIcon(((CSIMDlg*)::AfxGetMainWnd())->GetMyIcon(),TRUE);
	SetIcon(((CSIMDlg*)::AfxGetMainWnd())->GetMyIcon(),FALSE);

	//if(!GetParent() || !GetParent()->IsWindowVisible()){
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	//}

	m_Picture.SetEditable(FALSE);
	m_Picture.SetWindowPos(NULL,0,0,ITEM_IMAGE_SIZE_X+6,ITEM_IMAGE_SIZE_Y+6,SWP_NOMOVE);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,"Attribute"); m_List.SetColumnWidth(0,193);
	m_List.InsertColumn(1,"Value"); m_List.SetColumnWidth(1,50);

	SetDlgItemInt(IDC_EDIT_VI_DESCID,*m_pSit->GetItem()->GetItemDescId(),FALSE);
	SetDlgItemText(IDC_EDIT_VI_NAME,*m_pSit->GetItemName());
	SetDlgItemText(IDC_EDIT_VI_ITEMDESC,*m_pSit->GetItemDesc());
	SetDlgItemText(IDC_EDIT_VI_AUTHOR,*m_pSit->GetItemAuthor());

	CString txt;
	txt.Format("Size: %ux%u; Slotted: %u", m_pSit->GetItem()->GetItemInvSize()->cx, m_pSit->GetItem()->GetItemInvSize()->cy, m_pSit->GetItem()->GetSlottedArray()->GetSize());
	SetDlgItemText(IDC_STATIC_VI_INVSIZE,txt);

	m_Picture.SetImage(m_pSit->GetItemPicture());

	// attributes
	CPtrArray arr;
	LoadAttrsFile(&arr);

	int nCountAttrs = 0;
	if(m_pSit->GetItem()->GetItemData()->size == 130){
		for(int i = 0; i < 8; i++){
			ULONG attrId = *(ULONG*)&m_pSit->GetItem()->GetItemData()->buf[0x4A+(i*sizeof(ULONG))];
			short attrVal = *(short*)&m_pSit->GetItem()->GetItemData()->buf[0x6A+(i*sizeof(short))];
			
			if(attrId){
				++nCountAttrs;
				PST_ATTR pFoundAttr = NULL;
				for(int a = 0; a < arr.GetSize(); a++){
					PST_ATTR pAttr = (PST_ATTR)arr.GetAt(a);
					if(pAttr->id == attrId){
						pFoundAttr = pAttr;
						break;
					}
				}
				int nInd;
				if(pFoundAttr){
					nInd = m_List.InsertItem(m_List.GetItemCount(),pFoundAttr->txt);
				}else{
					nInd = m_List.InsertItem(m_List.GetItemCount(),IToS(attrId) + " [No text... yet]");
				}
				m_List.SetItemText(nInd,1,IToS(attrVal));
			}
		}
	}

	txt.Format("Item Attributes [%u/%u]",nCountAttrs,8);
	SetDlgItemText(IDC_STATIC_VI_FRAMEATTRS,txt);

	for(int i = 0; i < arr.GetSize(); i++){
		PST_ATTR pAttr = (PST_ATTR)arr.GetAt(i);
		SAFE_DELETE(pAttr);
	}
	arr.RemoveAll();

	SetDlgItemInt(IDC_EDIT_VI_MINLVL,m_pSit->GetItem()->GetItemData()->buf[0x14],FALSE);
	SetDlgItemInt(IDC_EDIT_VI_LVL,m_pSit->GetItem()->GetItemData()->buf[0x19],FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewItemDlg::OnButtonViViewdump(){
	CViewDumpDlg dlg(this);
	dlg.SetItem(m_pSit->GetItem());
	dlg.DoModal();
}

void CViewItemDlg::LoadAttrsFile(CPtrArray* pArr){
	char myPath[MAX_PATH];
	GetModuleFileName(NULL,myPath,MAX_PATH);
	strrchr(myPath,'\\')[0] = 0;
	strcat(myPath,SIM_ITEM_ATTR_FILE);

	FILE* f = fopen(myPath,"rb");
	if(f){
		fseek(f,0,SEEK_END);
		ULONG flen = ftell(f);
		rewind(f);
		char* data = new char[flen+1];
		data[flen]=0;
		if(data){
			if(fread(data,flen,1,f)==1){
				char* tok = strtok(data,"\n");
				while(tok){
					if(!isdigit(tok[0])){
						// comment line
					}else{
						char* mid = strchr(tok,'=');
						if(mid){
							mid[0] = 0;
							++mid;
							CString attrNum(tok);
							CString attrData(mid);
							attrNum.TrimLeft(); attrNum.TrimRight();
							attrData.TrimLeft(); attrData.TrimRight();

							PST_ATTR pAttr = new ST_ATTR;
							pAttr->id = DWORD(atoi(attrNum));
							pAttr->txt = attrData;
							pArr->Add(pAttr);
						}else{
							// not a valid line
						}
					}
					tok = strtok(NULL,"\n");
				}
			}else{
				// failed to read data
			}
			SAFE_DELETE(data);
		}else{
			// failed to alloc data
		}
		fclose(f);
	}else{
		// failed to open file
	}	
}


void CViewItemDlg::OnRclickListViAttrlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_List.GetItemCount()){
		CMenu menu;
		menu.LoadMenu(IDR_MENU_VIEWITEM_COPY_POPUP);
		CMenu* sub = menu.GetSubMenu(0);
		POINT pt;
		::GetCursorPos(&pt);
		sub->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	}

	*pResult = 0;
}

void CViewItemDlg::OnPopupCopy() 
{
	CString txt;
	for(int i = 0; i < m_List.GetItemCount(); i++){
		txt += m_List.GetItemText(i,0) + CString(": ") + m_List.GetItemText(i,1) + CString("\r\n");
	}
	ClipboardSetText(txt);
}

BOOL CViewItemDlg::ClipboardSetText(LPCTSTR lpszBuffer){
	BOOL bSuccess = FALSE;
	if (::OpenClipboard(NULL)){
		::EmptyClipboard();
		int nSize = _tcslen(lpszBuffer);
		HGLOBAL hGlobal = ::GlobalAlloc(GMEM_ZEROINIT, (nSize+1)*sizeof(TCHAR));
		if (hGlobal){
			LPTSTR lpszData = (LPTSTR) ::GlobalLock(hGlobal);
			if (lpszData){
				_tcscpy(lpszData, lpszBuffer);
				::GlobalUnlock(hGlobal);
#ifdef _UNICODE
				::SetClipboardData(CF_UNICODETEXT, hGlobal);
#else
				::SetClipboardData(CF_TEXT, hGlobal);
#endif
				bSuccess = TRUE;
			}
		}
		::CloseClipboard();
	}
	return bSuccess;
}