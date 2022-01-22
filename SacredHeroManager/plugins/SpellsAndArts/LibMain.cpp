//
// includes
//
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "spells.h"

//
// defines
//


//
// globals
//
HINSTANCE g_hInstance = NULL;
unsigned char* g_ucHeroData = NULL;
unsigned long g_ulHeroDataSize = 0;
HWND g_hWndDialog = NULL;

//
// structs
//
typedef struct _tag_spell{
	unsigned short id;
	unsigned char level;
	char name[MAX_SPELL_LENGTH];
	unsigned char guts[0x16-(sizeof(short)+sizeof(char))];
} SPELL,*LPSPELL;

//
// DLL Entry Point
//
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved){
	g_hInstance = hInstance;
	return TRUE;
}

//
// gets a spell name by ID number
//
static char* GetSpellName(UINT nId){
	static char szSpell[MAX_SPELL_LENGTH];
	szSpell[0] = 0;
	for(int i = 0; i < TOTAL_SPELLS; i++){
		const char* pSpell = &g_Spells[i][0];
		char* dup = strdup(pSpell);
		char* spellName = strchr(dup,'=');
		spellName[0] = 0;
		++spellName;
		if((UINT)atoi(dup)==nId){
			strcpy(szSpell,spellName);
		}
		free(dup);
		if(szSpell[0] != 0){ break; }
	}
	return szSpell;
}

//
// sets up dialog
//
void SetUpDialog(){

	// set up slider
	HWND hWndSlider = GetDlgItem(g_hWndDialog,IDC_SLIDER_LEVEL);
	SendMessage(hWndSlider,TBM_SETRANGEMIN,FALSE,0);
	SendMessage(hWndSlider,TBM_SETRANGEMAX,FALSE,255);
	SendMessage(hWndSlider,TBM_SETTICFREQ,15,0);

	HWND hWndListbox = GetDlgItem(g_hWndDialog,IDC_LIST_SPELLS);

	unsigned char ucTotalSpells = g_ucHeroData[0x483];
	for(unsigned char i = 0; i < ucTotalSpells; i++){

		LPSPELL pSpell = new SPELL;

		memcpy(pSpell->guts, &g_ucHeroData[0x489 + (22*i) + (sizeof(short)+sizeof(char))], 0x16-(sizeof(short)+sizeof(char)));
		pSpell->id = *(unsigned short*)&g_ucHeroData[0x489 + (22*i)];
		pSpell->level = g_ucHeroData[0x489 + (22*i) + sizeof(short)];
		strcpy(pSpell->name,GetSpellName(pSpell->id));

		int nIndex = (int)SendMessage(hWndListbox,LB_ADDSTRING,0,(LPARAM)pSpell->name);
		SendMessage(hWndListbox,LB_SETITEMDATA,nIndex,(LPARAM)pSpell);
	}

	if(ucTotalSpells > 0){
		SendMessage(hWndListbox,LB_SETCURSEL,0,0);
	}
}

//
// selection changed
//
void OnSelectChange(){

	HWND hWndListbox = GetDlgItem(g_hWndDialog,IDC_LIST_SPELLS);
	HWND hWndSlider = GetDlgItem(g_hWndDialog,IDC_SLIDER_LEVEL);

	int nIndex = SendMessage(hWndListbox,LB_GETCURSEL,0,0);
	if(nIndex != LB_ERR){
		LPSPELL pSpell = (LPSPELL)SendMessage(hWndListbox,LB_GETITEMDATA,nIndex,0);

		SetDlgItemInt(g_hWndDialog,IDC_EDIT_LEVEL,pSpell->level,FALSE);
		SendMessage(hWndSlider, TBM_SETPOS, TRUE, pSpell->level);
		EnableWindow(hWndSlider,TRUE);
	}else{
		EnableWindow(hWndSlider,FALSE);
	}
}

//
// sets the level of an ID in the data
//
void SetLevel(const LPSPELL pSpell){
	unsigned char ucTotalSpells = g_ucHeroData[0x483];
	for(unsigned char i = 0; i < ucTotalSpells; i++){
		unsigned short usId = *(unsigned short*)&g_ucHeroData[0x489 + (22*i)];
		if(usId == pSpell->id){
			g_ucHeroData[0x489 + (22*i) + sizeof(short)] = pSpell->level;
			break;
		}
	}
}

//
// IDOK hit
//
void OnOK(){
	HWND hWndListbox = GetDlgItem(g_hWndDialog,IDC_LIST_SPELLS);
	int nTotalItems = SendMessage(hWndListbox,LB_GETCOUNT,0,0);
	for(int i = 0; i < nTotalItems; i++){
		SetLevel((LPSPELL)SendMessage(hWndListbox,LB_GETITEMDATA,i,0));
	}
}

//
// dialog proc
//
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg, WPARAM wParam, LPARAM lParam){

	unsigned short wmId, wmEvent;

	switch(uMsg){
		case WM_INITDIALOG:
			g_hWndDialog = hwndDlg;
			SetUpDialog();
			break;
		case WM_DESTROY:
			{
				HWND hWndListbox = GetDlgItem(g_hWndDialog,IDC_LIST_SPELLS);
				int nTotalItems = SendMessage(hWndListbox,LB_GETCOUNT,0,0);
				for(int i = 0; i < nTotalItems; i++){
					LPSPELL pSpell = (LPSPELL)SendMessage(hWndListbox,LB_GETITEMDATA,i,0);
					delete pSpell;
				}
			}
			break;
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch(wmId){
				case IDC_LIST_SPELLS:
					OnSelectChange();
					break;
				case IDOK:
					OnOK();
					EndDialog(hwndDlg, IDOK);
					break;
				case IDCANCEL:
					EndDialog(hwndDlg, IDCANCEL);
					break;
			}
			break;
		case WM_HSCROLL:
			{
				HWND hWndSlider = GetDlgItem(g_hWndDialog,IDC_SLIDER_LEVEL);
				int nPos = SendMessage(hWndSlider,TBM_GETPOS,0,0);
				SetDlgItemInt(hwndDlg,IDC_EDIT_LEVEL,nPos,FALSE);

				HWND hWndListbox = GetDlgItem(g_hWndDialog,IDC_LIST_SPELLS);
				int nIndex = SendMessage(hWndListbox,LB_GETCURSEL,0,0);
				if(nIndex != LB_ERR){
					LPSPELL pSpell = (LPSPELL)SendMessage(hWndListbox,LB_GETITEMDATA,nIndex,0);
					pSpell->level = nPos;
				}
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

//
// gets the library details
//
void __stdcall seaseven_library_details(char* szLibraryName /* MAX 255 chars */,
										char* szAuthor /* MAX 255 chars */){
	strcpy(szLibraryName, "Spells (The Anti-Rune)");
	strcpy(szAuthor, "SonicMouse");
}

//
// item data transaction routine export
//
void __stdcall seaseven_plugin_entry(HWND hWndParentApp /* HWND to SeaSeven application */,
									 unsigned char* lpucHeroData /* array of hero data */,
									 unsigned long ulHeroDataSize /* size of hero data */,
									 const char* szHeroPath /* path to current PAX/PTX file */,
									 unsigned long* pulReserved /* reserved... always null */){
	g_ucHeroData = lpucHeroData;
	g_ulHeroDataSize = ulHeroDataSize;
	int rc = DialogBoxParam(g_hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN),hWndParentApp,DialogProc,0);
	if(rc==IDOK){
		MessageBox(hWndParentApp,"Successfully updated hero's skill and attribute bonus","Success...",MB_OK|MB_ICONINFORMATION);
	}
}
