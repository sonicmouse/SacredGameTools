#if !defined(AFX_IMPORTITEMDLG_H__634A7F66_BDA6_4B29_9AE1_63A67F4051C1__INCLUDED_)
#define AFX_IMPORTITEMDLG_H__634A7F66_BDA6_4B29_9AE1_63A67F4051C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportItemDlg dialog

#include "HoverButton.h"
#include "Item.h"

class CImportItemDlg : public CDialog
{
// Construction
public:
	CImportItemDlg(CWnd* pParent = NULL);   // standard constructor

	CItem* GetImportedItem(){ return &m_Item; }

private:
	CItem m_Item;

// Dialog Data
	//{{AFX_DATA(CImportItemDlg)
	enum { IDD = IDD_IMPORTITEM_DIALOG };
	CComboBox	m_ComboTo;
	CHoverButton	m_ButtonOK;
	CHoverButton	m_ButtonCancel;
	CHoverButton	m_Browse;
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
	afx_msg void OnButtonBrowseimportitem();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTITEMDLG_H__634A7F66_BDA6_4B29_9AE1_63A67F4051C1__INCLUDED_)
