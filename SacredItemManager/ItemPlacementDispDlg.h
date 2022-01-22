#if !defined(AFX_ITEMPLACEMENTDISPDLG_H__32D251BE_8823_431D_A688_AFD489166715__INCLUDED_)
#define AFX_ITEMPLACEMENTDISPDLG_H__32D251BE_8823_431D_A688_AFD489166715__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemPlacementDispDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemPlacementDispDlg dialog

#include "ItemPlacementDisp.h"

class CItemPlacementDispDlg : public CDialog
{
// Construction
public:
	CItemPlacementDispDlg(CWnd* pParent = NULL);   // standard constructor

	void SetHero(CHero* pHero);
	void SetSelection(CItem* pItem, int nIndex = -1);
	void DisableDisplays(BOOL bDisable);

// Dialog Data
	//{{AFX_DATA(CItemPlacementDispDlg)
	enum { IDD = IDD_ITEMPLACEMENTDISP_DIALOG };
	CItemPlacementDisp	m_Ch;
	CItemPlacementDisp	m_Bp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemPlacementDispDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemPlacementDispDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMPLACEMENTDISPDLG_H__32D251BE_8823_431D_A688_AFD489166715__INCLUDED_)
