// Hero.h: interface for the CHero class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SACRED_HERO_H__
#define __SACRED_HERO_H__

#include "SectionInventory.h"

class CHero
{
public:
	CHero(const CString& szHeroFile);
	virtual ~CHero();

	BOOL IsLoaded(){ return m_bLoaded; }
	BOOL CompileHeroFile(const CString& szFile);

	// item stuff
	ULONG GetItemCount(BOOL bItemsOnly = FALSE);
	PST_ITEM GetItem(int nIndex);

	void SetItemUpdated(int nIndex, int nItem, BOOL bUpdated){ m_Inv.SetItemUpdated(nIndex,nItem,bUpdated); }
	BOOL GetItemUpdated(int nIndex, int nItem){ return m_Inv.GetItemUpdated(nIndex, nItem); }
	void ResetItemUpdated(){ m_Inv.ResetItemUpdated(); }

	PST_PLACEMENT_ITEM GetItemPlacement(int nInvType, CPoint pt);
	PST_PLACEMENT_ITEM GetItemPlacement(int nInvType, ULONG index);
	CSize GetRowSpan(int nInvType);
	CSize GetItemInvSize(ULONG index);
	void ShrinkItem(ULONG index){ m_Inv.ShrinkItem(index); }

	int GetSlottedItemsCopy(ULONG slotIndex, CPtrArray* pArray){ return m_Inv.GetSlottedItemsCopy(slotIndex,pArray); }
	
	void DeleteItem(ULONG slotIndex){ m_Inv.DeleteItem(slotIndex); }
	void DeleteSlottedItem(ULONG slotIndex, int subSlotIndex){ m_Inv.DeleteSlottedItem(slotIndex,subSlotIndex); }

	ULONG ImportItem(PST_BUFFER pItemBuffer, CSize szItem, int nInv){
		return m_Inv.ImportItem(pItemBuffer, szItem, nInv); }

	BOOL ImportItemToNextAvalibleSlot(ULONG slotIndex, PST_BUFFER pItemBuffer, int nSlotColor){
		return m_Inv.ImportItemToNextAvalibleSlot(slotIndex,pItemBuffer,nSlotColor);
	}

	CString GetMyName(){ return m_szName; }

private:
	BOOL DecompileHeroFile();
	void ClearSections();
	BOOL ReplaceSection(ULONG ulSection, PST_BUFFER pBuffer);
	void SetMyName(PST_BUFFER pBuffer);

private:
	CString m_szName;

	CString m_szHeroFile;
	BOOL m_bLoaded;
	unsigned char m_ucFluffHeader[0xF8];
	CPtrArray m_arrSections;

	CSectionInventory m_Inv;
	BOOL m_bSectionInvLoaded;
	BOOL m_bPlacementLoadedBP;
	BOOL m_bPlacementLoadedCH;
};

#endif /* __SACRED_HERO_H__ */
