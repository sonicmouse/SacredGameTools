// ItemPlacementDisp.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "ItemPlacementDisp.h"
#include "MemDC.h"
#include "SIMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemPlacementDisp

CItemPlacementDisp::CItemPlacementDisp()
{
	m_ulInvType = 0;
	m_pHero = NULL;
	m_nSelItemInd = -1;
	m_pSelItem = NULL;
	m_bDisabled = FALSE;
	m_fontText.CreateFont(13, 0, 0, 0, FW_NORMAL,
					  FALSE, FALSE, 0, ANSI_CHARSET,
					  OUT_TT_PRECIS, 
					  CLIP_DEFAULT_PRECIS,
					  ANTIALIASED_QUALITY, 
					  DEFAULT_PITCH|FF_SWISS, "Arial");
}

CItemPlacementDisp::~CItemPlacementDisp()
{
	m_fontText.DeleteObject();
}


BEGIN_MESSAGE_MAP(CItemPlacementDisp, CStatic)
	//{{AFX_MSG_MAP(CItemPlacementDisp)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemPlacementDisp message handlers

void CItemPlacementDisp::OnPaint(){
	CPaintDC basedc(this);
	CRect rc; GetClientRect(rc);
	CMemDC memdc(&basedc,&rc);
	CDC* pDC = &memdc;
	
	// erase bg
	pDC->FillSolidRect(rc,RGB(220,225,220));

	if(!m_pHero || !m_ulInvType){
		int oldBkMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF oldTxtColor = pDC->SetTextColor(RGB(0,100,0));
		CFont* pOldFont = pDC->SelectObject(GetParent()->GetFont());
		pDC->DrawText("No hero loaded at the moment...",rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		pDC->SelectObject(pOldFont);
		pDC->SetTextColor(oldTxtColor);
		pDC->SetBkMode(oldBkMode);
	}else{
		// set the grid pen
		CPen gridPen;
		gridPen.CreatePen(PS_SOLID,1,RGB(0,100,0));
		CPen* oldPen = pDC->SelectObject(&gridPen);

		// set up item boxes
		CSize gridSize = m_pHero->GetItemManager()->GetPlacementInvSize(m_ulInvType);
		int spanWidth = rc.Width()/gridSize.cx;
		int spanHeight = rc.Height()/gridSize.cy;

		// draw grid lines
		for(int x = 0; x < gridSize.cx+1; x++){
			pDC->MoveTo(x*spanWidth,0);
			pDC->LineTo(x*spanWidth,rc.Height());
		}
		for(int y = 0; y < gridSize.cy+1; y++){
			pDC->MoveTo(0,y*spanHeight);
			pDC->LineTo(rc.Width(),y*spanHeight);
		}
		
		// done with grid pen
		pDC->SelectObject(oldPen);
		gridPen.DeleteObject();

		// paint out of bounds
		PaintOOB(pDC);

		// draw items
		CFont* pOldTxtFont = pDC->SelectObject(&m_fontText);
		int oldBkMode = pDC->SetBkMode(TRANSPARENT);
		int oldTxtColor = pDC->SetTextColor(RGB(255,255,255));
		for(int i = 0; i < m_pHero->GetItemManager()->GetTotalMainItems(); i++){
			CItem* pItem = (CItem*)m_pHero->GetItemManager()->GetMainItemByIndex(i);
			if(*pItem->GetItemInvStorage()==m_ulInvType){
				CRect itemRect = CreateItemRect(pItem);
				BOOL bThisSel = m_pSelItem && (*m_pSelItem->GetItemId()==*pItem->GetItemId());
				if(bThisSel){
					pDC->FillSolidRect(itemRect,RGB(180,185,180));
					pDC->Draw3dRect(itemRect,RGB(100,105,100),RGB(200,205,200));
					pDC->SetTextColor(RGB(100,105,100));
				}else{
					pDC->FillSolidRect(itemRect,RGB(150,155,150));
					pDC->Draw3dRect(itemRect,RGB(200,205,200),RGB(100,105,100));
					pDC->SetTextColor(RGB(250,255,250));
				}
				if(pItem->GetItemUpdatedFullCheck()){
					CRect smRc = itemRect;
					smRc.left += (bThisSel?3:2);
					smRc.top += (bThisSel?3:2);
					smRc.right = smRc.left + 2;
					smRc.bottom = smRc.top + 2;
					pDC->FillSolidRect(smRc,RGB(255,0,0));
				}
				if(pItem->GetSlottedArray()->GetSize()){
					char txt[127];
					wsprintf(txt,"%u",pItem->GetSlottedArray()->GetSize());
					pDC->DrawText(txt,strlen(txt),itemRect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				}
			}
		}
		pDC->SetTextColor(oldTxtColor);
		pDC->SetBkMode(oldBkMode);
		pDC->SelectObject(pOldTxtFont);

		// paint disabled mask...
		if(m_bDisabled){
			CBitmap maskBmp;
			CreateMaskBitmap(pDC,&maskBmp,rc,RGB(0,255,0),RGB(100,100,100));
			TransparentBmp(pDC,0,0,&maskBmp,RGB(0,255,0));
			maskBmp.DeleteObject();
		}
	}
}

CRect CItemPlacementDisp::CreateItemRect(CItem* pItem){
	CRect rc; GetClientRect(rc);
	CSize gridSize = m_pHero->GetItemManager()->GetPlacementInvSize(m_ulInvType);
	int spanWidth = rc.Width()/gridSize.cx;
	int spanHeight = rc.Height()/gridSize.cy;

	CRect fin;
	fin.left = pItem->GetItemInvPlacement()->x*spanWidth;
	fin.top = pItem->GetItemInvPlacement()->y*spanHeight;
	fin.right = fin.left + pItem->GetItemInvSize()->cx*spanWidth;
	fin.bottom = fin.top + pItem->GetItemInvSize()->cy*spanHeight;

	return fin;
}

//
// paints out of bounds
//
void CItemPlacementDisp::PaintOOB(CDC* pDC){
	CRect rc; GetClientRect(rc);
	CSize gridSize = m_pHero->GetItemManager()->GetPlacementInvSize(m_ulInvType);
	int spanWidth = rc.Width()/gridSize.cx;
	int spanHeight = rc.Height()/gridSize.cy;

	CRect oobright;
	oobright.left = 1+gridSize.cx*spanWidth;
	oobright.top = 0;
	oobright.right = oobright.left + (rc.Width()-oobright.left);
	oobright.bottom = rc.Height();

	CRect oobbottom;
	oobbottom.left = 0;
	oobbottom.right = rc.Width();
	oobbottom.top = 1+gridSize.cy*spanHeight;
	oobbottom.bottom = oobbottom.top + (rc.Height()-oobbottom.top);

	CBrush hatchBrush;
	hatchBrush.CreateHatchBrush(HS_DIAGCROSS,RGB(200,200,200));

	COLORREF oldCol = pDC->SetBkColor(RGB(255,255,255));
	pDC->FillRect(oobbottom,&hatchBrush);
	pDC->FillRect(oobright,&hatchBrush);
	pDC->SetBkColor(oldCol);

	hatchBrush.DeleteObject();
}

void CItemPlacementDisp::OnLButtonDown(UINT nFlags, CPoint point){
	if(!m_bDisabled && m_pHero){
		for(int i = 0; i < m_pHero->GetItemManager()->GetTotalMainItems(); i++){
			CItem* pItem = (CItem*)m_pHero->GetItemManager()->GetMainItemByIndex(i);
			if(*pItem->GetItemInvStorage()==m_ulInvType){
				CRect itemRect = CreateItemRect(pItem);
				if(itemRect.PtInRect(point)){
					CSIMDlg* pDlg = (CSIMDlg*)AfxGetMainWnd();
					pDlg->SelectItemById(*pItem->GetItemId());
				}
			}
		}
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CItemPlacementDisp::OnLButtonDblClk(UINT nFlags, CPoint point){
	if(!m_bDisabled && m_pHero){
		for(int i = 0; i < m_pHero->GetItemManager()->GetTotalMainItems(); i++){
			CItem* pItem = (CItem*)m_pHero->GetItemManager()->GetMainItemByIndex(i);
			if(*pItem->GetItemInvStorage()==m_ulInvType){
				CRect itemRect = CreateItemRect(pItem);
				if(itemRect.PtInRect(point)){
					CSIMDlg* pDlg = (CSIMDlg*)AfxGetMainWnd();
					pDlg->SelectItemById(*pItem->GetItemId());
					pDlg->ClickViewButton();
				}
			}
		}
	}
	CStatic::OnLButtonDblClk(nFlags, point);
}

//
// make sure you DeleteObject() on the pBitmap after this returns
//
BOOL CItemPlacementDisp::CreateMaskBitmap(CDC* pDC, CBitmap* pBitmap, CRect rc, COLORREF maskColor, COLORREF lineColor){

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	pBitmap->CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
	CBitmap* pOldBmp = memDC.SelectObject(pBitmap);

	CBrush hatchBrush;
	hatchBrush.CreateHatchBrush(HS_DIAGCROSS,lineColor);
	COLORREF oldCol = memDC.SetBkColor(maskColor);
	memDC.FillRect(rc,&hatchBrush);
	memDC.SetBkColor(oldCol);
	hatchBrush.DeleteObject();

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();

	return TRUE;
}

//
// transparent painting of a bitmap
//
void CItemPlacementDisp::TransparentBmp(CDC* pDC, int x, int y, CBitmap* pBmp, COLORREF colTrans){
	CDC hdcTemp;
	hdcTemp.CreateCompatibleDC(pDC);
	hdcTemp.SelectObject(pBmp);
	BITMAP bm;
	CPoint ptSize;
	pBmp->GetObject(sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	hdcTemp.DPtoLP(&ptSize,1);
	CDC hdcBack, hdcObject, hdcMem, hdcSave;
	hdcBack.CreateCompatibleDC(pDC);
	hdcObject.CreateCompatibleDC(pDC);
	hdcMem.CreateCompatibleDC(pDC);
	hdcSave.CreateCompatibleDC(pDC);
	CBitmap bmAndBack, bmAndObject, bmAndMem, bmSave;
	bmAndBack.CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject.CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem.CreateCompatibleBitmap(pDC, ptSize.x, ptSize.y);
	bmSave.CreateCompatibleBitmap(pDC, ptSize.x, ptSize.y);
	CBitmap* bmBackOld   = hdcBack.SelectObject(&bmAndBack);
	CBitmap* bmObjectOld = hdcObject.SelectObject(&bmAndObject);
	CBitmap* bmMemOld    = hdcMem.SelectObject(&bmAndMem);
	CBitmap* bmSaveOld   = hdcSave.SelectObject(&bmSave);
	hdcTemp.SetMapMode(pDC->GetMapMode());
	hdcSave.BitBlt(0, 0, ptSize.x, ptSize.y, &hdcTemp, 0, 0, SRCCOPY);
	COLORREF cColor = hdcTemp.SetBkColor(colTrans);
	hdcObject.BitBlt(0, 0, ptSize.x, ptSize.y, &hdcTemp, 0, 0, SRCCOPY);
	hdcTemp.SetBkColor(cColor);
	hdcBack.BitBlt(0, 0, ptSize.x, ptSize.y, &hdcObject, 0, 0, NOTSRCCOPY);
	hdcMem.BitBlt(0, 0, ptSize.x, ptSize.y, pDC, x, y, SRCCOPY);
	hdcMem.BitBlt(0, 0, ptSize.x, ptSize.y, &hdcObject, 0, 0, SRCAND);
	hdcTemp.BitBlt(0, 0, ptSize.x, ptSize.y, &hdcBack, 0, 0, SRCAND);
	hdcMem.BitBlt(0, 0, ptSize.x, ptSize.y, &hdcTemp, 0, 0, SRCPAINT);
	pDC->BitBlt(x, y, ptSize.x, ptSize.y, &hdcMem, 0, 0, SRCCOPY);
	hdcTemp.BitBlt(0, 0, ptSize.x, ptSize.y, &hdcSave, 0, 0, SRCCOPY);
	CBitmap* tmpBmp = hdcBack.SelectObject(bmBackOld);
	tmpBmp->DeleteObject();
	tmpBmp = hdcObject.SelectObject(bmObjectOld);
	tmpBmp->DeleteObject();
	tmpBmp = hdcMem.SelectObject(bmMemOld);
	tmpBmp->DeleteObject();
	tmpBmp = hdcSave.SelectObject(bmSaveOld);
	tmpBmp->DeleteObject();
	hdcMem.DeleteDC();
	hdcBack.DeleteDC();
	hdcObject.DeleteDC();
	hdcSave.DeleteDC();
	hdcTemp.DeleteDC();
}

