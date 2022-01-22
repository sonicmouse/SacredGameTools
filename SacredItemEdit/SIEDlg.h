// SIEDlg.h : header file
//

#if !defined(AFX_SIEDLG_H__994801D6_09F1_4FCB_95E6_44B1E96DDE65__INCLUDED_)
#define AFX_SIEDLG_H__994801D6_09F1_4FCB_95E6_44B1E96DDE65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSIEDlg dialog

#include "PlacementDlg.h"
#include "TreeCtrlEx.h"
#include "Hero.h"

class CSIEDlg : public CDialog
{
// Construction
public:
	CSIEDlg(CWnd* pParent = NULL);	// standard constructor

	void OnPlacementSelectionChange(int invType, ULONG itemIndex);
	HICON GetMyIcon(){ return m_hIcon; }
	void UpdateWindows();

private:
	void RefreshList();
	BOOL GetSelectedItem(ST_ITEM_DATA** pData, ST_ITEM_HEADER** pHeader);
	BOOL GetSelectedItem(int& nItemIndex, int& nSlotIndex);
	BOOL GetSelectedItem(ULONG& ulItemId, int& nSlotIndex);
	BOOL ReloadHero(BOOL bQuiet = FALSE);
	void SelectItemByItemId(ULONG itemId);
	void ClearItemSelection();

private:
	CHero* m_pHero;
	CPlacementDlg* m_pPlacementDlg;
	CFont m_fontBold;

// Dialog Data
	//{{AFX_DATA(CSIEDlg)
	enum { IDD = IDD_SACREDITEMEDIT_DIALOG };
	CTreeCtrlEx	m_Tree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSIEDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowse();
	afx_msg void OnDestroy();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonUpdate();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonExport();
	afx_msg void OnButtonAbout();
	afx_msg void OnButtonGrid();
	afx_msg void OnSelchangedTreeItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonClose();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonDup();
	afx_msg void OnButtonCompress();
	afx_msg void OnButtonUnslot();
	afx_msg void OnButtonImportslot();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIEDLG_H__994801D6_09F1_4FCB_95E6_44B1E96DDE65__INCLUDED_)
