#if !defined(AFX_PLACEMENTDISPLAY_H__18815C0F_8DDF_4BD1_8313_146B0D441810__INCLUDED_)
#define AFX_PLACEMENTDISPLAY_H__18815C0F_8DDF_4BD1_8313_146B0D441810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlacementDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlacementDisplay window

#define LINE_WIDTH 1

#include "Hero.h"

class CPlacementDisplay : public CStatic
{
// Construction
public:
	CPlacementDisplay();

	void Repaint(){ Invalidate(FALSE); }
	
	ULONG GetInvType(){ return m_nInvType; }

	void SetDetails(CHero* pHero, int nInvType){
		m_ulSelected = 0xFFFFFFFF;
		m_pHero = pHero;
		m_nInvType = nInvType;
		Repaint();
	}

	void ClearSelection(){
		m_ulSelected = 0xFFFFFFFF;
		Repaint();
	}

	void SetSelectedItem(ULONG ulIndex){
		m_ulSelected = ulIndex;
		Repaint();
	}

	void Disable(BOOL bDisable){
		m_bDisabled = bDisable;
		Repaint();
	}

private:
	void PaintArray(CDC* pDC, PST_PLACEMENT_ITEM pItem, CPoint pt, CSize sp, COLORREF col, BOOL bSel);
	void PaintOOB(CDC* pDC);
	PST_PLACEMENT_ITEM GetItemFromPoint(CPoint pt);
	BOOL CreateMaskBitmap(CDC* pDC, CBitmap* pBitmap, CRect rc, COLORREF maskColor, COLORREF lineColor);
	void TransparentBmp(CDC* pDC, int x, int y, CBitmap* pBmp, COLORREF colTrans);

private:
	CHero* m_pHero;
	int m_nInvType;
	ULONG m_ulSelected;
	BOOL m_bDisabled;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlacementDisplay)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlacementDisplay();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlacementDisplay)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLACEMENTDISPLAY_H__18815C0F_8DDF_4BD1_8313_146B0D441810__INCLUDED_)
