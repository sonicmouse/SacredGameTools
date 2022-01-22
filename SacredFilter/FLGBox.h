#ifndef __FLSTUDIO_GROUPBOX_H__
#define __FLSTUDIO_GROUPBOX_H__

// FLGBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFLGBox window

class CFLGBox : public CButton{

private:
	void DrawEtchedBorder(CDC* pDC, CRect rc, COLORREF colMain, COLORREF colLight);
	void DrawTextBox(CDC* pDC, const char* szText, COLORREF colText, COLORREF colBgFill, COLORREF colLight, COLORREF colMain);

private:
	CFont m_fontText;

// Construction
public:
	CFLGBox();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFLGBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFLGBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFLGBox)
	afx_msg void OnPaint();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* __FLSTUDIO_GROUPBOX_H__ */
