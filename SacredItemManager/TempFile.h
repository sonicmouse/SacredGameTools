// TempFile.h: interface for the CTempFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TEMP_FILE_H__
#define __TEMP_FILE_H__

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

class CTempFile  
{
public:
	CTempFile();
	virtual ~CTempFile();

	char* GetFileName(){ return m_szTempFilePath; }
	bool CopyOverFile(const char* szToWhere);

private:
	void CreateTempFile();

private:
	char m_szTempFilePath[MAX_PATH];
};

#endif /* __TEMP_FILE_H__ */
