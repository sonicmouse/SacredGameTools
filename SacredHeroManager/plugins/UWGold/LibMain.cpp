//
// includes
//
#include <windows.h>

//
// defines
//
#define SACRED_GOLD_OFFSET_UW 0x41B

//
// globals
//
HINSTANCE g_hInstance = NULL;

//
// DLL Entry Point
//
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved){
	g_hInstance = hInstance;
	return TRUE;
}

//
// gets the library details
//
void __stdcall seaseven_library_details(char* szLibraryName /* MAX 255 chars */,
										char* szAuthor /* MAX 255 chars */){
	strcpy(szLibraryName, "*UW* Maximize Gold");
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

	// retrieve the current amount of gold from the data
	int nCurrentGold = *(int*)&lpucHeroData[SACRED_GOLD_OFFSET_UW];

	// compile a message to display
	char szMsgTxt[255];
	wsprintf(szMsgTxt, "THIS PLUGIN IS FOR SACRED UNDERWORLD ONLY!\n\nCurrent Gold Amount: %i\n\nAre you sure you wish to give this hero the\nmaximum amount of gold possible (2,147,483,647)?", nCurrentGold);
	
	// ask if they really want to do this
	if(MessageBox(hWndParentApp,szMsgTxt, "Confirm...", MB_YESNO|MB_ICONQUESTION)==IDYES){
		// set the maximum amount of gold a hero can carry in the hero data
		*(int*)&lpucHeroData[SACRED_GOLD_OFFSET_UW] = 0x7FFFFFFF;
		// display a message letting them know its been done
		MessageBox(hWndParentApp,"Gold has been maximized to 2,147,483,647!","Success", MB_ICONINFORMATION|MB_OK);
	}

	// bye bye
}
