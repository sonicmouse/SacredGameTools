// TempFile.cpp: implementation of the CTempFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemManager.h"
#include "TempFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTempFile::CTempFile()
{
	while(true){
		GetTempPath(MAX_PATH,m_szTempFilePath);

		if(m_szTempFilePath[strlen(m_szTempFilePath)-1] != '\\'){
			strcat(m_szTempFilePath,"\\");
		}

		const char* alpha_const = "bcdfghjklmnpqrstvwx";
		const char* alpha_vowel = "aeiouy";
		const char* num = "1234567890";

		char rndStr[30];
		memset(rndStr,0,30);
		bool even = true;
		for(int i = 0; i < 20; i++){
			if(even)
				rndStr[i]=alpha_vowel[rand()%strlen(alpha_vowel)];
			else
				rndStr[i]=alpha_const[rand()%strlen(alpha_const)];
			even=!even;
		}

		rndStr[i++] = num[rand()%strlen(num)];
		rndStr[i] = num[rand()%strlen(num)];

		strcat(m_szTempFilePath,rndStr);
		strcat(m_szTempFilePath,".tmp~");
		
		FILE* f = fopen(m_szTempFilePath,"r");
		if(f){
			fclose(f);
		}else{
			break;
		}
	}

	FILE* f = fopen(m_szTempFilePath,"w");
	if(f){ fclose(f); }
}

CTempFile::~CTempFile()
{
	_unlink(m_szTempFilePath);
}

bool CTempFile::CopyOverFile(const char* szToWhere){
	bool bSuccess = false;
	FILE* fdest = fopen(szToWhere,"wb");
	if(fdest){
		FILE* fsrc = fopen(m_szTempFilePath,"rb");
		if(fsrc){
			fseek(fsrc,0,SEEK_END);
			unsigned long ulSrcFileLen = ftell(fsrc);
			rewind(fsrc);
			unsigned char* pSrcBuffer = new unsigned char[ulSrcFileLen];
			if(pSrcBuffer){
				if(fread(pSrcBuffer,ulSrcFileLen,1,fsrc)==1){
					if(fwrite(pSrcBuffer,ulSrcFileLen,1,fdest)==1){
						bSuccess = true;
					}else{
						//failed to write to dest
					}
				}else{
					// failed to read src file
				}
				delete pSrcBuffer;
			}else{
				// out of memory
			}
			fclose(fsrc);
		}else{
			// count open source
		}
		fflush(fdest);
		fclose(fdest);
	}else{
		// couldnt open dest file
	}
	return bSuccess;
}
