// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemManager.h"
#include "Item.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem(){
	m_pData.buf = NULL;
	Reset();
}

CItem::CItem(CItem* pItem){
	m_pData.buf = NULL;
	Reset();
	if(pItem){
		*this = *pItem;
	}
}

CItem::CItem(CItem& rItem){
	m_pData.buf = NULL;
	Reset();
	*this = rItem;
}

CItem::~CItem()
{
	Reset();
}

void CItem::Reset(){
	m_ulDescId = 0;
	m_ulItemId = 0;
	m_ulInvStored = -1;
	m_ptInvPlacement.x = -1;
	m_ptInvPlacement.y = -1;
	m_szInvSize.cx = 1; // default of 1x1
	m_szInvSize.cy = 1;
	m_usType = 386;
	m_ulTail[0] = 0;
	m_ulTail[1] = 0;
	m_ulTail[2] = BADCODE;
	m_pParentItem = 0;
	m_bItemUpdated = FALSE;
	SAFE_DELETE(m_pData.buf);
	m_pData.size = 0;
	ClearSlottedArray();
}

void CItem::ClearSlottedArray(){
	for(int i = 0; i < m_arrSlotted.GetSize(); i++){
		CItem* pItem = (CItem*)m_arrSlotted.GetAt(i);
		SAFE_DELETE(pItem);
	}
	m_arrSlotted.RemoveAll();
	m_ucSlotCount = 0;
}

BOOL CItem::GetItemUpdatedFullCheck(){
	if(m_bItemUpdated){ return TRUE; }
	for(int i = 0; i < m_arrSlotted.GetSize(); i++){
		CItem* pCur = (CItem*)m_arrSlotted.GetAt(i);
		if(*pCur->GetItemUpdated()){
			return TRUE;
		}
	}
	return FALSE;
}
