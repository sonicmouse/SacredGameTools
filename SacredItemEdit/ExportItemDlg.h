#if !defined(AFX_EXPORTITEMDLG_H__036949AB_1553_4ADA_A621_FD1077CFCE06__INCLUDED_)
#define AFX_EXPORTITEMDLG_H__036949AB_1553_4ADA_A621_FD1077CFCE06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportItemDlg dialog

#include "ImageViewer.h"

class CExportItemDlg : public CDialog
{
// Construction
public:
	CExportItemDlg(CWnd* pParent = NULL);   // standard constructor

	void SetItemBuffer(PST_BUFFER pBuffer){ m_pBuffer = pBuffer; }
	void SetItemInvSize(CSize sz){ m_invSize = sz; }

private:
	PST_BUFFER m_pBuffer;
	CSize m_invSize;

// Dialog Data
	//{{AFX_DATA(CExportItemDlg)
	enum { IDD = IDD_EXPORTITEM_DIALOG };
	CImageViewer	m_Picture;
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

#endif // !defined(AFX_EXPORTITEMDLG_H__036949AB_1553_4ADA_A621_FD1077CFCE06__INCLUDED_)
