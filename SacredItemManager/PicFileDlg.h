#ifndef __PICTURE_FILE_DLG_H__
#define __PICTURE_FILE_DLG_H__

/////////////////////////////////////////////////////////////////////////////
// CPicFileDlg dialog

#include "ItemPicDisp.h"

class CPicFileDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CPicFileDlg)

public:
	CPicFileDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

	virtual void OnFileNameChange();
	virtual BOOL OnInitDialog();
	virtual int  DoModal();

	CItemPicDisp m_Img;

protected:
	//{{AFX_MSG(CPicFileDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* __PICTURE_FILE_DLG_H__ */
