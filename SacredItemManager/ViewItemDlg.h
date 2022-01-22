#if !defined(AFX_VIEWITEMDLG_H__D9935D7C_142C_4384_A44B_A477DEDE04ED__INCLUDED_)
#define AFX_VIEWITEMDLG_H__D9935D7C_142C_4384_A44B_A477DEDE04ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewItemDlg dialog

#include "ItemPicDisp.h"
#include "SITFile.h"
#include "HoverButton.h"

typedef struct _tag_st_attr{
	ULONG id;
	CString txt;
} ST_ATTR,*PST_ATTR;

class CViewItemDlg : public CDialog
{
// Construction
public:
	CViewItemDlg(CWnd* pParent = NULL);   // standard constructor

	void SetSITFile(CSITFile* pSit){ m_pSit = pSit; }

private:
	BOOL ClipboardSetText(LPCTSTR lpszBuffer);
	void LoadAttrsFile(CPtrArray* pArr);
	CString IToS(DWORD dwVal){
		CString t;
		t.Format("%u",dwVal);
		return t;
	}
private:
	CSITFile* m_pSit;
	CPtrArray m_arrAttrs;

// Dialog Data
	//{{AFX_DATA(CViewItemDlg)
	enum { IDD = IDD_VIEWITEM_DIALOG };
	CListCtrl	m_List;
	CHoverButton	m_ButtonCancel;
	CHoverButton	m_ButtonViewDump;
	CItemPicDisp	m_Picture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonViViewdump();
	afx_msg void OnRclickListViAttrlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWITEMDLG_H__D9935D7C_142C_4384_A44B_A477DEDE04ED__INCLUDED_)
