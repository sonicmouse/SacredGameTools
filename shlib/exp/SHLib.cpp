// SHLib.cpp: implementation of the CSHLib class.
//
//////////////////////////////////////////////////////////////////////

//#ifndef __NO_STDAFX
#include "stdafx.h"
//#endif
#include "SHLib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSHLib::CSHLib()
{
	m_hModule = LoadLibrary("shlib.dll");
	if(m_hModule){
		m_lpProcCreateHero = (__SHLIB_CREATE_HERO)GetProcAddress(m_hModule,"shlib_CreateHero");
		m_lpProcDestroyHero = (__SHLIB_DESTROY_HERO)GetProcAddress(m_hModule,"shlib_DestroyHero");
		m_lpProcCompileHero = (__SHLIB_COMPILE_HERO)GetProcAddress(m_hModule,"shlib_CompileHero");
		m_lpProcGetSectionsList = (__SHLIB_GET_SECTIONS_LIST)GetProcAddress(m_hModule,"shlib_GetSectionsList");
		m_lpProcDecompileHero = (__SHLIB_DECOMPILE_HERO)GetProcAddress(m_hModule,"shlib_DecompileHero");
		m_lpProcGetHeroSection = (__SHLIB_GET_HERO_SECTION)GetProcAddress(m_hModule,"shlib_GetHeroSection");
		m_lpProcSetHeroSection = (__SHLIB_SET_HERO_SECTION)GetProcAddress(m_hModule,"shlib_SetHeroSection");
		m_lpProcCompress = (__SHLIB_COMPRESS)GetProcAddress(m_hModule,"shlib_Compress");
		m_lpProcUncompress = (__SHLIB_UNCOMPRESS)GetProcAddress(m_hModule,"shlib_Uncompress");
	}else{
		m_lpProcCreateHero = NULL;
		m_lpProcDestroyHero = NULL;
		m_lpProcCompileHero = NULL;
		m_lpProcDecompileHero = NULL;
		m_lpProcGetSectionsList = NULL;
		m_lpProcGetHeroSection = NULL;
		m_lpProcSetHeroSection = NULL;
		m_lpProcCompress = NULL;
		m_lpProcUncompress = NULL;
	}
}

CSHLib::~CSHLib()
{
	if(m_hModule){
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

BOOL CSHLib::IsFullyLoaded(){
	return (m_hModule &&
			m_lpProcCreateHero &&
			m_lpProcDestroyHero &&
			m_lpProcCompileHero &&
			m_lpProcDecompileHero &&
			m_lpProcGetSectionsList &&
			m_lpProcGetHeroSection &&
			m_lpProcSetHeroSection &&
			m_lpProcCompress &&
			m_lpProcUncompress);
}

HHERO CSHLib::CreateHero(){
	return (*m_lpProcCreateHero)( );
}

void CSHLib::DestroyHero(HHERO hHero){
	(*m_lpProcDestroyHero)( hHero );
}

BOOL CSHLib::CompileHero(HHERO hHero, const char* szPath){
	return (*m_lpProcCompileHero)( hHero, szPath );
}

int CSHLib::DecompileHero(HHERO hHero, const char* szPath){
	return (*m_lpProcDecompileHero)( hHero, szPath );
}

int CSHLib::GetSectionsList(HHERO hHero, ULONG* lpulArray){
	return (*m_lpProcGetSectionsList)( hHero, lpulArray );
}

int CSHLib::GetHeroSection(HHERO hHero, ULONG ulSectionType, UCHAR** lpBuffer){
	return (*m_lpProcGetHeroSection)( hHero, ulSectionType, lpBuffer );
}

BOOL CSHLib::SetHeroSection(HHERO hHero, ULONG ulSectionType, const UCHAR* lpData, int nDataSize){
	return (*m_lpProcSetHeroSection)( hHero, ulSectionType, lpData, nDataSize );
}

int CSHLib::Compress(unsigned char* dest, unsigned long* destLen,
					 const unsigned char* source, unsigned long sourceLen, int level){
	return (*m_lpProcCompress)( dest, destLen, source, sourceLen, level );
}

int CSHLib::Uncompress(unsigned char* dest, unsigned long* destLen,
					   const unsigned char* source, unsigned long sourceLen){
	return (*m_lpProcUncompress)( dest, destLen, source, sourceLen );
}
