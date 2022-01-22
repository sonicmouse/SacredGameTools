/*
a program which removes the key values at:
HKEY_CURRENT_USER\Software\Ascaron Entertainment\Sacred
from the registry.

Why? seems that the CDKey bug is associated with it.

-SonicMouse
*/

#include <windows.h>
#include <stdio.h>
#include <conio.h>

int GetRegStringValue(HKEY hKey, const char* szIdentifier, char* szBuffer, int nBufferLen){
	unsigned long ulType = REG_SZ;
	unsigned long ulTempBufferLen = nBufferLen;
	if(RegQueryValueEx(hKey,szIdentifier,0,&ulType,(unsigned char*)szBuffer,&ulTempBufferLen)==ERROR_SUCCESS){
		return (int)ulTempBufferLen;
	}
	return -1;
}

char* ExpandBackslashes(const char* pszString){
	// count how many exist...
	int nCountBackslashes = 0;
	for(int i = 0; i < (int)strlen(pszString); i++){
		if(pszString[i]=='\\'){
			++nCountBackslashes;
		}
	}
	
	// create a new buffer
	int nSizeExpanded = strlen(pszString)+nCountBackslashes;
	char* pszNewBuffer = new char[nSizeExpanded+1];

	// copy it over, expanding backslashes
	int nAtIndex = 0;
	for(i = 0; i < (int)strlen(pszString); i++){
		pszNewBuffer[nAtIndex++] = pszString[i];
		if(pszString[i] == '\\'){
			pszNewBuffer[nAtIndex++] = '\\';
		}
	}
	// terminate string
	pszNewBuffer[i+1] = 0;
	// done.. dont forget to delete
	return pszNewBuffer;
}

//
// pep
//
void main(){
	
	printf("This program will hopefully fix the CDKey bug issue you seem to be having...\n");
	printf("Make sure \"Sacred\" is uninstalled before continuing.\n\n");
	printf("NOTE: An undo file will be created incase you change your mind later.\n\n");
	printf("Are you sure you want to continue? (y/n): ");

	char rc = getch();
	if(rc != 'y' && rc != 'Y'){
		printf("\n\nAborted by user...\nPress any key to continue");
		getch();
		return;
	}

	HKEY hkey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Ascaron Entertainment\\Sacred",0,KEY_ALL_ACCESS,&hkey)==ERROR_SUCCESS){
		
		// save install key
		char szInstallKey[1024];
		szInstallKey[0] = 0;
		GetRegStringValue(hkey,"InstallKey",szInstallKey,1024);

		// save install location
		char szInstallLocation[1024];
		szInstallLocation[0] = 0;
		GetRegStringValue(hkey,"InstallLocation",szInstallLocation,1024);

		// save Language
		char szLanguage[1024];
		szLanguage[0] = 0;
		GetRegStringValue(hkey,"Language",szLanguage,1024);

		// save updated
		char szUpdated[1024];
		szUpdated[0] = 0;
		GetRegStringValue(hkey,"Updated",szUpdated,1024);

		// save Version
		char szVersion[1024];
		szVersion[0] = 0;
		GetRegStringValue(hkey,"Version",szVersion,1024);

		// now create a reg file in the same path...
		char szMyPath[MAX_PATH];
		GetModuleFileName(NULL,szMyPath,MAX_PATH);
		strrchr(szMyPath,'\\')[0] = 0;
		strcat(szMyPath,"\\sacred_backup.reg");

		FILE* f = fopen(szMyPath,"w");
		if(f){
			fprintf(f,"REGEDIT4\n\n");
			fprintf(f,"[HKEY_CURRENT_USER\\Software\\Ascaron Entertainment\\Sacred]\n");
			
			if(strlen(szInstallKey)){
				fprintf(f,"\"InstallKey\"=\"%s\"\n", szInstallKey);
			}
			if(strlen(szInstallLocation)){
				char* tempPath = ExpandBackslashes(szInstallLocation);
				fprintf(f,"\"InstallLocation\"=\"%s\"\n", tempPath);
				delete tempPath;
			}
			if(strlen(szLanguage)){
				fprintf(f,"\"Language\"=\"%s\"\n", szLanguage);
			}
			if(strlen(szUpdated)){
				fprintf(f,"\"Updated\"=\"%s\"\n", szUpdated);
			}
			if(strlen(szVersion)){
				fprintf(f,"\"Version\"=\"%s\"\n", szVersion);
			}

			fclose(f);

			// now delete them all...
			RegDeleteValue(hkey,"InstallKey");
			RegDeleteValue(hkey,"InstallLocation");
			RegDeleteValue(hkey,"Language");
			RegDeleteValue(hkey,"Updated");
			RegDeleteValue(hkey,"Version");

			// done with key...
			RegCloseKey(hkey);

			// done...
			printf("\n\nAll appropriate changes were made.\nTry installing \"Sacred\" and all patches again.\n\n");
		}else{
			// done with key...
			RegCloseKey(hkey);
			// fopen failed...
			printf("\n\nUnable to create a backup reg file.\nNothing has been done...\nAborting.\n\n");
		}

	}else{
		printf("\n\nUnable to open Sacred's registry key.\nMake sure you are administrator of this machine.\n\n");
	}
	
	printf("Press any key to continue");
	getch();
}
