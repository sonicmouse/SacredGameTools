// SSDDlg.h : header file
//

#if !defined(AFX_SSDDLG_H__2FB8F53D_4052_43B7_BE3F_C7EE361B8516__INCLUDED_)
#define AFX_SSDDLG_H__2FB8F53D_4052_43B7_BE3F_C7EE361B8516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSSDDlg dialog

#define TARGET_SECTION (0xC7)
#define GOLD_OFFSET (0x3D3)

class CSSDDlg : public CDialog
{
// Construction
public:
	CSSDDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSSDDlg)
	enum { IDD = IDD_SACREDSUGARDADDY_DIALOG };
	CSpinButtonCtrl	m_SpinGold;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSSDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowse();
	afx_msg void OnDestroy();
	afx_msg void OnStaticMaxgold();
	afx_msg void OnButtonUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDDLG_H__2FB8F53D_4052_43B7_BE3F_C7EE361B8516__INCLUDED_)
