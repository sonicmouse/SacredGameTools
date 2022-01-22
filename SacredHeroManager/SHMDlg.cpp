// SHMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredHeroManager.h"
#include "SHMDlg.h"
#include "SacredDir.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSHMDlg dialog

CSHMDlg::CSHMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSHMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSHMDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pHeroSection = NULL;
	m_ulHeroSectionSize = 0;
}

void CSHMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSHMDlg)
	DDX_Control(pDX, IDC_LIST_PLUGINS, m_ListPlugins);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSHMDlg, CDialog)
	//{{AFX_MSG_MAP(CSHMDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEHERO, OnButtonBrowsehero)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSEHERO, OnButtonClosehero)
	ON_BN_CLICKED(IDC_BUTTON_RELOADPLUGINS, OnButtonReloadplugins)
	ON_BN_CLICKED(IDC_BUTTON_RUNPLUGIN, OnButtonRunplugin)
	ON_BN_CLICKED(IDC_BUTTON_RELOADHERO, OnButtonReloadhero)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PLUGINS, OnDblclkListPlugins)
	ON_BN_CLICKED(IDC_BUTTON_UPDATEHERO, OnButtonUpdatehero)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSHMDlg message handlers

BOOL CSHMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	UpdateWindows();

	m_ListPlugins.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListPlugins.InsertColumn(0,"Title"); m_ListPlugins.SetColumnWidth(0,140);
	m_ListPlugins.InsertColumn(1,"Author"); m_ListPlugins.SetColumnWidth(1,80);

	DoPlugins();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//
// browses for heros
//
CString CSHMDlg::BrowseForHero(){
	CSacredDir sacredDir;
	sacredDir.FindSacredDirectory();
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	char fileBuffer[MAX_PATH];
	fileBuffer[0] = 0;
	ofn.lStructSize = sizeof(ofn);
	ofn.hInstance = AfxGetInstanceHandle();
	ofn.hwndOwner = GetSafeHwnd();
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.lpstrFilter = "All Supported Heros Types\0*.PAX;*.PTX\0Sacred Heros (*.PAX)\0*.PAX\0Hero Template Files (*.PTX)\0*.PTX\0\0";
	ofn.lpstrTitle = "Find a Sacred Hero File...";
	ofn.lpstrFile = fileBuffer;
	ofn.nMaxFile = MAX_PATH;
	if(sacredDir.GetSacredDirectory().GetLength()){
		ofn.lpstrInitialDir = (const char*)sacredDir.GetSacredDirectory();
	}
	GetOpenFileName(&ofn);
	return fileBuffer;
}

//
// loads the hero
//
void CSHMDlg::LoadHero(const CString& szPath){
	// get our shlib
	CSHLib* pLib = ((CSHMApp*)AfxGetApp())->GetSHLib();

	// create the hero
	HHERO hHero = pLib->CreateHero();

	// decompile the hero
	int rc = pLib->DecompileHero(hHero,szPath);
	if(rc != ERROR_SHLIB_SUCCESS){
		MessageBox("Failed to decompile this hero!","Error...",MB_ICONHAND|MB_OK);
		pLib->DestroyHero(hHero);
		return;
	}

	// get 0xC7's section size
	int nSize = pLib->GetHeroSection(hHero,SECTION_HERO,NULL);
	if(nSize == -1){
		pLib->DestroyHero(hHero);
		MessageBox("This hero is corrupt as far as this program thinks", "Error...",MB_ICONHAND|MB_OK);
		return;
	}

	// alloc a buffer
	unsigned char* pBuf = new unsigned char[nSize];
	if(!pBuf){
		pLib->DestroyHero(hHero);
		MessageBox("You are completly out of memory!", "Error...",MB_ICONHAND|MB_OK);
		return;
	}

	// retrieve the data
	pLib->GetHeroSection(hHero,SECTION_HERO,&pBuf);

	// set the hero name
	SetHeroName(hHero);

	// done with hero
	pLib->DestroyHero(hHero);

	// set the text
	SetDlgItemText(IDC_EDIT_HERO,szPath);

	// save the buffer
	if(m_pHeroSection){ delete m_pHeroSection; }
	m_pHeroSection = pBuf;
	m_ulHeroSectionSize = nSize;

	// update the windows
	UpdateWindows();
}

//
// button clicked to browse for hero
//
void CSHMDlg::OnButtonBrowsehero(){
	// get a hero
	CString szPath = BrowseForHero();
	if(!szPath.GetLength()){ return; }
	LoadHero(szPath);
}

//
// sets the name from an HHERO
//
void CSHMDlg::SetHeroName(HHERO hHero){
	SetDlgItemText(IDC_STATIC_HF,"Current Hero");
	if(!hHero){	return; }

	// get our shlib
	CSHLib* pLib = ((CSHMApp*)AfxGetApp())->GetSHLib();

	// get section size
	int nSize = pLib->GetHeroSection(hHero,SECTION_HEROATAGLANCE,NULL);
	if(nSize != -1){
		unsigned char* pBuf = new unsigned char[nSize];
		pLib->GetHeroSection(hHero,SECTION_HEROATAGLANCE,&pBuf);
		SetDlgItemText(IDC_STATIC_HF,"Current Hero [" + GetHeroName(pBuf) + "]");
		delete pBuf;
	}
}

//
// returns hero name from a buffer
//
CString CSHMDlg::GetHeroName(unsigned char* lpBuffer){
	CString retName;
	// move up to name
	UCHAR* p = &lpBuffer[sizeof(ULONG)*2];
	// create a name buffer
	char nameBuff[HERO_NAME_SIZE];
	// convert name buffer from unicode to ansi
	int rc = WideCharToMultiByte(CP_ACP,0,(WCHAR*)p,-1,nameBuff,HERO_NAME_SIZE,NULL,NULL);
	// if it failed, give up
	if(!rc){
		return "Unknown Name";
	}
	// go thru and kill special chars
	for(int i = 0; i < rc-1; i++){
		if(nameBuff[i]=='\\'){
			if(nameBuff[i+1]=='c'){
				// check for 1 ulong
				if(rc-i >= 11){ // 8 + 3
					i += 9;
				}
			}else if(nameBuff[i+1]=='g'){
				// check for 2 ULONGs 
				if(rc-i >= 19){ // 16 + 3
					i += 17;
				}
			}else{
				// tack it on
				retName += nameBuff[i];
			}
		}else{
			// tack it on
			retName += nameBuff[i];
		}
	}
	return (!retName.GetLength()?"Unknown Name":retName);
}

//
// clears the hero buffer
//
void CSHMDlg::ClearHeroBuffer(){
	if(m_pHeroSection){
		delete m_pHeroSection;
	}
	m_pHeroSection = NULL;
	m_ulHeroSectionSize = 0;
	UpdateWindows();
}

//
// WM_DESTROY
//
void CSHMDlg::OnDestroy(){
	DestroyIcon(m_hIcon);
	ClearHeroBuffer();

	CDialog::OnDestroy();
}

//
// closes the current hero
//
void CSHMDlg::OnButtonClosehero(){
	ClearHeroBuffer();
	SetDlgItemText(IDC_EDIT_HERO,"");
}

//
// updates the windows
//
void CSHMDlg::UpdateWindows(){
	BOOL bData = (m_pHeroSection && m_ulHeroSectionSize);
	GetDlgItem(IDC_BUTTON_CLOSEHERO)->EnableWindow(bData);
	GetDlgItem(IDC_BUTTON_RELOADHERO)->EnableWindow(bData);
	GetDlgItem(IDC_BUTTON_UPDATEHERO)->EnableWindow(bData);
	GetDlgItem(IDC_LIST_PLUGINS)->EnableWindow(bData);
	GetDlgItem(IDC_BUTTON_RUNPLUGIN)->EnableWindow(bData);
	if(!bData){
		SetHeroName(NULL);
	}
}

//
// does the plugins
//
void CSHMDlg::DoPlugins(){
	m_Plugins.FreePlugins();
	m_ListPlugins.DeleteAllItems();

	char myPath[MAX_PATH];
	GetModuleFileName(NULL,myPath,MAX_PATH);
	strrchr(myPath,'\\')[1] = 0;
	strcat(myPath,"plugins\\");

#ifdef _DEBUG
	strcpy(myPath,"E:\\proj32\\SacredHeroManager\\plugins\\");
#endif

	if(!m_Plugins.LoadPlugins(myPath)){
		MessageBox(CString("There are currently no plugins installed to:\n\n") + myPath + CString("\n\nMake sure you have plugins in this directory,\notherwise this program is useless."),"Error...",MB_ICONHAND|MB_OK);
		return;
	}
	
	// assign each an id
	for(int i = 0; i < m_Plugins.GetPluginCount(); i++){
		m_Plugins.SetPluginId(i,i);
		int nInd = m_ListPlugins.InsertItem(i,m_Plugins.GetPluginTitle(i));
		m_ListPlugins.SetItemText(nInd,1,m_Plugins.GetPluginAuthor(i));
		m_ListPlugins.SetItemData(nInd,i);
	}
}

//
// reloads the plugin
//
void CSHMDlg::OnButtonReloadplugins(){
	DoPlugins();
}

//
// runs the plugin
//
void CSHMDlg::OnButtonRunplugin(){
	if(!m_ListPlugins.GetSelectedCount() || !m_pHeroSection){ return; }

	POSITION pos = m_ListPlugins.GetFirstSelectedItemPosition();
	int nInd = m_ListPlugins.GetNextSelectedItem(pos);
	if(nInd != -1){
		int nId = m_ListPlugins.GetItemData(nInd);

		CString szHero;
		GetDlgItemText(IDC_EDIT_HERO,szHero);

		m_Plugins.RunPluginEntry(nId,GetSafeHwnd(),m_pHeroSection,m_ulHeroSectionSize,szHero);
	}
}

//
// button - reload hero
//
void CSHMDlg::OnButtonReloadhero(){
	CString szHero;
	GetDlgItemText(IDC_EDIT_HERO,szHero);
	LoadHero(szHero);
}

//
// runs the plugin by double-clicking on it
//
void CSHMDlg::OnDblclkListPlugins(NMHDR* pNMHDR, LRESULT* pResult){
	OnButtonRunplugin();
	*pResult = 0;
}

//
// updates the hero
//
void CSHMDlg::OnButtonUpdatehero(){
	if(!m_pHeroSection){ return; }

	// get hero path
	CString szHero;
	GetDlgItemText(IDC_EDIT_HERO,szHero);
	
	if(!szHero.GetLength()){ return; }

	// confirm backup
	if(MessageBox("Would you like to create a backup (*.BAK) file?", "Confirm...", MB_ICONQUESTION|MB_YESNO)==IDYES){
		char szFin[MAX_PATH];
		strcpy(szFin,szHero);
		strcat(szFin,".bak");
		while(true){
			// if we can read it, its already there
			FILE* f = fopen(szFin,"r");
			if(f){
				// close it, and tack on another .bak
				fclose(f);
				strcat(szFin,".bak");
			}else{
				// test for writing.. 
				f = fopen(szFin,"w");
				if(f){
					// if we can, we are all good
					fclose(f);
					break;
				}else{
					// if we cant, tack on another .bak
					strcat(szFin,".bak");
				}
			}
		}
		CopyFile(szHero,szFin,FALSE);
	}

	// get our shlib
	CSHLib* pLib = ((CSHMApp*)AfxGetApp())->GetSHLib();

	// create the hero
	HHERO hHero = pLib->CreateHero();

	// decompile it
	pLib->DecompileHero(hHero,szHero);

	// update the section
	pLib->SetHeroSection(hHero,SECTION_HERO,m_pHeroSection,m_ulHeroSectionSize);

	// compile it
	pLib->CompileHero(hHero,szHero);

	// no need for hero anymore
	pLib->DestroyHero(hHero);

	MessageBox("Hero updated!","Success...", MB_ICONINFORMATION|MB_OK);
}
