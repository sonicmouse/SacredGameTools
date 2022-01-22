//
// includes
//
#include <windows.h>
#include <stdio.h>

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
	strcpy(szLibraryName, "Dump (for analysing)");
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

	int nResp = MessageBox(hWndParentApp,"YES: Export Dump\nNO: Import Dump\nCANCEL: Exit Plugin","Choose...",MB_ICONQUESTION|MB_YESNOCANCEL);

	if(nResp == IDYES){
		// export

		char tempFileName[MAX_PATH];
		strcpy(tempFileName,strrchr(szHeroPath,'\\')+1);
		strrchr(tempFileName,'.')[1] = 0;
		strcat(tempFileName,"ssd");

		OPENFILENAME ofn;
		memset(&ofn,0,sizeof(ofn));
		char fileBuffer[MAX_PATH];
		strcpy(fileBuffer,tempFileName);

		ofn.lStructSize = sizeof(ofn);
		ofn.hInstance = g_hInstance;
		ofn.hwndOwner = hWndParentApp;
		ofn.Flags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
		ofn.lpstrFilter = "Sea Seven Dumps (*.SSD)\0*.SSD\0All Files (*.*)\0*.*\0\0";
		ofn.lpstrTitle = "Export Sea Seven Dump...";
		ofn.lpstrFile = fileBuffer;
		ofn.nMaxFile = MAX_PATH;
		
		ofn.lpstrDefExt = "ssd";
		if(GetSaveFileName(&ofn)==0){ return; }

		FILE* f = fopen(fileBuffer,"wb");

		if(f){
			fwrite(lpucHeroData,ulHeroDataSize,1,f);
			fclose(f);
			MessageBox(hWndParentApp,"Successfully exported dump!","Success...", MB_ICONINFORMATION|MB_OK);
		}else{
			// failed open
			MessageBox(hWndParentApp,"Failed to open target file!\n(Make sure it is not in use!)","Error...", MB_ICONHAND|MB_OK);
		}

	}else if(nResp == IDNO){
		// import

		OPENFILENAME ofn;
		memset(&ofn,0,sizeof(ofn));
		char fileBuffer[MAX_PATH];
		fileBuffer[0] = 0;
		ofn.lStructSize = sizeof(ofn);
		ofn.hInstance = g_hInstance;
		ofn.hwndOwner = hWndParentApp;
		ofn.Flags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
		ofn.lpstrFilter = "Sea Seven Dumps (*.SSD)\0*.SSD\0All Files (*.*)\0*.*\0\0";
		ofn.lpstrTitle = "Import Sea Seven Dump...";
		ofn.lpstrFile = fileBuffer;
		ofn.nMaxFile = MAX_PATH;
		if(GetOpenFileName(&ofn)==0){ return; }

		FILE* f = fopen(fileBuffer,"rb");
		if(f){
			fseek(f,0,SEEK_END);
			int nFileLen = ftell(f);
			rewind(f);
			if(nFileLen == (int)ulHeroDataSize){
				if(fread(lpucHeroData,ulHeroDataSize,1,f)==1){
					// success
					MessageBox(hWndParentApp,"Successfully imported dump!","Success...", MB_ICONINFORMATION|MB_OK);
				}else{
					// file error
					MessageBox(hWndParentApp,"File Error!\nThere was a file error when trying to read from the target file!","Error...", MB_ICONHAND|MB_OK);
				}
			}else{
				// size mismatch
				MessageBox(hWndParentApp,"Size mismatch!\nYou are trying to import a dump that is either too large or too small!","Error...", MB_ICONHAND|MB_OK);
			}
			fclose(f);
		}else{
			// failed open
			MessageBox(hWndParentApp,"Failed to open target file!\n(Make sure it is not in use!)","Error...", MB_ICONHAND|MB_OK);
		}

	}else{
		// exit
	}
}
