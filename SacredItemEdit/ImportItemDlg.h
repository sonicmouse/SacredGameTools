#if !defined(AFX_IMPORTITEMDLG_H__5E2C39A8_7691_40B4_9E48_7818EFBDBFFD__INCLUDED_)
#define AFX_IMPORTITEMDLG_H__5E2C39A8_7691_40B4_9E48_7818EFBDBFFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportItemDlg dialog

#include "Hero.h"
#include "SITFile.h"
#include "SITFileDlg.h"

class CImportItemDlg : public CDialog
{
// Construction
public:
	CImportItemDlg(CWnd* pParent = NULL);   // standard constructor

	void SetHeroFile(CHero* pHero){
		m_pHero = pHero;
	}

private:
	CHero* m_pHero;

// Dialog Data
	//{{AFX_DATA(CImportItemDlg)
	enum { IDD = IDD_IMPORTITEM_DIALOG };
	CComboBox	m_ComboTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonImportBrowse();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTITEMDLG_H__5E2C39A8_7691_40B4_9E48_7818EFBDBFFD__INCLUDED_)
