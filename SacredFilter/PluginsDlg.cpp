// PluginsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredFilter.h"
#include "PluginsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPluginsDlg dialog


CPluginsDlg::CPluginsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPluginsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluginsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPluginsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPluginsDlg, CDialog)
	//{{AFX_MSG_MAP(CPluginsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluginsDlg message handlers

//
//
//
void CPluginsDlg::OnCancel(){ ShowWindow(SW_HIDE); }
void CPluginsDlg::OnOK(){ ShowWindow(SW_HIDE); }

