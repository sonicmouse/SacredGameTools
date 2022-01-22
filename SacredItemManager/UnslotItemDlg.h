#if !defined(AFX_UNSLOTITEMDLG_H__7F2425FE_AE2C_4449_BC10_9CE3DF57F87E__INCLUDED_)
#define AFX_UNSLOTITEMDLG_H__7F2425FE_AE2C_4449_BC10_9CE3DF57F87E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UnslotItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUnslotItemDlg dialog

#include "HoverButton.h"

class CUnslotItemDlg : public CDialog
{
// Construction
public:
	CUnslotItemDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL UnslotToBackpack(){ return m_bUnslotToBackpack; }

private:
	BOOL m_bUnslotToBackpack;

// Dialog Data
	//{{AFX_DATA(CUnslotItemDlg)
	enum { IDD = IDD_UNSLOTITEM_DIALOG };
	CComboBox	m_ComboTo;
	CHoverButton	m_ButtonOK;
	CHoverButton	m_ButtonCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnslotItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUnslotItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNSLOTITEMDLG_H__7F2425FE_AE2C_4449_BC10_9CE3DF57F87E__INCLUDED_)
