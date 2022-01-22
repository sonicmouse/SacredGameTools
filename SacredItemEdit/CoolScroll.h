#ifndef __COOL_SCROLL_H__
#define __COOL_SCROLL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolScroll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolScroll window

#define TIMER_ID_SCROLL_TEXT 0xAC765DA

class CCoolScroll : public CWnd
{
// Construction
public:
	CCoolScroll();

// Attributes
public:

// Operations
public:
	void SetBackgroundBitmap(UINT nResource);
	void SetScrollText(CString szText){ m_szText = szText; }
	void SetScrollTextColor(COLORREF col){ m_colText = col; }
	void SetBackgroundColor(COLORREF col){ m_colBg = col; }
	void DrawFrame(BOOL bFrame){ m_bFrameIt = bFrame; }
	void SetTextShadow(COLORREF col){ m_colShadow = col; }
	void SetShadowDrop(int drop){ m_shadowDrop = drop; }

	BOOL Create(CWnd* pParent, int iIdControl);

	void DoScroll(BOOL bDo);

private:
	void CalculateTextRect(HDC dc, CRect& rc);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolScroll)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolScroll();

private:
	UINT m_nBmpResource;
	CString m_szText;
	COLORREF m_colText;
	COLORREF m_colBg;
	BOOL m_bIsCreated;
	HBITMAP m_bmpBg;
	CString m_szWndClass;
	int n_currPos;
	CSize m_sizeBgBmp;
	BOOL m_bFrameIt;
	CFont m_Font;
	COLORREF m_colShadow;
	int m_shadowDrop;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCoolScroll)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* __COOL_SCROLL_H__ */
