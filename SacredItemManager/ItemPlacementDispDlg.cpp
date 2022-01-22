// ItemPlacementDispDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "ItemPlacementDispDlg.h"
#include "SIMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemPlacementDispDlg dialog


CItemPlacementDispDlg::CItemPlacementDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemPlacementDispDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemPlacementDispDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CItemPlacementDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemPlacementDispDlg)
	DDX_Control(pDX, IDC_PLACEMENT_CHEST, m_Ch);
	DDX_Control(pDX, IDC_PLACEMENT_BACKPACK, m_Bp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemPlacementDispDlg, CDialog)
	//{{AFX_MSG_MAP(CItemPlacementDispDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemPlacementDispDlg message handlers

void CItemPlacementDispDlg::OnCancel(){
	ShowWindow(SW_HIDE);
}
void CItemPlacementDispDlg::OnOK(){}

LRESULT CItemPlacementDispDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message==WM_GETMINMAXINFO){
		MINMAXINFO* pMMI = (MINMAXINFO*)lParam;
		pMMI->ptMinTrackSize.x = 321; //361
		pMMI->ptMinTrackSize.y = 261;
		return 0;
	}	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CItemPlacementDispDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if(!m_Bp.m_hWnd || !::IsWindow(m_Bp.m_hWnd)){
		return;
	}

	m_Ch.SetWindowPos(NULL,5,5,cx-10,cy/2-5,SWP_NOACTIVATE|SWP_NOZORDER);
	m_Bp.SetWindowPos(NULL,5,cy/2+5,cx-10,cy-cy/2-10,SWP_NOACTIVATE|SWP_NOZORDER);

	m_Bp.Repaint();
	m_Ch.Repaint();	
}

void CItemPlacementDispDlg::SetHero(CHero* pHero){
	m_Bp.SetSelItem(NULL);
	m_Bp.SetHero(pHero);

	m_Ch.SetSelItem(NULL);
	m_Ch.SetHero(pHero);
}

void CItemPlacementDispDlg::SetSelection(CItem* pItem, int nIndex/* = -1*/){
	m_Bp.SetSelItem(pItem,nIndex);
	m_Ch.SetSelItem(pItem,nIndex);
}

void CItemPlacementDispDlg::DisableDisplays(BOOL bDisable){
	m_Bp.SetDisabled(bDisable);
	m_Ch.SetDisabled(bDisable);
}

BOOL CItemPlacementDispDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CSIMDlg* pDlg = (CSIMDlg*)AfxGetMainWnd();

	SetIcon(pDlg->GetMyIcon(),TRUE);
	SetIcon(pDlg->GetMyIcon(),FALSE);

	m_Ch.SetInvType(SECTION_PLACEMENT_CH);
	m_Bp.SetInvType(SECTION_PLACEMENT_BP);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

