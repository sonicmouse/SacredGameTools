// SacredDir.h: interface for the CSacredDir class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SACRED_DIRECTORY_H__
#define __SACRED_DIRECTORY_H__

class CSacredDir  
{
public:
	CSacredDir();
	virtual ~CSacredDir();

	BOOL FindSacredDirectory();
	CString GetSacredDirectory(){ return m_szSacredDir; }

private:
	int GetRegStringValue(HKEY hKey, const char* szIdentifier, char* szBuffer, int nBufferLen);

private:
	CString m_szSacredDir;

};

#endif /* __SACRED_DIRECTORY_H__ */
