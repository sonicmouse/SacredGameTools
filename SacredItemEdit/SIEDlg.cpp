// SIEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "SIEDlg.h"

#include "EditItemDlg.h"
#include "ExportItemDlg.h"
#include "AboutDlg.h"
#include "DupItemDlg.h"
#include "ImportItemDlg.h"
#include "ImportToSlotDlg.h"

#define ROOT_ITEM_ID 0xFFFFFFFF

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIEDlg dialog

CSIEDlg::CSIEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSIEDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pHero = NULL;
	m_pPlacementDlg = NULL;
}

void CSIEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIEDlg)
	DDX_Control(pDX, IDC_TREE_ITEMS, m_Tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSIEDlg, CDialog)
	//{{AFX_MSG_MAP(CSIEDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_GRID, OnButtonGrid)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ITEMS, OnSelchangedTreeItems)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_DUP, OnButtonDup)
	ON_BN_CLICKED(IDC_BUTTON_COMPRESS, OnButtonCompress)
	ON_BN_CLICKED(IDC_BUTTON_UNSLOT, OnButtonUnslot)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTSLOT, OnButtonImportslot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIEDlg message handlers

BOOL CSIEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	if(__argc > 1){
		CStringArray sitFileArr;
		for(int i = 1; i < __argc; i++){
			if(CSITFile::IsValidSITFileQuickScan(__argv[i])){
				sitFileArr.Add(__argv[i]);
			}
		}
		if(sitFileArr.GetSize()){
			// TODO: view array of files...
			CString tmp; tmp.Format("TODO: %u valid files...", sitFileArr.GetSize());
			MessageBox(tmp);
			EndDialog(0);
			return TRUE;
		}
	}

	m_fontBold.CreateFont(14, 0, 0, 0, FW_BOLD,
					  FALSE, FALSE, 0, ANSI_CHARSET,
					  OUT_TT_PRECIS, 
					  CLIP_DEFAULT_PRECIS,
					  ANTIALIASED_QUALITY, 
					  DEFAULT_PITCH|FF_SWISS, "Arial");

	GetDlgItem(IDC_STATIC_CHARFILE)->SetFont(&m_fontBold);
	GetDlgItem(IDC_STATIC_ACT_FRAME)->SetFont(&m_fontBold);
	GetDlgItem(IDC_BUTTON_UPDATE)->SetFont(&m_fontBold);
	GetDlgItem(IDC_STATIC_TOTALITEMS)->SetFont(&m_fontBold);

	m_pPlacementDlg = new CPlacementDlg(this);
	m_pPlacementDlg->Create(IDD_PLACEMENT_DIALOG,this);
	m_pPlacementDlg->UpdateWindow();
	CenterWindow();
	CRect mainRc,myRc;
	GetWindowRect(mainRc);
	m_pPlacementDlg->GetClientRect(myRc);
	m_pPlacementDlg->SetWindowPos(NULL,mainRc.left+((mainRc.Width()/2)-(myRc.Width()/2)),mainRc.top + mainRc.Height(),0,0,SWP_NOSIZE);
	m_pPlacementDlg->ShowWindow(SW_SHOW);

	UpdateWindows();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSIEDlg::OnButtonBrowse() 
{
	CFileDlg fileDlg(TRUE,
					 "Find Sacred Hero File...",
					 GetSafeHwnd(),
					 "Sacred Hero Files (HERO##.PAX)|*.PAX|All Files (*.*)|*.*||");
	CString rc;
	if( !(rc = fileDlg.GetResult()).GetLength()){ return; }

	SetDlgItemText(IDC_EDIT_HERO,rc);

	OnButtonRefresh();
}

void CSIEDlg::OnDestroy(){
	if(m_pPlacementDlg){
		m_pPlacementDlg->DestroyWindow();
		SAFE_DELETE(m_pPlacementDlg);
	}
	SAFE_DELETE(m_pHero);
	DestroyIcon(m_hIcon);
	m_fontBold.DeleteObject();
	CDialog::OnDestroy();	
}

void CSIEDlg::RefreshList(){

	UpdateWindows();

	SetDlgItemText(IDC_STATIC_TOTALITEMS,"Total Items: 0");
	m_Tree.DeleteAllItems();

	if(!m_pHero){ return; }

	BOOL bShowZero = FALSE;//IsDlgButtonChecked(IDC_CHECK_SHOWZERO);
	BOOL bShowBreaks = FALSE;//IsDlgButtonChecked(IDC_CHECK_SHOWBREAKS);

	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT;

	char szHeroName[HERO_NAME_SIZE + 127];
	sprintf(szHeroName, "\"%s\"", m_pHero->GetMyName());
	tvInsert.item.pszText = szHeroName;

	HTREEITEM hRoot = m_Tree.InsertItem(&tvInsert);
	m_Tree.SetItemData(hRoot,ROOT_ITEM_ID);
	m_Tree.SetItemColor(hRoot,RGB(0,0,255));
	m_Tree.SetItemBold(hRoot,TRUE);

	for(ULONG i = 0; i < m_pHero->GetItemCount(); i++){
		PST_ITEM pItem = m_pHero->GetItem(i);
		if(pItem->header.itemid == 0){
			if(bShowZero){
				HTREEITEM hRootItem = m_Tree.InsertItem(TVIF_TEXT, "Zero Item", 0, 0, 0, 0, 0, hRoot, NULL);
				m_Tree.SetItemData(hRootItem,0);
				m_Tree.EnsureVisible(hRootItem);
			}
		}else if(pItem->header.itemid == NULL_ITEM){
			if(bShowBreaks){
				HTREEITEM hRootItem = m_Tree.InsertItem(TVIF_TEXT, "-----------------", 0, 0, 0, 0, 0, hRoot, NULL);
				m_Tree.SetItemData(hRootItem,NULL_ITEM);
				m_Tree.EnsureVisible(hRootItem);
			}
		}else{
			CString txt;
			txt.Format("%u", pItem->header.itemid);
			HTREEITEM hRootItem = m_Tree.InsertItem(TVIF_TEXT, txt, 0, 0, 0, 0, 0, hRoot, NULL);
			m_Tree.SetItemData(hRootItem,i);
			if(m_pHero->GetItemUpdated(i,1)){
				m_Tree.SetItemColor(hRootItem,RGB(255,0,0));
				m_Tree.SetItemBold(hRootItem,TRUE);
			}else{
				m_Tree.SetItemColor(hRootItem,GetSysColor(COLOR_WINDOWTEXT));
				m_Tree.SetItemBold(hRootItem,FALSE);
			}
			m_Tree.EnsureVisible(hRootItem);
			UINT bitMask = 1;
			for(int b = 0; b < pItem->header.slotcount; b++){
				bitMask *= 2;
				PST_ITEM_DATA pData = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(b);
				txt.Format("%u", pData->itemid);
				HTREEITEM hSlotRoot = m_Tree.InsertItem(TVIF_TEXT, txt, 0, 0, 0, 0, 0, hRootItem, NULL);
				m_Tree.SetItemData(hSlotRoot,b);
				m_Tree.EnsureVisible(hSlotRoot);
				if(m_pHero->GetItemUpdated(i,bitMask)){
					m_Tree.SetItemColor(hSlotRoot,RGB(255,0,0));
					m_Tree.SetItemBold(hSlotRoot,TRUE);
				}else{
					m_Tree.SetItemColor(hSlotRoot,GetSysColor(COLOR_WINDOWTEXT));
					m_Tree.SetItemBold(hSlotRoot,FALSE);
				}
			}
		}
	}
	m_Tree.EnsureVisible(hRoot);

	CString totItems;
	totItems.Format("Total Items: %u", m_pHero->GetItemCount(TRUE));
	SetDlgItemText(IDC_STATIC_TOTALITEMS,totItems);

	UpdateWindows();
}

BOOL CSIEDlg::GetSelectedItem(int& nItemIndex, int& nSlotIndex){
	// we have to have a hero loaded
	if(!m_pHero){ return FALSE; }
	HTREEITEM hItem = m_Tree.GetSelectedItem();
	if(!hItem || m_Tree.GetItemData(hItem)==ROOT_ITEM_ID){ return FALSE; }

	ULONG parentData = m_Tree.GetItemData(m_Tree.GetParentItem(hItem));
	ULONG myData = m_Tree.GetItemData(hItem);

	PST_ITEM pItem = NULL; // root item
	PST_ITEM_DATA pItemData = NULL; // item data
	
	if(parentData != ROOT_ITEM_ID){
		nItemIndex = parentData;
		nSlotIndex = myData;
	}else{
		nItemIndex = myData;
		nSlotIndex = -1;
	}

	return TRUE;
}

BOOL CSIEDlg::GetSelectedItem(ULONG& ulItemId, int& nSlotIndex){
	if(!m_pHero){ return FALSE; }
	HTREEITEM hItem = m_Tree.GetSelectedItem();
	if(!hItem || m_Tree.GetItemData(hItem)==ROOT_ITEM_ID){ return FALSE; }

	ULONG parentData = m_Tree.GetItemData(m_Tree.GetParentItem(hItem));
	ULONG myData = m_Tree.GetItemData(hItem);

	PST_ITEM pItem = NULL; // root item
	PST_ITEM_DATA pItemData = NULL; // item data
	
	if(parentData != ROOT_ITEM_ID){
		PST_ITEM pItem = m_pHero->GetItem(parentData);
		ulItemId = pItem->header.slotindex;
		nSlotIndex = myData;
	}else{
		PST_ITEM pItem = m_pHero->GetItem(myData);
		ulItemId = pItem->header.slotindex;
		nSlotIndex = -1;
	}

	return TRUE;
}

BOOL CSIEDlg::GetSelectedItem(ST_ITEM_DATA** pData, ST_ITEM_HEADER** pHeader){
	// we have to have a hero loaded
	if(!m_pHero){ return FALSE; }
	HTREEITEM hItem = m_Tree.GetSelectedItem();
	if(!hItem || m_Tree.GetItemData(hItem)==ROOT_ITEM_ID){ return FALSE; }

	ULONG parentData = m_Tree.GetItemData(m_Tree.GetParentItem(hItem));
	ULONG myData = m_Tree.GetItemData(hItem);

	PST_ITEM pItem = NULL; // root item
	PST_ITEM_DATA pItemData = NULL; // item data
	
	if(parentData != ROOT_ITEM_ID){
		// this is a slotted item... parent data is an index to an item
		// so, we get the item at parent node, then find the myData in the array
		pItem = m_pHero->GetItem(parentData);
		pItemData = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(myData);
		pItem = NULL;
	}else{
		// parent is root, so this is a main item
		pItem = m_pHero->GetItem(myData);
		pItemData = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(pItem->itemdataarr.GetSize()-1);
	}

	*pData = pItemData;
	*pHeader = (pItem?&pItem->header:NULL);

	return TRUE;
}

void CSIEDlg::OnButtonExport() 
{
	PST_ITEM_DATA pItemData = NULL;
	PST_ITEM_HEADER pItemHeader = NULL;

	if(!GetSelectedItem(&pItemData,&pItemHeader)){ return; }

	ST_BUFFER buf;
	buf.size = pItemData->payload.size;
	buf.buf = new UCHAR[buf.size];
	memcpy(buf.buf, pItemData->payload.buf, buf.size);

	// set the item id correctly!
	*(ULONG*)buf.buf = (pItemHeader?pItemHeader->itemid:pItemData->itemid);

	m_pPlacementDlg->DisableDisplay(TRUE);

	// save it...
	CExportItemDlg dlg;

	if(pItemHeader){
		CSize sz = m_pHero->GetItemInvSize(pItemHeader->slotindex);
		if(sz == CSize(0,0)){
			sz.cx = 1;
			sz.cy = 1;
		}
		dlg.SetItemInvSize(sz);
	}

	dlg.SetItemBuffer(&buf);
	dlg.DoModal();
	SAFE_DELETE(buf.buf);

	m_pPlacementDlg->DisableDisplay(FALSE);
}

void CSIEDlg::OnButtonDelete(){
	ULONG ulItemId = 0;
	int ulSubSlotIndex = -1;

	if(!GetSelectedItem(ulItemId,ulSubSlotIndex)){ return; }

	m_pPlacementDlg->DisableDisplay(TRUE);

	if(ulSubSlotIndex == -1 && m_pHero->GetSlottedItemsCopy(ulItemId,NULL)){
		if(MessageBox("The item you are about to delete has items in its slots.\nIf you delete this item, you will also be deleting the items in its slots.\n\nDo you want to continue?\n\nNOTE: If you want to save the items in its slots, click the `Unslot Item` button.\n\n(Final changes do not take place until you `Save and Update Hero`)", "Confirm...", MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO){
			m_pPlacementDlg->DisableDisplay(FALSE);
			return;
		}
	}else{
		if(MessageBox("Are you sure you want to delete this item?\n\n(Final changes do not take place until you `Save and Update Hero`)", "Confirm...", MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO){
			m_pPlacementDlg->DisableDisplay(FALSE);
			return;
		}
	}

	if(ulSubSlotIndex == -1){
		// its a full itme
		m_pHero->DeleteItem(ulItemId);
	}else{
		// its a slotted item
		m_pHero->DeleteSlottedItem(ulItemId,ulSubSlotIndex);
	}

	ClearItemSelection();

	m_pPlacementDlg->SetHero(m_pHero);
	RefreshList();

	m_pPlacementDlg->DisableDisplay(FALSE);
}

void CSIEDlg::ClearItemSelection(){
	m_Tree.Select(NULL,TVGN_CARET);
	m_pPlacementDlg->ClearSelectedIndex();
	UpdateWindows();
}

void CSIEDlg::OnButtonEdit() 
{
	PST_ITEM_DATA pItemData = NULL;
	PST_ITEM_HEADER pItemHeader = NULL;

	if(!GetSelectedItem(&pItemData,&pItemHeader)){ return; }
	int nMainItem, nSlotIndex;
	if(!GetSelectedItem(nMainItem,nSlotIndex)){ return; }

	m_pPlacementDlg->DisableDisplay(TRUE);
	CEditItemDlg dlg(this);
	dlg.SetItemDetails(pItemData, pItemHeader);
	
	// save these numbers...
	ULONG origSlotIndex, origFeedFood; 
	UCHAR origSlotCount;
	if(pItemHeader){
		origSlotIndex = pItemHeader->slotindex;
		origSlotCount = pItemHeader->slotcount;
		origFeedFood = pItemHeader->feedfood;
	}
	if(dlg.DoModal()==IDCANCEL){
		m_pPlacementDlg->DisableDisplay(FALSE);
		return;
	}
	// ...and replace.  People are not allowed to edit these numbers.
	if(pItemHeader){
		pItemHeader->feedfood = origFeedFood;
		pItemHeader->slotcount = origSlotCount;
		pItemHeader->slotindex = origSlotIndex;
	}

	m_pPlacementDlg->DisableDisplay(FALSE);

	if(nSlotIndex==-1){
		m_pHero->SetItemUpdated(nMainItem,1,TRUE);
	}else{
		int bitMask = 2;
		for(int i = 0; i < nSlotIndex; i++){bitMask *= 2;}
		m_pHero->SetItemUpdated(nMainItem,bitMask,TRUE);
	}

	RefreshList();

	SelectItemByItemId(origSlotIndex);
}

BOOL CSIEDlg::ReloadHero(BOOL bQuiet/* = FALSE*/){
	if(!m_pHero){ return FALSE; }

	CString target;
	GetDlgItemText(IDC_EDIT_HERO,target);

	if(!bQuiet){
		if(MessageBox("Would you like to create a backup file just incase we screwed anything up? :)","Confirm...",MB_YESNO|MB_ICONQUESTION)==IDYES){

			char fileName[MAX_PATH];
			char* fName = strrchr(target,'\\') + 1;
			sprintf(fileName,"BACKUP_%s", fName);

			CFileDlg fileDlg(FALSE,
							 "Save Sacred Hero File...",
							 GetSafeHwnd(),
							 "Sacred Hero (*.PAX)|*.PAX||",
							 "pax",
							 fileName);

			CString rc;
			if( !(rc = fileDlg.GetResult()).GetLength() ){
				MessageBox("Character update cancelled by user!","Cancelled...", MB_ICONEXCLAMATION|MB_OK);
				return FALSE;
			}

			CopyFile(target,rc,FALSE);
		}
	}

	if(m_pHero->CompileHeroFile(target)){
		OnButtonRefresh();
		if(!bQuiet){
			MessageBox("Updated character successfully!","Success...",MB_ICONINFORMATION|MB_OK);
		}
		return TRUE;
	}else{
		if(!bQuiet){
			MessageBox("Update character failed!","Failed...",MB_ICONHAND|MB_OK);
		}
		return FALSE;
	}
	return FALSE;
}

void CSIEDlg::OnButtonUpdate() 
{
	ReloadHero();
}

void CSIEDlg::OnButtonRefresh() 
{
	CString heroFile;
	GetDlgItemText(IDC_EDIT_HERO,heroFile);

	m_pPlacementDlg->ClearHero();

	if(!heroFile.GetLength()){ return; }

	GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BROWSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);

	SAFE_DELETE(m_pHero);
	m_pHero = new CHero(heroFile);

	if(!m_pHero || !m_pHero->IsLoaded()){
		SAFE_DELETE(m_pHero);
		MessageBox("Unable to load this hero file.","Error...", MB_ICONHAND|MB_OK);
	}

	ClearItemSelection();
	RefreshList();

	GetDlgItem(IDC_BUTTON_BROWSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(TRUE);

	m_pPlacementDlg->SetHero(m_pHero);
}

void CSIEDlg::OnButtonAbout(){
	CAboutDlg dlg(this);
	dlg.DoModal();
}

void CSIEDlg::OnButtonGrid(){
	m_pPlacementDlg->ShowWindow((m_pPlacementDlg->IsWindowVisible()?SW_HIDE:SW_SHOW));
}

void CSIEDlg::OnPlacementSelectionChange(int invType, ULONG itemIndex){

	UpdateWindows();
	if(!m_pHero){ return; }

	SelectItemByItemId( itemIndex );

}

void CSIEDlg::SelectItemByItemId(ULONG itemId){

	HTREEITEM hRoot = m_Tree.GetRootItem();
	HTREEITEM hCurrent = m_Tree.GetChildItem(hRoot);
	while(hCurrent){
		PST_ITEM pItem = m_pHero->GetItem(m_Tree.GetItemData(hCurrent));
		if(pItem->header.slotindex == itemId){
			m_Tree.EnsureVisible(hCurrent);
			m_Tree.Select(hCurrent,TVGN_CARET);
			UpdateWindows();
			return;
		}
		hCurrent = m_Tree.GetNextItem(hCurrent, TVGN_NEXT);
	}	
}

void CSIEDlg::OnSelchangedTreeItems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	UpdateWindows();

	m_pPlacementDlg->ClearSelectedIndex();

	PST_ITEM_DATA pData = NULL;
	PST_ITEM_HEADER pHeader = NULL;

	if(GetSelectedItem(&pData,&pHeader)){
		if(pHeader){
			m_pPlacementDlg->SetSelectedIndex(pHeader->slotindex);
		}
	}
}


void CSIEDlg::OnButtonClose() 
{
	SetDlgItemText(IDC_EDIT_HERO,"");
	m_pPlacementDlg->ClearHero();
	SAFE_DELETE(m_pHero);
	RefreshList();
}

void CSIEDlg::OnButtonImport(){

	if(!m_pHero){ return; }

	m_pPlacementDlg->DisableDisplay(TRUE);

	CImportItemDlg dlg(this);
	dlg.SetHeroFile(m_pHero);
	if(dlg.DoModal()==IDCANCEL){
		m_pPlacementDlg->DisableDisplay(FALSE);
		return;
	}

	m_pPlacementDlg->SetHero(m_pHero);
	m_pPlacementDlg->DisableDisplay(FALSE);
	RefreshList();
}

void CSIEDlg::OnButtonImportslot(){
	if(!m_pHero){ return; }

	ULONG ulItemId;
	int nSlotIndex;
	if(!GetSelectedItem(ulItemId,nSlotIndex)){ return; }

	m_pPlacementDlg->DisableDisplay(TRUE);

	CImportToSlotDlg dlg(this);

	dlg.SetHero(m_pHero);
	dlg.SetMainItem(ulItemId);

	if(dlg.DoModal()==IDCANCEL){
		m_pPlacementDlg->DisableDisplay(FALSE);
		return;
	}

	m_pPlacementDlg->SetHero(m_pHero);
	m_pPlacementDlg->DisableDisplay(FALSE);
	RefreshList();

	SelectItemByItemId(ulItemId);
}

void CSIEDlg::OnButtonDup(){

	PST_ITEM_DATA pData = NULL;
	PST_ITEM_HEADER pHeader = NULL;
	if(!GetSelectedItem(&pData,&pHeader)){ return; }

	m_pPlacementDlg->DisableDisplay(TRUE);

	CDupItemDlg dlg(this);
	if(dlg.DoModal()==IDCANCEL){
		m_pPlacementDlg->DisableDisplay(FALSE);
		return;
	}

	m_pPlacementDlg->DisableDisplay(FALSE);

	int nTimes = dlg.GetTimes();
	BOOL bShrink = dlg.GetShrink();
	BOOL bToChest = dlg.GetToChest();

	CSize itemSize;

	if(!bShrink && pHeader){
		itemSize = m_pHero->GetItemInvSize(pHeader->slotindex);
	}

	if(bShrink){
		itemSize.cx = 1;
		itemSize.cy = 1;
	}

	if(!itemSize.cx || !itemSize.cy){
		itemSize.cx = (itemSize.cx?itemSize.cx:1);
		itemSize.cy = (itemSize.cy?itemSize.cy:1);
	}

	int nInvSection = (bToChest?SECTION_PLACEMENT_CH:SECTION_PLACEMENT_BP);

	ST_BUFFER temp;
	temp.size = pData->payload.size;
	temp.buf = new UCHAR[temp.size];
	memcpy(temp.buf,pData->payload.buf,temp.size);

	ULONG ulLastItemId = 0;
	for(int i = 0; i < nTimes; i++){
		ULONG itemId = m_pHero->ImportItem(&temp,itemSize,nInvSection);
		if(!itemId){
			break;
		}else{
			ulLastItemId = itemId;
		}
	}

	SAFE_DELETE(temp.buf);

	m_pPlacementDlg->SetHero(m_pHero);
	RefreshList();

	if(ulLastItemId){
		SelectItemByItemId(ulLastItemId);
	}

	if(i!=nTimes){
		CString txt;
		txt.Format("Failed to duplicate all %u items you wanted due to lack of space.\nBut, %u were duplicated before space became limited.",nTimes,i);
		MessageBox(txt,"Notice...", MB_ICONINFORMATION|MB_OK);
	}
}

void CSIEDlg::UpdateWindows(){

	PST_ITEM_DATA pItemData = NULL;
	PST_ITEM_HEADER pItemHeader = NULL;

	BOOL bSel = GetSelectedItem(&pItemData,&pItemHeader);

	BOOL bHero = (m_pHero!=NULL);

	CSize itemSize = CSize(1,1);
	int nSlottedItemsInMainItem = 0;
	if(pItemHeader && m_pHero){
		itemSize = m_pHero->GetItemInvSize(pItemHeader->slotindex);
		nSlottedItemsInMainItem = m_pHero->GetSlottedItemsCopy(pItemHeader->slotindex,NULL);
	}

	BOOL bSlotItem = pItemData && !pItemHeader;
	BOOL bRegItem = pItemData && pItemHeader;
	
	GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(bHero);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(bHero);
	//GetDlgItem(IDC_BUTTON_BROWSE)
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(bSel);
	GetDlgItem(IDC_BUTTON_IMPORT)->EnableWindow(bHero);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(bSel);
	//GetDlgItem(IDC_BUTTON_GRID)
	GetDlgItem(IDC_BUTTON_DUP)->EnableWindow(bSel);
	GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(bSel);
	GetDlgItem(IDC_BUTTON_UNSLOT)->EnableWindow(bSlotItem);
	GetDlgItem(IDC_BUTTON_COMPRESS)->EnableWindow(bRegItem);
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(bHero);
	GetDlgItem(IDC_BUTTON_COMPRESS)->EnableWindow((itemSize.cx > 1 || itemSize.cy > 1));
	GetDlgItem(IDC_BUTTON_IMPORTSLOT)->EnableWindow(bRegItem && (nSlottedItemsInMainItem < 8));
}

void CSIEDlg::OnButtonCompress(){
	if(!m_pHero){ return; }

	ULONG ulItemId = 0;
	int ulSubSlotIndex = -1;
	if(!GetSelectedItem(ulItemId,ulSubSlotIndex)){ return; }

	if(ulSubSlotIndex != -1){ return; }

	m_pHero->ShrinkItem(ulItemId);

	m_pPlacementDlg->SetHero(m_pHero);
	RefreshList();

	SelectItemByItemId(ulItemId);
}

void CSIEDlg::OnButtonUnslot(){
	if(!m_pHero){ return; }

	ULONG ulItemId = 0;
	int nSubSlotIndex = -1;
	if(!GetSelectedItem(ulItemId,nSubSlotIndex)){ return; }

	if(nSubSlotIndex == -1){ return; }

	CPtrArray arr;
	if(m_pHero->GetSlottedItemsCopy(ulItemId,&arr)==0){ return; }

	for(int i = 0; i < arr.GetSize(); i++){
		PST_ITEM_DATA pItemData = (PST_ITEM_DATA)arr.GetAt(i);
		if(i == nSubSlotIndex){
			ST_BUFFER buf;
			buf.size = pItemData->payload.size;
			buf.buf = new UCHAR[buf.size];
			memcpy(buf.buf,pItemData->payload.buf,buf.size);
			
			m_pHero->ImportItem(&buf,CSize(1,1),SECTION_PLACEMENT_BP);

			SAFE_DELETE(buf.buf);
		}
		SAFE_DELETE(pItemData->payload.buf);
		SAFE_DELETE(pItemData);
	}

	// delete this slotted item...
	m_pHero->DeleteSlottedItem(ulItemId,nSubSlotIndex);

	m_pPlacementDlg->SetHero(m_pHero);
	RefreshList();

	SelectItemByItemId(ulItemId);
}

