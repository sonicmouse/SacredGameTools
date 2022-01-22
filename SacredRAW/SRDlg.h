// SRDlg.h : header file
//

#if !defined(AFX_SRDLG_H__454A6BCB_AD83_452C_90AD_C9C239242BF6__INCLUDED_)
#define AFX_SRDLG_H__454A6BCB_AD83_452C_90AD_C9C239242BF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSRDlg dialog

#include "TabPages.h"

class CSRDlg : public CDialog
{
// Construction
public:
	CSRDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSRDlg)
	enum { IDD = IDD_SACREDRAW_DIALOG };
	CTabPages	m_Tabs;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSRDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowse();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRDLG_H__454A6BCB_AD83_452C_90AD_C9C239242BF6__INCLUDED_)
