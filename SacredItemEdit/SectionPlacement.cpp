// SectionPlacement.cpp: implementation of the CSectionPlacement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "SectionPlacement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSectionPlacement::CSectionPlacement()
{
	m_ulPlacementType = 0;

	m_Placement.placement.buf = NULL;
	m_Placement.placement.size = 0;

	m_Placement.tail.buf = NULL;
	m_Placement.tail.size = 0;
}

CSectionPlacement::~CSectionPlacement()
{
	Reset();
}

BOOL CSectionPlacement::SetPlacementBuffer(PST_BUFFER pBuffer){

	UCHAR* p = pBuffer->buf;
	ULONG size = pBuffer->size;

	m_Placement = *(ST_PLACEMENT*)p;
	ULONG ulTotalSlots = m_Placement.width*m_Placement.height;

	p += 12/*header size*/+1024/*fluff size*/;
	size -= 12 + 1024;

	m_Placement.placement.size = ulTotalSlots * sizeof(ST_PLACEMENT_ITEM);
	m_Placement.placement.buf = new UCHAR[m_Placement.placement.size];
	memcpy(m_Placement.placement.buf,p,m_Placement.placement.size);
	
	p += m_Placement.placement.size;
	size -= m_Placement.placement.size;

	m_Placement.tail.size = size;
	m_Placement.tail.buf = new UCHAR[m_Placement.tail.size];
	memcpy(m_Placement.tail.buf,p,m_Placement.tail.size);

	return TRUE;
}

void CSectionPlacement::Reset(){
	SAFE_DELETE(m_Placement.placement.buf);
	SAFE_DELETE(m_Placement.tail.buf);
	m_Placement.placement.size = 0;
	m_Placement.tail.size = 0;
}

PST_PLACEMENT_ITEM CSectionPlacement::GetItemPlacement(int x, int y){
	return (PST_PLACEMENT_ITEM)&m_Placement.placement.buf[(y*m_Placement.width+x)*sizeof(ST_PLACEMENT_ITEM)];
}

BOOL CSectionPlacement::CompilePlacement(PST_BUFFER pBuffer){

	pBuffer->size = 12/*header size*/+1024/*fluff size*/;
	pBuffer->size += m_Placement.placement.size;
	pBuffer->size += m_Placement.tail.size;

	pBuffer->buf = new UCHAR[pBuffer->size];

	UCHAR* p = pBuffer->buf;
	*(ST_PLACEMENT*)p = m_Placement;
	p += 12/*header size*/+1024/*fluff size*/;

	memcpy(p,m_Placement.placement.buf,m_Placement.placement.size);
	p += m_Placement.placement.size;

	memcpy(p,m_Placement.tail.buf,m_Placement.tail.size);

	return TRUE;
}

PST_PLACEMENT_ITEM CSectionPlacement::GetItemPlacement(ULONG index){
	for(int x = 0; x < m_Placement.width; x++){
		for(int y = 0; y < m_Placement.height; y++){
			PST_PLACEMENT_ITEM p = GetItemPlacement(x,y);
			if(p->index == index){
				return p;
			}
		}
	}
	return NULL;
}

CSize CSectionPlacement::GetItemInvSize(ULONG index){
	PST_PLACEMENT_ITEM pItem = GetItemPlacement(index);
	if(pItem){
		return CSize(pItem->width,pItem->height);
	}
	return CSize(0,0);
}

void CSectionPlacement::DeleteItem(ULONG index){
	CSize rowSpan = GetRowSpan();
	for(int x = 0; x < rowSpan.cx; x++){
		for(int y = 0; y < rowSpan.cy; y++){
			PST_PLACEMENT_ITEM pItem = GetItemPlacement(x,y);
			if(pItem->index == index){
				// got it
				int width = pItem->width;
				int height = pItem->height;
				for(int xx = x; xx < x+width; xx++){
					for(int yy = y; yy < y+height; yy++){
						PST_PLACEMENT_ITEM pSubItem = GetItemPlacement(xx,yy);
						pSubItem->height = 0;
						pSubItem->index = 0;
						pSubItem->itemid = 0;
						pSubItem->slotused = 0;
						pSubItem->unknown = 0;
						pSubItem->width = 0;
					}
				}
				return;
			}
		}
	}
}

void CSectionPlacement::ShrinkItem(ULONG index){
	CSize rowSpan = GetRowSpan();
	for(int x = 0; x < rowSpan.cx; x++){
		for(int y = 0; y < rowSpan.cy; y++){
			PST_PLACEMENT_ITEM pItem = GetItemPlacement(x,y);
			if(pItem->index == index){
				int width = pItem->width;
				int height = pItem->height;
				for(int xx = x; xx < x+width; xx++){
					for(int yy = y; yy < y+height; yy++){
						PST_PLACEMENT_ITEM pSubItem = GetItemPlacement(xx,yy);
						if(x == xx && y==yy){
							pSubItem->height = 1;
							pSubItem->width = 1;
							pSubItem->slotused = 1;
						}else{
							pSubItem->height = 0;
							pSubItem->index = 0;
							pSubItem->itemid = 0;
							pSubItem->slotused = 0;
							pSubItem->unknown = 0;
							pSubItem->width = 0;
						}
					}
				}
				return;
			}
		}
	}
}

BOOL CSectionPlacement::SetItemAt(ULONG index, ULONG itemId, CSize szItem, CPoint pt){
	int itemSize = szItem.cx*szItem.cy;
	bool* fitArray = new bool[itemSize];
	memset(fitArray,false,itemSize);

	CSize rowSpan = GetRowSpan();

	int onIndex = 0;
	for(int x = pt.x; x < __min(pt.x+szItem.cx,rowSpan.cx); x++){
		for(int y = pt.y; y < __min(pt.y+szItem.cy,rowSpan.cy); y++){
			PST_PLACEMENT_ITEM pItem = GetItemPlacement(x,y);
			if(pItem && !pItem->slotused){
				fitArray[onIndex++] = true;
			}
		}
	}

	for(int i = 0; i < itemSize; i++){
		if(!fitArray[i]){
			SAFE_DELETE(fitArray);
			return FALSE;
		}
	}

	SAFE_DELETE(fitArray);

	// add item...
	for(x = pt.x; x < pt.x+szItem.cx; x++){
		for(int y = pt.y; y < pt.y+szItem.cy; y++){
			PST_PLACEMENT_ITEM pItem = GetItemPlacement(x,y);
			pItem->slotused = true;
			if(x==pt.x&&y==pt.y){
				pItem->width = (UCHAR)szItem.cx;
				pItem->height = (UCHAR)szItem.cy;
				pItem->index = index;
				pItem->itemid = itemId;
				pItem->unknown = 0;
			}else{
				pItem->width = (UCHAR)pt.x;
				pItem->height = (UCHAR)pt.y;
			}
		}
	}

	return TRUE;
}
