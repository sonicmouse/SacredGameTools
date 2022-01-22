#if !defined(AFX_SITFILEDLG_H__57CC4220_A32A_4DBB_8282_EED7C8038020__INCLUDED_)
#define AFX_SITFILEDLG_H__57CC4220_A32A_4DBB_8282_EED7C8038020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SITFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSITFileDlg dialog

#include "ImageViewer.h"

class CSITFileDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CSITFileDlg)

public:
	CSITFileDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

	virtual void OnFileNameChange();
	virtual BOOL OnInitDialog();
	virtual int  DoModal();

	CImageViewer m_Img;

private:
	HWND FindChildByClass(HWND hWndParent, const char* szChildTitle, int skip);

protected:
	//{{AFX_MSG(CSITFileDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonViewDump();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SITFILEDLG_H__57CC4220_A32A_4DBB_8282_EED7C8038020__INCLUDED_)
