#if !defined(AFX_HOVERBUTTON_H__2634C5A9_93D2_447A_86A4_06AE935B421B__INCLUDED_)
#define AFX_HOVERBUTTON_H__2634C5A9_93D2_447A_86A4_06AE935B421B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoverButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoverButton window

#define TIMER_ID_OVER_BUTTON 0xBADF01
#define HOV_HEI 1

class CHoverButton : public CButton
{
// Construction
public:
	CHoverButton();

	void SetMyDetails(UINT nBmpResId, COLORREF transCol, BOOL bDrawFoc){ m_nBmpResId = nBmpResId; m_bDrawFoc = bDrawFoc; m_colTransparent = transCol; } 
	void SetMyBkColor(COLORREF col){m_colBkCol=col;}

private:
	BOOL m_bIsOverButton;
	UINT m_nBmpResId;
	BOOL m_bDrawFoc;
	COLORREF m_colBkCol;
	COLORREF m_colTransparent;

	void TransparentBmp(CDC* pDC, int x, int y, CBitmap* pBmp, COLORREF colTrans);
	void ShadowBlt(CDC* pDC, int x, int y, UINT bmpID, COLORREF colTrans, COLORREF colBg);
	void GrayBlt(CDC* pDC, int x, int y, UINT bmpID, COLORREF colTrans, COLORREF colBg);
	void GetDisabledBitmap(CBitmap &bmp,COLORREF background, COLORREF transpColor);

	COLORREF LightenColor(COLORREF col, double factor);
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoverButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHoverButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHoverButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOVERBUTTON_H__2634C5A9_93D2_447A_86A4_06AE935B421B__INCLUDED_)
