#if !defined(AFX_ITEMEDITHELPERDLG_H__F9952DA5_92D3_4175_B3DC_F0810033B894__INCLUDED_)
#define AFX_ITEMEDITHELPERDLG_H__F9952DA5_92D3_4175_B3DC_F0810033B894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemEditHelperDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemEditHelperDlg dialog

#include "item.h"

class CItemEditHelperDlg : public CDialog
{
// Construction
public:
	CItemEditHelperDlg(CWnd* pParent = NULL);   // standard constructor

	void SetItem(CItem* pItem){ m_pItem = pItem; }

private:
	void LoadItemAttributesData();
	BOOL CurrentAttrDiffrent();

private:
	CItem* m_pItem;

// Dialog Data
	//{{AFX_DATA(CItemEditHelperDlg)
	enum { IDD = IDD_ITEMEDITHELPER_DIALOG };
	CSpinButtonCtrl	m_SpinAttr;
	CComboBox	m_ComboSlots;
	CComboBox	m_ComboAttrNum;
	CComboBox	m_ComboAttr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemEditHelperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemEditHelperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboAttribute();
	afx_msg void OnSelchangeComboItemAttrs();
	afx_msg void OnChangeEditAttrId();
	afx_msg void OnRadioAttrNum();
	afx_msg void OnRadioAttrText();
	afx_msg void OnButtonApplyAttrChange();
	afx_msg void OnSelchangeComboSlots();
	afx_msg void OnRadioSlotNone();
	afx_msg void OnRadioSlotBronze();
	afx_msg void OnRadioSlotGold();
	afx_msg void OnRadioSlotGreen();
	afx_msg void OnRadioSlotPlat();
	afx_msg void OnRadioSlotSilver();
	afx_msg void OnButtonReloadattrdb();
	afx_msg void OnButtonEditattrdb();
	afx_msg void OnChangeEditAttrValue();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMEDITHELPERDLG_H__F9952DA5_92D3_4175_B3DC_F0810033B894__INCLUDED_)
