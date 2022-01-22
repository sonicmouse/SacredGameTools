#if !defined(AFX_DUPITEMDLG_H__6D3B665C_0C9F_4A62_AA38_E35DCAF163F7__INCLUDED_)
#define AFX_DUPITEMDLG_H__6D3B665C_0C9F_4A62_AA38_E35DCAF163F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DupItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDupItemDlg dialog

class CDupItemDlg : public CDialog
{
// Construction
public:
	CDupItemDlg(CWnd* pParent = NULL);   // standard constructor

	int GetTimes(){ return m_nTimes; }
	BOOL GetShrink(){ return m_bShrink; }
	BOOL GetToChest(){ return m_bToChest; }

private:
	int m_nTimes;
	BOOL m_bShrink;
	BOOL m_bToChest;

// Dialog Data
	//{{AFX_DATA(CDupItemDlg)
	enum { IDD = IDD_DUPITEM_DIALOG };
	CComboBox	m_Combo;
	CSpinButtonCtrl	m_Spin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDupItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDupItemDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUPITEMDLG_H__6D3B665C_0C9F_4A62_AA38_E35DCAF163F7__INCLUDED_)
