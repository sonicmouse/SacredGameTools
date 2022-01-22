#if !defined(AFX_VIEWDUMPDLG_H__857FC7A9_EA38_4899_8E3F_2D0E4DC4B819__INCLUDED_)
#define AFX_VIEWDUMPDLG_H__857FC7A9_EA38_4899_8E3F_2D0E4DC4B819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDumpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewDumpDlg dialog

#include "PPDumpCtrl.h"
#include "Item.h"

class CViewDumpDlg : public CDialog
{
// Construction
public:
	CViewDumpDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void SetItem(CItem* pItem){ m_pItem = pItem; }

private:
	CPPDumpCtrl* m_pHexCtrl;
	CItem* m_pItem;

// Dialog Data
	//{{AFX_DATA(CViewDumpDlg)
	enum { IDD = IDD_VIEWDUMP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDumpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewDumpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDUMPDLG_H__857FC7A9_EA38_4899_8E3F_2D0E4DC4B819__INCLUDED_)
