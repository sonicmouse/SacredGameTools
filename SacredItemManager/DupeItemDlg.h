#if !defined(AFX_DUPEITEMDLG_H__E63E4AAD_BBBD_4FEE_BD86_5B20DDFE710C__INCLUDED_)
#define AFX_DUPEITEMDLG_H__E63E4AAD_BBBD_4FEE_BD86_5B20DDFE710C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DupeItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDupeItemDlg dialog

#include "HoverButton.h"
#include "Item.h"

class CDupeItemDlg : public CDialog
{
// Construction
public:
	CDupeItemDlg(CWnd* pParent = NULL);   // standard constructor

	void SetItemToDupe(CItem* pItem){ m_pItem = pItem; }
	
	BOOL GetShrinkItem(){ return m_bShrinkItem; }
	BOOL GetCarrySlots(){ return m_bCarrySlots; }
	BOOL GetToBackpack(){ return m_bToBackpack; }
	int GetHowManyDupes(){ return m_nTotalDupes; }

private:
	CItem* m_pItem;
	BOOL m_bShrinkItem;
	BOOL m_bCarrySlots;
	BOOL m_bToBackpack;
	int m_nTotalDupes;

// Dialog Data
	//{{AFX_DATA(CDupeItemDlg)
	enum { IDD = IDD_DUPEITEM_DIALOG };
	CComboBox	m_ComboTo;
	CHoverButton	m_ButtonOk;
	CHoverButton	m_ButtonCancel;
	CSpinButtonCtrl	m_SpinTotal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDupeItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDupeItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUPEITEMDLG_H__E63E4AAD_BBBD_4FEE_BD86_5B20DDFE710C__INCLUDED_)
