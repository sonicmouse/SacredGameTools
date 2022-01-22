//
// includes
//
#include <windows.h>
#include "resource.h"

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
// DLL Entry Point
//
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved){
	g_hInstance = hInstance;
	return TRUE;
}

//
// dialog proc
//
BOOL CALLBACK DialogProc(HWND hwndDlg,UINT uMsg, WPARAM wParam, LPARAM lParam){

	unsigned short wmId, wmEvent;

	switch(uMsg){
		case WM_INITDIALOG:
			g_hWndDialog = hwndDlg;
			{
				SetDlgItemInt(hwndDlg,IDC_EDIT_SKILLS,*(unsigned short*)&g_ucHeroData[0x3CF],FALSE);
				SetDlgItemInt(hwndDlg,IDC_EDIT_ATTR,g_ucHeroData[0x3B0],FALSE);
			}
			break;
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch(wmId){
				case IDOK:
					{
						*(unsigned short*)&g_ucHeroData[0x3CF] = (unsigned short)GetDlgItemInt(hwndDlg,IDC_EDIT_SKILLS,NULL,FALSE);
						g_ucHeroData[0x3B0] = (unsigned char)GetDlgItemInt(hwndDlg,IDC_EDIT_ATTR,NULL,FALSE);
					}
					EndDialog(hwndDlg, IDOK);
					break;
				case IDCANCEL:
					EndDialog(hwndDlg, IDCANCEL);
					break;
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
	strcpy(szLibraryName, "Give Skills and Attributes");
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
