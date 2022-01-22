/*
	DLL To decompile and recompile Sacred Heros (PAX) files
	Author: Andrew Heinlein
	Date: Wednesday, January 19th, 2005

	Final Build 1.0.0.1:
		Thursday, January 20th, 2005
	Final Build 1.0.0.2:
		Monday, May 23rd, 2005 [Underworld Expansion]

*/

//
// memory leak detection
//
#ifdef _DEBUG
	#define CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

//
// includes
//
#include <windows.h>
#include <time.h>
#include "HHEROArray.h"
#include "zlib.h"

//
// globals
//
HINSTANCE g_hInstance = NULL;
CHHEROArray g_heroArray;

//
// dll entry
//
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved){
	switch(dwReason){
		case DLL_PROCESS_ATTACH:
			{
				// memory leak detection
#ifdef _DEBUG
				_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
				srand(time(NULL));
				g_hInstance = hInstance;
			}
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}
	// successful entry
	return TRUE;
}

////////////////////////////////////////////////
// exports
////////////////////////////////////////////////

HHERO __stdcall shlib_CreateHero(){
	return g_heroArray.AllocateHero();
}

void __stdcall shlib_DestroyHero(HHERO hHero){
	g_heroArray.KillHero(hHero);
}

bool __stdcall shlib_CompileHero(HHERO hHero, const char* szPath){
	CHero* pHero = g_heroArray.GetHero(hHero);
	return (pHero?pHero->CompileHero(szPath):false);
}

int __stdcall shlib_DecompileHero(HHERO hHero, const char* szPath){
	CHero* pHero = g_heroArray.GetHero(hHero);
	return (pHero?pHero->DecompileHero(szPath):false);
}

int __stdcall shlib_GetSectionsList(HHERO hHero, unsigned long* lpulArray){
	CHero* pHero = g_heroArray.GetHero(hHero);
	return (pHero?pHero->GetSectionsList(lpulArray):0);
}

int __stdcall shlib_GetHeroSection(HHERO hHero, ULONG ulSectionType, UCHAR** lpBuffer){
	CHero* pHero = g_heroArray.GetHero(hHero);
	return (pHero?pHero->GetHeroSection(ulSectionType,lpBuffer):false);
}

bool __stdcall shlib_SetHeroSection(HHERO hHero, ULONG ulSectionType, const UCHAR* lpData, int nDataSize){
	CHero* pHero = g_heroArray.GetHero(hHero);
	return (pHero?pHero->SetHeroSection(ulSectionType,lpData,nDataSize):false);
}

int __stdcall shlib_Compress(unsigned char* dest, unsigned long* destLen,
							 const unsigned char* source, unsigned long sourceLen, int level){
	return compress2(dest,destLen,source,sourceLen,level);
}

int __stdcall shlib_Uncompress(unsigned char* dest, unsigned long* destLen,
							 const unsigned char* source, unsigned long sourceLen){
	return uncompress(dest,destLen,source,sourceLen);
}
