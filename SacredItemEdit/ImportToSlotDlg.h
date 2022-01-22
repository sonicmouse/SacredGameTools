#if !defined(AFX_IMPORTTOSLOTDLG_H__0115DD97_C0CF_45C8_B605_078D9CE2BB74__INCLUDED_)
#define AFX_IMPORTTOSLOTDLG_H__0115DD97_C0CF_45C8_B605_078D9CE2BB74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportToSlotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportToSlotDlg dialog

#include "Hero.h"

class CImportToSlotDlg : public CDialog
{
// Construction
public:
	CImportToSlotDlg(CWnd* pParent = NULL);   // standard constructor

	void SetHero(CHero* pHero){
		m_pHero = pHero;
	}

	void SetMainItem(ULONG ulItemId){
		m_ulItemId = ulItemId;
	}

private:
	int GetSelectedColor();

private:
	CHero* m_pHero;
	ULONG m_ulItemId;

// Dialog Data
	//{{AFX_DATA(CImportToSlotDlg)
	enum { IDD = IDD_IMPORTTOSLOT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportToSlotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportToSlotDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowseItos();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTTOSLOTDLG_H__0115DD97_C0CF_45C8_B605_078D9CE2BB74__INCLUDED_)
