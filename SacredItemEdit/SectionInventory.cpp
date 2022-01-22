// SectionInventory.cpp: implementation of the CSectionInventory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "SectionInventory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSectionInventory::CSectionInventory(){

}

CSectionInventory::~CSectionInventory(){
	ClearItemBuffer();
}

BOOL CSectionInventory::SetInventoryData(PST_BUFFER pBuffer){
	// clear everything...
	ClearItemBuffer();

	// set a pointer and get total items
	unsigned char* p = pBuffer->buf;
	ULONG ulTotalItems = *(ULONG*)p;
	p+=sizeof(ULONG);

#ifdef _DEBUG
	TRACE1("\n\nCounting %u items...\n\n", ulTotalItems);
#endif

	bool bIsCleanup = false;

	// go thru every item
	for(ULONG i = 0; i < ulTotalItems; i++){
		// new item
		PST_ITEM pItem = new ST_ITEM;
		// transfer over the header
		pItem->header = *(ST_ITEM_HEADER*)p;
		// if the header is null, or NULL_ITEM, its a spacer
		if(pItem->header.itemid == 0){
#ifdef _DEBUG
			TRACE1("ZERO ITEM FOUND [ind: %u] ++++++++++++++++\n\n", i);
#endif
			//this is just a null item.. but still an item!
			p += sizeof(ULONG);
			m_arrItems.Add(pItem);
		}else if(pItem->header.itemid == NULL_ITEM){
#ifdef _DEBUG
			TRACE0("0xBAADFOOD BREAK FOUND ****************\n\n");
#endif
			PST_BUFFER pBfBuff = new ST_BUFFER;
			pBfBuff->size = NULL_ITEM_SIZE;
			pBfBuff->buf = new UCHAR[pBfBuff->size];
			memcpy(pBfBuff->buf,p,pBfBuff->size);
			pItem->itemdataarr.Add(pBfBuff);
			p += pBfBuff->size;
			m_arrItems.Add(pItem);
			// this doesnt count as an "item", so take it out of count and
			// keep track of how many we found
			if(!bIsCleanup){ --i; }
			++m_nBadFoodCount;
		}else{
			// see if it legit
			if(VerifyItem(pItem->header.feedfood,i,p-pBuffer->buf,pBuffer)){
				SAFE_DELETE(pItem);
				ClearItemBuffer();
				return FALSE;
			}
#ifdef _DEBUG
			TRACE3("----> Found MAIN item [%u] type %u [ind: %u]\n", pItem->header.itemid, pItem->header.itemtype,i);
#endif
			// move up to items
			p += sizeof(pItem->header);

			// go thru every item
			for(int s = 0; s < pItem->header.slotcount+1; s++){
				PST_ITEM_DATA pItemData = new ST_ITEM_DATA;

				pItemData->itemid = *(ULONG*)p;

				ULONG ulPayloadSize = DEFAULT_ITEM_PAYLOAD_SIZE;

				if(pItem->header.itemtype == 257){
					ulPayloadSize = DEFAULT_ITEM_PAYLOAD_SIZE_LARGE;
				}else if((pItem->header.itemtype == 385) && (pItemData->itemid == TYPE_385_SIZE_INCLUDED)){
					ulPayloadSize = __max(*(ULONG*)&p[sizeof(ULONG)*3],DEFAULT_ITEM_PAYLOAD_SIZE_LARGE);
				}

				pItemData->payload.size = ulPayloadSize;
				pItemData->payload.buf = new UCHAR[ulPayloadSize];
				memcpy(pItemData->payload.buf,p,ulPayloadSize);
				p += ulPayloadSize;

				pItem->itemdataarr.Add(pItemData);

#ifdef _DEBUG
				TRACE2("+++++++++++++> Found SUB item %u [ind: %u]\n", pItemData->itemid,s);
#endif
			}
			// add tail bytes
			pItem->tail1 = *(ULONG*)p;
			p += sizeof(ULONG);
			pItem->tail2 = *(ULONG*)p;
			p += sizeof(ULONG);
			pItem->tail3 = *(ULONG*)p;
			p += sizeof(ULONG);
			// finally, add it
			m_arrItems.Add(pItem);
		}
		// check for any remaining bad foods
		if(i == ulTotalItems - 1){
			ULONG leftOver = pBuffer->size-(p-pBuffer->buf);
			if(leftOver){
				bIsCleanup = true;
				--i;
			}
		}
	}

#ifdef _DEBUG
	TRACE1("BAD FOOD FOUND: %u\n", m_nBadFoodCount);
#endif

	if((ULONG)m_arrItems.GetSize()-m_nBadFoodCount != ulTotalItems){
		return FALSE;
	}

	for(int r = 0; r < m_arrItems.GetSize(); r++){
		m_arrUpdated.Add(0);
	}

	return TRUE;
}

void CSectionInventory::ClearItemBuffer(){
	m_arrUpdated.RemoveAll();
	while(m_arrItems.GetSize()>0){
		PST_ITEM pItem = (PST_ITEM)m_arrItems.GetAt(0);
		for(int i = 0; i < pItem->itemdataarr.GetSize(); i++){
			if(pItem->header.itemid == NULL_ITEM){
				PST_BUFFER pBfBuff = (PST_BUFFER)pItem->itemdataarr.GetAt(i);
				SAFE_DELETE(pBfBuff->buf);
				SAFE_DELETE(pBfBuff);
			}else{
				PST_ITEM_DATA pItemData = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(i);
				SAFE_DELETE(pItemData->payload.buf);
				SAFE_DELETE(pItemData);
			}
		}
		SAFE_DELETE(pItem);
		m_arrItems.RemoveAt(0);
	}
	m_nBadFoodCount = 0;
}

BOOL CSectionInventory::SetPlacementDataBP(PST_BUFFER pBuffer){
	return m_SectionBP.SetPlacementBuffer(pBuffer);
}

BOOL CSectionInventory::SetPlacementDataCH(PST_BUFFER pBuffer){
	return m_SectionCH.SetPlacementBuffer(pBuffer);
}

BOOL CSectionInventory::GetInventoryData(PST_BUFFER pBuffer,BOOL bIgnoreFiller/* = FALSE*/){
	// now we compile this bitch :D

	pBuffer->buf = NULL;
	pBuffer->size = sizeof(ULONG);

	// calculate buffer size...
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		PST_ITEM pItem = (PST_ITEM)m_arrItems.GetAt(i);
		if(pItem->header.itemid == 0){
			if(!bIgnoreFiller){
				pBuffer->size += sizeof(ULONG);
			}
		}else if(pItem->header.itemid == NULL_ITEM){
			if(!bIgnoreFiller){
				pBuffer->size += ((PST_BUFFER)pItem->itemdataarr.GetAt(0))->size;
			}
		}else{
			pBuffer->size += sizeof(pItem->header);
			for(int q = 0; q < pItem->header.slotcount+1; q++){
				PST_ITEM_DATA pItemData = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(q);
				pBuffer->size += pItemData->payload.size;
			}
			pBuffer->size += sizeof(pItem->tail1);
			pBuffer->size += sizeof(pItem->tail2);
			pBuffer->size += sizeof(pItem->tail3);
		}
	}

	// allocate the buffer
	pBuffer->buf = new UCHAR[pBuffer->size];
	if(!pBuffer->buf){ return FALSE; }

	// compile it
	UCHAR* p = pBuffer->buf;

	// count how many zero items there are
	int countZero = 0;
	for(int z = 0; z < m_arrItems.GetSize(); z++){
		if(((PST_ITEM)m_arrItems.GetAt(z))->header.itemid == 0){
			++countZero;
		}
	}

	// set total items
	*(ULONG*)p = (ULONG)m_arrItems.GetSize()-(!bIgnoreFiller?m_nBadFoodCount:m_nBadFoodCount+countZero);
	p += sizeof(ULONG);

	// go thru each item and write it out
	for(i = 0; i < m_arrItems.GetSize(); i++){
		PST_ITEM pItem = (PST_ITEM)m_arrItems.GetAt(i);
		if(pItem->header.itemid == 0){
			if(!bIgnoreFiller){
				*(ULONG*)p = 0;
				p += sizeof(ULONG);
			}
		}else if(pItem->header.itemid == NULL_ITEM){
			if(!bIgnoreFiller){
				PST_BUFFER pBfBuff = (PST_BUFFER)pItem->itemdataarr.GetAt(0);
				memcpy(p,pBfBuff->buf,pBfBuff->size);
				p += pBfBuff->size;
			}
		}else{
			// write item header
			memcpy(p,&pItem->header,sizeof(pItem->header));
			p += sizeof(pItem->header);
			// write item data
			for(int s = 0; s < pItem->header.slotcount+1; s++){
				PST_ITEM_DATA pItemData = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(s);
				memcpy(p,pItemData->payload.buf,pItemData->payload.size);
				p += pItemData->payload.size;
			}
			// write tail
			*(ULONG*)p = pItem->tail1;
			p += sizeof(ULONG);
			*(ULONG*)p = pItem->tail2;
			p += sizeof(ULONG);
			*(ULONG*)p = pItem->tail3;
			p += sizeof(ULONG);
		}
	}
	return TRUE;
}

BOOL CSectionInventory::VerifyItem(ULONG ulType, ULONG index, ULONG ulOffsetNow, PST_BUFFER pBuffer){
	if(ulType == ITEM_TYPE){ return FALSE; }

	CString txt;
	txt.Format("Found odd item type at offset [0x%08X] index [%u]\n\nWould you like to save this error to disk and email it to the author so a fix may be made?\n\nIf so, send the error file to sacred@mouseindustries.com\n\nThanks", ulOffsetNow,index);
	if(MessageBox(AfxGetMainWnd()->GetSafeHwnd(),txt,"Item Error...", MB_ICONQUESTION|MB_YESNO)==IDYES){

		char fileTitle[MAX_PATH];
		sprintf(fileTitle, "%X [%u].sbd", ulOffsetNow,index);

		CString txt; txt.Format("Save Dump [0x%08X, %u]", ulOffsetNow,index);

		CFileDlg fileDlg(FALSE,
						 txt,
						 AfxGetMainWnd()->GetSafeHwnd(),
						 "Sacred Binary Dump (*.SBD)|*.SBD||",
						 "sbd",
						 fileTitle);

		CString rc;
		if((rc = fileDlg.GetResult()).GetLength()){
			FILE* f = fopen(rc,WRITE_BINARY);
			if(f){
				fwrite(pBuffer->buf,pBuffer->size,1,f);
				fclose(f);
			}
		}
	}

	return TRUE;
}

void CSectionInventory::DeleteItem(ULONG slotIndex){
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		PST_ITEM pItem = (PST_ITEM)m_arrItems.GetAt(i);
		if(pItem->header.slotindex == slotIndex){
			for(int s = 0; s < pItem->itemdataarr.GetSize(); s++){
				PST_ITEM_DATA pItemData = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(s);
				SAFE_DELETE(pItemData->payload.buf);
				SAFE_DELETE(pItemData);
			}
			SAFE_DELETE(pItem);
			m_arrItems.RemoveAt(i);
			m_arrUpdated.RemoveAt(i);
			m_SectionBP.DeleteItem(slotIndex);
			m_SectionCH.DeleteItem(slotIndex);
			return;
		}
	}
}

int CSectionInventory::GetSlottedItemsCopy(ULONG slotIndex, CPtrArray* pArray){
	int nCount = 0;
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		PST_ITEM pItem = (PST_ITEM)m_arrItems.GetAt(i);
		if(pItem->header.slotindex == slotIndex){
			for(int s = 0; s < pItem->itemdataarr.GetSize()-1; s++){
				++nCount;
				if(pArray){
					PST_ITEM_DATA pItemData = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(s);

					PST_ITEM_DATA pCopy = new ST_ITEM_DATA;
					pCopy->itemid = pItemData->itemid;
					pCopy->payload.size = pItemData->payload.size;
					pCopy->payload.buf = new UCHAR[pItemData->payload.size];
					memcpy(pCopy->payload.buf,pItemData->payload.buf, pItemData->payload.size);

					pArray->Add(pCopy);
				}
			}
			return nCount;
		}
	}
	return nCount;
}

BOOL CSectionInventory::ImportItemToNextAvalibleSlot(ULONG slotIndex, PST_BUFFER pItemBuffer, int nSlotColor){
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		PST_ITEM pItem = (PST_ITEM)m_arrItems.GetAt(i);
		if(pItem->header.slotindex == slotIndex){

			PST_ITEM_DATA pNewItem = new ST_ITEM_DATA;
			pNewItem->itemid = *(ULONG*)pItemBuffer->buf;
			pNewItem->payload.size = pItemBuffer->size;
			pNewItem->payload.buf = new UCHAR[pItemBuffer->size];
			memcpy(pNewItem->payload.buf,pItemBuffer->buf,pItemBuffer->size);

			int totalSlottedItems = pItem->itemdataarr.GetSize()-1;
			pItem->itemdataarr.InsertAt(totalSlottedItems,pNewItem);
			
			++pItem->header.slotcount;

			PST_ITEM_DATA pMainItem = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(pItem->itemdataarr.GetSize()-1);
			pMainItem->payload.buf[0x0C+totalSlottedItems] = nSlotColor;

			m_arrUpdated.SetAt(i,m_arrUpdated.GetAt(i)|1);

			return TRUE;
		}
	}
	return FALSE;
}

void CSectionInventory::DeleteSlottedItem(ULONG slotIndex, int subSlotIndex){
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		PST_ITEM pItem = (PST_ITEM)m_arrItems.GetAt(i);
		if(pItem->header.slotindex == slotIndex){
			for(int s = 0; s < pItem->itemdataarr.GetSize()-1; s++){
				if(s==subSlotIndex){
					PST_ITEM_DATA pItemData = (PST_ITEM_DATA)pItem->itemdataarr.GetAt(s);
					SAFE_DELETE(pItemData->payload.buf);
					SAFE_DELETE(pItemData);
					pItem->itemdataarr.RemoveAt(s);
					--pItem->header.slotcount;
					break;
				}
			}
			return;
		}
	}
}

ULONG CSectionInventory::CreateUniqueItemId(){
	ULONG ulMax = 0;
	for(int i = 0; i < m_arrItems.GetSize(); i++){
		PST_ITEM pItem = (PST_ITEM)m_arrItems.GetAt(i);
		if(pItem->header.itemid && (pItem->header.itemid != NULL_ITEM)){
			ulMax = __max(ulMax, pItem->header.slotindex);
		}
	}
	return ++ulMax;
}

ULONG CSectionInventory::ImportItem(PST_BUFFER pItemBuffer, CSize szItem, int nInv){

	CSectionPlacement* place = NULL;
	if(nInv == SECTION_PLACEMENT_BP){
		place = &m_SectionBP;
	}else if(nInv == SECTION_PLACEMENT_CH){
		place = &m_SectionCH;
	}else{
		return 0;
	}

	ULONG slotIndex = CreateUniqueItemId();

	CSize rowSpan = place->GetRowSpan();
	bool bInserted = false;
	for(int x = 0; x < rowSpan.cx; x++){
		for(int y = 0; y < rowSpan.cy; y++){
			if(place->SetItemAt(slotIndex, *(ULONG*)pItemBuffer->buf, szItem,CPoint(x,y))){
				bInserted = true;
				break;
			}
		}
		if(bInserted){ break; }
	}
	if(!bInserted){ return 0; }

	// create a new header
	PST_ITEM pNewItem = new ST_ITEM;
	memset(&pNewItem->header,0,sizeof(pNewItem->header));
	pNewItem->header.feedfood = ITEM_TYPE;
	pNewItem->header.itemid = *(ULONG*)pItemBuffer->buf;
	pNewItem->header.itemid2 = pNewItem->header.itemid;
	pNewItem->header.itemtype = 386; // TODO
	pNewItem->header.sixtyfour = 64;
	pNewItem->header.slotindex = slotIndex;
	pNewItem->tail3 = 3131949278;

	// create new data in the header
	PST_ITEM_DATA pNewItemData = new ST_ITEM_DATA;

	pNewItemData->payload.size = pItemBuffer->size;
	pNewItemData->payload.buf = new UCHAR[pItemBuffer->size];

	memcpy(pNewItemData->payload.buf,pItemBuffer->buf,pItemBuffer->size);
	pNewItemData->itemid = *(ULONG*)pNewItemData->payload.buf;

	pNewItem->itemdataarr.Add(pNewItemData);

	// finally add it
	m_arrItems.Add(pNewItem);
	m_arrUpdated.Add(1);

	return slotIndex;
}
