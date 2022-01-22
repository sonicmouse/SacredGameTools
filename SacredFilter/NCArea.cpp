// NCArea.cpp: implementation of the CNCArea class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCArea.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNCArea::CNCArea()
{
	m_nCyTitleBar = 24;

	m_nCxFrame = 2;
	m_nCyFrame = 2;

	// 16x14
	m_nCxSysButton = 16;
	m_nCySysButton = 14;

	m_nCxSysButtonSpacing = 2;

	m_hIcon = NULL;

	m_bMouseCaptured = FALSE;
	m_nButtonDown = SYS_BUTTON_NONE;

	m_bIsOnTop = FALSE;

	m_Font.CreateFont(14, 0, 0, 0, FW_BOLD,
				  FALSE, FALSE, 0, ANSI_CHARSET,
				  OUT_TT_PRECIS, 
				  CLIP_DEFAULT_PRECIS,
				  ANTIALIASED_QUALITY, 
				  DEFAULT_PITCH|FF_SWISS, "Arial");
}

CNCArea::~CNCArea()
{
	m_Font.DeleteObject();
}

//
// return TRUE if we handled this message, if we did, the return code is in pResult
// return FALSE if we dont want this message, let the DefWindowProc handle it
//
BOOL CNCArea::HandleNCArea(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult){

	switch(message){
		// Non-Client creation
		case WM_NCCREATE:
			{
				// add our stuff to the sysmenu
				HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
				if(hSysMenu){
					AppendMenu(hSysMenu,MF_SEPARATOR,0,NULL);
					AppendMenu(hSysMenu, MF_STRING, SC_STAYONTOP, _T("Stay On Top"));
					CheckMenuItem(hSysMenu,SC_STAYONTOP,(m_bIsOnTop?MF_CHECKED:MF_UNCHECKED));
				}

				// get the current size of the non-client area
				CRect rcWin = GetFullWindowRect(hWnd);
				// now we have to accomodate for our new borders and titlebar
				rcWin.right += m_nCxFrame*2;
				rcWin.bottom += m_nCyFrame*2;
				rcWin.bottom += m_nCyTitleBar;
				// now size the non-client area
				SetWindowPos(hWnd,NULL,0,0,
									  rcWin.Width(),
									  rcWin.Height(),
									  SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOCOPYBITS|SWP_NOOWNERZORDER);
				// get the full client area
				rcWin = GetFullVisibleWindowRect(hWnd);
				// clip it
				HRGN hRgn = CreateRectRgn(rcWin.left,rcWin.top,rcWin.right,rcWin.bottom);
				SetWindowRgn(hWnd,hRgn,FALSE);
			}
			// return false since we want the default
			// handler to also process this
			return FALSE;
		// Non-Client Calculate Size
		case WM_NCCALCSIZE:
			{
				// calculate the client rectangle
				NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*)lParam;
				// we have to do this manually, otherwise it gets
				// all fucked up for some reason
				int nCxDlgFrame = GetSystemMetrics(SM_CXDLGFRAME);
				int nCyDlgFrame = GetSystemMetrics(SM_CYDLGFRAME);
				int nCyTitleBar = GetSystemMetrics(SM_CYCAPTION);
				((CRect&)lpncsp->rgrc[0]).DeflateRect(nCxDlgFrame+m_nCxFrame,
							   nCyDlgFrame+m_nCyFrame+nCyTitleBar+m_nCyTitleBar,
							   nCxDlgFrame+m_nCxFrame,
							   nCyDlgFrame+m_nCyFrame);
			}
			*pResult = 0;
			break;
		// Non-Client activate
		case WM_NCACTIVATE:
			{
				// don't care at this point
			}
			*pResult = 0;
			break;
		// Non-Client Paint
		case WM_NCPAINT:
			{
				// TRACE("IN PAINT\n");
				// get windows device context for painting
				CWindowDC odc(CWnd::FromHandle(hWnd));
				// we dont want to paint to the client of this window
				odc.ExcludeClipRect(GetFullClientRect(hWnd));
				// get the full size of the window
				CRect rc = GetFullWindowRect(hWnd);

				// memory drawing for flicker-free //////////////////
				CDC dc;
				dc.CreateCompatibleDC(&odc);
				CBitmap memBmp;
				memBmp.CreateCompatibleBitmap(&odc,rc.Width(),rc.Height());
				CBitmap* pOldMemBmp = dc.SelectObject(&memBmp);
				/////////////////////////////////////////////////////

				// draw the contents of the non-client area /////////
				// we dont want to paint to the client of this window
				dc.ExcludeClipRect(GetFullClientRect(hWnd));
				// draw the frame
				DrawFrame(&dc,hWnd);
				// draw titlebar
				DrawTitleBar(&dc,hWnd);
				/////////////////////////////////////////////////////

				// memory drawing for flicker-free //////////////////
				odc.BitBlt(0,0,rc.Width(),rc.Height(),&dc,0,0,SRCCOPY);
				dc.SelectObject(pOldMemBmp);
				memBmp.DeleteObject();
				dc.DeleteDC();
				/////////////////////////////////////////////////////
			}
			*pResult = 0;
			break;
		// sys command - check for appended item
		case WM_SYSCOMMAND:
			{
				if(LOWORD(wParam) == SC_STAYONTOP){
					m_bIsOnTop = !m_bIsOnTop;
					SetWindowPos(hWnd,(m_bIsOnTop?HWND_TOPMOST:HWND_NOTOPMOST),0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
					HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
					if(hSysMenu){
						CheckMenuItem(hSysMenu,SC_STAYONTOP,(m_bIsOnTop?MF_CHECKED:MF_UNCHECKED));
					}
					RepaintNonClientArea(hWnd);
				}
			}
			return FALSE;
		// Client mouse action
		case WM_LBUTTONUP:
			OnLButtonUp(hWnd,lParam);
			return FALSE;
		// we lost capture of the mouse
		case WM_CAPTURECHANGED:
			if(MouseCaptured()){
				ReleaseMouse();
				m_nButtonDown = SYS_BUTTON_NONE;
				RepaintNonClientArea(hWnd);
			}
			return FALSE;
		// Non-Client mouse buttons...
		case WM_NCLBUTTONDOWN:
			*pResult = 0;
			return OnNCLButtonDown(hWnd, ConvertPointsToCPoint(hWnd,lParam));
		case WM_NCLBUTTONUP:
			*pResult = 0;
			return OnNCLButtonUp(hWnd, ConvertPointsToCPoint(hWnd,lParam));
		case WM_NCRBUTTONDOWN:
			*pResult = 0;
			return OnNCRButtonDown(hWnd, ConvertPointsToCPoint(hWnd,lParam));
		case WM_NCRBUTTONUP:
			*pResult = 0;
			return OnNCRButtonUp(hWnd, ConvertPointsToCPoint(hWnd,lParam));
		// following Non-Client messages we dont care about
		case WM_NCMOUSEMOVE:
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCXBUTTONDOWN:
		case WM_NCXBUTTONUP:
		case WM_NCXBUTTONDBLCLK:
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
		case WM_NCMBUTTONDBLCLK:
		case WM_NCMOUSEHOVER:
		case WM_NCMOUSELEAVE:
			{
				// do nothing...
			}
			*pResult = 0;
			break;
		// default - let DefWindowProc() handle it
		default:
			return FALSE;
	}
	// return TRUE if we handled the
	// message properly and return code is
	// in *pResult
	return TRUE;
}

//
//
//
void CNCArea::DrawFrame(CDC* pDC, HWND hWnd){

	CRect frameRect = GetFullVisibleWindowRect(hWnd);

	// fill/erase the entire non-client area
	pDC->FillSolidRect(frameRect, GetSysColor(COLOR_3DFACE));

	COLORREF colLight = GetSysColor(COLOR_3DHILIGHT);
	COLORREF colMain = GetSysColor(COLOR_3DSHADOW);

	// draw light frame
	pDC->Draw3dRect(frameRect,colLight,colLight);
	frameRect.DeflateRect(1,1);
	pDC->Draw3dRect(frameRect,colLight,colLight);
	// draw dark frame
	frameRect.InflateRect(1,1,0,0);
	pDC->Draw3dRect(frameRect,colMain,colMain);
}

//
// draws the titlebar
//
void CNCArea::DrawTitleBar(CDC* pDC, HWND hWnd){
	// get the titlebar rectangle
	CRect tBarRc = GetTitleBarRect(hWnd);

	// erase the bg
	pDC->FillSolidRect(tBarRc,GetSysColor(COLOR_3DFACE));

	// draw our path
	pDC->BeginPath();
	pDC->MoveTo(tBarRc.left,tBarRc.top);
	pDC->LineTo(GetSysButtonRect(TOTAL_SYS_BUTTONS-1,hWnd).left ,tBarRc.top);
	pDC->LineTo(GetSysButtonRect(TOTAL_SYS_BUTTONS-1,hWnd).left - tBarRc.Height(),tBarRc.top+tBarRc.Height());
	pDC->LineTo(tBarRc.left,tBarRc.top+tBarRc.Height());
	pDC->LineTo(tBarRc.left,tBarRc.top);
	pDC->EndPath();
	// only paint to our path
	pDC->SelectClipPath(RGN_COPY);

	// do what you want to do in path
	FillGradientRect(pDC,tBarRc,GetSysColor(COLOR_BTNTEXT),GetSysColor(COLOR_3DFACE));

	// clear path
	pDC->SelectClipRgn(NULL,RGN_COPY);
	
	// draw the buttons
	UINT nFlags;
	CRect rcSysButton = GetSysButtonRect(SYS_BUTTON_CLOSE,hWnd);
	nFlags = FLAT_BUTTONS | ((m_nButtonDown == SYS_BUTTON_CLOSE)?DFCS_CAPTIONCLOSE|DFCS_PUSHED:DFCS_CAPTIONCLOSE);
	pDC->DrawFrameControl(rcSysButton,DFC_CAPTION,nFlags);

	rcSysButton = GetSysButtonRect(SYS_BUTTON_MIN,hWnd);
	nFlags = FLAT_BUTTONS | ((m_nButtonDown == SYS_BUTTON_MIN)?DFCS_CAPTIONMIN|DFCS_PUSHED:DFCS_CAPTIONMIN);
	pDC->DrawFrameControl(rcSysButton,DFC_CAPTION,nFlags);

	rcSysButton = GetSysButtonRect(SYS_BUTTON_PIN,hWnd);
	DrawPinButton(pDC,rcSysButton,m_bIsOnTop,(m_nButtonDown == SYS_BUTTON_PIN));

	// draw icon
	int nTextLeft = tBarRc.left + m_nCxSysButtonSpacing;
	if(HasIcon()){
		CRect iconRect = GetIconRect(hWnd);
		DrawIconEx(pDC->m_hDC,iconRect.left,iconRect.top,m_hIcon,ICON_WIDTH,ICON_HEIGHT,0,NULL,DI_NORMAL);
		nTextLeft += ICON_WIDTH + m_nCxSysButtonSpacing;
	}

	// draw text

	CFont* pOldFont = pDC->SelectObject(&m_Font);

	int nWindowTxtLen = GetWindowTextLength(hWnd)+1;
	char* pText = new char[nWindowTxtLen+1];
	GetWindowText(hWnd,pText,nWindowTxtLen+1);
	CSize txtSize = pDC->GetTextExtent(pText);

	COLORREF oldTxtColor = pDC->SetTextColor(GetSysColor(COLOR_3DFACE));
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(nTextLeft+1,tBarRc.top + ((tBarRc.Height()/2)-(txtSize.cy/2))+1,pText);
	pDC->SetTextColor(GetSysColor(COLOR_3DSHADOW));
	pDC->TextOut(nTextLeft,tBarRc.top + ((tBarRc.Height()/2)-(txtSize.cy/2)),pText);
	pDC->SetBkMode(oldBkMode);
	pDC->SetTextColor(oldTxtColor);

	delete pText;

	pDC->SelectObject(pOldFont);
}

#define BLEND(s,e,p) ( RGB( int(double(p)/100*GetRValue(e) + (1-double(p)/100)*GetRValue(s)), \
							int(double(p)/100*GetGValue(e) + (1-double(p)/100)*GetGValue(s)), \
							int(double(p)/100*GetBValue(e) + (1-double(p)/100)*GetBValue(s)) ) )

void CNCArea::FillGradientRect(CDC* pDC, CRect rc, COLORREF colStart, COLORREF colEnd){
	for(int i = rc.left; i < rc.right; i++){
		CPen pn;
		pn.CreatePen(PS_SOLID,1,BLEND(colStart,colEnd,i*100/rc.Width()));
		CPen* oldPen = pDC->SelectObject(&pn);
		pDC->MoveTo(i,rc.top);
		pDC->LineTo(i,rc.bottom);
		pDC->SelectObject(oldPen);
		pn.DeleteObject();	
	}
}

//
//
//
CRect CNCArea::GetSysButtonRect(int nIndex, HWND hWnd){
	CRect hWndRect = GetTitleBarRect(hWnd);
	CRect rc;
	rc.left = hWndRect.right-(m_nCxSysButtonSpacing*(nIndex+1))-(m_nCxSysButton*(nIndex+1));
	rc.top = hWndRect.top + ((hWndRect.Height()/2)-(m_nCySysButton/2));
	rc.right = rc.left + m_nCxSysButton;
	rc.bottom = rc.top + m_nCySysButton;
	return rc;
}

//
//
//
CRect CNCArea::GetIconRect(HWND hWnd){
	CRect hWndRect = GetTitleBarRect(hWnd);
	CRect rc;
	rc.left = hWndRect.left + m_nCxSysButtonSpacing;
	rc.top = hWndRect.top + ((hWndRect.Height()/2)-(ICON_HEIGHT/2));
	rc.right = rc.left + ICON_WIDTH;
	rc.bottom = rc.top + ICON_HEIGHT;
	return rc;
}

//
// draws the pin button
//
void CNCArea::DrawPinButton(CDC* pDC, CRect rc, BOOL bStuck, BOOL bPushed){
	
	UINT nState = DFCS_BUTTONPUSH|FLAT_BUTTONS;
	if(bPushed){ nState |= DFCS_PUSHED; }

	pDC->DrawFrameControl(rc,DFC_BUTTON,nState);
	
	if(bPushed){
		rc.left += 1;
		rc.top += 1;
		rc.bottom += 1;
		rc.right += 1;
	}

	if(bStuck){
		rc.DeflateRect(3,3);
		pDC->FillSolidRect(rc,0);
	}else{
		rc.left += (rc.Width()/2)-1;
		rc.top += (rc.Height()/2)-1;
		rc.right = rc.left + 2;
		rc.bottom = rc.top + 2;
		pDC->FillSolidRect(rc,0);
	}
}

//
//
//
CRect CNCArea::GetTitleBarRect(HWND hWnd){
	CRect rc = GetFullVisibleWindowRect(hWnd);
	rc.DeflateRect(m_nCxFrame,m_nCyFrame);
	rc.bottom = rc.top + m_nCyTitleBar-1;// -1 for a 1 pixel gap between title bar and client area
	return rc;
}

//
//
//
CRect CNCArea::GetFullWindowRect(HWND hWnd){
	CWnd* pWindow = CWnd::FromHandle(hWnd);
	CRect rcWin;
	pWindow->GetWindowRect(rcWin);
	pWindow->ScreenToClient(rcWin);
	rcWin.bottom -= rcWin.top;
	rcWin.top -= rcWin.top;
	rcWin.right -= rcWin.left;
	rcWin.left -= rcWin.left;
	return rcWin;
}

//
//
//
CRect CNCArea::GetFullVisibleWindowRect(HWND hWnd){
	int nCxDlgFrame = GetSystemMetrics(SM_CXDLGFRAME);
	int nCyDlgFrame = GetSystemMetrics(SM_CYDLGFRAME);
	int nCyTitleBar = GetSystemMetrics(SM_CYCAPTION);

	CRect rc = GetFullWindowRect(hWnd);
	rc.DeflateRect(nCxDlgFrame,
				   nCyDlgFrame+nCyTitleBar,
				   nCxDlgFrame,
				   nCyDlgFrame);
	return rc;
}

//
//
//
CRect CNCArea::GetFullClientRect(HWND hWnd){
	CRect rc = GetFullVisibleWindowRect(hWnd);
	rc.DeflateRect(m_nCxFrame,
				   m_nCyFrame+m_nCyTitleBar,
				   m_nCxFrame,
				   m_nCyFrame);
	return rc;
}

//
//
//
CPoint CNCArea::ConvertPointsToCPoint(HWND hWnd, unsigned long points){
	POINTS pts = *(POINTS*)&points;
	CPoint pt = CPoint(pts.x,pts.y);
	CRect rc = GetFullClientRect(hWnd);
	ScreenToClient(hWnd,&pt);
	pt.y += rc.top;
	pt.x += rc.left;
	return pt;
}

//
//
//
POINTS CNCArea::ConvertCPointToPoints(HWND hWnd, CPoint pt){
	CRect rc = GetFullClientRect(hWnd);
	pt.x -= rc.left;
	pt.y -= rc.top;
	ClientToScreen(hWnd,&pt);
	unsigned long points = MAKELPARAM(pt.x,pt.y);
	return *(POINTS*)&points;
}

//
// mouse buttons - return FALSE if we want
// the DefWindowProc() to handle it, TRUE
// if we want to block it
//
BOOL CNCArea::OnNCLButtonDown(HWND hWnd, CPoint pt){
	CRect tBarRc = GetTitleBarRect(hWnd);
	// has to be in the title bar
	if(!tBarRc.PtInRect(pt)){ return FALSE; }

	// if its in the icon, open a context menu
	CRect iconRc = GetIconRect(hWnd);
	if(iconRc.PtInRect(pt) && HasIcon()){
		CPoint ptMenu(iconRc.left,iconRc.bottom);
		POINTS& pts = ConvertCPointToPoints(hWnd,ptMenu);
		SendMessage(hWnd,WM_CONTEXTMENU,(WPARAM)hWnd,*(unsigned long*)&pts);
		return TRUE;
	}

	// find if it is in a system button
	int nIndex = -1;
	for(int i = 0; i < TOTAL_SYS_BUTTONS; i++){
		CRect rc = GetSysButtonRect(i,hWnd);
		if(rc.PtInRect(pt)){
			nIndex = i;
			break;
		}
	}

	if(nIndex != -1){
		CaptureMouse(hWnd);
		m_nButtonDown = nIndex;
		RepaintNonClientArea(hWnd);
		return TRUE;
	}

	return FALSE;
}

void CNCArea::OnLButtonUp(HWND hWnd, LPARAM lParam){
	if(!MouseCaptured()){ return; }
	ReleaseMouse();

	// translate the mouse coords to window coords
	CRect rc = GetFullClientRect(hWnd);
	CPoint pt((short)LOWORD(lParam),(short)HIWORD(lParam));
	pt.y += rc.top;
	pt.x += rc.left;
	
	// find what sys button we are currently over
	int nIndex = -1;
	for(int i = 0; i < TOTAL_SYS_BUTTONS; i++){
		CRect rc = GetSysButtonRect(i,hWnd);
		if(rc.PtInRect(pt)){
			nIndex = i;
			break;
		}
	}

	// if thw button down is the one we are over, then
	// preform the task
	if(nIndex != -1 && m_nButtonDown == nIndex){
		switch(nIndex){
			case SYS_BUTTON_CLOSE:
				SendMessage(hWnd,WM_SYSCOMMAND,SC_CLOSE,0);
				break;
			case SYS_BUTTON_MIN:
				m_nButtonDown = SYS_BUTTON_NONE;
				RepaintNonClientArea(hWnd);
				SendMessage(hWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
				break;
			case SYS_BUTTON_PIN:
				m_nButtonDown = SYS_BUTTON_NONE;
				SendMessage(hWnd,WM_SYSCOMMAND,SC_STAYONTOP,0);
				break;
		}
	}else{
		m_nButtonDown = SYS_BUTTON_NONE;
		RepaintNonClientArea(hWnd);
	}
}

BOOL CNCArea::OnNCRButtonDown(HWND hWnd, CPoint pt){
	CRect tBarRc = GetTitleBarRect(hWnd);
	// has to be in the title bar
	if(!tBarRc.PtInRect(pt)){ return FALSE; }
	// find if it is in a system button
	for(int i = 0; i < TOTAL_SYS_BUTTONS; i++){
		CRect rc = GetSysButtonRect(i,hWnd);
		if(rc.PtInRect(pt)){
			// if it is, block this message
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CNCArea::OnNCLButtonUp(HWND hWnd, CPoint pt){
	// nothing yet...
	return FALSE;
}

BOOL CNCArea::OnNCRButtonUp(HWND hWnd, CPoint pt){
	// nothing yet...
	return FALSE;
}
