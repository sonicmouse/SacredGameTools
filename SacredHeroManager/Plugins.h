// Plugins.h: interface for the CPlugins class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PLUGINS_H__
#define __PLUGINS_H__

typedef void (CALLBACK* __PLUGIN_LIBDETAILS)( char* szLibName,
											  char* szAuthor );

typedef void (CALLBACK* __PLUGIN_ENTRY)( HWND hWndParentApp,
										 unsigned char* lpucHeroData,
										 unsigned long ulHeroDataSize,
										 const char* szHeroPath,
										 unsigned long* pulReserved );

typedef struct _tag_plugin{
	char title[256];		// title of the DLL
	char author[256];		// author of the DLL
	char path[MAX_PATH];	// path to the DLL
	HMODULE mod;			// handle to the DLL
	__PLUGIN_LIBDETAILS procDetails;	// proc to lib details
	__PLUGIN_ENTRY procEntry;			// proc to entry
	int id;
} PLUGIN, *LPPLUGIN;

//
// class
//
class CPlugins  
{
public:
	CPlugins();
	virtual ~CPlugins();

	void FreePlugins();
	int LoadPlugins(const CString& szDir);
	int GetPluginCount(){ return m_arrPlugins.GetSize(); }

	void SetPluginId(int nIndex, int nId);
	int GetPluginId(int nIndex);

	CString GetPluginTitle(int nId);
	CString GetPluginAuthor(int nId);
	void RunPluginEntry( int nId,
						 HWND hWndParentApp,
						 unsigned char* lpucHeroData,
						 unsigned long ulHeroDataSize,
						 const char* szHeroPath,
						 unsigned long* pulReserved = NULL );

private:
	CPtrArray m_arrPlugins;
};

#endif /* __PLUGINS_H__ */
