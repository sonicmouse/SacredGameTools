// ScriptManager.h: interface for the CScriptManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

class CScriptManager  
{
public:
	CScriptManager();
	virtual ~CScriptManager();

	int SetScriptDirectory(const CString& szDir);

	BOOL AddScript(const CString& szScriptPath);
	void RemoveAllScripts();

	int GetScriptCount(){ return m_arrScripts.GetSize(); }
	CScriptObject* GetScript(int nIndex){ return (CScriptObject*)m_arrScripts.GetAt(nIndex); }

	// i guess...
	CString run_GetPluginDescription(int nScriptIndex);

private:
	CPtrArray m_arrScripts;

};

#endif /* __SCRIPT_MANAGER_H__ */
