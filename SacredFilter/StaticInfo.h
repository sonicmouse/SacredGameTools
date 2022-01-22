#ifndef __STATIC_INFO_H__
#define __STATIC_INFO_H__

// StaticInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticInfo window

class CStaticInfo : public CStatic
{
// Construction
public:
	CStaticInfo();
	void SetInfo(const CString& szTitle, const CString& szInfo){
		m_szTitle = szTitle;
		m_szInfo = szInfo;
		Repaint();
	}

	void SetSplitSize(int nSize){ m_nSplitSize = nSize; Repaint(); }
	void SetColorTitle(COLORREF col){ m_colTitle = col; Repaint(); }
	void SetColorInfo(COLORREF col){ m_colInfo = col; Repaint(); }

	void Repaint(){ Invalidate(FALSE); }

private:
	CString m_szTitle;
	CString m_szInfo;
	CFont m_FontTitle;
	CFont m_FontInfo;
	COLORREF m_colTitle;
	COLORREF m_colInfo;
	int m_nSplitSize;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticInfo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticInfo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticInfo)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* __STATIC_INFO_H__ */
