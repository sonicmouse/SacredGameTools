// HoverButton.cpp : implementation file
//

#include "stdafx.h"
#include "HoverButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoverButton

CHoverButton::CHoverButton()
{
	m_bIsOverButton = FALSE;
	m_nBmpResId = -1;
	m_bDrawFoc = TRUE;
	m_colBkCol = GetSysColor(COLOR_BTNFACE);
}

CHoverButton::~CHoverButton()
{
	
}


BEGIN_MESSAGE_MAP(CHoverButton, CButton)
	//{{AFX_MSG_MAP(CHoverButton)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoverButton message handlers

//
//
//
COLORREF CHoverButton::LightenColor(COLORREF col, double factor){
	if(factor>0.0&&factor<=1.0){
		BYTE red,green,blue,lightred,lightgreen,lightblue;
		red = GetRValue(col);
		green = GetGValue(col);
		blue = GetBValue(col);
		lightred = (BYTE)((factor*(255-red)) + red);
		lightgreen = (BYTE)((factor*(255-green)) + green);
		lightblue = (BYTE)((factor*(255-blue)) + blue);
		col = RGB(lightred,lightgreen,lightblue);
	}
	return(col);
}

//
//
//
void CHoverButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct){
	CRect rect = lpDrawItemStruct->rcItem;
	BOOL isDown = (lpDrawItemStruct->itemState & ODS_SELECTED);
	BOOL isDef = (lpDrawItemStruct->itemState & ODS_DEFAULT);
	BOOL isDis = (lpDrawItemStruct->itemState & ODS_DISABLED);
	BOOL isFoc = (lpDrawItemStruct->itemState & ODS_FOCUS);
	BOOL isOver = m_bIsOverButton;

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	COLORREF col_border_normal = LightenColor(GetSysColor(COLOR_BTNFACE),0.35);
	COLORREF col_border_mouseover = GetSysColor(COLOR_HIGHLIGHT);
	COLORREF col_bg_mouseover = LightenColor(col_border_mouseover, 0.85);
	COLORREF col_bg_mousedown = LightenColor(col_border_mouseover, 0.40);
	COLORREF col_bg_normal = GetSysColor(COLOR_BTNFACE);
	m_colBkCol = col_bg_normal; // fix for system update colors
	COLORREF col_text_mousedown = GetSysColor(COLOR_HIGHLIGHTTEXT);
	COLORREF col_text_normal = GetSysColor(COLOR_BTNTEXT);
	COLORREF col_text_dis = GetSysColor(COLOR_GRAYTEXT);

	COLORREF col_border = col_border_normal;
	COLORREF col_inside = col_bg_normal;
	COLORREF col_text = col_text_normal;

	if(isOver){
		col_border = col_border_mouseover;
		col_inside = col_bg_mouseover;
		col_text = col_text_normal;
	}
	if(isDown){
		col_border = col_border_mouseover;
		col_inside = col_bg_mousedown;
		col_text = col_text_mousedown;
	}
	if(isDis){
		col_border = col_border_normal;
		col_inside = col_bg_normal;
		col_text = col_text_dis;
	}
	
	// erase bg to spec color
	dc.FillSolidRect(rect, m_colBkCol);

	// draw border
	CPen border_pen;
	CBrush bg_brush;
	bg_brush.CreateSolidBrush(col_inside);
	border_pen.CreatePen(PS_SOLID,1,col_border);
	CPen* oldPen = dc.SelectObject(&border_pen);
	CBrush* oldBr = dc.SelectObject(&bg_brush);
	dc.RoundRect(rect,CPoint(10,10));
	dc.SelectObject(oldBr);
	dc.SelectObject(oldPen);
	bg_brush.DeleteObject();
	border_pen.DeleteObject();

	// draw focus rect
	if(isFoc && m_bDrawFoc){
		CRect focus_rect = rect;
		focus_rect.DeflateRect(3,3,3,3);
		dc.DrawFocusRect(focus_rect);
	}
	// draw text
	COLORREF oldTextCol = dc.SetTextColor(col_text);
	COLORREF oldBkCol = dc.SetBkColor(GetSysColor(COLOR_WINDOW));
	int oldBkMode = dc.SetBkMode(TRANSPARENT);
	CString wnd_text;
	GetWindowText(wnd_text);
	if(m_nBmpResId != -1){
		CBitmap bmp;
		if(bmp.LoadBitmap(m_nBmpResId)){
			BITMAP sbmp;
			bmp.GetObject(sizeof(sbmp),&sbmp);
			int bmpLeft, bmpTop;
			if(wnd_text.GetLength()){
				CSize te = dc.GetTextExtent(wnd_text);
				if(wnd_text.Find('&')!=-1){
					CSize amp = dc.GetTextExtent("&");
					te.cx -= amp.cx;
				}
				CRect drawArea;
				drawArea.left = 0;
				drawArea.top = 0;
				drawArea.right = __max(te.cx, sbmp.bmWidth);
				drawArea.bottom = te.cy + sbmp.bmHeight;
				drawArea.left = (rect.Width()/2)-(drawArea.Width()/2);
				drawArea.top = (rect.Height()/2)-(drawArea.Height()/2);
				drawArea.right += drawArea.left;
				drawArea.bottom += drawArea.top;
				dc.DrawText(wnd_text, drawArea, DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
				bmpLeft = drawArea.left + ((drawArea.Width()/2) - (sbmp.bmWidth/2)) - (isOver&&!isDown?HOV_HEI:0);
				bmpTop = drawArea.top - (isOver&&!isDown?HOV_HEI:0);
			}else{
				bmpLeft = (rect.Width()/2) - (sbmp.bmWidth/2) - (isOver&&!isDown?HOV_HEI:0);
				bmpTop = (rect.Height()/2) - (sbmp.bmHeight/2) - (isOver&&!isDown?HOV_HEI:0);
			}
			if(isDis){
				GrayBlt(&dc,bmpLeft,bmpTop,m_nBmpResId,m_colTransparent,col_inside);
			}else{
				if(isOver && !isDown){
					ShadowBlt(&dc,bmpLeft,bmpTop,m_nBmpResId,m_colTransparent,col_inside);
				}else{
					TransparentBmp(&dc,bmpLeft,bmpTop,&bmp,m_colTransparent);
				}
			}
			bmp.DeleteObject();
		}
	}else{
		// fix ampersand
		CString tmpTxt = wnd_text;
		while(tmpTxt.Find('&')!= -1){
			tmpTxt.Replace("&","");
		}

		CSize te = dc.GetTextExtent(tmpTxt);

		CRect drawRect;
		drawRect.left = (rect.Width()/2)-(te.cx/2);
		drawRect.top = (rect.Height()/2)-(te.cy/2);
		drawRect.right = drawRect.left + te.cx;
		drawRect.bottom = drawRect.top + te.cy;

		if(wnd_text.GetLength()){
			dc.DrawText(wnd_text,drawRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		}
	}
	dc.SetTextColor(oldTextCol);
	dc.SetBkColor(oldBkCol);
	dc.SetBkMode(oldBkMode);
	dc.Detach();
}

//
//
//
void CHoverButton::OnMouseMove(UINT nFlags, CPoint point){
	if(!m_bIsOverButton){
		CPoint pt;
		GetCursorPos(&pt);
		CWnd* overwnd = WindowFromPoint(pt);
		m_bIsOverButton = overwnd && (overwnd->m_hWnd == m_hWnd);
		if(m_bIsOverButton){
			SetTimer(TIMER_ID_OVER_BUTTON,1,NULL);
			Invalidate(FALSE);
		}
	}
	CButton::OnMouseMove(nFlags, point);
}

//
//
//
void CHoverButton::OnTimer(UINT nIDEvent){
	if(nIDEvent == TIMER_ID_OVER_BUTTON){
		CPoint pt;
		GetCursorPos(&pt);
		CWnd* overwnd = WindowFromPoint(pt);
		m_bIsOverButton = (overwnd && (overwnd->m_hWnd == m_hWnd));
		if(!m_bIsOverButton){
			KillTimer(TIMER_ID_OVER_BUTTON);
			Invalidate(FALSE);
		}
		return;
	}
	CButton::OnTimer(nIDEvent);
}

//
//
//
void CHoverButton::OnDestroy(){
	KillTimer(TIMER_ID_OVER_BUTTON);
	CButton::OnDestroy();
}

//
//
//
void CHoverButton::ShadowBlt(CDC* pDC, int x, int y, UINT bmpID, COLORREF colTrans, COLORREF colBg){

	GrayBlt(pDC,x+HOV_HEI,y+HOV_HEI,bmpID,colTrans,colBg);

	CBitmap overBmp;
	overBmp.LoadBitmap(bmpID);
	TransparentBmp(pDC,x,y,&overBmp,colTrans);
}

//
//
//
void CHoverButton::GrayBlt(CDC* pDC, int x, int y, UINT bmpID, COLORREF colTrans, COLORREF colBg){
	CBitmap disbmp;
	disbmp.LoadBitmap(bmpID);

	BITMAP sBmp;
	disbmp.GetBitmap(&sBmp);

	GetDisabledBitmap(disbmp, colBg, colTrans);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* old = memDC.SelectObject(&disbmp);
	pDC->BitBlt(x,y,sBmp.bmWidth,sBmp.bmHeight,&memDC,0,0,SRCCOPY);
	memDC.SelectObject(old);
	memDC.DeleteDC();
	disbmp.DeleteObject();
}

//
//
//
void CHoverButton::GetDisabledBitmap(CBitmap &bmp,COLORREF background, COLORREF transpColor){

	CDC ddc;
	COLORREF bgcol,col,discol;
	BITMAP BitMap;

	bmp.GetBitmap(&BitMap);
	ddc.CreateCompatibleDC(NULL);
	CBitmap * pddcOldBmp = ddc.SelectObject(&bmp);

	// use this to get the background color, takes into account color shifting
	CDC ddc2;
	CBrush brush;
	CBitmap bmp2;
	ddc2.CreateCompatibleDC(NULL);
	bmp2.CreateCompatibleBitmap(&ddc,BitMap.bmWidth,BitMap.bmHeight);
	col=GetSysColor(COLOR_3DFACE);
	brush.CreateSolidBrush(col);
	CBitmap * pddcOldBmp2 = ddc2.SelectObject(&bmp2);
	CRect rect(0,0,BitMap.bmWidth,BitMap.bmHeight);
	ddc2.FillRect(rect,&brush);
	bgcol=transpColor;
	brush.DeleteObject();
	ddc2.SelectObject(pddcOldBmp2);
	discol=GetSysColor(COLOR_BTNSHADOW);

	for(int i=0;i<BitMap.bmWidth;++i){
		for(int j=0;j<BitMap.bmHeight;++j){
			col=ddc.GetPixel(i,j);
			if(col!=bgcol){
				BYTE r = GetRValue(col);
				BYTE g = GetGValue(col);
				BYTE b = GetBValue(col);
				int avgcol = (r+g+b)/3;
				double factor = avgcol/255.0;
				ddc.SetPixel(i,j,LightenColor(discol,factor));
			}
			else{
				if(background)ddc.SetPixel(i,j,background);
			}
		}
	}
	ddc.SelectObject(pddcOldBmp);
}

//
//
//
void CHoverButton::TransparentBmp(CDC* pDC, int x, int y, CBitmap* pBmp, COLORREF colTrans){
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

