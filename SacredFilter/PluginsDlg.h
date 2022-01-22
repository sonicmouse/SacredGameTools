#if !defined(AFX_PLUGINSDLG_H__F9C20600_BFCE_41F8_B16E_FF705983151C__INCLUDED_)
#define AFX_PLUGINSDLG_H__F9C20600_BFCE_41F8_B16E_FF705983151C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluginsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPluginsDlg dialog

class CPluginsDlg : public CDialog
{
// Construction
public:
	CPluginsDlg(CWnd* pParent = NULL);   // standard constructor

private:


// Dialog Data
	//{{AFX_DATA(CPluginsDlg)
	enum { IDD = IDD_PLUGINS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPluginsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPluginsDlg)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLUGINSDLG_H__F9C20600_BFCE_41F8_B16E_FF705983151C__INCLUDED_)
