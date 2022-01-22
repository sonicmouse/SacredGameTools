// SectionInventory.h: interface for the CSectionInventory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SECTION_INVENTORY_H__
#define __SECTION_INVENTORY_H__

#include "SectionPlacement.h"

class CSectionInventory
{
public:
	CSectionInventory();
	virtual ~CSectionInventory();

	int GetItemCount(){ return m_arrItems.GetSize(); }
	PST_ITEM GetItem(int nIndex){ return (PST_ITEM)m_arrItems.GetAt(nIndex); }

	void DeleteItem(ULONG slotIndex);
	int GetSlottedItemsCopy(ULONG slotIndex, CPtrArray* pArray);
	void DeleteSlottedItem(ULONG slotIndex, int subSlotIndex);

	ULONG ImportItem(PST_BUFFER pItemBuffer, CSize szItem, int nInv);
	BOOL ImportItemToNextAvalibleSlot(ULONG slotIndex, PST_BUFFER pItemBuffer, int nSlotColor);

	void ShrinkItem(ULONG slotIndex){
		m_SectionBP.ShrinkItem(slotIndex);
		m_SectionCH.ShrinkItem(slotIndex);
	}

	BOOL SetInventoryData(PST_BUFFER pBuffer);
	BOOL GetInventoryData(PST_BUFFER pBuffer, BOOL bIgnoreFiller = FALSE);

	BOOL SetPlacementDataBP(PST_BUFFER pBuffer);
	BOOL SetPlacementDataCH(PST_BUFFER pBuffer);

	CSectionPlacement* GetPlacementBP(){ return &m_SectionBP; }
	CSectionPlacement* GetPlacementCH(){ return &m_SectionCH; }

	// item update tracking
	void SetItemUpdated(int nIndex, int nItem, BOOL bUpdated){
		UINT val = m_arrUpdated.GetAt(nIndex);
		if(bUpdated){val |= nItem;}else{val &= ~nItem;}
		m_arrUpdated.SetAt(nIndex,val);
	}
	BOOL GetItemUpdated(int nIndex, int nItem){return ((m_arrUpdated.GetAt(nIndex) & nItem) == (UINT)nItem);}
	void ResetItemUpdated(){ for(int i = 0; i < m_arrUpdated.GetSize(); i++){ m_arrUpdated.SetAt(i,0); }}

private:
	void ClearItemBuffer();
	BOOL VerifyItem(ULONG ulType, ULONG index, ULONG ulOffsetNow, PST_BUFFER pBuffer);
	ULONG CreateUniqueItemId();

private:
	CPtrArray m_arrItems;
	CUIntArray m_arrUpdated;
	int m_nBadFoodCount;

	CSectionPlacement m_SectionBP;
	CSectionPlacement m_SectionCH;
};

#endif /* __SECTION_INVENTORY_H__ */
