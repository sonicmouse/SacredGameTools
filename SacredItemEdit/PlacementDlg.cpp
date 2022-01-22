// PlacementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "PlacementDlg.h"
#include "SIEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlacementDlg dialog


CPlacementDlg::CPlacementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlacementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlacementDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPlacementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlacementDlg)
	DDX_Control(pDX, IDC_STATIC_CH, m_Ch);
	DDX_Control(pDX, IDC_STATIC_BP, m_Bp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlacementDlg, CDialog)
	//{{AFX_MSG_MAP(CPlacementDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlacementDlg message handlers

void CPlacementDlg::OnOK(){}

void CPlacementDlg::OnCancel(){
	ShowWindow(SW_HIDE);
}

BOOL CPlacementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSIEDlg* pDlg = (CSIEDlg*)AfxGetMainWnd();

	SetIcon(pDlg->GetMyIcon(), TRUE);
	SetIcon(pDlg->GetMyIcon(), FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlacementDlg::SetHero(CHero* pHero){
	m_Bp.SetDetails(pHero,SECTION_PLACEMENT_BP);
	m_Ch.SetDetails(pHero,SECTION_PLACEMENT_CH);
}

void CPlacementDlg::ClearHero(){
	m_Bp.SetDetails(NULL,SECTION_PLACEMENT_BP);
	m_Ch.SetDetails(NULL,SECTION_PLACEMENT_CH);
}

void CPlacementDlg::OnSize(UINT nType, int cx, int cy) 
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

LRESULT CPlacementDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message==WM_GETMINMAXINFO){
		MINMAXINFO* pMMI = (MINMAXINFO*)lParam;
		pMMI->ptMinTrackSize.x = 361;
		pMMI->ptMinTrackSize.y = 261;
		return 0;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CPlacementDlg::SetSelectedIndex(ULONG ulIndex){
	m_Ch.SetSelectedItem(ulIndex);
	m_Bp.SetSelectedItem(ulIndex);
}

void CPlacementDlg::ClearSelectedIndex(ULONG nInv /*= 0xFFFFFFFF*/){
	if(m_Ch.GetInvType()==nInv){
		m_Ch.ClearSelection();
	}else if(m_Bp.GetInvType()==nInv){
		m_Bp.ClearSelection();
	}else if(nInv==0xFFFFFFFF){
		m_Ch.ClearSelection();
		m_Bp.ClearSelection();
	}
}

void CPlacementDlg::DisableDisplay(BOOL bDisable, ULONG nInv/* = 0xFFFFFFFF*/){
	if(m_Ch.GetInvType()==nInv){
		m_Ch.Disable(bDisable);
	}else if(m_Bp.GetInvType()==nInv){
		m_Bp.Disable(bDisable);
	}else if(nInv==0xFFFFFFFF){
		m_Ch.Disable(bDisable);
		m_Bp.Disable(bDisable);
	}
}
