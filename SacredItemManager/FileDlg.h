// FileDlg.h: interface for the CFileDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __FILE_DIALOG_H__
#define __FILE_DIALOG_H__

class CFileDlg  
{
public:
	CFileDlg(BOOL bOpen,
			 const CString& szTitle,
			 HWND hWndOwner = NULL,
			 const CString& szFilter = "All Files (*.*)|*.*||",
			 const CString& szDefaultExt = "",
			 const CString& szDefaultFileTitle = "");
	virtual ~CFileDlg();

public:
	CString GetResult();
	char* ConvertFilter();

private:
	BOOL m_bOpen;
	CString m_szTitle;
	HWND m_hWndOwner;
	CString m_szFilter;
	CString m_szExt;
	CString m_szFileTitle;
};

#endif /* __FILE_DIALOG_H__ */
