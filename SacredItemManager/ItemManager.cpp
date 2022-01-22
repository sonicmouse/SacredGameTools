// ItemManager.cpp: implementation of the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemManager.h"
#include "ItemManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemManager::CItemManager()
{

}

CItemManager::~CItemManager()
{
	Reset();
}

//
// loads all the items that are placed
//
BOOL CItemManager::LoadPlacement(ULONG ulPlacementType, PST_BUFFER lpBuffer){

	// make sure we dont duplicate sections
	for(int i = 0; i < m_arrPlacementPages.GetSize(); i++){
		PST_PLACEMENT pPlace = (PST_PLACEMENT)m_arrPlacementPages.GetAt(i);
		if(pPlace->type == ulPlacementType){
			SAFE_DELETE(pPlace->tail.buf);
			SAFE_DELETE(pPlace);
			m_arrPlacementPages.RemoveAt(i);
			break;
		}
	}
	// add it
	bool bGood = false;
	PST_PLACEMENT pPlace = new ST_PLACEMENT;
	if(pPlace){
		memset(pPlace,0,sizeof(*pPlace));

		UCHAR* p = lpBuffer->buf;
		ULONG size = lpBuffer->size;

		*pPlace = *(ST_PLACEMENT*)lpBuffer->buf;
		p += 12/*header size*/+1024/*fluff size*/;
		size -= 12 + 1024;

		pPlace->type = ulPlacementType;

		// now we are at the placement data...
		for(int x = 0; x < pPlace->width; x++){
			for(int y = 0; y < pPlace->height; y++){
				PST_PLACEMENT_ITEM ptr = (PST_PLACEMENT_ITEM)&p[(y*pPlace->width+x)*sizeof(ST_PLACEMENT_ITEM)];
				if(ptr->itemid && ptr->slotused && ptr->index){
					CItem* pItem = new CItem();
					if(pItem){
						*pItem->GetItemInvStorage() = ulPlacementType;
						*pItem->GetItemId() = ptr->index;
						*pItem->GetItemDescId() = ptr->itemid;
						//*pItem->GetItemInvUnknown() = ptr->unknown;

						pItem->GetItemInvSize()->cx = ptr->width;
						pItem->GetItemInvSize()->cy = ptr->height;
						pItem->GetItemInvPlacement()->x = x;
						pItem->GetItemInvPlacement()->y = y;

						m_arrItems.Add(pItem);
					}
				}
			}
		}

		int nSizePlacementBuffer = sizeof(ST_PLACEMENT_ITEM)*(pPlace->width*pPlace->height);
		p += nSizePlacementBuffer;
		size -= nSizePlacementBuffer;

		pPlace->tail.buf = new UCHAR[size];
		if(pPlace->tail.buf){
			pPlace->tail.size = size;
			memcpy(pPlace->tail.buf,p,pPlace->tail.size);
			// add it
			m_arrPlacementPages.Add(pPlace);
			bGood = true;
		}else{
			// out of memory on tail alloc
			pPlace->tail.size = 0;
		}
	}else{
		// out of memory on placement alloc
	}
	if(!bGood){
		Reset();
	}

	return bGood;
}

//
// gets the width and height of a certain inventory
//
CSize CItemManager::GetPlacementInvSize(ULONG ulPlacementType){
	for(int i = 0; i < m_arrPlacementPages.GetSize(); i++){
		PST_PLACEMENT pPlace = (PST_PLACEMENT)m_arrPlacementPages.GetAt(i);
		if(pPlace->type == ulPlacementType){
			return CSize(pPlace->width,pPlace->height);
		}
	}
	return CSize(-1,-1);
}

//
// compiles a full item placement
//
BOOL CItemManager::CompilePlacement(ULONG ulPlacemntType, PST_BUFFER lpBuffer){
	PST_PLACEMENT pTargetPlace = NULL;
	for(int i = 0; i < m_arrPlacementPages.GetSize(); i++){
		PST_PLACEMENT pPlace = (PST_PLACEMENT)m_arrPlacementPages.GetAt(i);
		if(pPlace->type == ulPlacemntType){
			pTargetPlace = pPlace;
			break;
		}
	}
	if(!pTargetPlace){ return FALSE; }

	// calculate size
	int nSizePlacementBuffer = sizeof(ST_PLACEMENT_ITEM)*(pTargetPlace->width*pTargetPlace->height);

	lpBuffer->size = 12+1024; /* header and fluff */
	lpBuffer->size += nSizePlacementBuffer;
	lpBuffer->size += pTargetPlace->tail.size;

	// allocate it
	lpBuffer->buf = new UCHAR[lpBuffer->size];
	if(!lpBuffer->buf){
		lpBuffer->size = 0;
		return FALSE;
	}

	UCHAR* p = lpBuffer->buf;

	*(ST_PLACEMENT*)p = *pTargetPlace;
	p += 12+1024;

	memset(p,0,nSizePlacementBuffer);

	// compile placement...
	for(i = 0; i < m_arrItems.GetSize(); i++){
		CItem* pItem = (CItem*)m_arrItems.GetAt(i);
		if(*pItem->GetItemInvStorage()==ulPlacemntType){
			CPoint* pt = pItem->GetItemInvPlacement();
			for(int x = pt->x; x < pt->x+pItem->GetItemInvSize()->cx; x++){
				for(int y = pt->y; y < pt->y+pItem->GetItemInvSize()->cy; y++){
					PST_PLACEMENT_ITEM ptr = (PST_PLACEMENT_ITEM)&p[(y*pTargetPlace->width+x)*sizeof(ST_PLACEMENT_ITEM)];
					ptr->slotused = true;
					if((x==pt->x)&&(y==pt->y)){
						ptr->width = (UCHAR)pItem->GetItemInvSize()->cx;
						ptr->height = (UCHAR)pItem->GetItemInvSize()->cy;
						ptr->index = *pItem->GetItemId();
						ptr->itemid = *pItem->GetItemDescId();
					}else{
						ptr->width = (UCHAR)pt->x;
						ptr->height = (UCHAR)pt->y;
						ptr->unknown = 0;
					}
				}
			}
		}
	}
	
	p += nSizePlacementBuffer;

	memcpy(p,pTargetPlace->tail.buf,pTargetPlace->tail.size);

#ifdef _DEBUG
	p += pTargetPlace->tail.size;
	ASSERT((p-lpBuffer->buf)==(int)lpBuffer->size);
#endif

	return TRUE;
}
	

//
// loads the items in the inventory
//
BOOL CItemManager::LoadItemInventory(PST_BUFFER lpBuffer){

	// set a pointer and get total items
	UCHAR* p = lpBuffer->buf;
	ULONG ulTotalItems = *(ULONG*)p;
	p+=sizeof(ULONG);

	bool bIsCleanup = false;
	// go thru every item
	int nNotCountedItems = 0;
	for(ULONG i = 0; i < ulTotalItems; i++){

		PST_ITEM_HEADER pItemHeader = (ST_ITEM_HEADER*)p;

		if(!pItemHeader->itemdescid){
			p += sizeof(pItemHeader->itemdescid);
			++nNotCountedItems;
		}else if(pItemHeader->itemdescid == NULL_ITEM){
			p += NULL_ITEM_SIZE;
			if(!bIsCleanup){ --i; }
		}else{
			// check for parsing error...
			if(pItemHeader->feedfood != ITEM_TYPE){
				// bad error :/ (TODO)
#ifdef _DEBUG
//				MessageBox(AfxGetMainWnd()->GetSafeHwnd(),"bad hero","error", MB_ICONHAND|MB_OK);
#endif
				Reset();
				return FALSE;
			}
			// find or create item...
			CItem* pItem = GetOrCreateItem(pItemHeader);
			// move up to item data...
			p += sizeof(*pItemHeader);
			// go through each subitem
			for(int s = 0; s < pItemHeader->slotcount+1; s++){
				bool bIsMainItem = (s==pItemHeader->slotcount);

				ULONG itemDescId = *(ULONG*)p;

				// this is shitty code (poor attempt at trying to figure out horses)
				ULONG ulPayloadSize = DEFAULT_ITEM_PAYLOAD_SIZE;
				if(*pItem->GetItemType() == 257){
					ulPayloadSize = DEFAULT_ITEM_PAYLOAD_SIZE_LARGE;
				}else if( (*pItem->GetItemType() == 385) && (itemDescId == TYPE_385_SIZE_INCLUDED)){
					ulPayloadSize = __max(*(ULONG*)&p[sizeof(ULONG)*3],DEFAULT_ITEM_PAYLOAD_SIZE_LARGE);
				}

				if(bIsMainItem){
					PST_BUFFER lpItemData = pItem->GetItemData();
					lpItemData->buf = new UCHAR[ulPayloadSize];
					if(lpItemData){
						lpItemData->size = ulPayloadSize;
						memcpy(lpItemData->buf,p,ulPayloadSize);
					}
				}else{
					CItem* pNewItem = new CItem();
					if(pNewItem){
						pItem->GetSlottedArray()->Add(pNewItem);

						*pNewItem->GetItemDescId() = itemDescId;
						*pNewItem->GetItemParent() = (ULONG)pItem;

						PST_BUFFER lpItemData = pNewItem->GetItemData();
						lpItemData->buf = new UCHAR[ulPayloadSize];
						if(lpItemData->buf){
							lpItemData->size = ulPayloadSize;
							memcpy(lpItemData->buf,p,ulPayloadSize);
						}
					}
				}
				p += ulPayloadSize;

			}

			// add tail
			pItem->GetItemTail()[0] = *(ULONG*)p;
			p += sizeof(ULONG);
			pItem->GetItemTail()[1] = *(ULONG*)p;
			p += sizeof(ULONG);
			pItem->GetItemTail()[2] = *(ULONG*)p;
			p += sizeof(ULONG);

		}
		// check for any remaining bad foods
		if(i == ulTotalItems - 1){
			if(lpBuffer->size-(p-lpBuffer->buf)){
				bIsCleanup = true;
				--i;
			}
		}
	}// end (for)

	if((ULONG)m_arrItems.GetSize()+nNotCountedItems == ulTotalItems){
		return TRUE;
	}

	Reset();
	return FALSE;
}

//
// compiles an full item iventory
//
BOOL CItemManager::CompileItemInventory(PST_BUFFER lpBuffer){
	// calculate size
	lpBuffer->size = sizeof(ULONG);
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		CItem* pItem = (CItem*)m_arrItems.GetAt(i);
		// item has a header
		lpBuffer->size += sizeof(ST_ITEM_HEADER);
		// item has sub items
		for(int s = 0; s < pItem->GetSlottedArray()->GetSize(); s++){
			CItem* pSItem = (CItem*)pItem->GetSlottedArray()->GetAt(s);
			lpBuffer->size += pSItem->GetItemData()->size;
		}
		// item has a body
		lpBuffer->size += pItem->GetItemData()->size;
		// item has a tail
		lpBuffer->size += sizeof(ULONG)*3;
	}

	lpBuffer->buf = new UCHAR[lpBuffer->size];
	if(!lpBuffer->buf){
		lpBuffer->size = 0;
		return FALSE;
	}

	UCHAR* p = lpBuffer->buf;
	*(ULONG*)p = m_arrItems.GetSize();
	p += sizeof(ULONG);

	for(i = 0; i < m_arrItems.GetSize(); i++){
		CItem* pItem = (CItem*)m_arrItems.GetAt(i);
		// set up and write header
		ST_ITEM_HEADER itemHeader;
		memset(&itemHeader,0,sizeof(itemHeader));
		itemHeader.feedfood = ITEM_TYPE;
		itemHeader.sixtyfour = 64;
		itemHeader.itemdescid = *pItem->GetItemDescId();
		itemHeader.itemdescid2 = *pItem->GetItemDescId();
		itemHeader.itemid = *pItem->GetItemId();
		itemHeader.itemtype = *pItem->GetItemType();
		itemHeader.slotcount = (UCHAR)pItem->GetSlottedArray()->GetSize();
		memcpy(p,&itemHeader,sizeof(ST_ITEM_HEADER));
		p += sizeof(ST_ITEM_HEADER);
		// write sub items
		for(int s = 0; s < pItem->GetSlottedArray()->GetSize(); s++){
			CItem* pSItem = (CItem*)pItem->GetSlottedArray()->GetAt(s);
			memcpy(p,pSItem->GetItemData()->buf,pSItem->GetItemData()->size);
			p += pSItem->GetItemData()->size;
		}
		// write main item data
		memcpy(p,pItem->GetItemData()->buf,pItem->GetItemData()->size);
		p += pItem->GetItemData()->size;
		// item has a tail
		*(ULONG*)p = pItem->GetItemTail()[0];
		p += sizeof(ULONG);
		*(ULONG*)p = pItem->GetItemTail()[1];
		p += sizeof(ULONG);
		*(ULONG*)p = pItem->GetItemTail()[2];
		p += sizeof(ULONG);
	}

#ifdef _DEBUG
	ASSERT((p-lpBuffer->buf)==(int)lpBuffer->size);
#endif

	return TRUE;
}

//
// frees all memory associated with this class
//
void CItemManager::Reset(){
	for(int i = 0; i < m_arrPlacementPages.GetSize(); i++){
		PST_PLACEMENT pPlace = (PST_PLACEMENT)m_arrPlacementPages.GetAt(i);
		SAFE_DELETE(pPlace->tail.buf);
		SAFE_DELETE(pPlace);
	}
	m_arrPlacementPages.RemoveAll();
	for(i = 0; i < m_arrItems.GetSize(); i++){
		CItem* pItem = (CItem*)m_arrItems.GetAt(i);
		SAFE_DELETE(pItem);
	}
	m_arrItems.RemoveAll();
}

//
// based on items header.. gets a current item, and if its not there then
// it creates a new one
//
CItem* CItemManager::GetOrCreateItem(PST_ITEM_HEADER pItemHeader){
	CItem* pItem = NULL;
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		CItem* pCurItem = (CItem*)m_arrItems.GetAt(i);
		if(*pCurItem->GetItemId()==pItemHeader->itemid){
			pItem = pCurItem;
			*pItem->GetItemSlotCount() = pItemHeader->slotcount;
			*pItem->GetItemDescId() = pItemHeader->itemdescid;
			*pItem->GetItemType() = pItemHeader->itemtype;
			break;
		}
	}
	if(pItem){ return pItem; }
	// if we get here, its a item the person is wearing (or a horse)
	// we have to allocate it and set it up
	pItem = new CItem();
	if(pItem){
		*pItem->GetItemSlotCount() = pItemHeader->slotcount;
		*pItem->GetItemDescId() = pItemHeader->itemdescid;
		*pItem->GetItemId() = pItemHeader->itemid;
		*pItem->GetItemType() = pItemHeader->itemtype;
		m_arrItems.Add(pItem);
	}
	return pItem;
}

//
// gets an item by ID
//
CItem* CItemManager::GetItemById(ULONG ulId, int nIndex/*=-1*/){
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		CItem* pItem = (CItem*)m_arrItems.GetAt(i);
		if(*pItem->GetItemId()==ulId){
			if(nIndex != -1){
				for(int s = 0; s < pItem->GetSlottedArray()->GetSize(); s++){
					if(s == nIndex){
						return (CItem*)pItem->GetSlottedArray()->GetAt(s);
					}
				}
			}else{
				return pItem;
			}
		}
	}
	return NULL;
}

//
// finds if there is enough room to store this item.
// if there is, it will place the coords in pItem and return TRUE;
// pItem must have a target inventory placed in it before hand...
//
BOOL CItemManager::FindRoomForItem(CItem* pItem){
	// find the inv place
	PST_PLACEMENT pTargetPlace = NULL;
	for(int i = 0; i < m_arrPlacementPages.GetSize(); i++){
		PST_PLACEMENT pPlace = (PST_PLACEMENT)m_arrPlacementPages.GetAt(i);
		if(pPlace->type == *pItem->GetItemInvStorage()){
			pTargetPlace = pPlace;
			break;
		}
	}
	if(!pTargetPlace){ return FALSE; }
	// create a virtual array of items
	int virtSize = pTargetPlace->width*pTargetPlace->height;
	UCHAR* arr = new UCHAR[virtSize];
	// set them all to blank
	memset(arr,0,virtSize);
	// go thru each item and set it
	for(i = 0; i < m_arrItems.GetSize(); i++){
		CItem* pCurr = (CItem*)m_arrItems.GetAt(i);
		if(*pCurr->GetItemInvStorage() == pTargetPlace->type){
			for(int x = pCurr->GetItemInvPlacement()->x; x < pCurr->GetItemInvPlacement()->x+pCurr->GetItemInvSize()->cx; x++){
				for(int y = pCurr->GetItemInvPlacement()->y; y < pCurr->GetItemInvPlacement()->y+pCurr->GetItemInvSize()->cy; y++){
					arr[y*pTargetPlace->width+x] = 1;
				}
			}
		}
	}
	// find room for this item
	bool bFoundSpot = false;
	int nItemSize = pItem->GetItemInvSize()->cx*pItem->GetItemInvSize()->cy;
	bool* pbArr = new bool[nItemSize];
	for(int x = 0; x < pTargetPlace->width; x++){
		for(int y = 0; y < pTargetPlace->height; y++){
			memset(pbArr,false,nItemSize);
			int nCountSlots = 0;
			for(int xx = x; xx < __min(x+pItem->GetItemInvSize()->cx,pTargetPlace->width); xx++){
				for(int yy = y; yy < __min(y+pItem->GetItemInvSize()->cy,pTargetPlace->height); yy++){
					if(!arr[yy*pTargetPlace->width+xx]){
						pbArr[nCountSlots++] = true;
					}
				}
			}// end (for) loop thru adjacent slots
			bool bSet = true;
			for(int q = 0; q < nItemSize; q++){
				if(!pbArr[q]){ bSet = false; break; }
			}
			if(bSet){
				bFoundSpot = true;
				pItem->GetItemInvPlacement()->x = x;
				pItem->GetItemInvPlacement()->y = y;
				break;
			}
		}// end (for) y
		if(bFoundSpot){ break; }
	}// end (for) x
	SAFE_DELETE(pbArr);
	SAFE_DELETE(arr);
	return bFoundSpot;
}

//
// creates a unique id and sets it in pItem
//
void CItemManager::SetUniqueItemId(CItem* pItem){
	ULONG maxId = 0;
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		CItem* pItem = (CItem*)m_arrItems.GetAt(i);
		maxId = __max(*pItem->GetItemId(),maxId);
	}
	*pItem->GetItemId() = ++maxId;
}

//
// imports an item into said inventory. Returns FALSE if there is no room
//
BOOL CItemManager::ImportItem(CItem* pItem, ULONG ulPlacemntType, CItem** pulNewItem/* = NULL*/){
	CItem* pItemCopy = new CItem();
	*pItemCopy = *pItem;
	if(pItemCopy->GetItemInvSize()->cx < 1 || pItemCopy->GetItemInvSize()->cy < 1){
		pItemCopy->GetItemInvSize()->cx = 1;
		pItemCopy->GetItemInvSize()->cy = 1;
	}
	*pItemCopy->GetItemInvStorage() = ulPlacemntType;
	if(!FindRoomForItem(pItemCopy)){
		// no room...
		SAFE_DELETE(pItemCopy);
		return FALSE;
	}
	// if there is no item type, default it to 386
	if(!*pItemCopy->GetItemType()){
		*pItemCopy->GetItemType() = 386;
	}
	// set the id
	SetUniqueItemId(pItemCopy);
	// if user wants item back, let them have it
	if(pulNewItem){
		*pulNewItem = pItemCopy;
	}
	// its updated
	*pItemCopy->GetItemUpdated() = TRUE;
	// so are the slots
	for(int i = 0; i < pItemCopy->GetSlottedArray()->GetSize(); i++){
		CItem* pCur = (CItem*)pItemCopy->GetSlottedArray()->GetAt(i);
		*pCur->GetItemUpdated() = TRUE;
	}
	// add it to list
	m_arrItems.Add(pItemCopy);
	// success
	return TRUE;
}

//
// counts all the given items on hero
//
ULONG CItemManager::CountTotalItems(){
	ULONG nCount = 0;
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		nCount += 1 + ((CItem*)m_arrItems.GetAt(i))->GetSlottedArray()->GetSize();
	}
	return nCount;
}

//
// deletes and item from the list of items
// if nSubItemIndex isnt -1, it only deletes the subitem
//
void CItemManager::DeleteItem(CItem* pItem, int nSubItemIndex/* = -1*/){
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		CItem* pCur = (CItem*)m_arrItems.GetAt(i);
		if(*pCur == *pItem){
			// found it...
			if(nSubItemIndex != -1){
				// kill of just the sub item...
				CItem* pChild = (CItem*)pCur->GetSlottedArray()->GetAt(nSubItemIndex);
				SAFE_DELETE(pChild);
				pCur->GetSlottedArray()->RemoveAt(nSubItemIndex);
				--*pCur->GetItemSlotCount();
				*pCur->GetItemUpdated() = TRUE;
			}else{
				// kill off the whole item
				SAFE_DELETE(pCur);
				m_arrItems.RemoveAt(i);
			}
		}
	}
}
