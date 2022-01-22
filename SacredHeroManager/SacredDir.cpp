// SacredDir.cpp: implementation of the CSacredDir class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredHeroManager.h"
#include "SacredDir.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSacredDir::CSacredDir()
{

}

CSacredDir::~CSacredDir()
{

}

BOOL CSacredDir::FindSacredDirectory(){
	m_szSacredDir.Empty();
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Ascaron Entertainment\\Sacred",0,KEY_QUERY_VALUE,&hkey)==ERROR_SUCCESS){
		char szInstallLocation[1024];
		szInstallLocation[0] = 0;
		if(GetRegStringValue(hkey,"InstallLocation",szInstallLocation,1024) != -1){
			if(szInstallLocation[strlen(szInstallLocation)-1] != '\\'){
				strcat(szInstallLocation,"\\");
			}
			strcat(szInstallLocation,"save\\");
			m_szSacredDir = szInstallLocation;
			RegCloseKey(hkey);
			return TRUE;
		}
		RegCloseKey(hkey);
	}
	return FALSE;
}

int CSacredDir::GetRegStringValue(HKEY hKey, const char* szIdentifier, char* szBuffer, int nBufferLen){
	unsigned long ulType = REG_SZ;
	unsigned long ulTempBufferLen = nBufferLen;
	if(RegQueryValueEx(hKey,szIdentifier,0,&ulType,(unsigned char*)szBuffer,&ulTempBufferLen)==ERROR_SUCCESS){
		return (int)ulTempBufferLen;
	}
	return -1;
}
