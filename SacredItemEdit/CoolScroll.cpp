// CoolScroll.cpp : implementation file
//

#include "stdafx.h"
#include "CoolScroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolScroll

CCoolScroll::CCoolScroll(){
	m_nBmpResource = 0;
	m_szText = "empty";
	m_colText = RGB(0,0,0);
	m_bIsCreated = FALSE;
	m_bmpBg = NULL;
	m_colBg = RGB(255,255,255);
	n_currPos = 0;
	m_bFrameIt = TRUE;
	m_colShadow = RGB(180,180,180);
	m_shadowDrop = 1;
}

CCoolScroll::~CCoolScroll()
{
	if(m_bmpBg){::DeleteObject(m_bmpBg);}
	if(m_Font.m_hObject){m_Font.DeleteObject();}
}

BEGIN_MESSAGE_MAP(CCoolScroll, CWnd)
	//{{AFX_MSG_MAP(CCoolScroll)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCoolScroll message handlers

BOOL CCoolScroll::Create(CWnd* pParent, int iIdControl){
	CWnd* pOldWnd=pParent->GetDlgItem(iIdControl);
	if(m_szWndClass.IsEmpty()){
		m_szWndClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW/*|CS_GLOBALCLASS|CS_PARENTDC*/|CS_DBLCLKS,
										   LoadCursor(NULL,IDC_ARROW),
										   (HBRUSH)GetStockObject(WHITE_BRUSH) );
	}
	CRect rect;
	pOldWnd->GetWindowRect(&rect);
	pParent->ScreenToClient(&rect);
	pOldWnd->DestroyWindow();
	m_bIsCreated = CreateEx(WS_EX_NOPARENTNOTIFY,
							m_szWndClass,
							NULL,
							WS_CHILD|WS_VISIBLE,
							rect.left,
							rect.top,
							rect.right-rect.left,
							rect.bottom-rect.top,
							pParent->m_hWnd,
							(HMENU)iIdControl);

	m_Font.CreateFont(16, 0, 0, 0, FW_BOLD,
					   FALSE, FALSE, 0, ANSI_CHARSET,
					   OUT_DEFAULT_PRECIS, 
					   CLIP_DEFAULT_PRECIS,
					   ANTIALIASED_QUALITY, 
					   DEFAULT_PITCH|FF_SWISS, "Arial");

	if(m_bIsCreated){ UpdateWindow(); }
	return m_bIsCreated;
}

void CCoolScroll::OnPaint(){
	CPaintDC dc(this);
	HDC mhdc = dc.m_hDC;
	CRect rc; GetClientRect(rc);
	// back buffer
	HDC hdc = ::CreateCompatibleDC(mhdc);
	HBITMAP bbBmp = ::CreateCompatibleBitmap(mhdc,rc.Width(),rc.Height());
	HBITMAP oldBbBmp = (HBITMAP)::SelectObject(hdc,bbBmp);
	// set our font
	int oldBgMode = ::SetBkMode(hdc, TRANSPARENT);
	COLORREF oldBkColor = ::SetBkColor(hdc,0);
	COLORREF oldTxColor = ::SetTextColor(hdc,0);
	HFONT oldFont = (HFONT)::SelectObject(hdc, m_Font.m_hObject);

	// fill our background
	HBRUSH bgBrush = ::CreateSolidBrush(m_colBg);
	::FillRect(hdc,rc,bgBrush);
	::DeleteObject(bgBrush);
	// draw the bg bmp
	if(m_bmpBg){
		HDC bmpDC = ::CreateCompatibleDC(hdc);
		HBITMAP oldBmp = (HBITMAP)::SelectObject(bmpDC, m_bmpBg);
		CRect targetRect;
		int nLeft = (rc.right/2)-(m_sizeBgBmp.cx/2);
		int nTop = (rc.bottom/2)-(m_sizeBgBmp.cy/2);
		::BitBlt(hdc,nLeft,nTop,m_sizeBgBmp.cx,m_sizeBgBmp.cy,bmpDC,0,0,SRCCOPY);
		::SelectObject(bmpDC,oldBmp);
		::DeleteDC(bmpDC);
	}
	// draw the text
	CRect rectText;
	CalculateTextRect(hdc, rectText);
	rectText.top+=n_currPos;
	rectText.bottom+=n_currPos;
	if(m_bFrameIt){
		rectText.top += 2;
		rectText.bottom += 2;
	}
	if(rectText.bottom < -10){
		n_currPos = rc.bottom;
	}

	::SetTextColor(hdc, m_colShadow);
	CRect bgRect;
	bgRect.SetRect(rectText.left + m_shadowDrop, rectText.top + m_shadowDrop, rectText.right + m_shadowDrop, rectText.bottom + m_shadowDrop);
	::DrawText(hdc,m_szText,m_szText.GetLength(),bgRect,DT_CENTER|DT_NOPREFIX|DT_NOCLIP|DT_EXPANDTABS);

	::SetTextColor(hdc, m_colText);
	::DrawText(hdc,m_szText,m_szText.GetLength(),rectText,DT_CENTER|DT_NOPREFIX|DT_NOCLIP|DT_EXPANDTABS);

	// draw the frame
	if(m_bFrameIt){
		::DrawEdge(hdc,rc,EDGE_SUNKEN,BF_RECT);
	}

	// clean up
	::SelectObject(hdc, oldFont);
	::SetTextColor(hdc,oldTxColor);
	::SetBkColor(hdc,oldBkColor);
	::SetBkMode(hdc,oldBgMode);
	// backbuffer
	::BitBlt(mhdc,0,0,rc.right,rc.bottom, hdc,0,0,SRCCOPY);
	::SelectObject(mhdc,oldBbBmp);
	::DeleteObject(bbBmp);
	::DeleteDC(hdc);
}

void CCoolScroll::SetBackgroundBitmap(UINT nResource){
	if(m_bmpBg){::DeleteObject(m_bmpBg);}
	m_bmpBg = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(nResource));
	if(!m_bmpBg){return;}
	BITMAP bmp;
	::GetObject(m_bmpBg,sizeof(bmp),&bmp);
	m_sizeBgBmp.cx = bmp.bmWidth;
	m_sizeBgBmp.cy = bmp.bmHeight;
}

void CCoolScroll::DoScroll(BOOL bDo){
	if(bDo){
		SetTimer(TIMER_ID_SCROLL_TEXT,1,NULL);
		CRect rc;
		GetClientRect(rc);
		n_currPos = rc.bottom;
	}else{
		KillTimer(TIMER_ID_SCROLL_TEXT);
	}
}

void CCoolScroll::CalculateTextRect(HDC dc, CRect& rc){
	TEXTMETRIC tm;
	::GetTextMetrics(dc, &tm);
	int nCount = 1;
	for(int i = 0; i< m_szText.GetLength();i++){
		if(m_szText.GetAt(i)=='\n'){
			++nCount;
		}
	}
	CRect cliRect;
	GetClientRect(cliRect);
	rc.SetRect(0,0,cliRect.Width(),tm.tmHeight*nCount);
}

void CCoolScroll::OnTimer(UINT nIDEvent){
	if(TIMER_ID_SCROLL_TEXT == nIDEvent){
		static DWORD lastUpdate = 0;
		if(GetTickCount() >= lastUpdate + 50){
			lastUpdate = GetTickCount();
			--n_currPos;
			Invalidate(FALSE);
		}
		return;
	}
	CWnd::OnTimer(nIDEvent);
}

void CCoolScroll::OnClose(){
	KillTimer(TIMER_ID_SCROLL_TEXT);

	CWnd::OnClose();
}

