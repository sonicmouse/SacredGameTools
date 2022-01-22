// EditItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "EditItemDlg.h"
#include "SIMDlg.h"
#include "ItemEditHelperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditItemDlg dialog


CEditItemDlg::CEditItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditItemDlg)
	//}}AFX_DATA_INIT
	m_pHexCtrl = NULL;
	m_pOrigItem = NULL;
	m_LastSel.buf = NULL;
	m_LastSel.size = 0;
}


void CEditItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditItemDlg)
	DDX_Control(pDX, IDC_BUTTON_HELPER, m_ButtonHelper);
	DDX_Control(pDX, IDC_COMBO_BYTEORDER, m_ComboBo);
	DDX_Control(pDX, IDC_LIST_TRANSLATIONS, m_List);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
	DDX_Control(pDX, IDC_BUTTON_RESET_BUFFER, m_ButtonReset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditItemDlg, CDialog)
	//{{AFX_MSG_MAP(CEditItemDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_RESET_BUFFER, OnButtonResetBuffer)
	ON_CBN_SELCHANGE(IDC_COMBO_BYTEORDER, OnSelchangeComboByteorder)
	ON_BN_CLICKED(IDC_CHECK_DECIMAL_DATA_VIEW, OnCheckDecimalDataView)
	ON_COMMAND(IDM_EDITITEM_COPY, OnEdititemCopy)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TRANSLATIONS, OnRclickListTranslations)
	ON_BN_CLICKED(IDC_BUTTON_HELPER, OnButtonHelper)
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDM_PPDUMPCTRL_CHANGE_DATA, IDC_HEX_EDIT_CTRL, NotifyEditDataHexEditCtrl)
	ON_NOTIFY(UDM_PPDUMPCTRL_SELECTION, IDC_HEX_EDIT_CTRL, NotifySelectDataHexEditCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditItemDlg message handlers

BOOL CEditItemDlg::OnInitDialog() 
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

	// set title
	CString txt;
	txt.Format("Edit Item [Item Title: %u - Size %u bytes]", *m_pOrigItem->GetItemDescId(),m_pOrigItem->GetItemData()->size);
	SetWindowText(txt);

	// set up the interface...
	OnButtonResetBuffer();

	// set up list font
	m_ConvertFont.CreatePointFont(80,"MS Serif");

	// set up list ctrl
	m_List.SetFont(&m_ConvertFont);
	m_List.InsertColumn(0,"type"); m_List.SetColumnWidth(0,40);
	m_List.InsertColumn(1,"val"); m_List.SetColumnWidth(1,120);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// deco fonts...
	CFont* pFontBold = ((CSIMDlg*)::AfxGetMainWnd())->GetFontBold();
	GetDlgItem(IDC_STATIC_FNT)->SetFont(pFontBold);
	GetDlgItem(IDOK)->SetFont(pFontBold);
	GetDlgItem(IDC_BUTTON_HELPER)->SetFont(pFontBold);

	m_ComboBo.SetCurSel(1);

	GetDlgItem(IDC_BUTTON_HELPER)->EnableWindow(m_pOrigItem->GetItemData()->size == 130);

	return TRUE;
}

void CEditItemDlg::NotifyEditDataHexEditCtrl(NMHDR * pNMHDR, LRESULT * result){
    *result = 0;
	NM_PPDUMP_CTRL* pNotify = (NM_PPDUMP_CTRL*)pNMHDR;
	m_NewItem.GetItemData()->buf[pNotify->iAddress] = pNotify->iValue;
    m_pHexCtrl->RedrawWindow();
}

void CEditItemDlg::NotifySelectDataHexEditCtrl(NMHDR * pNMHDR, LRESULT * result){
    *result = 0;
	NM_PPDUMP_SEL* pNotify = (NM_PPDUMP_SEL*)pNMHDR;

	int begin = pNotify->iFirstAddr;
	int end = pNotify->iLastAddr + 1;
	int size = end-begin;

	SAFE_DELETE(m_LastSel.buf);
	m_LastSel.buf = new UCHAR[size];
	m_LastSel.size = size;

	memcpy(m_LastSel.buf,&m_NewItem.GetItemData()->buf[begin],size);

	UpdateConvertList();
}

void CEditItemDlg::OnDestroy(){
	if(m_pHexCtrl){
		m_pHexCtrl->DestroyWindow();
		SAFE_DELETE(m_pHexCtrl);
	}
	SAFE_DELETE(m_LastSel.buf);
	m_ConvertFont.DeleteObject();
	CDialog::OnDestroy();
}

void CEditItemDlg::OnButtonResetBuffer(){
	m_NewItem = *m_pOrigItem;
	m_OldItem = *m_pOrigItem;
	m_pHexCtrl->SetPointerData(m_pOrigItem->GetItemData()->size,m_NewItem.GetItemData()->buf, m_OldItem.GetItemData()->buf);
	SAFE_DELETE(m_LastSel.buf);
	m_LastSel.size = 0;
	UpdateConvertList();
}

void CEditItemDlg::UpdateConvertList(){

	m_List.DeleteAllItems();

	if(!m_LastSel.buf || !m_LastSel.size){ return; }

	ST_BUFFER rev;
	ReverseBuffer(&m_LastSel,&rev);

	bool bRev = (m_ComboBo.GetCurSel()!=1);

	// sel length
	int nInd = m_List.InsertItem(m_List.GetItemCount(),"Length");
	m_List.SetItemText(nInd,1,UToS(m_LastSel.size));

	// hex string...
	CString hex;
	for(int i = 0; i < (int)m_LastSel.size; i++){
		char hexTxt[3];
		wsprintf(hexTxt,"%02X", (bRev?rev.buf[i]:m_LastSel.buf[i]));
		hex += hexTxt;
	}
	nInd = m_List.InsertItem(m_List.GetItemCount(),"Hex");
	m_List.SetItemText(nInd,1,hex);

	// char...
	nInd = m_List.InsertItem(m_List.GetItemCount(),"Char");
	m_List.SetItemText(nInd,1,SToS(*(CHAR*)(bRev?rev.buf:m_LastSel.buf)));

	// uchar...
	nInd = m_List.InsertItem(m_List.GetItemCount(),"UChar");
	m_List.SetItemText(nInd,1,UToS(*(UCHAR*)(bRev?rev.buf:m_LastSel.buf)));

	if(m_LastSel.size >= sizeof(short)){
		// short
		nInd = m_List.InsertItem(m_List.GetItemCount(),"Short");
		m_List.SetItemText(nInd,1,SToS(*(short*)(bRev?rev.buf:m_LastSel.buf)));
		
		// ushort...
		nInd = m_List.InsertItem(m_List.GetItemCount(),"UShort");
		m_List.SetItemText(nInd,1,UToS(*(USHORT*)(bRev?rev.buf:m_LastSel.buf)));
	}

	if(m_LastSel.size >= sizeof(ULONG)){
		// long
		nInd = m_List.InsertItem(m_List.GetItemCount(),"Long");
		m_List.SetItemText(nInd,1,SToS(*(int*)(bRev?rev.buf:m_LastSel.buf)));

		// ulong
		nInd = m_List.InsertItem(m_List.GetItemCount(),"ULong");
		m_List.SetItemText(nInd,1,UToS(*(ULONG*)(bRev?rev.buf:m_LastSel.buf)));

		// float
		nInd = m_List.InsertItem(m_List.GetItemCount(),"Float");
		CString fl;
		fl.Format("%f", *(float*)(bRev?rev.buf:m_LastSel.buf));
		m_List.SetItemText(nInd,1,fl);
	}

	SAFE_DELETE(rev.buf);
}

void CEditItemDlg::ReverseBuffer(PST_BUFFER src, PST_BUFFER dst){
	dst->size = src->size;
	dst->buf = new UCHAR[src->size];
	for(int i = (int)src->size; i > 0; i--){
		dst->buf[src->size-i] = src->buf[i-1];
	}
}

void CEditItemDlg::OnSelchangeComboByteorder(){
	UpdateConvertList();
}

void CEditItemDlg::OnCheckDecimalDataView(){
	int begin, end;
	m_pHexCtrl->GetSelectRange(&begin,&end);
	if(IsDlgButtonChecked(IDC_CHECK_DECIMAL_DATA_VIEW)){
		m_pHexCtrl->ModifyStyles(PPDUMP_FIELD_DEC,PPDUMP_FIELD_HEX);
	}else{
		m_pHexCtrl->ModifyStyles(PPDUMP_FIELD_HEX,PPDUMP_FIELD_DEC);
	}
	m_pHexCtrl->SetSelectRange(begin,end);	
}

BOOL CEditItemDlg::IsNumber(const CString& szVal){
	for(int i = 0; i < szVal.GetLength(); i++){
		if(!isdigit(szVal.GetAt(i))){
			return FALSE;
		}
	}
	return TRUE;
}

void CEditItemDlg::OnOK(){

	// see if there really was a change...
	if( (memcmp(m_pOrigItem->GetItemData()->buf, m_NewItem.GetItemData()->buf, m_pOrigItem->GetItemData()->size)==0) ){
		CDialog::OnCancel();
		return;
	}

	memcpy(m_pOrigItem->GetItemData()->buf, m_NewItem.GetItemData()->buf, m_pOrigItem->GetItemData()->size);

	CDialog::OnOK();
}

void CEditItemDlg::OnEdititemCopy(){
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nIndex = m_List.GetNextSelectedItem(pos);
	if(nIndex != -1){
		CString txt = m_List.GetItemText(nIndex,1);
		ClipboardSetText(txt);
	}
}

BOOL CEditItemDlg::ClipboardSetText(LPCTSTR lpszBuffer){
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

void CEditItemDlg::OnRclickListTranslations(NMHDR* pNMHDR, LRESULT* pResult){
	if(m_List.GetSelectedCount()){
		POINT pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU_EDITITEM_COPY_POPUP);
		CMenu* pSub = menu.GetSubMenu(0);
		pSub->TrackPopupMenu(0,pt.x,pt.y,this);
		menu.DestroyMenu();
	}

	*pResult = 0;
}

void CEditItemDlg::OnButtonHelper() 
{
	if(m_NewItem.GetItemData()->size != 130){ return; }

	CItemEditHelperDlg dlg(this);
	dlg.SetItem(&m_NewItem);
	if(dlg.DoModal()==IDCANCEL){ OnButtonResetBuffer(); return; }
	m_pHexCtrl->RedrawWindow();
}
