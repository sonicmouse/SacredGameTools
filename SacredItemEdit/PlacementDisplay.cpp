// PlacementDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "PlacementDisplay.h"
#include "memdc.h"
#include "SIEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlacementDisplay

CPlacementDisplay::CPlacementDisplay()
{
	m_pHero = NULL;
	m_ulSelected = 0xFFFFFFFF;
	m_nInvType = SECTION_PLACEMENT_CH;
	m_bDisabled = FALSE;
}

CPlacementDisplay::~CPlacementDisplay()
{
}


BEGIN_MESSAGE_MAP(CPlacementDisplay, CStatic)
	//{{AFX_MSG_MAP(CPlacementDisplay)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlacementDisplay message handlers

void CPlacementDisplay::OnPaint(){
	CPaintDC odc(this);
	CRect rc; GetClientRect(rc);
	CMemDC dc(&odc,&rc);
	
	// erase bg
	dc.FillSolidRect(rc,RGB(255,255,255));
	
	// set the grid pen
	CPen gridPen;
	gridPen.CreatePen(PS_SOLID,LINE_WIDTH,RGB(0,0,0));
	CPen* oldPen = dc.SelectObject(&gridPen);

	if(m_pHero){
		// draw the grid
		CSize gridSize = m_pHero->GetRowSpan(m_nInvType);
		
		int spanWidth = rc.Width()/gridSize.cx;
		int spanHeight = rc.Height()/gridSize.cy;

		for(int x = 0; x < gridSize.cx+1; x++){
			dc.MoveTo(x*spanWidth,0);
			dc.LineTo(x*spanWidth,rc.Height());
		}
		for(int y = 0; y < gridSize.cy+1; y++){
			dc.MoveTo(0,y*spanHeight);
			dc.LineTo(rc.Width(),y*spanHeight);
		}
		// draw the items..
		for(x = 0; x < gridSize.cx; x++){
			for(y = 0; y < gridSize.cy; y++){
				PST_PLACEMENT_ITEM p = m_pHero->GetItemPlacement(m_nInvType,CPoint(x,y));
				if(p->slotused && p->itemid){
					PaintArray(&dc,p,CPoint(x,y),CSize(spanWidth,spanHeight),RGB(180,180,180),m_ulSelected==p->index);
				}
			}
		}
		// out of bounds
		PaintOOB(&dc);
		// paint disabled mask if its disabled
		if(m_bDisabled){
			CBitmap maskBmp;
			CreateMaskBitmap(&dc,&maskBmp,rc,RGB(0,255,0),RGB(150,150,150));

			TransparentBmp(&dc,0,0,&maskBmp,RGB(0,255,0));

			maskBmp.DeleteObject();
		}
	}else{
		CFont* pOldFont = dc.SelectObject(GetParent()->GetFont());
		int oldBkMode = dc.SetBkMode(TRANSPARENT);
		COLORREF oldTxtColor = dc.SetTextColor(RGB(255,0,0));
		dc.DrawText("No hero file currently loaded...",rc,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		dc.SetTextColor(oldTxtColor);
		dc.SetBkMode(oldBkMode);
		dc.SelectObject(pOldFont);
	}

	dc.SelectObject(oldPen);
}

void CPlacementDisplay::PaintOOB(CDC* pDC){
	CRect rc; GetClientRect(rc);
	CSize gridSize = m_pHero->GetRowSpan(m_nInvType);
	int spanWidth = rc.Width()/gridSize.cx;
	int spanHeight = rc.Height()/gridSize.cy;

	CRect oobright;
	oobright.left = LINE_WIDTH+gridSize.cx*spanWidth;
	oobright.top = 0;
	oobright.right = oobright.left + (rc.Width()-oobright.left);
	oobright.bottom = rc.Height();

	CRect oobbottom;
	oobbottom.left = 0;
	oobbottom.right = rc.Width();
	oobbottom.top = LINE_WIDTH+gridSize.cy*spanHeight;
	oobbottom.bottom = oobbottom.top + (rc.Height()-oobbottom.top);

	CBrush hatchBrush;
	hatchBrush.CreateHatchBrush(HS_DIAGCROSS,RGB(200,200,200));

	COLORREF oldCol = pDC->SetBkColor(RGB(255,255,255));
	pDC->FillRect(oobbottom,&hatchBrush);
	pDC->FillRect(oobright,&hatchBrush);
	pDC->SetBkColor(oldCol);

	hatchBrush.DeleteObject();
}

void CPlacementDisplay::PaintArray(CDC* pDC, PST_PLACEMENT_ITEM pItem, CPoint pt, CSize sp, COLORREF col, BOOL bSel){
	CRect sq;
	sq.left = LINE_WIDTH+pt.x*sp.cx;
	sq.top =  LINE_WIDTH+pt.y*sp.cy;
	sq.right = sq.left + (sp.cx*pItem->width);
	sq.bottom = sq.top + (sp.cy*pItem->height);
	sq.DeflateRect(0,0,LINE_WIDTH,LINE_WIDTH);
	pDC->FillSolidRect(sq,col);
	pDC->Draw3dRect(sq,RGB(220,220,220),RGB(127,127,127));
	if(bSel){
		CPen pen; pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
		CPen* oldPen = pDC->SelectObject(&pen);

		pDC->MoveTo(sq.left,sq.top);
		pDC->LineTo(sq.right,sq.bottom);
		pDC->MoveTo(sq.right,sq.top);
		pDC->LineTo(sq.left,sq.bottom);

		pDC->SelectObject(oldPen);
		pen.DeleteObject();
	}
}

void CPlacementDisplay::OnLButtonDown(UINT nFlags, CPoint point){
	if(!m_bDisabled && m_pHero){
		PST_PLACEMENT_ITEM p = GetItemFromPoint(point);
		m_ulSelected = (p?p->index:0xFFFFFFFF);
		if(p){
			CSIEDlg* pDlg = (CSIEDlg*)::AfxGetMainWnd();
			pDlg->OnPlacementSelectionChange(m_nInvType,m_ulSelected);
		}
		Repaint();
	}
	CStatic::OnLButtonDown(nFlags, point);
}

PST_PLACEMENT_ITEM CPlacementDisplay::GetItemFromPoint(CPoint pt){
	CRect rc; GetClientRect(rc);
	CSize gridSize = m_pHero->GetRowSpan(m_nInvType);
	int spanWidth = rc.Width()/gridSize.cx;
	int spanHeight = rc.Height()/gridSize.cy;
	for(int x = 0; x < gridSize.cx; x++){
		for(int y = 0; y < gridSize.cy; y++){
			PST_PLACEMENT_ITEM p = m_pHero->GetItemPlacement(m_nInvType,CPoint(x,y));
			if(p->slotused && p->itemid){
				CRect sq;
				sq.left = LINE_WIDTH+x*spanWidth;
				sq.top = LINE_WIDTH+y*spanHeight;
				sq.right = sq.left + (spanWidth*p->width);
				sq.bottom = sq.top + (spanHeight*p->height);
				sq.DeflateRect(0,0,LINE_WIDTH,LINE_WIDTH);
				if(sq.PtInRect(pt)){ return p; }
			}
		}
	}
	return NULL;
}

//
// make sure you DeleteObject() on the pBitmap after this returns
//
BOOL CPlacementDisplay::CreateMaskBitmap(CDC* pDC, CBitmap* pBitmap, CRect rc, COLORREF maskColor, COLORREF lineColor){

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

void CPlacementDisplay::TransparentBmp(CDC* pDC, int x, int y, CBitmap* pBmp, COLORREF colTrans){
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
