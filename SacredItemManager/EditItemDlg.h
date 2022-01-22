#if !defined(AFX_EDITITEMDLG_H__3CA5F025_FB74_4B98_A73C_FB319F9F9562__INCLUDED_)
#define AFX_EDITITEMDLG_H__3CA5F025_FB74_4B98_A73C_FB319F9F9562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditItemDlg dialog

#include "HoverButton.h"
#include "PPDumpCtrl.h"
#include "Item.h"

class CEditItemDlg : public CDialog
{
// Construction
public:
	CEditItemDlg(CWnd* pParent = NULL);   // standard constructor

	void SetItem(CItem* pItem){
		m_pOrigItem = pItem;
	}

private:
	BOOL ClipboardSetText(LPCTSTR lpszBuffer);
	void UpdateConvertList();
	void ReverseBuffer(PST_BUFFER src, PST_BUFFER dst);
	CString SToS(int nVal){
		CString retBuff;
		retBuff.Format("%i", nVal);
		return retBuff;
	}
	CString UToS(ULONG ulVal){
		CString retBuff;
		retBuff.Format("%u", ulVal);
		return retBuff;
	}
	BOOL IsNumber(const CString& szVal);
private:
	CPPDumpCtrl* m_pHexCtrl;
	CItem* m_pOrigItem;
	CItem m_NewItem;
	CItem m_OldItem;
	ST_BUFFER m_LastSel;
	CFont m_ConvertFont;

// Dialog Data
	//{{AFX_DATA(CEditItemDlg)
	enum { IDD = IDD_EDITITEM_DIALOG };
	CHoverButton	m_ButtonHelper;
	CComboBox	m_ComboBo;
	CListCtrl	m_List;
	CHoverButton	m_ButtonOK;
	CHoverButton	m_ButtonCancel;
	CHoverButton	m_ButtonReset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonResetBuffer();
	afx_msg void OnSelchangeComboByteorder();
	afx_msg void OnCheckDecimalDataView();
	virtual void OnOK();
	afx_msg void OnEdititemCopy();
	afx_msg void OnRclickListTranslations(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonHelper();
	//}}AFX_MSG
	afx_msg void NotifyEditDataHexEditCtrl(NMHDR * pNMHDR, LRESULT * result);
	afx_msg void NotifySelectDataHexEditCtrl(NMHDR * pNMHDR, LRESULT * result);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITITEMDLG_H__3CA5F025_FB74_4B98_A73C_FB319F9F9562__INCLUDED_)
