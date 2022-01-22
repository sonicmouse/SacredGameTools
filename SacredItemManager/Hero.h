// Hero.h: interface for the CHero class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __HERO_H__
#define __HERO_H__

#define SECTION_INVENTORY 0xC8
#define SECTION_PLACEMENT_BP 0xCA
#define SECTION_PLACEMENT_CH 0xCB
#define SECTION_HEROATAGLANCE 0xC3
#define SECTION_HERO 0xC7

#define HERO_NAME_SIZE (128/2)

#include "ItemManager.h"

class CHero  
{
public:
	CHero();
	virtual ~CHero();

	BOOL CompileHeroFile(const CString& szPath);
	BOOL LoadHeroFile(const CString& szPath);
	void CloseHeroFile();

	CString GetHeroName(){ return m_szName; }
	
	CItemManager* GetItemManager(){ return &m_ItemManager; }

private:
	CSIMApp* GetApp(){ return ((CSIMApp*)::AfxGetApp()); }
	CSHLib* GetSHLib(){ return GetApp()->GetSHLib(); }
	BOOL GetHeroSection(ULONG ulSection, PST_BUFFER lpBuffer);
	void SetHeroName(PST_BUFFER lpBuffer);

private:
	HHERO m_hHero;
	CItemManager m_ItemManager;
	CString m_szName;
};

#endif /* __HERO_H__ */
