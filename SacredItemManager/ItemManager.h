// ItemManager.h: interface for the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#include "item.h"

#pragma pack(push,1)

// placement page file description
typedef struct _tag_placement{
	USHORT unknown1;
	ULONG unknown2;
	USHORT unknown3;
	USHORT width;
	USHORT height;
	UCHAR fluff[1024];
	ST_BUFFER tail;
	ULONG type;
} ST_PLACEMENT,*PST_PLACEMENT;

// helper struct for item placement
typedef struct _tag_placement_item{
	ULONG index;
	UCHAR slotused;
	UCHAR width;
	UCHAR height;
	UCHAR unknown;
	ULONG itemid;
} ST_PLACEMENT_ITEM,*PST_PLACEMENT_ITEM;

// helper struct for item header
typedef struct _tag_item_header{
	ULONG itemdescid;
	ULONG feedfood;
	USHORT itemtype;
	ULONG sixtyfour;
	ULONG itemid;
	ULONG itemdescid2;
	UCHAR body[0xE9];
	UCHAR slotcount;
} ST_ITEM_HEADER,*PST_ITEM_HEADER;
#pragma pack(pop)

#define NULL_ITEM 0xBAADF00D
#define NULL_ITEM_SIZE (sizeof(ULONG)*5)

#define DEFAULT_ITEM_PAYLOAD_SIZE 0x82
#define DEFAULT_ITEM_PAYLOAD_SIZE_LARGE 0x379
#define TYPE_385_SIZE_INCLUDED 33554432

#define ITEM_TYPE 0xFEEDF00D

class CItemManager  
{
public:
	CItemManager();
	virtual ~CItemManager();

	int GetTotalMainItems(){ return m_arrItems.GetSize(); }
	CItem* GetMainItemByIndex(int nIndex){ return (CItem*)m_arrItems.GetAt(nIndex); }
	CItem* GetItemById(ULONG ulId, int nIndex = -1);

	CSize GetPlacementInvSize(ULONG ulPlacementType);

	BOOL ImportItem(CItem* pItem, ULONG ulPlacemntType, CItem** pulNewItem = NULL);
	void DeleteItem(CItem* pItem, int nSubItemIndex = -1);

	ULONG CountTotalItems();

	// 
	BOOL LoadPlacement(ULONG ulPlacementType, PST_BUFFER lpBuffer);
	BOOL LoadItemInventory(PST_BUFFER lpBuffer);

	BOOL CompilePlacement(ULONG ulPlacemntType, PST_BUFFER lpBuffer);
	BOOL CompileItemInventory(PST_BUFFER lpBuffer);

	void Reset();

	BOOL FindRoomForItem(CItem* pItem);

private:
	CItem* GetOrCreateItem(PST_ITEM_HEADER pItemHeader);
	void SetUniqueItemId(CItem* pItem);

private:
	CPtrArray m_arrPlacementPages;
	CPtrArray m_arrItems;
};

#endif /* __ITEM_MANAGER_H__ */
