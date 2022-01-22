// PicFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "PicFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPicFileDlg

IMPLEMENT_DYNAMIC(CPicFileDlg, CFileDialog)

CPicFileDlg::CPicFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CPicFileDlg, CFileDialog)
	//{{AFX_MSG_MAP(CPicFileDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPicFileDlg::OnFileNameChange(){
	if(IsDlgButtonChecked(IDC_CHECK_SHOW)){
		CString pathname = GetPathName();
		if(pathname.GetLength()){
			BeginWaitCursor();
			if(m_Img.SetImage(pathname)){
				EndWaitCursor();
				return;
			}
		}
		EndWaitCursor();
	}
	m_Img.SetImage(NULL);
}

BOOL CPicFileDlg::OnInitDialog(){
	m_Img.SubclassDlgItem(IDC_STATIC_IMGV,this);
	m_Img.SetEditable(FALSE);
	m_Img.SetWindowPos(NULL,0,0,ITEM_IMAGE_SIZE_X+6,ITEM_IMAGE_SIZE_Y+6,SWP_NOMOVE);
	CheckDlgButton(IDC_CHECK_SHOW,1);
	OnFileNameChange();
	return CFileDialog::OnInitDialog();
}

int CPicFileDlg::DoModal(){
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DIALOG_PICBROWSE);
	m_ofn.Flags |= OFN_ENABLETEMPLATE;
	return CFileDialog::DoModal();
}
