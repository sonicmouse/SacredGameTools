#if !defined(AFX_DUMPVIEWERDLG_H__BA3024B7_012C_4473_BC41_E58CAFAB6E15__INCLUDED_)
#define AFX_DUMPVIEWERDLG_H__BA3024B7_012C_4473_BC41_E58CAFAB6E15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DumpViewerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDumpViewerDlg dialog

#include "PPDumpCtrl.h"

class CDumpViewerDlg : public CDialog
{
// Construction
public:
	CDumpViewerDlg(CWnd* pParent = NULL);   // standard constructor

	void SetDump(PST_BUFFER pBuffer){ m_pBuffer = pBuffer; }

private:
	PST_BUFFER m_pBuffer;
	CPPDumpCtrl* m_pHexCtrl;

// Dialog Data
	//{{AFX_DATA(CDumpViewerDlg)
	enum { IDD = IDD_DUMPVIEWER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDumpViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDumpViewerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMPVIEWERDLG_H__BA3024B7_012C_4473_BC41_E58CAFAB6E15__INCLUDED_)
