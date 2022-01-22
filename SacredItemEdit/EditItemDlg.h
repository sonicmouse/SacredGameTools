#if !defined(AFX_EDITITEMDLG_H__C4CEFA7E_0F0C_4197_B56F_4942EE4AE922__INCLUDED_)
#define AFX_EDITITEMDLG_H__C4CEFA7E_0F0C_4197_B56F_4942EE4AE922__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditItemDlg dialog

#include "PPDumpCtrl.h"

#define VIEW_DATA_ONLY 0
#define VIEW_HEADER_ONLY 1
#define VIEW_HEADER_AND_DATA 2

class CEditItemDlg : public CDialog
{
// Construction
public:
	CEditItemDlg(CWnd* pParent = NULL);   // standard constructor

	void SetItemDetails(PST_ITEM_DATA pData, PST_ITEM_HEADER pHeader){
		m_pOrigData = pData;
		m_pOrigHeader = pHeader;
	}

private:
	WORD ReverseBytes(WORD val);
	DWORD ReverseBytes(DWORD val);
	void UpdateConvertList();
	BOOL ClipboardSetText(LPCTSTR lpszBuffer);

private:
	CPPDumpCtrl* m_pHexCtrl;
	CFont m_ToolTipFont;
	CFont m_ConvertFont;

	PST_ITEM_DATA m_pOrigData;
	PST_ITEM_HEADER m_pOrigHeader;

	UCHAR* m_pOldData;
	UCHAR* m_pNewData;
	ULONG m_ulDataSize;
	UCHAR* m_pEditOffsetNew;

	ST_BUFFER m_LastSel;

// Dialog Data
	//{{AFX_DATA(CEditItemDlg)
	enum { IDD = IDD_EDITITEM_DIALOG };
	CComboBox	m_ComboView;
	CComboBox	m_ComboBo;
	CListCtrl	m_Convert;
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
	afx_msg void OnSelchangeComboByteorder();
	afx_msg void OnCheckDec();
	afx_msg void OnSelchangeComboView();
	afx_msg void OnButtonReset();
	afx_msg void OnUpdateCopy(CCmdUI* pCmdUI);
	afx_msg void OnCopy();
	afx_msg void OnRclickListConvert(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnButtonImHeader();
	afx_msg void OnButtonImData();
	afx_msg void OnButtonExHeader();
	afx_msg void OnButtonExData();
	//}}AFX_MSG
	afx_msg void NotifyEditDataHexEditCtrl(NMHDR * pNMHDR, LRESULT * result);
	afx_msg void NotifySelectDataHexEditCtrl(NMHDR * pNMHDR, LRESULT * result);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITITEMDLG_H__C4CEFA7E_0F0C_4197_B56F_4942EE4AE922__INCLUDED_)
