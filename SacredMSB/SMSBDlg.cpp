// SMSBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredMSB.h"
#include "SMSBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMSBDlg dialog

CSMSBDlg::CSMSBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSBDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMSBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSBDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSMSBDlg, CDialog)
	//{{AFX_MSG_MAP(CSMSBDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_MAX, OnButtonMax)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSBDlg message handlers

BOOL CSMSBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSMSBDlg::OnButtonBrowse() 
{
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));

	char fileBuffer[MAX_PATH];
	fileBuffer[0] = 0;

	ofn.lStructSize = sizeof(ofn);
	ofn.hInstance = AfxGetInstanceHandle();
	ofn.hwndOwner = GetSafeHwnd();
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.lpstrFilter = "Sacred Heros (*.PAX)\0*.PAX\0\0";
	ofn.lpstrTitle = "Find a Sacred Hero...";
	ofn.lpstrFile = fileBuffer;
	ofn.nMaxFile = MAX_PATH;
	
	if(!GetOpenFileName(&ofn)){ return; }

	SetDlgItemText(IDC_EDIT_HERO,fileBuffer);
	GetDlgItem(IDC_BUTTON_MAX)->EnableWindow(TRUE);
}

void CSMSBDlg::OnButtonAbout() 
{
	CString txt;
	txt = "Thanks goes out to \"Alexis Pernicious\" for her efforts in\ntrying to find the Sacred's \"Survival Bonus\" offset!\n\n";
	txt+= "Without her, this program wouldn't have been possible.";
	MessageBox(txt,"About...", MB_ICONINFORMATION|MB_OK);
}

void CSMSBDlg::OnButtonMax(){

	CSMSBApp* pApp = (CSMSBApp*)AfxGetApp();

	CString targetHero;
	GetDlgItemText(IDC_EDIT_HERO, targetHero);

	HHERO hHero = pApp->GetSHLib()->CreateHero();
	int rc = pApp->GetSHLib()->DecompileHero(hHero,targetHero);

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
		MessageBox("This hero is all fucked up... sorry, I can't work with it.", "Error...", MB_ICONHAND|MB_OK);
		return;
	}

	// 3rd int is SB%
	*(ULONG*)&pTargetSection[2*sizeof(unsigned long)] = 0x0030FFFF;

	pApp->GetSHLib()->SetHeroSection(hHero,TARGET_SECTION,pTargetSection,int(ulTargetSectionLength));
	if(pApp->GetSHLib()->CompileHero(hHero,targetHero)){
		MessageBox("Hero now has a \"Survival Bonus\" of 99%!","Success...",MB_ICONINFORMATION|MB_OK);
	}else{
		MessageBox("Something went severely wrong... nothing has changed.","Error...",MB_ICONHAND|MB_OK);
	}

	pApp->GetSHLib()->DestroyHero(hHero);
	delete pTargetSection;
}
