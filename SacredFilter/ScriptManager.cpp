// ScriptManager.cpp: implementation of the CScriptManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredFilter.h"
#include "ScriptManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptManager::CScriptManager(){

}

CScriptManager::~CScriptManager()
{
	RemoveAllScripts();
}

//
//
//
int CScriptManager::SetScriptDirectory(const CString& szDir){
	WIN32_FIND_DATA fd;
	HANDLE seq = FindFirstFile(szDir + "*.*", &fd);
	if(seq && (seq != INVALID_HANDLE_VALUE)){
		BOOL keepGoing = TRUE;
		while(keepGoing){
			if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY){
				CString currPlugin = szDir + CString(fd.cFileName);
				AddScript(currPlugin);
			}
			keepGoing = FindNextFile(seq, &fd);
		}
		FindClose(seq);
	}
	return m_arrScripts.GetSize();
}

//
//
//
BOOL CScriptManager::AddScript(const CString& szScriptPath){
	bool bJS = false;
	int ind = szScriptPath.ReverseFind('.');
	if(ind != -1){
		if(szScriptPath.Mid(ind).CompareNoCase(".JS")==0){
			bJS = true;
		}
	}
	CScriptObject* pObj = new CScriptObject();
	if(pObj){
		if(bJS){
			pObj->SetLanguage(LANGUAGE_JSCRIPT);
		}
		if(pObj->LoadScript(szScriptPath)){
			m_arrScripts.Add(pObj);
			return TRUE;
		}
		delete pObj;
	}
	return FALSE;
}

//
//
//
void CScriptManager::RemoveAllScripts(){
	for(int i = 0; i < m_arrScripts.GetSize(); i++){
		CScriptObject* pObj = (CScriptObject*)m_arrScripts.GetAt(i);
		if(pObj){
			pObj->Reset();
			delete pObj;
		}
	}
	m_arrScripts.RemoveAll();
}

//
//
//
CString CScriptManager::run_GetPluginDescription(int nScriptIndex){
	CScriptObject* pObj = GetScript(nScriptIndex);
	CString szRet;
	for(int i = 0; i < pObj->GetMethodsCount(); i++){
		CString curMethod = pObj->GetNameAt(i);
		// see if this method at this index is what we want
		if(curMethod.CompareNoCase("GetPluginDescription")==0){
			// this method has a return value
			if(pObj->GetMethodHasRetVal(i)){
				// this method has no param count
				if(pObj->GetMethodParamCount(i)==0){
					// now we execute the method
					// set the return value up
					VARIANT varRet; VariantInit(&varRet);
					// set the safearray up
					SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0/*LBOUND*/, 0/*param count*/);
					// finally run it
					if(pObj->RunProcedure(curMethod,&psa,&varRet)){
						szRet = (LPCTSTR)_bstr_t(varRet);
					}else{
						// TODO: failed to run this procedure
					}
					// clean up
					SafeArrayDestroy(psa);
					break;
				}
			}
		}
	}
	return szRet;
}
