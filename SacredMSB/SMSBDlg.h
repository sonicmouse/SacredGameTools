// SMSBDlg.h : header file
//

#if !defined(AFX_SMSBDLG_H__1F0B894E_7560_4980_B31E_583E3B371268__INCLUDED_)
#define AFX_SMSBDLG_H__1F0B894E_7560_4980_B31E_583E3B371268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSMSBDlg dialog

#define TARGET_SECTION (0xC4)

class CSMSBDlg : public CDialog
{
// Construction
public:
	CSMSBDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSMSBDlg)
	enum { IDD = IDD_SACREDMSB_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSMSBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonAbout();
	afx_msg void OnButtonMax();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSBDLG_H__1F0B894E_7560_4980_B31E_583E3B371268__INCLUDED_)
