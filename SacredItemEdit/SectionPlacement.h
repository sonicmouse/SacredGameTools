// SectionPlacement.h: interface for the CSectionPlacement class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SECTION_PLACEMENT_H__
#define __SECTION_PLACEMENT_H__

class CSectionPlacement  
{
public:
	CSectionPlacement();
	virtual ~CSectionPlacement();

	BOOL CompilePlacement(PST_BUFFER pBuffer);

	void SetPlacementType(ULONG nType){m_ulPlacementType = nType; }
	ULONG GetPlacementType(){ return m_ulPlacementType; }

	BOOL SetPlacementBuffer(PST_BUFFER pBuffer);

	void Reset();

	// get info
	PST_PLACEMENT_ITEM GetItemPlacement(int x, int y);
	PST_PLACEMENT_ITEM GetItemPlacement(ULONG index);
	CSize GetItemInvSize(ULONG index);
	void ShrinkItem(ULONG index);
	CSize GetRowSpan(){ return CSize(m_Placement.width,m_Placement.height); }

	// edit
	void DeleteItem(ULONG index);
	BOOL SetItemAt(ULONG index, ULONG itemId, CSize szItem, CPoint pt);

private:
	ULONG m_ulPlacementType;
	ST_PLACEMENT m_Placement;
};

#endif /* __SECTION_PLACEMENT_H__ */
