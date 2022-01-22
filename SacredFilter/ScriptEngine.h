// ScriptEngine.h: interface for the CScriptEngine class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SCRIPT_ENGINE_H__
#define __SCRIPT_ENGINE_H__

#import "msscript.ocx" no_namespace

#define LANGUAGE_JSCRIPT	_T("JScript")
#define LANGUAGE_VBSCRIPT	_T("VBScript")

#define METHODS_CASE_SENSITIVE true

class CScriptEngine  
{
public:
	CScriptEngine(LPCTSTR szScriptPath);
	virtual ~CScriptEngine();

	CStringArray* GetErrorArray(){ return &m_arrErrors; }

	// script interfaces
public:
	bool Run_GetPluginDescription(LPTSTR* pDesc);
	bool Run_OnPluginInit();
	bool Run_OnPluginTerminate();
	bool Run_OnConnectToServer(LPCTSTR szServerName);

private:
	void CreateScriptEngine();
	bool LoadScriptFromFile(LPCTSTR szScriptPath);
	void SetScriptLanguageByExt(LPCTSTR szScriptPath);
	LPCTSTR GrabLastEngineError();
	bool MethodExists(LPCTSTR szMethod, bool bHasRetVal, int nParamCount, bool bMethodCS = METHODS_CASE_SENSITIVE);

private:
	// script engine interface
	IScriptControlPtr m_pScript;
	// script path
	TCHAR m_szScriptPath[MAX_PATH];
	// error array
	CStringArray m_arrErrors;
};

#endif /* __SCRIPT_ENGINE_H__ */
