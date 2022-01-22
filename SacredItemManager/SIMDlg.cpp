// SIMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "SIMDlg.h"

#include "TempFile.h"
#include "FileDlg.h"
#include "DupeItemDlg.h"
#include "EditItemDlg.h"
#include "ExportItemDlg.h"
#include "ImportItemDlg.h"
#include "UnslotItemDlg.h"
#include "AboutDlg.h"
#include "ViewItemDlg.h"
#include "SITFile.h"
#include "EditIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIMDlg dialog

CSIMDlg::CSIMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSIMDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pHero = NULL;
	m_pItemDisp = NULL;
}

void CSIMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIMDlg)
	DDX_Control(pDX, IDC_BUTTON_EDIT_DESC_ID, m_ButtonEditId);
	DDX_Control(pDX, IDC_BUTTON_VIEWITEM, m_ButtonView);
	DDX_Control(pDX, IDC_BUTTON_IMPORT_ITEM, m_ButtonImport);
	DDX_Control(pDX, IDC_BUTTON_EXPORT_ITEM, m_ButtonExport);
	DDX_Control(pDX, IDC_BUTTON_EDIT_ITEM, m_ButtonEditItem);
	DDX_Control(pDX, IDC_BUTTON_UNSLOT_ITEM, m_ButtonUnslot);
	DDX_Control(pDX, IDC_BUTTON_DELETEITEM, m_ButtonDelete);
	DDX_Control(pDX, IDC_BUTTON_SHRINKITEM, m_ButtonShrink);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_ButtonHelp);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_CURRENT_HERO, m_ButtonBrowse);
	DDX_Control(pDX, IDC_BUTTON_DUPLICATE_ITEM, m_ButtonDupeitem);
	DDX_Control(pDX, IDC_BUTTON_SAVE_CURRENT_HERO, m_ButtonUpdateCurrent);
	DDX_Control(pDX, IDC_BUTTON_RELOAD_CURRENT_HERO, m_ButtonReloadCurrent);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_CURRENT_HERO, m_ButtonCloseCurrent);
	DDX_Control(pDX, IDCANCEL, m_ButtonExit);
	DDX_Control(pDX, IDC_BUTTON_ABOUT, m_ButtonAbout);
	DDX_Control(pDX, IDC_BUTTON_PLACEMENT, m_ButtonPlacement);
	DDX_Control(pDX, IDC_TREE_ITEMS, m_TreeItems);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSIMDlg, CDialog)
	//{{AFX_MSG_MAP(CSIMDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_CURRENT_HERO, OnButtonBrowseCurrentHero)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_CURRENT_HERO, OnButtonCloseCurrentHero)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ITEMS, OnSelchangedTreeItems)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_CURRENT_HERO, OnButtonSaveCurrentHero)
	ON_BN_CLICKED(IDC_BUTTON_PLACEMENT, OnButtonPlacement)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD_CURRENT_HERO, OnButtonReloadCurrentHero)
	ON_BN_CLICKED(IDC_BUTTON_DUPLICATE_ITEM, OnButtonDuplicateItem)
	ON_BN_CLICKED(IDC_BUTTON_SHRINKITEM, OnButtonShrinkitem)
	ON_BN_CLICKED(IDC_BUTTON_DELETEITEM, OnButtonDeleteitem)
	ON_BN_CLICKED(IDC_BUTTON_UNSLOT_ITEM, OnButtonUnslotItem)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_ITEM, OnButtonEditItem)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_ITEM, OnButtonExportItem)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_ITEM, OnButtonImportItem)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_VIEWITEM, OnButtonViewitem)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_ITEMS, OnDblclkTreeItems)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DESC_ID, OnButtonEditDescId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIMDlg message handlers

//
// on WM_INITDIALOG
//
BOOL CSIMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//
	// command line items...
	//
	if(__argc > 1){
		CSITFile sit;
		if(sit.DecompileSitFile(__argv[1])){
			CViewItemDlg dlg(this);
			dlg.SetSITFile(&sit);
			dlg.DoModal();
		}else{
			MessageBox(CString(__argv[1]) + "\nis not a valid SIT file!","Error...",MB_ICONHAND|MB_OK|MB_SYSTEMMODAL);
		}
		EndDialog(IDCANCEL);
		return TRUE;
	}

	//
	// icons
	//
	SetIcon(GetMyIcon(), TRUE);
	SetIcon(GetMyIcon(), FALSE);

	//
	// fonts
	//
	GetFontBold()->CreateFont(14, 0, 0, 0, FW_BOLD,
					  FALSE, FALSE, 0, ANSI_CHARSET,
					  OUT_TT_PRECIS, 
					  CLIP_DEFAULT_PRECIS,
					  ANTIALIASED_QUALITY, 
					  DEFAULT_PITCH|FF_SWISS, "Arial");

	GetDlgItem(IDC_STATIC_FRAME_CURRENT_HERO)->SetFont(GetFontBold());
	GetDlgItem(IDC_STATIC_FRAME_ITEM_ACTIONS)->SetFont(GetFontBold());
	GetDlgItem(IDC_STATIC_FRAME_MISC)->SetFont(GetFontBold());
	GetDlgItem(IDC_BUTTON_SAVE_CURRENT_HERO)->SetFont(GetFontBold());
	GetDlgItem(IDC_BUTTON_BROWSE_CURRENT_HERO)->SetFont(GetFontBold());
	GetDlgItem(IDC_STATIC_TOTAL_ITEMS)->SetFont(GetFontBold());
	GetDlgItem(IDC_BUTTON_EXPORT_ITEM)->SetFont(GetFontBold());
	GetDlgItem(IDC_BUTTON_IMPORT_ITEM)->SetFont(GetFontBold());

	//
	// item placement dlg
	//
	m_pItemDisp = new CItemPlacementDispDlg(this);
	if(!m_pItemDisp){
		MessageBox("Error trying to allocate a placement display dialog.\nAborting...", "Error...", MB_ICONHAND|MB_OK);
		return TRUE;
	}
	m_pItemDisp->Create(IDD_ITEMPLACEMENTDISP_DIALOG,this);
	m_pItemDisp->UpdateWindow();

	//
	// window placement
	//
	CenterWindow();
	CRect mainRc,myRc;
	GetWindowRect(mainRc);
	m_pItemDisp->GetClientRect(myRc);
	m_pItemDisp->SetWindowPos(NULL,mainRc.left+((mainRc.Width()/2)-(myRc.Width()/2)),mainRc.top + mainRc.Height(),0,0,SWP_NOSIZE);
	m_pItemDisp->ShowWindow(SW_SHOW);

	//
	// update the windows
	//
	UpdateWindows();

	//
	// bye
	//
	return TRUE;  // return TRUE  unless you set the focus to a control
}

//
// on WM_DESTROY
//
void CSIMDlg::OnDestroy(){

	if(m_pItemDisp){
		m_pItemDisp->DestroyWindow();
		SAFE_DELETE(m_pItemDisp);
	}

	SAFE_DELETE(m_pHero);

	GetFontBold()->DeleteObject();
	DestroyIcon(GetMyIcon());

	CDialog::OnDestroy();	
}

//
// on button BROWSE FOR HERO
//
void CSIMDlg::OnButtonBrowseCurrentHero(){
	m_pItemDisp->DisableDisplays(TRUE);
	CFileDlg fileDlg(TRUE,
					 "Find Sacred Hero File...",
					 GetSafeHwnd(),
					 "All Supported Heros Types|*.PAX;*.PTX|Sacred Heros (*.PAX)|*.PAX|Hero Template Files (*.PTX)|*.PTX||");
	CString rc;
	if( !(rc = fileDlg.GetResult()).GetLength()){ m_pItemDisp->DisableDisplays(FALSE); return; }

	OnButtonCloseCurrentHero();

	m_pHero = new CHero();

	if(!GetHero() || !GetHero()->LoadHeroFile(rc)){
		SAFE_DELETE(m_pHero);
		MessageBox("Failed to load this hero file!","Error...", MB_ICONHAND|MB_OK);
		m_pItemDisp->DisableDisplays(FALSE);
		return;
	}

	SetDlgItemText(IDC_EDIT_CURRENT_HERO,rc);
	RefreshList();

	m_pItemDisp->DisableDisplays(FALSE);
}

//
// on button CLOSE HERO
//
void CSIMDlg::OnButtonCloseCurrentHero(){
	SetDlgItemText(IDC_EDIT_CURRENT_HERO,"");
	if(GetHero()){
		GetHero()->CloseHeroFile();
		SAFE_DELETE(m_pHero);
	}

	m_pItemDisp->SetSelection(NULL);
	m_pItemDisp->SetHero(NULL);
	
	RefreshList();
}

//
// refreshes the list
//
void CSIMDlg::RefreshList(){
	
	CString totItemsTxt;
	totItemsTxt.Format("Total Items: %u", 0);
	SetDlgItemText(IDC_STATIC_TOTAL_ITEMS,totItemsTxt);

	m_TreeItems.DeleteAllItems();
	UpdateWindows();
	if(!GetHero()){ return; }

	HTREEITEM hRootItem = m_TreeItems.InsertItem(CString("\"") + GetHero()->GetHeroName() + CString("\""));
	m_TreeItems.SetItemData(hRootItem,~0);
	m_TreeItems.SetItemBold(hRootItem,TRUE);
	m_TreeItems.SetItemColor(hRootItem,RGB(0,0,180));

	CItemManager* pManager = GetHero()->GetItemManager();
	for(int i = 0; i < pManager->GetTotalMainItems(); i++){
		CItem* pItem = pManager->GetMainItemByIndex(i);

		HTREEITEM hMainItem = m_TreeItems.InsertItem(IToS(*pItem->GetItemDescId()),hRootItem);
		m_TreeItems.SetItemData(hMainItem,*pItem->GetItemId());
		m_TreeItems.EnsureVisible(hMainItem);

		if(*pItem->GetItemUpdated()){
			m_TreeItems.SetItemBold(hMainItem,TRUE);
			m_TreeItems.SetItemColor(hMainItem,RGB(200,0,0));
		}

		for(int s = 0; s < pItem->GetSlottedArray()->GetSize(); s++){
			CItem* pSlottedItem = (CItem*)pItem->GetSlottedArray()->GetAt(s);

			HTREEITEM hSlottedItem = m_TreeItems.InsertItem(IToS(*pSlottedItem->GetItemDescId()),hMainItem);
			m_TreeItems.SetItemData(hSlottedItem,s);
			m_TreeItems.EnsureVisible(hSlottedItem);

			if(*pSlottedItem->GetItemUpdated()){
				m_TreeItems.SetItemBold(hSlottedItem,TRUE);
				m_TreeItems.SetItemColor(hSlottedItem,RGB(200,0,0));
			}
		}
	}

	m_TreeItems.EnsureVisible(hRootItem);

	m_pItemDisp->SetHero(GetHero());
	m_pItemDisp->SetSelection(NULL);

	UpdateWindows();

	totItemsTxt.Format("Total Items: %u", GetHero()->GetItemManager()->CountTotalItems());
	SetDlgItemText(IDC_STATIC_TOTAL_ITEMS,totItemsTxt);
}

//
// selects an item by item id
//
void CSIMDlg::SelectItemById(ULONG ulItemId, int nSlotIndex/* = -1*/){
	HTREEITEM hRoot = m_TreeItems.GetRootItem();
	HTREEITEM hCurrent = m_TreeItems.GetChildItem(hRoot);
	while(hCurrent){
		if(m_TreeItems.GetItemData(hCurrent)==ulItemId){
			if(nSlotIndex != -1){
				HTREEITEM hSlotItem = m_TreeItems.GetChildItem(hCurrent);
				int nCount = 0;
				while(hSlotItem){
					if(nCount++ == nSlotIndex){
						m_TreeItems.EnsureVisible(hSlotItem);
						m_TreeItems.Select(hSlotItem,TVGN_CARET);
						return;
					}
					hSlotItem = m_TreeItems.GetNextItem(hSlotItem, TVGN_NEXT);
				}
			}else{
				m_TreeItems.EnsureVisible(hCurrent);
				m_TreeItems.Select(hCurrent,TVGN_CARET);
				return;
			}
		}
		hCurrent = m_TreeItems.GetNextItem(hCurrent, TVGN_NEXT);
	}
}

//
// gets the current selected item by index
//
BOOL CSIMDlg::GetCurrentSelection(CItem** lpItemMain, int* lpnIndexSlotted){
	*lpItemMain = NULL;
	*lpnIndexSlotted = -1;
	if(!GetHero()){ return FALSE; }

	HTREEITEM hSelectedItem = m_TreeItems.GetSelectedItem();
	if(!hSelectedItem){ return FALSE; }

	ULONG ulSelItemData = m_TreeItems.GetItemData(hSelectedItem);
	if(ulSelItemData == ~0){ return FALSE; }

	HTREEITEM hSelectedParentItem = m_TreeItems.GetParentItem(hSelectedItem);
	ULONG ulSelParentItemData = m_TreeItems.GetItemData(hSelectedParentItem);

	BOOL bMainItem = (ulSelParentItemData==~0);

	if(bMainItem){
		*lpItemMain = GetHero()->GetItemManager()->GetItemById(ulSelItemData);
	}else{
		*lpItemMain = GetHero()->GetItemManager()->GetItemById(ulSelParentItemData);
		*lpnIndexSlotted = (int)ulSelItemData;
	}

	return TRUE;
}

//
// gets the current selected item
//
BOOL CSIMDlg::GetCurrentSelection(CItem** lpItemMain, CItem** lpItemSlotted){
	*lpItemMain = NULL;
	*lpItemSlotted = NULL;
	if(!GetHero()){ return FALSE; }

	HTREEITEM hSelectedItem = m_TreeItems.GetSelectedItem();
	if(!hSelectedItem){ return FALSE; }

	ULONG ulSelItemData = m_TreeItems.GetItemData(hSelectedItem);
	if(ulSelItemData == ~0){ return FALSE; }

	HTREEITEM hSelectedParentItem = m_TreeItems.GetParentItem(hSelectedItem);
	ULONG ulSelParentItemData = m_TreeItems.GetItemData(hSelectedParentItem);

	BOOL bMainItem = (ulSelParentItemData==~0);

	if(bMainItem){
		*lpItemMain = GetHero()->GetItemManager()->GetItemById(ulSelItemData);
	}else{
		*lpItemMain = GetHero()->GetItemManager()->GetItemById(ulSelParentItemData);
		*lpItemSlotted = GetHero()->GetItemManager()->GetItemById(ulSelParentItemData,ulSelItemData);
	}

	return TRUE;
}

//
// notify that the selection on the tree has changed...
//
void CSIMDlg::OnSelchangedTreeItems(NMHDR* pNMHDR, LRESULT* pResult){
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CItem* lpMain,*lpChild;
	if(GetCurrentSelection(&lpMain,&lpChild)){
		m_pItemDisp->SetSelection(lpMain);
	}else{
		m_pItemDisp->SetSelection(NULL);
	}

	UpdateWindows();

	*pResult = 0;
}

//
// on button save current hero
//
void CSIMDlg::OnButtonSaveCurrentHero(){

	if(!m_pHero){ return; }

	m_pItemDisp->DisableDisplays(TRUE);

	CString target;
	GetDlgItemText(IDC_EDIT_CURRENT_HERO,target);

	if(MessageBox("Would you like to create a backup file just incase we screwed anything up? :)","Confirm...",MB_YESNO|MB_ICONQUESTION)==IDYES){

		char fileName[MAX_PATH];
		char* fName = strrchr(target,'\\') + 1;
		sprintf(fileName,"BACKUP_%s", fName);

		char* ext = strrchr(fName,'.');
		ext = (ext?++ext:"pax");

		CFileDlg fileDlg(FALSE,
						 "Save Sacred Hero File...",
						 GetSafeHwnd(),
						 "All Supported Heros Types|*.PAX;*.PTX|Sacred Heros (*.PAX)|*.PAX|Hero Template Files (*.PTX)|*.PTX||",
						 ext,
						 fileName);

		CString rc;
		if( !(rc = fileDlg.GetResult()).GetLength() ){
			MessageBox("Hero update cancelled by user!","Cancelled...", MB_ICONEXCLAMATION|MB_OK);
			m_pItemDisp->DisableDisplays(FALSE);
			return;
		}
		CopyFile(target,rc,FALSE);
	}
	
	CTempFile tempFile;

	if( m_pHero->CompileHeroFile(tempFile.GetFileName()) ){
		if(tempFile.CopyOverFile(target)){
			MessageBox("Updated hero successfully!","Success...",MB_ICONINFORMATION|MB_OK);
			OnButtonReloadCurrentHero();
		}else{
			MessageBox("Update hero failed...\nNothing has been written due to a file error.","Error...", MB_ICONHAND|MB_OK);
		}
	}else{
		MessageBox("Update hero failed at application level!\nNothing has been written.","Failed...",MB_ICONHAND|MB_OK);
	}
	m_pItemDisp->DisableDisplays(FALSE);
}

//
// on button show/hide placement
//
void CSIMDlg::OnButtonPlacement(){
	m_pItemDisp->ShowWindow((m_pItemDisp->IsWindowVisible()?SW_HIDE:SW_SHOW));
}

//
// updates all the main windows based on status of selection and hero
//
void CSIMDlg::UpdateWindows(){
	CItem* lpMain,*lpChild;
	BOOL bSel = GetCurrentSelection(&lpMain,&lpChild);
	BOOL bHero = (m_pHero != NULL);
	CSize invSize = (lpMain&&!lpChild?*lpMain->GetItemInvSize():CSize(1,1));

	GetDlgItem(IDC_BUTTON_CLOSE_CURRENT_HERO)->EnableWindow(bHero);
	GetDlgItem(IDC_BUTTON_RELOAD_CURRENT_HERO)->EnableWindow(bHero);
	GetDlgItem(IDC_BUTTON_SAVE_CURRENT_HERO)->EnableWindow(bHero);
	GetDlgItem(IDC_BUTTON_DUPLICATE_ITEM)->EnableWindow(bSel);
	GetDlgItem(IDC_BUTTON_SHRINKITEM)->EnableWindow(invSize.cx>1||invSize.cy>1);
	GetDlgItem(IDC_BUTTON_DELETEITEM)->EnableWindow(bSel);
	GetDlgItem(IDC_BUTTON_UNSLOT_ITEM)->EnableWindow(lpChild!=NULL);
	GetDlgItem(IDC_BUTTON_EDIT_ITEM)->EnableWindow(bSel);
	GetDlgItem(IDC_BUTTON_EXPORT_ITEM)->EnableWindow(bSel);
	GetDlgItem(IDC_BUTTON_IMPORT_ITEM)->EnableWindow(bHero);
	GetDlgItem(IDC_BUTTON_VIEWITEM)->EnableWindow(bSel);
	GetDlgItem(IDC_BUTTON_EDIT_DESC_ID)->EnableWindow(bSel);
}

//
// reloads the current hero
//
void CSIMDlg::OnButtonReloadCurrentHero(){

	CString currHero;
	GetDlgItemText(IDC_EDIT_CURRENT_HERO,currHero);
	OnButtonCloseCurrentHero();

	m_pHero = new CHero();

	if(!GetHero() || !GetHero()->LoadHeroFile(currHero)){
		SAFE_DELETE(m_pHero);
		MessageBox("Failed to load this hero file!","Error...", MB_ICONHAND|MB_OK);
		return;
	}

	SetDlgItemText(IDC_EDIT_CURRENT_HERO,currHero);
	RefreshList();

}

//
// duplicates selected item
//
void CSIMDlg::OnButtonDuplicateItem(){
	CItem* pMainItem, *pChildItem;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }

	m_pItemDisp->DisableDisplays(TRUE);

	CItem* pItemToDupe = (!pChildItem?pMainItem:pChildItem);

	CDupeItemDlg dlg(this);
	dlg.SetItemToDupe( pItemToDupe );
	if(dlg.DoModal()==IDCANCEL){ m_pItemDisp->DisableDisplays(FALSE); return; }

	CItem newItem(pItemToDupe);

	if(dlg.GetShrinkItem()){
		newItem.GetItemInvSize()->cx = 1;
		newItem.GetItemInvSize()->cy = 1;
	}

	if(!dlg.GetCarrySlots()){
		newItem.ClearSlottedArray();
	}

	ULONG ulPlacementType = (dlg.GetToBackpack()?SECTION_PLACEMENT_BP:SECTION_PLACEMENT_CH);

	CItem* pLastItem = NULL;
	int nCountAdded = 0;
	for(int i = 0; i < dlg.GetHowManyDupes(); i++){
		BOOL rc = GetHero()->GetItemManager()->ImportItem(&newItem,ulPlacementType,&pLastItem);
		if(!rc){
			break;
		}else{
			++nCountAdded;
		}
	}

	RefreshList();

	if(pLastItem){
		SelectItemById(*pLastItem->GetItemId());
		m_pItemDisp->SetSelection(pLastItem);
	}else{
		SelectItemById(*pItemToDupe->GetItemId());
		m_pItemDisp->SetSelection(pItemToDupe);
	}

	if(nCountAdded != dlg.GetHowManyDupes()){
		if(!nCountAdded){
			MessageBox("NO items were duplicated due to a total lack of space in selected inventory!","Error...", MB_ICONHAND|MB_OK);
		}else{
			CString txt;
			txt.Format("We have run out of space in the selected inventory.\nHowever, I managed to add %u out of the %u items in which you requested.", nCountAdded, dlg.GetHowManyDupes());
			MessageBox(txt,"Notice...", MB_ICONINFORMATION|MB_OK);
		}
	}

	m_pItemDisp->DisableDisplays(FALSE);
}

//
// shrinks the selected item
//
void CSIMDlg::OnButtonShrinkitem(){
	CItem* pMainItem, *pChildItem;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }

	pMainItem->GetItemInvSize()->cx = 1;
	pMainItem->GetItemInvSize()->cy = 1;
	*pMainItem->GetItemUpdated() = TRUE;

	RefreshList();

	m_pItemDisp->SetSelection(pMainItem);
	SelectItemById(*pMainItem->GetItemId());
}

//
// deletes the selected item
//
void CSIMDlg::OnButtonDeleteitem(){
	CItem* pMainItem, *pChildItem;
	int nChildItem = -1;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }
	if(!GetCurrentSelection(&pMainItem,&nChildItem)){ return; }

	m_pItemDisp->DisableDisplays(TRUE);

	CItem* pItemToDelete = (!pChildItem?pMainItem:pChildItem);

	CString txt;
	txt.Format("You are about to delete an item.\n\nPlease take note that if this item has items slotted\nwithin it (which this one %s), that the slotted\nitems will also be deleted.\n\nIf you want to save the slotted items, hilite the item and\nclick click the `Unslot Item` button.\n\nAnd as always, changes do NOT take place until you\nclick the `Update` button.\n\nWould you like to continue?\n\nNOTE: The message when you click \"Unslot\" applies to\ndeleting slotted items also.",(pItemToDelete->GetSlottedArray()->GetSize()?"DOES":"does NOT"));

	if(MessageBox(txt,"Confirm...", MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO){ m_pItemDisp->DisableDisplays(FALSE); return; }

	GetHero()->GetItemManager()->DeleteItem(pMainItem,nChildItem);

	RefreshList();

	if(nChildItem != -1){
		m_pItemDisp->SetSelection(pMainItem);
		SelectItemById(*pMainItem->GetItemId());
	}else{
		m_pItemDisp->SetSelection(NULL);
	}

	m_pItemDisp->DisableDisplays(FALSE);
}

//
// if the item is a slotted item, it moves it off of that item
// and into whatever inventory main was in
//
void CSIMDlg::OnButtonUnslotItem(){

	CItem* pMainItem, *pChildItem;
	int nChildItem = -1;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }
	if(!GetCurrentSelection(&pMainItem,&nChildItem)){ return; }

	if(nChildItem == -1){ return; }

	m_pItemDisp->DisableDisplays(TRUE);

	MessageBox("This was taken out due to the fact that once an item is slotted,\nthe item itself gets the slotted items attributes placed within the\nitem it was slotted in.  Because of this, if you unslot it with a\nprogram, the min. lvl on the parent item shoots way up and is\nunfixable (unless you know how to edit items).\n\nSo, if you want to unslot an item, duplicate the slotted\nitems then go to the blacksmith in Sacred and have him\nunslot them correctly.\n\nActually, if you really really want to try, then go for it...\n\nYou have been warned :)","Notice...", MB_ICONINFORMATION|MB_OK);

	// find out to where
	CUnslotItemDlg dlg(this);
	if(dlg.DoModal()==IDCANCEL){
		m_pItemDisp->DisableDisplays(FALSE);
		return;
	}

	// gets a copy of the slotted item...
	CItem nSlotted = *pMainItem << nChildItem;
	
	//CItem nSlotted = *(CItem*)pMainItem->GetSlottedArray()->GetAt(nChildItem);

	// sets the size
	*nSlotted.GetItemInvSize() = CSize(1,1);

	ULONG ulToInv = (dlg.UnslotToBackpack()?SECTION_PLACEMENT_BP:SECTION_PLACEMENT_CH);

	// imports it
	CItem* pNewItem = NULL;
	if(!GetHero()->GetItemManager()->ImportItem(&nSlotted, ulToInv,&pNewItem)){
		MessageBox("Unable to import this slotted item due to lack of space in\ninventory (or you are currently wearing this item)\n\nPlease clear out some of this inventory first.","Error...", MB_ICONHAND|MB_OK);
		m_pItemDisp->DisableDisplays(FALSE);
		return;
	}

	// remove it from main item
	CItem* pOldItem = ((CItem*)pMainItem->GetSlottedArray()->GetAt(nChildItem));
	SAFE_DELETE( pOldItem );
	pMainItem->GetSlottedArray()->RemoveAt(nChildItem);
	--*pMainItem->GetItemSlotCount();
	*pMainItem->GetItemUpdated() = TRUE;

	RefreshList();

	// select new item
	m_pItemDisp->SetSelection(pMainItem);
	SelectItemById(*pMainItem->GetItemId());

	m_pItemDisp->DisableDisplays(FALSE);
	
}

//
// opens an edit item dlg
//
void CSIMDlg::OnButtonEditItem(){
	CItem* pMainItem, *pChildItem;
	int nChildItem = -1;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }
	if(!GetCurrentSelection(&pMainItem,&nChildItem)){ return; }

	m_pItemDisp->DisableDisplays(TRUE);

	CItem* pItemToEdit = (!pChildItem?pMainItem:pChildItem);

	CEditItemDlg dlg(this);
	dlg.SetItem(pItemToEdit);
	if(dlg.DoModal()==IDCANCEL){ m_pItemDisp->DisableDisplays(FALSE); return; }

	*pItemToEdit->GetItemUpdated() = TRUE;

	RefreshList();

	// select new item
	m_pItemDisp->SetSelection(pItemToEdit);
	SelectItemById(*pItemToEdit->GetItemId());

	m_pItemDisp->DisableDisplays(FALSE);
}

//
// exports an item
//
void CSIMDlg::OnButtonExportItem(){
	CItem* pMainItem, *pChildItem;
	int nChildItem = -1;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }
	if(!GetCurrentSelection(&pMainItem,&nChildItem)){ return; }

	m_pItemDisp->DisableDisplays(TRUE);

	CItem* pItemToExport = (!pChildItem?pMainItem:pChildItem);

	CExportItemDlg dlg(this);
	dlg.SetItem(pItemToExport);
	if(dlg.DoModal()==IDCANCEL){
		m_pItemDisp->DisableDisplays(FALSE);
		return;
	}

	m_pItemDisp->DisableDisplays(FALSE);
}

//
// imports an item
//
void CSIMDlg::OnButtonImportItem(){
	if(!GetHero()){ return; }

	m_pItemDisp->DisableDisplays(TRUE);

	CImportItemDlg dlg(this);
	if(dlg.DoModal()==IDCANCEL){ m_pItemDisp->DisableDisplays(FALSE); return; }

	CItem* pItem = dlg.GetImportedItem();

	if(!GetHero()->GetItemManager()->ImportItem(pItem,*pItem->GetItemInvStorage(),&pItem)){
		MessageBox("Failed to import this item due to total lack of space in selected inventory.\n(Try `shrinking` the item on import)","Error...",MB_ICONHAND|MB_OK);
		m_pItemDisp->DisableDisplays(FALSE);
		return;
	}

	RefreshList();

	// select new item
	m_pItemDisp->SetSelection(pItem);
	SelectItemById(*pItem->GetItemId());

	m_pItemDisp->DisableDisplays(FALSE);
}

//
// view an item
//
void CSIMDlg::OnButtonViewitem(){
	CItem* pMainItem, *pChildItem;
	int nChildItem = -1;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }
	if(!GetCurrentSelection(&pMainItem,&nChildItem)){ return; }

	CItem* pItemToView = (!pChildItem?pMainItem:pChildItem);

	CSITFile sit;
	sit.SetItem(pItemToView);

	m_pItemDisp->DisableDisplays(TRUE);
	CViewItemDlg dlg(this);
	dlg.SetSITFile(&sit);
	dlg.DoModal();
	m_pItemDisp->DisableDisplays(FALSE);
}

//
// user double clicked on tree view
//
void CSIMDlg::OnDblclkTreeItems(NMHDR* pNMHDR, LRESULT* pResult){
	OnButtonViewitem();
	*pResult = 0;
}

//
// about button
//
void CSIMDlg::OnButtonAbout(){
	m_pItemDisp->DisableDisplays(TRUE);
	CAboutDlg dlg(this);
	dlg.DoModal();
	m_pItemDisp->DisableDisplays(FALSE);
}

//
// help button
//
void CSIMDlg::OnButtonHelp(){
#ifndef _DEBUG
	ShellExecute(GetSafeHwnd(),"open","http://www.mouseindustries.com/sacred/simhelp/",NULL,NULL,SW_SHOWNORMAL);
#else
	/*
	CItem* pMainItem, *pChildItem;
	int nChildItem = -1;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }
	if(!GetCurrentSelection(&pMainItem,&nChildItem)){ return; }

	CItem item = *pMainItem;

	GetHero()->GetItemManager()->DeleteItem(pMainItem);

	ULONG idNow = 1;
	for(int i = 0; i < 100; i++){
		for(int q = 0; q < 8; q++){
			*(ULONG*)&item.GetItemData()->buf[0x4A+(q*sizeof(ULONG))] = idNow++;
			*(USHORT*)&item.GetItemData()->buf[0x6A+(q*sizeof(USHORT))] = q+1;
		}
		GetHero()->GetItemManager()->ImportItem(&item,SECTION_PLACEMENT_BP);
	}

	RefreshList();

	// select new item
	m_pItemDisp->SetSelection(NULL);
	*/

	CItem* pMainItem, *pChildItem;
	int nChildItem = -1;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }

	CItem item = *pMainItem;

	GetHero()->GetItemManager()->DeleteItem(pMainItem);

	for(int i = 5765; i < 5790; i++){
		*(ULONG*)item.GetItemData()->buf = i;
		*item.GetItemDescId() = i;
		GetHero()->GetItemManager()->ImportItem(&item,SECTION_PLACEMENT_BP);
	}

	RefreshList();

	// select new item
	m_pItemDisp->SetSelection(NULL);


#endif
}

//
// cancel button or X
//
void CSIMDlg::OnCancel(){
	
	if(MessageBox("Are you sure you want to exit?","Confirm...",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDNO){
		return;
	}

	CDialog::OnCancel();
}

void CSIMDlg::OnButtonEditDescId(){
	CItem* pMainItem, *pChildItem;
	int nChildItem = -1;
	if(!GetCurrentSelection(&pMainItem,&pChildItem)){ return; }
	if(!GetCurrentSelection(&pMainItem,&nChildItem)){ return; }

	m_pItemDisp->DisableDisplays(TRUE);

	CItem* pItemToEdit = (!pChildItem?pMainItem:pChildItem);

	CEditIDDlg dlg(this);
	dlg.SetDescId((ULONG*)pItemToEdit->GetItemDescId());
	if(dlg.DoModal()==IDCANCEL){ m_pItemDisp->DisableDisplays(FALSE); return; }

	*(ULONG*)pItemToEdit->GetItemData()->buf = *(ULONG*)pItemToEdit->GetItemDescId();
	*pItemToEdit->GetItemUpdated() = TRUE;

	RefreshList();

	// select new item
	m_pItemDisp->SetSelection(pItemToEdit);
	SelectItemById(*pItemToEdit->GetItemId());
	
	m_pItemDisp->DisableDisplays(FALSE);
}
