// ScriptEngine.cpp: implementation of the CScriptEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredFilter.h"
#include "ScriptEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptEngine::CScriptEngine(LPCTSTR szScriptPath){
	CreateScriptEngine();
	// set the script path
	_tcscpy(m_szScriptPath, szScriptPath);
	// try to load the script file...
	if(!LoadScriptFromFile(szScriptPath)){
		// we have an error trying to load this file
		m_arrErrors.Add(_T("Unable to load this script!"));
		LPCTSTR err = GrabLastEngineError();
		if(_tcslen(err)){ m_arrErrors.Add(err); }
	}
}

CScriptEngine::~CScriptEngine(){
	m_pScript->Reset();
}

//
//
//
void CScriptEngine::CreateScriptEngine(){
	HRESULT hr = m_pScript.CreateInstance(__uuidof(ScriptControl));
	// will throw an exception if creation fails
	_com_util::CheckError(hr);

	// we will not get here if creation fails...

	// set a default language
	m_pScript->PutLanguage(_bstr_t(LANGUAGE_VBSCRIPT));
	// allow msgboxs and inputboxes
	m_pScript->PutAllowUI(VARIANT_TRUE);
	// allow activex object creation in script
	m_pScript->PutUseSafeSubset(VARIANT_FALSE);
}

//
//
//
bool CScriptEngine::LoadScriptFromFile(LPCTSTR szScriptPath){
	
	SetScriptLanguageByExt(szScriptPath);

	bool bSuccess = false;
	FILE* f = fopen(szScriptPath,"rb");
	if(f){
		fseek(f,0,SEEK_END);
		long nLen = ftell(f);
		rewind(f);
		char* fileGuts = new char[nLen+1];
		if(fileGuts){
			fileGuts[nLen]=0;
			if(fread(fileGuts,nLen,1,f)==1){
				// now, add this code to the script control
				try{
					m_pScript->AddCode(_bstr_t(fileGuts));
					bSuccess = true;
				} catch(...) {
					// do nothing here for now...
					// last error is stored in MSSCRIPT.OCX
				}
			}else{
				// unable to read from file
			}
			delete fileGuts;
		}else{
			// out of memory on fileGuts
		}
		fclose(f);
	}else{
		// fopen() failed
	}
	return bSuccess;
}

//
//
//
void CScriptEngine::SetScriptLanguageByExt(LPCTSTR szScriptPath){
	m_pScript->Reset();
	TCHAR* pExt = _tcsrchr(szScriptPath,'.');
	try{
		if(pExt){
			if(_tcsicmp(pExt,_T(".VBS"))==0){
				// language is VBScript
				m_pScript->PutLanguage(_bstr_t(LANGUAGE_VBSCRIPT));
			}else if(_tcsicmp(pExt,_T(".JS"))==0){
				// language is JScript
				m_pScript->PutLanguage(_bstr_t(LANGUAGE_JSCRIPT));
			}else{
				// language will default to VBScript (change if you want)
				m_pScript->PutLanguage(_bstr_t(LANGUAGE_VBSCRIPT));
			}
		}else{
			// language will default to VBScript (change if you want)
			m_pScript->PutLanguage(_bstr_t(LANGUAGE_VBSCRIPT));
		}
	} catch(...){
#ifdef _DEBUG
		ASSERT(false);
#endif
	}
}

//
//
//
LPCTSTR CScriptEngine::GrabLastEngineError(){
	static TCHAR szError[1024];

	szError[0]=0;

	try{
		IScriptErrorPtr pError = m_pScript->GetError();
		if(pError){
			_bstr_t err = _bstr_t("[Desc: ") + pError->GetDescription() + _bstr_t("]") + 
						  _bstr_t("[Line: ") + _bstr_t(pError->GetLine()) + _bstr_t("]") +
						  _bstr_t("[Text: ") + pError->GetText() + _bstr_t("]");

			_tcscpy(szError,err);

			pError->Clear();
		}else{
			// no error object
#ifdef _DEBUG
			ASSERT(false);
#endif
		}
	} catch(...){
#ifdef _DEBUG
		ASSERT(false);
#endif
	}

	return szError;
}

//
//
//
bool CScriptEngine::MethodExists(LPCTSTR szMethod,
								 bool bHasRetVal, 
								 int nParamCount, 
								 bool bMethodCS/* = METHODS_CASE_SENSITIVE*/){
	bool bSuccess = false;
	try{
		IScriptProcedureCollectionPtr pIProcedures = m_pScript->GetProcedures();

		long nCount = pIProcedures->GetCount();

		for(long i=1; i <= nCount; i++){
			IScriptProcedurePtr pIProcPtr = pIProcedures->GetItem( _variant_t(i) );
			bool bFound = false;
			if(bMethodCS){
				bFound = _tcscmp(pIProcPtr->GetName(),szMethod)==0;
			}else{
				bFound = _tcsicmp(pIProcPtr->GetName(),szMethod)==0;
			}
			if(bFound){
				if(pIProcPtr->GetNumArgs()==nParamCount){
					if( (pIProcPtr->GetHasReturnValue() && bHasRetVal) || 
						(!pIProcPtr->GetHasReturnValue() && !bHasRetVal) ){
						bSuccess = true;
						break;
					}
				}
			}
		}
	}catch(...){
		// failed somewhere
#ifdef _DEBUG
		ASSERT(false);
#endif
	}
	return bSuccess;
}

//
// script interfaces
//
#define GETPLUGINDESCRIPTION _T("GetPluginDescription")
bool CScriptEngine::Run_GetPluginDescription(LPTSTR* pDesc){
	bool bSuccess = false;
	if(MethodExists(GETPLUGINDESCRIPTION,true,0)){
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0/*LBOUND*/, 0/*param count*/);
		if(psa){
			try{
				VARIANT varRet = m_pScript->Run(_bstr_t(GETPLUGINDESCRIPTION),&psa);
				if(pDesc){
					*pDesc = new TCHAR[_tcslen(_bstr_t(varRet))+1];
					_tcscpy(*pDesc,_bstr_t(varRet));
				}
				bSuccess = true;
			}catch(...){
				// failed to run the script
				m_arrErrors.Add(_T("Failed to run GetPluginDescription()"));
				LPCTSTR err = GrabLastEngineError();
				if(_tcslen(err)){ m_arrErrors.Add(err); }
			}
			SafeArrayDestroy(psa);
		}else{
			// failed to allocate a safe array
#ifdef _DEBUG
			ASSERT(FALSE);
#endif
		}
	}
	return bSuccess;
}

#define ONPLUGININIT _T("OnPluginInit")
bool CScriptEngine::Run_OnPluginInit(){
	bool bSuccess = false;
	if(MethodExists(ONPLUGININIT,false,0)){
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0/*LBOUND*/, 0/*param count*/);
		if(psa){
			try{
				m_pScript->Run(_bstr_t(ONPLUGININIT),&psa);
				bSuccess = true;
			}catch(...){
				// failed to run the script
				m_arrErrors.Add(_T("Failed to run OnPluginInit()"));
				LPCTSTR err = GrabLastEngineError();
				if(_tcslen(err)){ m_arrErrors.Add(err); }
			}
			SafeArrayDestroy(psa);
		}else{
			// failed to allocate a safe array
#ifdef _DEBUG
			ASSERT(FALSE);
#endif
		}
	}
	return bSuccess;
}

#define ONPLUGINTERMINATE _T("OnPluginTerminate")
bool CScriptEngine::Run_OnPluginTerminate(){
	bool bSuccess = false;
	if(MethodExists(ONPLUGINTERMINATE,false,0)){
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0/*LBOUND*/, 0/*param count*/);
		if(psa){
			try{
				m_pScript->Run(_bstr_t(ONPLUGINTERMINATE),&psa);
				bSuccess = true;
			}catch(...){
				// failed to run the script
				m_arrErrors.Add(_T("Failed to run OnPluginTerminate()"));
				LPCTSTR err = GrabLastEngineError();
				if(_tcslen(err)){ m_arrErrors.Add(err); }
			}
			SafeArrayDestroy(psa);
		}else{
			// failed to allocate a safe array
#ifdef _DEBUG
			ASSERT(FALSE);
#endif
		}
	}
	return bSuccess;
}

#define ONCONNECTTOSERVER _T("OnConnectToServer")
bool CScriptEngine::Run_OnConnectToServer(LPCTSTR szServerName){
	bool bSuccess = false;
	if(MethodExists(ONCONNECTTOSERVER,false,0)){
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0/*LBOUND*/, 1/*param count*/);
		if(psa){
			// TODO: add our param to the safe array
			VARIANT varParam0; VariantInit(&varParam0);
			long lIndicies = 0;
			SafeArrayPutElement(psa,&lIndicies,&varParam0);

			try{
				m_pScript->Run(_bstr_t(ONCONNECTTOSERVER),&psa);
				bSuccess = true;
			}catch(...){
				// failed to run the script
				m_arrErrors.Add(_T("Failed to run OnConnectToServer()"));
				LPCTSTR err = GrabLastEngineError();
				if(_tcslen(err)){ m_arrErrors.Add(err); }
			}
			SafeArrayDestroy(psa);
		}else{
			// failed to allocate a safe array
#ifdef _DEBUG
			ASSERT(FALSE);
#endif
		}
	}
	return bSuccess;
}
