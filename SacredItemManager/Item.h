// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEM_H__
#define __ITEM_H__

#define BADCODE 0xBAADC0DE

class CItem  
{
public:
	CItem(CItem* pItem);
	CItem(CItem& rItem);
	CItem();
	virtual ~CItem();

	// methods
public:
	void Reset();
	void ClearSlottedArray();
	BOOL GetItemUpdatedFullCheck();

	ULONG* GetItemDescId(){ return &m_ulDescId; }
	ULONG* GetItemId(){ return &m_ulItemId; }
	ULONG* GetItemInvStorage(){ return &m_ulInvStored; }
	CPoint* GetItemInvPlacement(){ return &m_ptInvPlacement; }
	CSize* GetItemInvSize(){ return &m_szInvSize; }
	ST_BUFFER* GetItemData(){ return &m_pData; }
	USHORT* GetItemType(){ return &m_usType; }
	UCHAR* GetItemSlotCount(){ return &m_ucSlotCount; }
	CPtrArray* GetSlottedArray(){ return &m_arrSlotted; }
	ULONG* GetItemTail(){ return m_ulTail; }
	ULONG* GetItemParent(){ return &m_pParentItem; }
	BOOL* GetItemUpdated(){ return &m_bItemUpdated; }

	// operators
public:

	// moves a slotted item from this's slot array out by slot index...
	// be careful with this, you have to stay within bounds :)
	/*
	Example:
	CItem a,b;
	b = a << 2;
	'b' now has 'a's 2nd slotted index, 'a' does not change
	*/
	CItem& operator<<(int nSlotIndex){
		return *(CItem*)m_arrSlotted.GetAt(nSlotIndex);
	}

	// adds rItem to this's slot
	/*
	Example:
	CItem a,b,c;
	c = a + b;
	'c' is now equal to 'a' but with 'b' as one of 'a's slots
	'a' and 'b' do not change
	*/
	CItem& operator+(CItem& rItem){
		// copy it over to a new item
		CItem* pCopy = new CItem();
		*pCopy = rItem;
		// the copy can not have any slots since its a slotted item now, so
		// remove all the slotted items from it
		for(int i = 0; i < pCopy->GetSlottedArray()->GetSize(); i++){
			CItem* pItem = (CItem*)pCopy->GetSlottedArray()->GetAt(i);
			SAFE_DELETE(pItem);
		}
		*pCopy->GetItemSlotCount() = 0;
		pCopy->GetSlottedArray()->RemoveAll();
		// add our clean item to this's slot array
		m_arrSlotted.Add(pCopy);
		++m_ucSlotCount;
		return *this;
	}

	// compairitive == operator
	/*
	Example:
	CItem a,b;
	bool bSame = (a==b);
	'a' and 'b' are exactly the same if (a==b) returns true
	*/
	bool operator==(CItem& rItem){
		if( (m_ulDescId == *rItem.GetItemDescId()) &&
			(m_ulItemId == *rItem.GetItemId()) &&
			(m_ulInvStored == *rItem.GetItemInvStorage()) &&
			(m_ptInvPlacement == *rItem.GetItemInvPlacement()) &&
			(m_szInvSize == *rItem.GetItemInvSize()) &&
			(m_usType == *rItem.GetItemType()) &&
			(m_ucSlotCount == *rItem.GetItemSlotCount()) &&
			(m_ulTail[0] == rItem.GetItemTail()[0]) &&
			(m_ulTail[1] == rItem.GetItemTail()[1]) &&
			(m_ulTail[2] == rItem.GetItemTail()[2]) &&
			(m_pParentItem == *rItem.GetItemParent()) &&
			(m_bItemUpdated == *rItem.GetItemUpdated()) ){
			if( rItem.GetItemData()->size == m_pData.size ){
				if(rItem.GetItemData()->buf && rItem.GetItemData()->size){
					if(memcmp(rItem.GetItemData()->buf,m_pData.buf, m_pData.size)!=0){
						return false;
					}
				}
			}else{
				return false;
			}
			bool bMatchSlotted = true;
			if( rItem.GetSlottedArray()->GetSize() == m_arrSlotted.GetSize() ){
				for(int i = 0; i < rItem.GetSlottedArray()->GetSize(); i++){
					CItem* pCurR = (CItem*)rItem.GetSlottedArray()->GetAt(i);
					CItem* pMe = (CItem*)m_arrSlotted.GetAt(i);
					if((*pCurR == *pMe)==false){
						bMatchSlotted = false;
						break;
					}
				}
			}
			if(bMatchSlotted){
				return true;
			}
		}
		return false;
	}

	// assignment = operator
	/*
	Example:
	CItem a,b;
	a = b;
	'a' will now be an exact copy of 'b'... slots and all :D
	*/
	CItem& operator=(CItem& rItem) {
		// clear out the item which is getting the data
		Reset();
		// basic members
		m_ulDescId = *rItem.GetItemDescId();
		m_ulItemId = *rItem.GetItemId();
		m_ulInvStored = *rItem.GetItemInvStorage();
		m_ptInvPlacement = *rItem.GetItemInvPlacement();
		m_szInvSize = *rItem.GetItemInvSize();
		m_usType = *rItem.GetItemType();
		m_ucSlotCount = *rItem.GetItemSlotCount();
		m_ulTail[0] = rItem.GetItemTail()[0];
		m_ulTail[1] = rItem.GetItemTail()[1];
		m_ulTail[2] = rItem.GetItemTail()[2];
		m_pParentItem = *rItem.GetItemParent();
		m_bItemUpdated = *rItem.GetItemUpdated();
		// item data
		if(rItem.GetItemData()->buf && rItem.GetItemData()->size){
			m_pData.buf = new UCHAR[rItem.GetItemData()->size];
			if(m_pData.buf){
				m_pData.size = rItem.GetItemData()->size;
				memcpy(m_pData.buf,rItem.GetItemData()->buf,rItem.GetItemData()->size);
			}
		}
		// recursive for slotted
		for(int i = 0; i < rItem.GetSlottedArray()->GetSize(); i++){
			CItem* pCur = (CItem*)rItem.GetSlottedArray()->GetAt(i);
			CItem* pCopy = new CItem();
			*pCopy = *pCur;
			m_arrSlotted.Add(pCopy);
		}
		// return me
		return *this;
	}

	// members
private:
	ULONG m_ulDescId;
	ULONG m_ulItemId;
	ULONG m_ulInvStored;
	CPoint m_ptInvPlacement;
	CSize m_szInvSize;
	ST_BUFFER m_pData;
	USHORT m_usType;
	UCHAR m_ucSlotCount;
	CPtrArray m_arrSlotted;
	ULONG m_ulTail[3];
	ULONG m_pParentItem;
	BOOL m_bItemUpdated;
};

#endif /* __ITEM_H__ */
