#if !defined(AFX_PAGEDLG_H__0B7F29AF_F012_4FBE_95C4_5F3A75DCEEE4__INCLUDED_)
#define AFX_PAGEDLG_H__0B7F29AF_F012_4FBE_95C4_5F3A75DCEEE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageDlg dialog

#include "PPDumpCtrl.h"

class CPageDlg : public CDialog
{
// Construction
public:
	CPageDlg(CWnd* pParent = NULL);   // standard constructor

	void SetPageData(const UCHAR* pData, int nSize);
	void SetPageType(int nType){ m_nType = nType; }

	int GetPageType(){ return m_nType; }
	void DoSizing();

private:
	UCHAR* m_pBuffer;
	int m_nSize;

	UCHAR* m_pOldBuffer;
	int m_nOldSize;

	int m_nType;
	CPPDumpCtrl* m_pHexCtrl;

// Dialog Data
	//{{AFX_DATA(CPageDlg)
	enum { IDD = IDD_PAGE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDLG_H__0B7F29AF_F012_4FBE_95C4_5F3A75DCEEE4__INCLUDED_)
