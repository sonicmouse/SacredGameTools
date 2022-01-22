// SHMDlg.h : header file
//

#if !defined(AFX_SHMDLG_H__124D0D91_4FA9_42B5_BCC6_7A1F954F6936__INCLUDED_)
#define AFX_SHMDLG_H__124D0D91_4FA9_42B5_BCC6_7A1F954F6936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSHMDlg dialog

#include "Plugins.h"

#define SECTION_HEROATAGLANCE 0xC3
#define SECTION_HERO 0xC7
#define HERO_NAME_SIZE (128/2)

class CSHMDlg : public CDialog
{
// Construction
public:
	CSHMDlg(CWnd* pParent = NULL);	// standard constructor

private:
	void ClearHeroBuffer();
	void UpdateWindows();
	void SetHeroName(HHERO hHero);
	CString GetHeroName(unsigned char* lpBuffer);
	void DoPlugins();
	CString BrowseForHero();
	void LoadHero(const CString& szPath);

private:
	unsigned char* m_pHeroSection;
	unsigned long m_ulHeroSectionSize;
	CPlugins m_Plugins;

// Dialog Data
	//{{AFX_DATA(CSHMDlg)
	enum { IDD = IDD_SACREDHEROMANAGER_DIALOG };
	CListCtrl	m_ListPlugins;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSHMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSHMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowsehero();
	afx_msg void OnDestroy();
	afx_msg void OnButtonClosehero();
	afx_msg void OnButtonReloadplugins();
	afx_msg void OnButtonRunplugin();
	afx_msg void OnButtonReloadhero();
	afx_msg void OnDblclkListPlugins(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonUpdatehero();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHMDLG_H__124D0D91_4FA9_42B5_BCC6_7A1F954F6936__INCLUDED_)
