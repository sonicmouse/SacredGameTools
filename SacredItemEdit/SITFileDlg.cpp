// SITFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "SITFileDlg.h"
#include "SITFile.h"
#include "DumpViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSITFileDlg

IMPLEMENT_DYNAMIC(CSITFileDlg, CFileDialog)

CSITFileDlg::CSITFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CSITFileDlg, CFileDialog)
	//{{AFX_MSG_MAP(CSITFileDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND(IDC_BUTTON_VIEWDUMP, OnButtonViewDump)
END_MESSAGE_MAP()

void CSITFileDlg::OnFileNameChange(){
	CString pathname = GetPathName();
	if(pathname.GetLength()){
		BeginWaitCursor();
		CSITFile sit;
		if(sit.SetSITFile(pathname)){
			m_Img.SetImageData(sit.GetItemPicture());
			SetDlgItemText(IDC_EDIT_NAMEZ, sit.GetItemName());
			SetDlgItemText(IDC_EDIT_DESCR, sit.GetItemDescription());
			SetDlgItemText(IDC_EDIT_AUTHZ, sit.GetItemAuthor());
			SetDlgItemInt(IDC_EDIT_IDZ,*(ULONG*)sit.GetItem()->buf,FALSE);
			GetDlgItem(IDC_BUTTON_VIEWDUMP)->EnableWindow(TRUE);
			CString invSize; invSize.Format("%ux%u", sit.GetItemInvSize().cx, sit.GetItemInvSize().cy);
			SetDlgItemText(IDC_STATIC_INVSIZEZ,invSize);
			EndWaitCursor();
			return;
		}
		EndWaitCursor();
	}
	
	m_Img.DestroyImageData();
	SetDlgItemText(IDC_EDIT_NAMEZ, "");
	SetDlgItemText(IDC_EDIT_DESCR, "");
	SetDlgItemText(IDC_EDIT_AUTHZ, "");
	SetDlgItemText(IDC_EDIT_IDZ,"");
	SetDlgItemText(IDC_STATIC_INVSIZEZ,"");
	GetDlgItem(IDC_BUTTON_VIEWDUMP)->EnableWindow(FALSE);
}

BOOL CSITFileDlg::OnInitDialog(){
	m_Img.SubclassDlgItem(IDC_STATIC_IMG,this);
	m_Img.SetEditable(FALSE);
	m_Img.SetWindowPos(NULL,0,0,ITEM_IMAGE_SIZE_X+6,ITEM_IMAGE_SIZE_Y+6,SWP_NOMOVE);
	OnFileNameChange();
	return CFileDialog::OnInitDialog();
}

int CSITFileDlg::DoModal(){
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DIALOG_SITBROWSE);
	m_ofn.Flags |= OFN_ENABLETEMPLATE;
	return CFileDialog::DoModal();
}

void CSITFileDlg::OnButtonViewDump(){
	CString pathname = GetPathName();
	if(pathname.GetLength()){
		BeginWaitCursor();
		CSITFile sit;
		if(sit.SetSITFile(pathname)){
			EndWaitCursor();
			CDumpViewerDlg dlg(this);
			dlg.SetDump(sit.GetItem());
			dlg.DoModal();
		}
	}
}

void CSITFileDlg::OnSize(UINT nType, int cx, int cy){
	CFileDialog::OnSize(nType, cx, cy);

	if(!m_Img.m_hWnd || !::IsWindow(m_Img.m_hWnd)){ return; }

	// get the cancel button
	CRect canRc, wndRc;
	GetParent()->GetWindowRect(wndRc);
	CWnd* canButton = GetParent()->GetDlgItem(IDCANCEL);
	canButton->GetWindowRect(canRc);
	canRc.left -= wndRc.left;
	canRc.top -= wndRc.top;
	// place view button based on cancel button
	GetDlgItem(IDC_BUTTON_VIEWDUMP)->SetWindowPos(NULL,canRc.left-4,canRc.top+6,0,0,SWP_NOSIZE);

	CRect editRc;
	HWND hWndEdit = FindChildByClass(GetParent()->GetSafeHwnd(),"Edit",0);
	::GetWindowRect(hWndEdit,editRc);

	GetParent()->GetDlgItem(IDCANCEL)->GetWindowRect(canRc);

	CRect t;
	GetDlgItem(IDC_EDIT_IDZ)->GetWindowRect(t);
	GetDlgItem(IDC_EDIT_IDZ)->SetWindowPos(NULL,t.left,t.top,editRc.right-t.left,t.Height(), SWP_NOMOVE);

	GetDlgItem(IDC_EDIT_NAMEZ)->GetWindowRect(t);
	GetDlgItem(IDC_EDIT_NAMEZ)->SetWindowPos(NULL,t.left,t.top,canRc.right-t.left,t.Height(), SWP_NOMOVE);

	GetDlgItem(IDC_EDIT_DESCR)->GetWindowRect(t);
	GetDlgItem(IDC_EDIT_DESCR)->SetWindowPos(NULL,t.left,t.top,canRc.right-t.left,t.Height(), SWP_NOMOVE);

	GetDlgItem(IDC_EDIT_AUTHZ)->GetWindowRect(t);
	GetDlgItem(IDC_EDIT_AUTHZ)->SetWindowPos(NULL,t.left,t.top,canRc.right-t.left,t.Height(), SWP_NOMOVE);

}

HWND CSITFileDlg::FindChildByClass(HWND hWndParent, const char* szChildTitle, int skip){
	int count = 0;
	char temp[MAX_PATH];
	memset(temp, 0, MAX_PATH);
	if(!skip){skip = 1;}
	HWND firs = ::GetWindow((HWND)hWndParent, GW_CHILD);
	while(firs){
		::GetClassName(firs, temp, MAX_PATH);
		// stupid fix - strstr() will return nonzero if one param is blank and the other isn't
		if((!strlen(temp)) || (!strlen(szChildTitle))){
			if(strlen(temp) == strlen(szChildTitle))
				if(++count == skip)
					return firs;		
		}else{ 
			if(strstr(temp, szChildTitle)){
				if(++count == skip)
					return firs;
			}
		}
		firs = ::GetWindow(firs, GW_HWNDNEXT);
	}
	return NULL;
}
