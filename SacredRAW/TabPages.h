#if !defined(AFX_TABPAGES_H__8F056EB3_7403_4322_BF6C_2B1A0D26B571__INCLUDED_)
#define AFX_TABPAGES_H__8F056EB3_7403_4322_BF6C_2B1A0D26B571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabPages.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabPages window

#include "PageDlg.h"

class CTabPages : public CTabCtrl
{
// Construction
public:
	CTabPages();

	void AddPage(const UCHAR* pBuffer, int nSize, int nType);
	void ClearPages();

private:
	CPtrArray m_arrPages;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabPages)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabPages();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabPages)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPAGES_H__8F056EB3_7403_4322_BF6C_2B1A0D26B571__INCLUDED_)
