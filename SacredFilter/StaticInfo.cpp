// StaticInfo.cpp : implementation file
//

#include "stdafx.h"
#include "SacredFilter.h"
#include "StaticInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticInfo

CStaticInfo::CStaticInfo()
{
	m_colTitle = RGB(0,0,0);
	m_colInfo = RGB(0,0,0);
	m_nSplitSize = -1;
	m_FontTitle.CreateFont(14, 0, 0, 0, FW_BOLD,
				  FALSE, FALSE, 0, ANSI_CHARSET,
				  OUT_TT_PRECIS, 
				  CLIP_DEFAULT_PRECIS,
				  ANTIALIASED_QUALITY, 
				  DEFAULT_PITCH|FF_SWISS, "Arial");
	m_FontInfo.CreateFont(14, 0, 0, 0, FW_NORMAL,
				  FALSE, FALSE, 0, ANSI_CHARSET,
				  OUT_TT_PRECIS, 
				  CLIP_DEFAULT_PRECIS,
				  ANTIALIASED_QUALITY, 
				  DEFAULT_PITCH|FF_SWISS, "Arial");
}

CStaticInfo::~CStaticInfo()
{
	m_FontTitle.DeleteObject();
	m_FontInfo.DeleteObject();
}

BEGIN_MESSAGE_MAP(CStaticInfo, CStatic)
	//{{AFX_MSG_MAP(CStaticInfo)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticInfo message handlers

void CStaticInfo::OnPaint() 
{
	CPaintDC odc(this); // device context for painting

	CRect rc;
	GetClientRect(rc);

	// memory drawing for flicker-free //////////////////
	CDC dc;
	dc.CreateCompatibleDC(&odc);
	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(&odc,rc.Width(),rc.Height());
	CBitmap* pOldMemBmp = dc.SelectObject(&memBmp);
	/////////////////////////////////////////////////////

	dc.FillSolidRect(rc,GetSysColor(COLOR_BTNFACE));

	if(m_szTitle.GetLength()){
		int nOldBkMode = dc.SetBkMode(TRANSPARENT);
		COLORREF colOldTxtColor = dc.SetTextColor(m_colTitle);
		CFont* pOldFont = dc.SelectObject(&m_FontTitle);

		if(m_nSplitSize==-1){
			dc.TextOut(0,0,m_szTitle);
			CSize sizeTitle = dc.GetTextExtent(m_szTitle);
			dc.SetTextColor(m_colInfo);
			dc.SelectObject(&m_FontInfo);
			dc.TextOut(sizeTitle.cx,0,m_szInfo);
		}else{
			CRect leftRc = rc;
			leftRc.right = m_nSplitSize;
			dc.DrawText(m_szTitle,leftRc,DT_RIGHT);
			leftRc.right = rc.right;
			leftRc.left = m_nSplitSize;
			dc.SetTextColor(m_colInfo);
			dc.SelectObject(&m_FontInfo);
			dc.DrawText(m_szInfo,leftRc,DT_LEFT);
		}
		dc.SelectObject(pOldFont);
		dc.SetBkMode(nOldBkMode);
		dc.SetTextColor(colOldTxtColor);
	}

	// memory drawing for flicker-free //////////////////
	odc.BitBlt(0,0,rc.Width(),rc.Height(),&dc,0,0,SRCCOPY);
	dc.SelectObject(pOldMemBmp);
	memBmp.DeleteObject();
	dc.DeleteDC();
	/////////////////////////////////////////////////////
}
