// SRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredRAW.h"
#include "SRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSRDlg dialog

CSRDlg::CSRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSRDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSRDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSRDlg)
	DDX_Control(pDX, IDC_TAB_MAIN, m_Tabs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSRDlg, CDialog)
	//{{AFX_MSG_MAP(CSRDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRDlg message handlers

BOOL CSRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSRDlg::OnButtonBrowse(){

	m_Tabs.ClearPages();
	SetDlgItemText(IDC_EDIT_HERO,"");

	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));

	char fileBuffer[MAX_PATH];
	fileBuffer[0] = 0;

	ofn.lStructSize = sizeof(ofn);
	ofn.hInstance = AfxGetInstanceHandle();
	ofn.hwndOwner = GetSafeHwnd();
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.lpstrFilter = "Sacred Heros (*.PAX)\0*.PAX\0\0";
	ofn.lpstrTitle = "Find a Sacred Hero File...";
	ofn.lpstrFile = fileBuffer;
	ofn.nMaxFile = MAX_PATH;
	
	if(!GetOpenFileName(&ofn)){ return; }

	CSHLib* pShLib = ((CSRApp*)AfxGetApp())->GetSHLib();

	HHERO hHero = pShLib->CreateHero();
	int rc = pShLib->DecompileHero(hHero,fileBuffer);

	if(rc == ERROR_SHLIB_WRONGVERSION || rc == ERROR_SHLIB_FILE){
		if(rc == ERROR_SHLIB_WRONGVERSION){
			MessageBox("This hero file's version is incompadible with this program.","Error...",MB_ICONHAND|MB_OK);
		}else{
			MessageBox("There was a FILE error when trying to work with this file.","Error...",MB_ICONHAND|MB_OK);
		}
		pShLib->DestroyHero(hHero);
		return;
	}

	int nTotalSections = pShLib->GetSectionsList(hHero,NULL);

	ULONG* pSections = new ULONG[nTotalSections];
	pShLib->GetSectionsList(hHero,pSections);

	for(int i = 0; i < nTotalSections; i++){
		int nSectionSize = pShLib->GetHeroSection(hHero,pSections[i],NULL);
		if(nSectionSize != -1){
			UCHAR* pBuf = new UCHAR[nSectionSize];
			if(pBuf){
				pShLib->GetHeroSection(hHero,pSections[i],&pBuf);
				/*
				if(pSections[i]==199){
					FILE* f = fopen("c:\\199.txt","wb");
					if(f){
						fwrite(pBuf,nSectionSize,1,f);
						fclose(f);
					}
				}
				*/
				m_Tabs.AddPage(pBuf,nSectionSize,pSections[i]);
				delete pBuf;
			}
		}
	}

	delete pSections;
	pShLib->DestroyHero(hHero);

	SetDlgItemText(IDC_EDIT_HERO,fileBuffer);
}

void CSRDlg::OnDestroy(){
	m_Tabs.ClearPages();
	CDialog::OnDestroy();	
}

void CSRDlg::OnSize(UINT nType, int cx, int cy){
	CDialog::OnSize(nType, cx, cy);
	if(!m_Tabs.GetSafeHwnd() || !::IsWindow(m_Tabs.GetSafeHwnd())){ return; }

	CWnd* but = GetDlgItem(IDC_BUTTON_BROWSE);
	but->SetWindowPos(NULL,cx-55,5,50,20,SWP_NOZORDER|SWP_NOACTIVATE);

	CWnd* ed = GetDlgItem(IDC_EDIT_HERO);
	ed->SetWindowPos(NULL,5,5,cx-65,20,SWP_NOZORDER|SWP_NOACTIVATE);

	CRect rc;
	ed->GetClientRect(rc);

	m_Tabs.SetWindowPos(NULL,5,rc.bottom+15,cx-10,cy-rc.bottom-20,SWP_NOZORDER|SWP_NOACTIVATE);
}

LRESULT CSRDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message==WM_GETMINMAXINFO){
		MINMAXINFO* pMMI = (MINMAXINFO*)lParam;
		pMMI->ptMinTrackSize.x = 396;
		pMMI->ptMinTrackSize.y = 300;
		return 0;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
