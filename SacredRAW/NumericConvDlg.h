#if !defined(AFX_NUMERICCONVDLG_H__8330CBFB_AC7D_41F2_8DE5_D8FD1523B47F__INCLUDED_)
#define AFX_NUMERICCONVDLG_H__8330CBFB_AC7D_41F2_8DE5_D8FD1523B47F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumericConvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNumericConvDlg dialog

class CNumericConvDlg : public CDialog
{
// Construction
public:
	CNumericConvDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNumericConvDlg)
	enum { IDD = IDD_NUMERICCONV_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumericConvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNumericConvDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMERICCONVDLG_H__8330CBFB_AC7D_41F2_8DE5_D8FD1523B47F__INCLUDED_)
