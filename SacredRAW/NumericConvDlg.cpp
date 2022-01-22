// NumericConvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredRAW.h"
#include "NumericConvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumericConvDlg dialog


CNumericConvDlg::CNumericConvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumericConvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNumericConvDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNumericConvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNumericConvDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNumericConvDlg, CDialog)
	//{{AFX_MSG_MAP(CNumericConvDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumericConvDlg message handlers
