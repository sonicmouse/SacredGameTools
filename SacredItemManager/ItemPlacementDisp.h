#if !defined(AFX_ITEMPLACEMENTDISP_H__7F1A9FDB_F002_4E01_AB3E_04EFFC910C12__INCLUDED_)
#define AFX_ITEMPLACEMENTDISP_H__7F1A9FDB_F002_4E01_AB3E_04EFFC910C12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemPlacementDisp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemPlacementDisp window

#include "Hero.h"

class CItemPlacementDisp : public CStatic
{
// Construction
public:
	CItemPlacementDisp();

	void SetHero(CHero* pHero){ m_pHero = pHero; Repaint(); }
	void SetSelItem(CItem* pItem, int nIndex = -1){
		m_pSelItem = pItem;
		m_nSelItemInd = nIndex;
		Repaint();
	}
	void SetInvType(ULONG ulType){ m_ulInvType = ulType; }
	void Repaint(){ Invalidate(FALSE); }
	void SetDisabled(BOOL bDisabled){
		m_bDisabled = bDisabled;
		Repaint();
	}

private:
	void PaintOOB(CDC* pDC);
	CRect CreateItemRect(CItem* pItem);
	void TransparentBmp(CDC* pDC, int x, int y, CBitmap* pBmp, COLORREF colTrans);
	BOOL CreateMaskBitmap(CDC* pDC, CBitmap* pBitmap, CRect rc, COLORREF maskColor, COLORREF lineColor);

private:
	CHero* m_pHero;
	ULONG m_ulInvType;
	CItem* m_pSelItem;
	int m_nSelItemInd;
	BOOL m_bDisabled;
	CFont m_fontText;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemPlacementDisp)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CItemPlacementDisp();

	// Generated message map functions
protected:
	//{{AFX_MSG(CItemPlacementDisp)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMPLACEMENTDISP_H__7F1A9FDB_F002_4E01_AB3E_04EFFC910C12__INCLUDED_)
