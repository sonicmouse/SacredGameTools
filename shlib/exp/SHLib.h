// SHLib.h: interface for the CSHLib class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SHLIB_H__
#define __SHLIB_H__

#include <windows.h>

#define ERROR_SHLIB_SUCCESS 0x00
#define ERROR_SHLIB_WRONGVERSION 0x01
#define ERROR_SHLIB_FILE 0x02

typedef ULONG HHERO;

typedef HHERO (CALLBACK* __SHLIB_CREATE_HERO) ( );
typedef void (CALLBACK* __SHLIB_DESTROY_HERO) ( HHERO hHero );
typedef bool (CALLBACK* __SHLIB_COMPILE_HERO) ( HHERO hHero, const char* szPath );
typedef int (CALLBACK* __SHLIB_DECOMPILE_HERO) ( HHERO hHero, const char* szPath );
typedef int (CALLBACK* __SHLIB_GET_SECTIONS_LIST) ( HHERO hHero, unsigned long* lpulArray);
typedef int (CALLBACK* __SHLIB_GET_HERO_SECTION) ( HHERO hHero, ULONG ulSectionType, UCHAR** lpBuffer );
typedef bool (CALLBACK* __SHLIB_SET_HERO_SECTION) ( HHERO hHero, ULONG ulSectionType, const UCHAR* lpData, int nDataSize );
typedef int (CALLBACK* __SHLIB_COMPRESS) ( unsigned char* dest, unsigned long* destLen,
							 const unsigned char* source, unsigned long sourceLen, int level );
typedef int (CALLBACK* __SHLIB_UNCOMPRESS) ( unsigned char* dest, unsigned long* destLen,
							 const unsigned char* source, unsigned long sourceLen );

class CSHLib  
{
public:
	CSHLib();
	virtual ~CSHLib();

	BOOL IsFullyLoaded();

public:

	HHERO CreateHero();
	void DestroyHero(HHERO hHero);
	BOOL CompileHero(HHERO hHero, const char* szPath);
	int DecompileHero(HHERO hHero, const char* szPath);
	int GetSectionsList(HHERO hHero, ULONG* lpulArray);
	int GetHeroSection(HHERO hHero, ULONG ulSectionType, UCHAR** lpBuffer);
	BOOL SetHeroSection(HHERO hHero, ULONG ulSectionType, const UCHAR* lpData, int nDataSize);
	int Compress(unsigned char* dest, unsigned long* destLen,
							 const unsigned char* source, unsigned long sourceLen, int level);
	int Uncompress(unsigned char* dest, unsigned long* destLen,
							 const unsigned char* source, unsigned long sourceLen);

private:
	HMODULE m_hModule;
	__SHLIB_CREATE_HERO m_lpProcCreateHero;
	__SHLIB_DESTROY_HERO m_lpProcDestroyHero;
	__SHLIB_COMPILE_HERO m_lpProcCompileHero;
	__SHLIB_DECOMPILE_HERO m_lpProcDecompileHero;
	__SHLIB_GET_SECTIONS_LIST m_lpProcGetSectionsList;
	__SHLIB_GET_HERO_SECTION m_lpProcGetHeroSection;
	__SHLIB_SET_HERO_SECTION m_lpProcSetHeroSection;
	__SHLIB_COMPRESS m_lpProcCompress;
	__SHLIB_UNCOMPRESS m_lpProcUncompress;
};

#endif /* __SHLIB_H__ */
