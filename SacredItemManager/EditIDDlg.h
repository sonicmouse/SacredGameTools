#if !defined(AFX_EDITIDDLG_H__BED25542_6300_4C3F_8FAC_3B8617E8D0B7__INCLUDED_)
#define AFX_EDITIDDLG_H__BED25542_6300_4C3F_8FAC_3B8617E8D0B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditIDDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditIDDlg dialog

class CEditIDDlg : public CDialog
{
// Construction
public:
	CEditIDDlg(CWnd* pParent = NULL);   // standard constructor

	void SetDescId(ULONG* pID){ m_pID = pID; }

private:
	ULONG* m_pID;

// Dialog Data
	//{{AFX_DATA(CEditIDDlg)
	enum { IDD = IDD_EDITID_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditIDDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonWtfcid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITIDDLG_H__BED25542_6300_4C3F_8FAC_3B8617E8D0B7__INCLUDED_)
