// SSDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredSugarDaddy.h"
#include "SSDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSDDlg dialog

CSSDDlg::CSSDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSSDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSSDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSSDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSDDlg)
	DDX_Control(pDX, IDC_SPIN_GOLD, m_SpinGold);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSSDDlg, CDialog)
	//{{AFX_MSG_MAP(CSSDDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_STATIC_MAXGOLD, OnStaticMaxgold)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDDlg message handlers

BOOL CSSDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	m_SpinGold.SetBuddy(GetDlgItem(IDC_EDIT_GOLD));
	m_SpinGold.SetRange32(0,0x7FFFFFFF);

	SetDlgItemInt(IDC_EDIT_GOLD,0,TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//
// browse for a hero
//
void CSSDDlg::OnButtonBrowse(){

	CSSDApp* pApp = (CSSDApp*)AfxGetApp();

	SetDlgItemText(IDC_EDIT_HERO,"");
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_GOLD)->EnableWindow(FALSE);
	SetDlgItemInt(IDC_EDIT_GOLD,0,TRUE);

	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));

	char fileBuffer[MAX_PATH];
	fileBuffer[0] = 0;

	ofn.lStructSize = sizeof(ofn);
	ofn.hInstance = AfxGetInstanceHandle();
	ofn.hwndOwner = GetSafeHwnd();
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.lpstrFilter = "Sacred Heros (*.PAX)\0*.PAX\0\0";
	ofn.lpstrTitle = "Find a poor Sacred Hero...";
	ofn.lpstrFile = fileBuffer;
	ofn.nMaxFile = MAX_PATH;
	
	if(!GetOpenFileName(&ofn)){ return; }

	HHERO hHero = pApp->GetSHLib()->CreateHero();
	int rc = pApp->GetSHLib()->DecompileHero(hHero,fileBuffer);

	switch(rc){
		case ERROR_SHLIB_WRONGVERSION:
			pApp->GetSHLib()->DestroyHero(hHero);
			MessageBox("It seems that this hero file is the wrong version.\nMake sure you are using Sacred Plus.","Error...",MB_OK|MB_ICONHAND);
			return;
		case ERROR_SHLIB_FILE:
			pApp->GetSHLib()->DestroyHero(hHero);
			MessageBox("I have encountered a file IO error.\nMake sure this file isn't already in use.","Error...",MB_OK|MB_ICONHAND);
			return;
		case ERROR_SHLIB_SUCCESS:
			break;
	}

	// lets get all the sections out of the hero file...
	int nCountSections = pApp->GetSHLib()->GetSectionsList(hHero,NULL);
	ULONG* pSections = new ULONG[nCountSections];
	pApp->GetSHLib()->GetSectionsList(hHero,pSections);

	UCHAR* pTargetSection = NULL;
	ULONG ulTargetSectionLength = 0;
	for(int i = 0; i < nCountSections; i++){
		if(pSections[i]==TARGET_SECTION){
			// this is our section :D
			int nSectionSize = pApp->GetSHLib()->GetHeroSection(hHero,TARGET_SECTION,NULL);
			if(nSectionSize != -1){
				pTargetSection = new UCHAR[nSectionSize];
				if(pTargetSection){
					ulTargetSectionLength = nSectionSize;
					pApp->GetSHLib()->GetHeroSection(hHero,TARGET_SECTION,&pTargetSection);
				}
			}
			break;
		}
	}
	delete pSections;

	if(!pTargetSection || !ulTargetSectionLength){
		pApp->GetSHLib()->DestroyHero(hHero);
		return;
	}

	SetDlgItemInt(IDC_EDIT_GOLD,*(ULONG*)&pTargetSection[GOLD_OFFSET],TRUE);

	delete pTargetSection;

	SetDlgItemText(IDC_EDIT_HERO,fileBuffer);
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_GOLD)->EnableWindow(TRUE);

	pApp->GetSHLib()->DestroyHero(hHero);
}

void CSSDDlg::OnDestroy(){
	DestroyIcon(m_hIcon);
	CDialog::OnDestroy();	
}

void CSSDDlg::OnStaticMaxgold() 
{
	if(::IsWindowEnabled(GetDlgItem(IDC_EDIT_GOLD)->GetSafeHwnd())){
		SetDlgItemInt(IDC_EDIT_GOLD,0x7FFFFFFF);
	}
}

void CSSDDlg::OnButtonUpdate(){

	CSSDApp* pApp = (CSSDApp*)AfxGetApp();

	char fileBuffer[MAX_PATH];
	GetDlgItemText(IDC_EDIT_HERO,fileBuffer,MAX_PATH);

	HHERO hHero = pApp->GetSHLib()->CreateHero();
	int rc = pApp->GetSHLib()->DecompileHero(hHero,fileBuffer);

	switch(rc){
		case ERROR_SHLIB_WRONGVERSION:
			pApp->GetSHLib()->DestroyHero(hHero);
			MessageBox("It seems that this hero file is the wrong version.\nMake sure you are using Sacred Plus.","Error...",MB_OK|MB_ICONHAND);
			return;
		case ERROR_SHLIB_FILE:
			pApp->GetSHLib()->DestroyHero(hHero);
			MessageBox("I have encountered a file IO error.\nMake sure this file isn't already in use.","Error...",MB_OK|MB_ICONHAND);
			return;
		case ERROR_SHLIB_SUCCESS:
			break;
	}

	// lets get all the sections out of the hero file...
	int nCountSections = pApp->GetSHLib()->GetSectionsList(hHero,NULL);
	ULONG* pSections = new ULONG[nCountSections];
	pApp->GetSHLib()->GetSectionsList(hHero,pSections);

	UCHAR* pTargetSection = NULL;
	ULONG ulTargetSectionLength = 0;
	for(int i = 0; i < nCountSections; i++){
		if(pSections[i]==TARGET_SECTION){
			// this is our section :D
			int nSectionSize = pApp->GetSHLib()->GetHeroSection(hHero,TARGET_SECTION,NULL);
			if(nSectionSize != -1){
				pTargetSection = new UCHAR[nSectionSize];
				if(pTargetSection){
					ulTargetSectionLength = nSectionSize;
					pApp->GetSHLib()->GetHeroSection(hHero,TARGET_SECTION,&pTargetSection);
				}
			}
			break;
		}
	}
	delete pSections;

	if(!pTargetSection || !ulTargetSectionLength){
		pApp->GetSHLib()->DestroyHero(hHero);
		return;
	}

	*(ULONG*)&pTargetSection[GOLD_OFFSET] = GetDlgItemInt(IDC_EDIT_GOLD);

	pApp->GetSHLib()->SetHeroSection(hHero,TARGET_SECTION,pTargetSection,int(ulTargetSectionLength));
	if(pApp->GetSHLib()->CompileHero(hHero,fileBuffer)){
		MessageBox("Hero is no longer one poor bastard!","Success...",MB_ICONINFORMATION|MB_OK);
	}else{
		MessageBox("Something went severely wrong... nothing has changed.","Error...",MB_ICONHAND|MB_OK);
	}

	pApp->GetSHLib()->DestroyHero(hHero);

	delete pTargetSection;
}
