// Hero.h: interface for the CHero class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __HERO_H__
#define __HERO_H__

#include "PointerArray.h"

#define ERROR_SHLIB_SUCCESS 0x00
#define ERROR_SHLIB_WRONGVERSION 0x01
#define ERROR_SHLIB_FILE 0x02

#define HERO_HEADER7 0x07484D41
#define HERO_HEADER27 0x1B484D41

#define COMPRESSED_CODE 0xBAADC0DE

// 1 byte alignment
#pragma pack(push,1)

// hero section description header
typedef struct _tag_section_desc{
	unsigned long type;
	unsigned long offset;
	unsigned long sizeinflated;
} SECTION_DESC,*PSECTION_DESC;

typedef struct _tag_section{
	SECTION_DESC sd;
	bool compressed;
	unsigned char* pdata;
	unsigned long datalen;
} SECTION,*PSECTION;

// back to 8 byte alignment
#pragma pack(pop)

class CHero  
{
public:
	CHero();
	virtual ~CHero();
	
	bool SetHeroSection(unsigned long ulSectionType, const unsigned char* lpData, int nDataSize);
	int GetHeroSection(unsigned long ulSectionType, unsigned char** lpBuffer);
	int GetSectionsList(unsigned long* lpulArray);
	int DecompileHero(const char* szPath);
	bool CompileHero(const char* szPath);

private:
	void ResetSections();

private:
	unsigned char m_ucFluffHeader[0xF8];
	CPointerArray m_arrSections;
	unsigned long m_ulHeroVersion;

};

#endif /* __HERO_H__ */
