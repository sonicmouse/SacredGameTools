#if !defined(AFX_EXPORTITEMDLG_H__7FD2585F_F683_41C9_9A90_9BB1BE14714B__INCLUDED_)
#define AFX_EXPORTITEMDLG_H__7FD2585F_F683_41C9_9A90_9BB1BE14714B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportItemDlg dialog

#include "HoverButton.h"
#include "Item.h"
#include "ItemPicDisp.h"

class CExportItemDlg : public CDialog
{
// Construction
public:
	CExportItemDlg(CWnd* pParent = NULL);   // standard constructor

	void SetItem(CItem* pItem){ m_pItem = pItem; }

private:
	CItem* m_pItem;

// Dialog Data
	//{{AFX_DATA(CExportItemDlg)
	enum { IDD = IDD_EXPORTITEM_DIALOG };
	CItemPicDisp	m_Picture;
	CHoverButton	m_ButtonOk;
	CHoverButton	m_ButtonCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTITEMDLG_H__7FD2585F_F683_41C9_9A90_9BB1BE14714B__INCLUDED_)
