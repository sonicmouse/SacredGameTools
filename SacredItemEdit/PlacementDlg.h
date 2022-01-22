#if !defined(AFX_PLACEMENTDLG_H__A6574DB0_5A5D_4C84_A9D0_80057CDFE29B__INCLUDED_)
#define AFX_PLACEMENTDLG_H__A6574DB0_5A5D_4C84_A9D0_80057CDFE29B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlacementDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlacementDlg dialog

#include "Hero.h"
#include "PlacementDisplay.h"

class CPlacementDlg : public CDialog
{
// Construction
public:
	CPlacementDlg(CWnd* pParent = NULL);   // standard constructor

	void SetHero(CHero* pHero);
	void ClearHero();
	void SetSelectedIndex(ULONG ulIndex);
	void ClearSelectedIndex(ULONG nInv = 0xFFFFFFFF);
	void DisableDisplay(BOOL bDisable, ULONG nInv = 0xFFFFFFFF);

// Dialog Data
	//{{AFX_DATA(CPlacementDlg)
	enum { IDD = IDD_PLACEMENT_DIALOG };
	CPlacementDisplay	m_Ch;
	CPlacementDisplay	m_Bp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlacementDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlacementDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLACEMENTDLG_H__A6574DB0_5A5D_4C84_A9D0_80057CDFE29B__INCLUDED_)
