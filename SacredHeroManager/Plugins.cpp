// Plugins.cpp: implementation of the CPlugins class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredHeroManager.h"
#include "Plugins.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlugins::CPlugins()
{

}

CPlugins::~CPlugins()
{
	FreePlugins();
}

int CPlugins::LoadPlugins(const CString& szDir){
	WIN32_FIND_DATA fd;
	HANDLE seq = FindFirstFile(szDir + "*.dll", &fd);
	if(seq && (seq != INVALID_HANDLE_VALUE)){
		BOOL keepGoing = TRUE;
		while(keepGoing){
			CString currDll = szDir + CString(fd.cFileName);
			// currDll is a path to a plugin.. lets load her up
			HMODULE hMod = LoadLibrary(currDll);
			if(hMod){
				__PLUGIN_LIBDETAILS lpDet = (__PLUGIN_LIBDETAILS)GetProcAddress(hMod,"seaseven_library_details");
				__PLUGIN_ENTRY lpEnt = (__PLUGIN_ENTRY)GetProcAddress(hMod,"seaseven_plugin_entry");
				if(lpEnt && lpDet){
					LPPLUGIN plugin = new PLUGIN;
					plugin->mod = hMod;
					strcpy(plugin->path, currDll);
					plugin->procDetails = lpDet;
					plugin->procEntry = lpEnt;
					plugin->id = -1;
					(*lpDet)(plugin->title, plugin->author);
					m_arrPlugins.Add(plugin);
				}else{
					// insufficient exports
					FreeLibrary(hMod);
				}
			}else{
				// failed to load library
			}

			keepGoing = FindNextFile(seq, &fd);
		}
		FindClose(seq);
	}
	return GetPluginCount();
}

void CPlugins::FreePlugins(){
	for(int i = 0; i < m_arrPlugins.GetSize(); i++){
		LPPLUGIN plugin = (LPPLUGIN)m_arrPlugins.GetAt(i);
		FreeLibrary(plugin->mod);
		delete plugin;
	}
	m_arrPlugins.RemoveAll();
}

void CPlugins::SetPluginId(int nIndex, int nId){
	for(int i = 0; i < m_arrPlugins.GetSize(); i++){
		LPPLUGIN plugin = (LPPLUGIN)m_arrPlugins.GetAt(i);
		if(i == nIndex){
			plugin->id = nId;
			break;
		}
	}
}

int CPlugins::GetPluginId(int nIndex){
	for(int i = 0; i < m_arrPlugins.GetSize(); i++){
		LPPLUGIN plugin = (LPPLUGIN)m_arrPlugins.GetAt(i);
		if(i == nIndex){
			return plugin->id;
		}
	}
	return -1;
}

CString CPlugins::GetPluginTitle(int nId){
	for(int i = 0; i < m_arrPlugins.GetSize(); i++){
		LPPLUGIN plugin = (LPPLUGIN)m_arrPlugins.GetAt(i);
		if(plugin->id == nId){
			return plugin->title;
		}
	}
	return "";
}

CString CPlugins::GetPluginAuthor(int nId){
	for(int i = 0; i < m_arrPlugins.GetSize(); i++){
		LPPLUGIN plugin = (LPPLUGIN)m_arrPlugins.GetAt(i);
		if(plugin->id == nId){
			return plugin->author;
		}
	}
	return "";
}

void CPlugins::RunPluginEntry( int nId,
							   HWND hWndParentApp,
							   unsigned char* lpucHeroData,
							   unsigned long ulHeroDataSize,
							   const char* szHeroPath,
							   unsigned long* pulReserved/* = NULL */ ){
	for(int i = 0; i < m_arrPlugins.GetSize(); i++){
		LPPLUGIN plugin = (LPPLUGIN)m_arrPlugins.GetAt(i);
		if(plugin->id == nId){
			(*plugin->procEntry)(hWndParentApp,lpucHeroData,ulHeroDataSize,szHeroPath,pulReserved);
		}
	}
}