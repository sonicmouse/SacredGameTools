// SIMDlg.h : header file
//

#ifndef __SIM_DLG_H__
#define __SIM_DLG_H__

/////////////////////////////////////////////////////////////////////////////
// CSIMDlg dialog

#include "TreeCtrlEx.h"
#include "Hero.h"
#include "ItemPlacementDispDlg.h"
#include "HoverButton.h"

class CSIMDlg : public CDialog
{
// Construction
public:
	CSIMDlg(CWnd* pParent = NULL);	// standard constructor

	CSIMApp* GetApp(){ return ((CSIMApp*)::AfxGetApp()); }
	CFont* GetFontBold(){ return &m_fontBold; }
	HICON GetMyIcon(){ return m_hIcon; }
	CHero* GetHero(){ return m_pHero; }

	CString IToS(ULONG ulVal){ CString buf; buf.Format("%u", ulVal); return buf; }

	void SelectItemById(ULONG ulItemId, int nSlotIndex = -1);
	BOOL GetCurrentSelection(CItem** lpItemMain, CItem** lpItemSlotted);
	BOOL GetCurrentSelection(CItem** lpItemMain, int* lpnIndexSlotted);
	void RefreshList();
	void UpdateWindows();
	void ClickViewButton(){ OnButtonViewitem(); }

private:
	CFont m_fontBold;
	CHero* m_pHero;
	CItemPlacementDispDlg* m_pItemDisp;

// Dialog Data
	//{{AFX_DATA(CSIMDlg)
	enum { IDD = IDD_SACREDITEMMANAGER_DIALOG };
	CHoverButton	m_ButtonEditId;
	CHoverButton	m_ButtonView;
	CHoverButton	m_ButtonImport;
	CHoverButton	m_ButtonExport;
	CHoverButton	m_ButtonEditItem;
	CHoverButton	m_ButtonUnslot;
	CHoverButton	m_ButtonDelete;
	CHoverButton	m_ButtonShrink;
	CHoverButton	m_ButtonHelp;
	CHoverButton	m_ButtonBrowse;
	CHoverButton	m_ButtonDupeitem;
	CHoverButton	m_ButtonUpdateCurrent;
	CHoverButton	m_ButtonReloadCurrent;
	CHoverButton	m_ButtonCloseCurrent;
	CHoverButton	m_ButtonExit;
	CHoverButton	m_ButtonAbout;
	CHoverButton	m_ButtonPlacement;
	CTreeCtrlEx	m_TreeItems;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSIMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonBrowseCurrentHero();
	afx_msg void OnButtonCloseCurrentHero();
	afx_msg void OnSelchangedTreeItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSaveCurrentHero();
	afx_msg void OnButtonPlacement();
	afx_msg void OnButtonReloadCurrentHero();
	afx_msg void OnButtonDuplicateItem();
	afx_msg void OnButtonShrinkitem();
	afx_msg void OnButtonDeleteitem();
	afx_msg void OnButtonUnslotItem();
	afx_msg void OnButtonEditItem();
	afx_msg void OnButtonExportItem();
	afx_msg void OnButtonImportItem();
	afx_msg void OnButtonHelp();
	afx_msg void OnButtonAbout();
	virtual void OnCancel();
	afx_msg void OnButtonViewitem();
	afx_msg void OnDblclkTreeItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonEditDescId();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* __SIM_DLG_H__ */
