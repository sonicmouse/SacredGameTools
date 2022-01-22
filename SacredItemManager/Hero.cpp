// Hero.cpp: implementation of the CHero class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemManager.h"
#include "Hero.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHero::CHero()
{
	m_hHero = NULL;
}

CHero::~CHero()
{
	CloseHeroFile();
}

BOOL CHero::LoadHeroFile(const CString& szPath){
	// close off old hero file
	CloseHeroFile();
	// create a new hero
	m_hHero = GetSHLib()->CreateHero();
	// temp storage
	ST_BUFFER invBuf,placementBp,placementCh,heroAag,shero;
	// decompile it
	bool bDecompiled = false;
	int rcDecompile = ERROR_SHLIB_SUCCESS;
	if((rcDecompile=GetSHLib()->DecompileHero(m_hHero,szPath)) == ERROR_SHLIB_SUCCESS){
		if(GetHeroSection(SECTION_INVENTORY,&invBuf)){
			if(GetHeroSection(SECTION_PLACEMENT_BP,&placementBp)){
				if(GetHeroSection(SECTION_PLACEMENT_CH,&placementCh)){
					if(GetHeroSection(SECTION_HEROATAGLANCE,&heroAag)){
						if(GetHeroSection(SECTION_HERO,&shero)){
							bDecompiled = true;
						}else{
							// failed to get hero info
						}
					}else{
						// failed to get ataglance info
					}
				}else{
					// failed to get chest placement
				}
			}else{
				// failed to get backpack placement
			}
		}else{
			// failed to get inventory
		}
	}else{
		// failed to decompile hero
		if(rcDecompile == ERROR_SHLIB_WRONGVERSION){
			MessageBox(AfxGetMainWnd()->GetSafeHwnd(),"This hero file is the wrong version.\nThis program only works with Sacred Plus.","Error...",MB_OK|MB_ICONHAND);
		}else{
			// file error
			MessageBox(AfxGetMainWnd()->GetSafeHwnd(),"There was a file error when trying to decompile this hero.\nMake sure it is not in use.", "Error...", MB_OK|MB_ICONHAND);
		}
	}
	// did we make it?
	if(!bDecompiled){
		// free memory and die
		CloseHeroFile();
		SAFE_DELETE(invBuf.buf);
		SAFE_DELETE(placementBp.buf);
		SAFE_DELETE(placementCh.buf);
		SAFE_DELETE(heroAag.buf);
		SAFE_DELETE(shero.buf);
		return FALSE;
	}

	// now we set up the item manager
	bDecompiled = false;
	if(m_ItemManager.LoadPlacement(SECTION_PLACEMENT_BP,&placementBp)){
		if(m_ItemManager.LoadPlacement(SECTION_PLACEMENT_CH,&placementCh)){
			bDecompiled = true;
		}else{
			// failed to load placement chest
		}
	}else{
		// failed to load placement backpack
	}
	SAFE_DELETE(placementBp.buf);
	SAFE_DELETE(placementCh.buf);
	if(!bDecompiled){
		CloseHeroFile();
		SAFE_DELETE(heroAag.buf);
		SAFE_DELETE(shero.buf);
		SAFE_DELETE(invBuf.buf);
		return FALSE;
	}

	// now we load invBuf
	bDecompiled = false;
	if(m_ItemManager.LoadItemInventory(&invBuf)){
		bDecompiled = true;
	}else{
		// failed to load item inventory
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(),"There was an oddball error when trying to load this hero.\nThis usually happens when the hero has a horse.\nKill your damn horse... you dont need it :)","Error...",MB_ICONHAND|MB_OK);
	}
	SAFE_DELETE(invBuf.buf);
	if(!bDecompiled){
		CloseHeroFile();
		SAFE_DELETE(heroAag.buf);
		SAFE_DELETE(shero.buf);
		return FALSE;
	}

	// now we load the item name stuff
	SetHeroName(&heroAag);
	SAFE_DELETE(heroAag.buf);

	// now we load the hero details...
	// TODO: get EXP?
	
	//ULONG* pGold = (ULONG*)&shero.buf[0x3D3];
	//*pGold = 0x7FFFFFF0;
	
	// experience!
	//pGold = (ULONG*)&shero.buf[0x399];
	//*pGold = 0xABDBECFF;

	//GetSHLib()->SetHeroSection(m_hHero,SECTION_HERO,shero.buf,shero.size);

	/*
	FILE* f = fopen("c:\\shero.bin","wb");
	if(f){
		fwrite(shero.buf,shero.size,1,f);
		fclose(f);
	}
	*/

	SAFE_DELETE(shero.buf);

	// done...
	return TRUE;
}

BOOL CHero::CompileHeroFile(const CString& szPath){
	if(!m_hHero){ return FALSE; }
	
	ST_BUFFER pPlaceCH={0,0};
	ST_BUFFER pPlaceBP={0,0};
	ST_BUFFER pInv={0,0};

	bool bCompiled = false;
	if(GetItemManager()->CompileItemInventory(&pInv)){
		if(GetItemManager()->CompilePlacement(SECTION_PLACEMENT_BP,&pPlaceBP)){
			if(GetItemManager()->CompilePlacement(SECTION_PLACEMENT_CH,&pPlaceCH)){
				bCompiled = true;
			}else{
				// failed to compile chest placement
			}
		}else{
			// failed to compile backpack placement
		}
	}else{
		// failed to compile item inventory
	}
	if(bCompiled){
		GetSHLib()->SetHeroSection(m_hHero,SECTION_INVENTORY,pInv.buf,pInv.size);
		GetSHLib()->SetHeroSection(m_hHero,SECTION_PLACEMENT_BP,pPlaceBP.buf,pPlaceBP.size);
		GetSHLib()->SetHeroSection(m_hHero,SECTION_PLACEMENT_CH,pPlaceCH.buf,pPlaceCH.size);
	}

	SAFE_DELETE(pPlaceCH.buf);
	SAFE_DELETE(pPlaceBP.buf);
	SAFE_DELETE(pInv.buf);

	return (bCompiled?GetSHLib()->CompileHero(m_hHero,szPath):FALSE);
}

void CHero::CloseHeroFile(){
	m_ItemManager.Reset();
	if(m_hHero){
		GetSHLib()->DestroyHero(m_hHero);
	}
	m_hHero = NULL;
}

BOOL CHero::GetHeroSection(ULONG ulSection, PST_BUFFER lpBuffer){
	lpBuffer->buf = NULL;
	lpBuffer->size = GetSHLib()->GetHeroSection(m_hHero,ulSection,NULL);
	if(lpBuffer->size != -1){
		lpBuffer->buf = new UCHAR[lpBuffer->size];
		if(lpBuffer->buf){
			if(GetSHLib()->GetHeroSection(m_hHero,ulSection,&lpBuffer->buf)!=-1){
				return TRUE;
			}else{
				// no such animal
			}
		}else{
			// out of memory
		}
	}else{
		// no such animal
	}
	// failed
	lpBuffer->size = 0;
	SAFE_DELETE(lpBuffer->buf);
	return FALSE;
}

void CHero::SetHeroName(PST_BUFFER lpBuffer){
	// clear name
	m_szName.Empty();
	// move up to name
	UCHAR* p = &lpBuffer->buf[sizeof(ULONG)*2];
	// create a name buffer
	char nameBuff[HERO_NAME_SIZE];
	// convert name buffer from unicode to ansi
	int rc = WideCharToMultiByte(CP_ACP,0,(WCHAR*)p,-1,nameBuff,HERO_NAME_SIZE,NULL,NULL);
	// if it failed, give up
	if(!rc){
		m_szName = "Unknown";
	}
	// go thru and kill special chars
	for(int i = 0; i < rc-1; i++){
		if(nameBuff[i]=='\\'){
			if(nameBuff[i+1]=='c'){
				// check for 1 ulong
				if(rc-i >= 11){ // 8 + 3
					i += 9;
				}
			}else if(nameBuff[i+1]=='g'){
				// check for 2 ULONGs 
				if(rc-i >= 19){ // 16 + 3
					i += 17;
				}
			}else{
				// tack it on
				m_szName += nameBuff[i];
			}
		}else{
			// tack it on
			m_szName += nameBuff[i];
		}
	}
}

