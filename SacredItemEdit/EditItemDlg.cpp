// EditItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "EditItemDlg.h"

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
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pHexCtrl = NULL;
	m_ToolTipFont.CreatePointFont(80,"Courier");
	m_ConvertFont.CreatePointFont(80,"MS Serif");
	m_pOrigData = NULL;
	m_pOrigHeader = NULL;
	m_pOldData = NULL;
	m_pNewData = NULL;
	m_pEditOffsetNew = NULL;
	m_LastSel.buf = NULL;
	m_LastSel.size = 0;
}


void CEditItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditItemDlg)
	DDX_Control(pDX, IDC_COMBO_VIEW, m_ComboView);
	DDX_Control(pDX, IDC_COMBO_BYTEORDER, m_ComboBo);
	DDX_Control(pDX, IDC_LIST_CONVERT, m_Convert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditItemDlg, CDialog)
	//{{AFX_MSG_MAP(CEditItemDlg)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_BYTEORDER, OnSelchangeComboByteorder)
	ON_BN_CLICKED(IDC_CHECK_DEC, OnCheckDec)
	ON_CBN_SELCHANGE(IDC_COMBO_VIEW, OnSelchangeComboView)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_UPDATE_COMMAND_UI(IDM_COPY, OnUpdateCopy)
	ON_COMMAND(IDM_COPY, OnCopy)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CONVERT, OnRclickListConvert)
	ON_BN_CLICKED(IDC_BUTTON_IM_HEADER, OnButtonImHeader)
	ON_BN_CLICKED(IDC_BUTTON_IM_DATA, OnButtonImData)
	ON_BN_CLICKED(IDC_BUTTON_EX_HEADER, OnButtonExHeader)
	ON_BN_CLICKED(IDC_BUTTON_EX_DATA, OnButtonExData)
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDM_PPDUMPCTRL_CHANGE_DATA, IDC_HEX_EDIT_CTRL, NotifyEditDataHexEditCtrl)
	ON_NOTIFY(UDM_PPDUMPCTRL_SELECTION, IDC_HEX_EDIT_CTRL, NotifySelectDataHexEditCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditItemDlg message handlers

BOOL CEditItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(m_pOrigData!=NULL);

	m_ComboBo.SetCurSel(0);
	m_ComboView.SetCurSel(0);

	//if(!m_pOrigHeader){
		m_ComboView.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EX_HEADER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IM_HEADER)->EnableWindow(FALSE);
	//}

	// hex edit control details:
	// http://www.codeproject.com/miscctrl/cppdumpctrl.asp

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
	//m_pHexCtrl->SetTooltipText("Addr:%t0x%R%nData -----------%nHex:%t0x%H%nDec:%t%D%nBin:%t%B%");
	//m_pHexCtrl->GetTooltip()->SetFont(&m_ToolTipFont);
	
	m_Convert.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_Convert.SetFont(&m_ConvertFont);
	m_Convert.InsertColumn(0,"type");m_Convert.SetColumnWidth(0,40);
	m_Convert.InsertColumn(1,"val");m_Convert.SetColumnWidth(1,120);

	OnButtonReset();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditItemDlg::OnButtonReset() 
{
	SAFE_DELETE(m_pOldData);
	SAFE_DELETE(m_pNewData);

	m_ulDataSize = m_pOrigData->payload.size;
	if(m_pOrigHeader){
		m_ulDataSize += sizeof(*m_pOrigHeader);
	}
	m_pOldData = new UCHAR[m_ulDataSize];
	m_pNewData = new UCHAR[m_ulDataSize];

	int addTo = 0;
	if(m_pOrigHeader){
		memcpy(m_pOldData, m_pOrigHeader, sizeof(*m_pOrigHeader));
		memcpy(m_pNewData, m_pOrigHeader, sizeof(*m_pOrigHeader));
		addTo = sizeof(*m_pOrigHeader);
	}
	memcpy(m_pOldData + addTo, m_pOrigData->payload.buf, m_pOrigData->payload.size);
	memcpy(m_pNewData + addTo, m_pOrigData->payload.buf, m_pOrigData->payload.size);

	OnSelchangeComboView();
}

void CEditItemDlg::OnDestroy() 
{
	SAFE_DELETE(m_LastSel.buf);
	SAFE_DELETE(m_pOldData);
	SAFE_DELETE(m_pNewData);
	if(m_pHexCtrl){
		m_pHexCtrl->DestroyWindow();
		SAFE_DELETE(m_pHexCtrl);
	}
	m_ToolTipFont.DeleteObject();
	m_ConvertFont.DeleteObject();
	CDialog::OnDestroy();	
}

void CEditItemDlg::UpdateConvertList(){
	m_Convert.DeleteAllItems();
	if(!m_LastSel.buf || !m_LastSel.size){ return; }
	CString buf;

	// intel - as is. motorola - reversed

	UCHAR* range = m_LastSel.buf;
	ULONG size = m_LastSel.size;

	bool i = m_ComboBo.GetCurSel()==0;

	if(size >= sizeof(char)){
		// byte
		buf.Format("%hi", (char)range[0]);
		m_Convert.InsertItem(0,"byte");
		m_Convert.SetItemText(0,1,buf);

		// ubyte
		buf.Format("%u", (unsigned char)range[0]);
		m_Convert.InsertItem(1,"ubyte");
		m_Convert.SetItemText(1,1,buf);
	}

	if(size >= sizeof(short)){
		// short
		buf.Format("%hi", (short)(i?*(short*)range:ReverseBytes(*(WORD*)range)));
		m_Convert.InsertItem(2,"short");
		m_Convert.SetItemText(2,1,buf);

		// ushort
		buf.Format("%hu", (USHORT)(i?*(USHORT*)range:ReverseBytes(*(WORD*)range)));
		m_Convert.InsertItem(3,"ushort");
		m_Convert.SetItemText(3,1,buf);
	}

	if(size >= sizeof(long)){
		// long
		buf.Format("%i", (long)(i?*(long*)range:ReverseBytes(*(DWORD*)range)));
		m_Convert.InsertItem(4,"long");
		m_Convert.SetItemText(4,1,buf);

		// ulong
		buf.Format("%u", (ULONG)(i?*(ULONG*)range:ReverseBytes(*(DWORD*)range)));
		m_Convert.InsertItem(5,"ulong");
		m_Convert.SetItemText(5,1,buf);
	}

	if(size >= sizeof(float)){
		buf.Format("%f",(float)(i?*(float*)range:ReverseBytes(*(DWORD*)range)));
		m_Convert.InsertItem(6,"float");
		m_Convert.SetItemText(6,1,buf);
	}
}

void CEditItemDlg::NotifyEditDataHexEditCtrl(NMHDR * pNMHDR, LRESULT * result){
    *result = 0;
	NM_PPDUMP_CTRL* pNotify = (NM_PPDUMP_CTRL*)pNMHDR;
	m_pEditOffsetNew[pNotify->iAddress] = pNotify->iValue;
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

	memcpy(m_LastSel.buf,&m_pEditOffsetNew[begin],size);

	UpdateConvertList();
	
}

//
// replacement for ntohs();
//
WORD CEditItemDlg::ReverseBytes(WORD val){
	unsigned char temp[sizeof(WORD)], tempFin[sizeof(WORD)];
	*(WORD*)temp = val;
	tempFin[0] = temp[1];
	tempFin[1] = temp[0];
	return *(WORD*)tempFin;
}

//
// replacement for ntohl();
//
DWORD CEditItemDlg::ReverseBytes(DWORD val){
	unsigned char temp[sizeof(DWORD)], tempFin[sizeof(DWORD)];
	*(DWORD*)temp = val;
	tempFin[0] = temp[3];
	tempFin[1] = temp[2];
	tempFin[2] = temp[1];
	tempFin[3] = temp[0];
	return *(DWORD*)tempFin;
}

void CEditItemDlg::OnSelchangeComboByteorder() 
{
	UpdateConvertList();
}

void CEditItemDlg::OnCheckDec(){
	int begin, end;
	m_pHexCtrl->GetSelectRange(&begin,&end);
	if(IsDlgButtonChecked(IDC_CHECK_DEC)){
		m_pHexCtrl->ModifyStyles(PPDUMP_FIELD_DEC,PPDUMP_FIELD_HEX);
	}else{
		m_pHexCtrl->ModifyStyles(PPDUMP_FIELD_HEX,PPDUMP_FIELD_DEC);
	}
	m_pHexCtrl->SetSelectRange(begin,end);
}

void CEditItemDlg::OnSelchangeComboView(){

	UCHAR* startAddrOld = m_pOldData;
	UCHAR* endAddrOld = startAddrOld + m_ulDataSize;

	m_pEditOffsetNew = m_pNewData;

	switch(m_ComboView.GetCurSel()){
		case VIEW_DATA_ONLY:
			{
				// move up to data if there is a header
				if(m_pOrigHeader){
					startAddrOld += sizeof(ST_ITEM_HEADER);
					m_pEditOffsetNew += sizeof(ST_ITEM_HEADER);
				}
			}
			break;
		case VIEW_HEADER_ONLY:
			{
				if(!m_pOrigHeader){
					m_ComboView.SetCurSel(0);
					return;
				}
				// remove data
				endAddrOld -= m_pOrigData->payload.size;
			}
			break;
		case VIEW_HEADER_AND_DATA:
			{
				// do nothing.. we want it all
				if(!m_pOrigHeader){
					m_ComboView.SetCurSel(0);
					return;
				}
			}
			break;
	}

	ULONG sizeToView = endAddrOld-startAddrOld;

	m_pHexCtrl->SetPointerData(sizeToView,m_pEditOffsetNew,startAddrOld);
}


void CEditItemDlg::OnUpdateCopy(CCmdUI* pCmdUI){
	pCmdUI->Enable();	
}

void CEditItemDlg::OnCopy(){
	POSITION pos = m_Convert.GetFirstSelectedItemPosition();
	int nIndex = m_Convert.GetNextSelectedItem(pos);
	if(nIndex != -1){
		CString txt = m_Convert.GetItemText(nIndex,1);
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

void CEditItemDlg::OnRclickListConvert(NMHDR* pNMHDR, LRESULT* pResult){	
	*pResult = 0;

	POINT pt;
	GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_EDIT_COPY);
	CMenu* pSub = menu.GetSubMenu(0);
	pSub->TrackPopupMenu(0,pt.x,pt.y,this);

	menu.DestroyMenu();
}

void CEditItemDlg::OnOK(){
	UCHAR* p = m_pNewData;
	if(m_pOrigHeader){
		memcpy(m_pOrigHeader,p,sizeof(*m_pOrigHeader));
		p += sizeof(*m_pOrigHeader);
	}
	m_pOrigData->itemid = *(ULONG*)p;
	memcpy(m_pOrigData->payload.buf,p,m_pOrigData->payload.size);
	
	CDialog::OnOK();
}

void CEditItemDlg::OnButtonImHeader() 
{
	if(!m_pOrigHeader){ return; }

	CFileDlg fileDlg(TRUE,
					 "Import Sacred Header Dump...",
					 GetSafeHwnd(),
					 "Sacred Header Dumps (*.shd)|*.SHD|All Files (*.*)|*.*||");
	CString rc;
	if( !(rc = fileDlg.GetResult()).GetLength()){ return; }

	FILE* f = fopen(rc,READ_BINARY);
	if(f){
		fseek(f,0,SEEK_END);
		int nLen = ftell(f);
		rewind(f);
		if(nLen != sizeof(*m_pOrigHeader)){
			MessageBox("Size mismatch on header.  Must be no more/less than 256 bytes.","Import error...", MB_ICONHAND|MB_OK);
		}else{
			fread(m_pNewData,sizeof(*m_pOrigHeader),1,f);
			m_pHexCtrl->RedrawWindow();
		}
		fclose(f);
	}else{
		MessageBox("Unable to open file!","Error...", MB_ICONHAND|MB_OK);
	}
}

void CEditItemDlg::OnButtonImData(){

	CFileDlg fileDlg(TRUE,
					 "Import Sacred Item Data Dump...",
					 GetSafeHwnd(),
					 "Sacred Item Data Dumps (*.sid)|*.SID\0All Files (*.*)|*.*||");
	CString rc;
	if( !(rc = fileDlg.GetResult()).GetLength()){ return; }

	FILE* f = fopen(rc,READ_BINARY);
	if(f){
		fseek(f,0,SEEK_END);
		ULONG nLen = ftell(f);
		rewind(f);
		if(nLen != m_pOrigData->payload.size){
			MessageBox("Size mismatch on data. Must match the size of data currently in the buffer.","Import error...", MB_ICONHAND|MB_OK);
		}else{
			fread((m_pOrigHeader?m_pNewData+sizeof(*m_pOrigHeader):m_pNewData),nLen,1,f);
			m_pHexCtrl->RedrawWindow();
		}
		fclose(f);
	}else{
		MessageBox("Unable to open file!","Error...", MB_ICONHAND|MB_OK);
	}
}

void CEditItemDlg::OnButtonExHeader(){

	if(!m_pOrigHeader){ return; }

	char fileTitle[MAX_PATH];
	sprintf(fileTitle,"%u.shd", m_pOrigHeader->itemid);

	CFileDlg fileDlg(FALSE,
					 "Save Sacred Header Dump...",
					 GetSafeHwnd(),
					 "Sacred Header Dumps (*.SHD)|*.SHD||",
					 "shd",
					 fileTitle);

	CString rc;
	if( !(rc = fileDlg.GetResult()).GetLength() ){ return; }

	FILE* f = fopen(rc,WRITE_BINARY);
	if(f){
		fwrite(m_pNewData,sizeof(*m_pOrigHeader),1,f);
		fclose(f);
	}else{
		MessageBox("Unable to save file!","Error...", MB_ICONHAND|MB_OK);
	}
}

void CEditItemDlg::OnButtonExData(){

	char fileTitle[MAX_PATH];
	sprintf(fileTitle,"%u.sid", m_pOrigData->itemid);

	CFileDlg fileDlg(FALSE,
					 "Save Sacred Item Data Dump...",
					 GetSafeHwnd(),
					 "Sacred Item Data Dumps (*.SID)|*.SID||",
					 "sid",
					 fileTitle);

	CString rc;
	if( !(rc = fileDlg.GetResult()).GetLength() ){ return; }

	FILE* f = fopen(rc,WRITE_BINARY);
	if(f){
		fwrite((m_pOrigHeader?m_pNewData+sizeof(*m_pOrigHeader):m_pNewData),m_pOrigData->payload.size,1,f);
		fclose(f);
	}else{
		MessageBox("Unable to save file!","Error...", MB_ICONHAND|MB_OK);
	}
}
