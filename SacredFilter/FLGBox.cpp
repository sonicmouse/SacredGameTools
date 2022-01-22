// FLGBox.cpp : implementation file
//

#include "stdafx.h"
#include "FLGBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFLGBox

CFLGBox::CFLGBox()
{
	m_fontText.CreateFont(14, 0, 0, 0, FW_BOLD,
				  FALSE, FALSE, 0, ANSI_CHARSET,
				  OUT_TT_PRECIS, 
				  CLIP_DEFAULT_PRECIS,
				  ANTIALIASED_QUALITY, 
				  DEFAULT_PITCH|FF_SWISS, "Arial");
}

CFLGBox::~CFLGBox()
{
	m_fontText.DeleteObject();
}


BEGIN_MESSAGE_MAP(CFLGBox, CButton)
	//{{AFX_MSG_MAP(CFLGBox)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFLGBox message handlers

void CFLGBox::OnPaint(){
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(rc);

	// select in our font
	CFont* pOldFont = dc.SelectObject(&m_fontText);

	// fill the bg (taken out because this was painting over
	// controls that were inside the frame)
	//dc.FillSolidRect(rc,GetSysColor(COLOR_BTNFACE));

	// draw the etched border
	DrawEtchedBorder(&dc, rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DHILIGHT));

	// draw text box
	int nTxtLen = GetWindowTextLength();
	if(nTxtLen>0){
		char* szText = new char[nTxtLen+1];
		GetWindowText(szText,nTxtLen+1);
		DrawTextBox(&dc,
					szText,
					GetSysColor(COLOR_BTNFACE),
					GetSysColor(COLOR_BTNTEXT),
					GetSysColor(COLOR_3DHILIGHT),
					GetSysColor(COLOR_BTNFACE));
		delete szText;
	}

	// reset font
	dc.SelectObject(pOldFont);
}

void CFLGBox::DrawTextBox(CDC* pDC,
						  const char* szText, 
						  COLORREF colText, 
						  COLORREF colBgFill, 
						  COLORREF colLight, 
						  COLORREF colMain){

	// get the size of the text
	CSize sizeText = pDC->GetTextExtent(szText);

	// main border and bgfill
	CPen penMain;
	penMain.CreatePen(PS_SOLID,0,colMain);
	CPen* oldPen = pDC->SelectObject(&penMain);

	CBrush brushBgFill;
	brushBgFill.CreateSolidBrush(colBgFill);
	CBrush* pOldBrush = pDC->SelectObject(&brushBgFill);

	int nTriangleSide = sizeText.cy + 1;

	pDC->BeginPath();
	pDC->MoveTo(1,1);
	pDC->LineTo(4+sizeText.cx-1+nTriangleSide,1);
	pDC->LineTo(4+sizeText.cx-1,nTriangleSide+1);
	pDC->LineTo(1,nTriangleSide+1);
	pDC->LineTo(1,1);
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(pOldBrush);
	brushBgFill.DeleteObject();

	pDC->SelectObject(oldPen);
	penMain.DeleteObject();

	// light border pen
	CPen penLight;
	penLight.CreatePen(PS_SOLID,0,colLight);
	oldPen = pDC->SelectObject(&penLight);

	pDC->MoveTo(4+sizeText.cx+nTriangleSide,1);
	pDC->LineTo(4+sizeText.cx,nTriangleSide+1);
	pDC->LineTo(1,nTriangleSide+1);
	pDC->LineTo(1,nTriangleSide);

	pDC->SelectObject(oldPen);
	penLight.DeleteObject();

	// text
	COLORREF oldTxtCol = pDC->SetTextColor(colText);
	COLORREF oldBkCol = pDC->SetBkColor(colBgFill);
	pDC->TextOut(4,2,szText);
	pDC->SetBkColor(oldBkCol);
	pDC->SetTextColor(oldTxtCol);
}

void CFLGBox::DrawEtchedBorder(CDC* pDC, CRect rc, COLORREF colMain, COLORREF colLight){
	// draw light frame
	pDC->Draw3dRect(rc,colLight,colLight);
	rc.DeflateRect(1,1);
	pDC->Draw3dRect(rc,colLight,colLight);
	// draw dark frame
	rc.InflateRect(1,1,0,0);
	pDC->Draw3dRect(rc,colMain,colMain);
}

HBRUSH CFLGBox::CtlColor(CDC* pDC, UINT nCtlColor){
	GetParent()->Invalidate();
	return NULL;
}
