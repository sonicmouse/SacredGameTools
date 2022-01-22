// TabPages.cpp : implementation file
//

#include "stdafx.h"
#include "SacredRAW.h"
#include "TabPages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabPages

CTabPages::CTabPages()
{
}

CTabPages::~CTabPages()
{
}


BEGIN_MESSAGE_MAP(CTabPages, CTabCtrl)
	//{{AFX_MSG_MAP(CTabPages)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabPages message handlers

void CTabPages::AddPage(const UCHAR* pBuffer, int nSize, int nType){

	CString txt;
	txt.Format("%u [0x%X]",nType,nType);

	int nIndex = GetItemCount();
	InsertItem(nIndex,txt);

	CPageDlg* pPage = new CPageDlg(this);
	m_arrPages.Add(pPage);
	pPage->SetPageData(pBuffer,nSize);
	pPage->SetPageType(nType);
	pPage->Create(IDD_PAGE_DIALOG,this);
	pPage->SetParent(this);

	pPage->UpdateWindow();
	pPage->ShowWindow(SW_HIDE);
	
	SetCurSel(0);
	OnSelchange(NULL,NULL);
}

void CTabPages::ClearPages(){
	for(int i = 0; i < m_arrPages.GetSize(); i++){
		CPageDlg* p = (CPageDlg*)m_arrPages.GetAt(i);
		p->DestroyWindow();
		delete p;
	}
	m_arrPages.RemoveAll();
	DeleteAllItems();
}

void CTabPages::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult){

	int nCurSel = GetCurSel();
	if(nCurSel == -1){ return; }

	for(int i = 0; i < m_arrPages.GetSize(); i++){
		CPageDlg* p = (CPageDlg*)m_arrPages.GetAt(i);
		p->ShowWindow(SW_HIDE);
	}

	CPageDlg* pSel = (CPageDlg*)m_arrPages.GetAt(nCurSel);
	pSel->ShowWindow(SW_SHOW);
	
	CRect rc;
	GetClientRect(rc);

	CRect rcItem;
	GetItemRect(nCurSel,rcItem);

	rc.left += 2;
	rc.top += 2;
	rc.right -= 2;
	rc.bottom -= rcItem.Height()*GetRowCount() + GetRowCount() * 3 + 1;
	
	pSel->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOACTIVATE);

	if(pResult){*pResult = 0;}
}

void CTabPages::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	OnSelchange(NULL,NULL);	
}
