// HHEROArray.h: interface for the CHHEROArray class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __HHERO_ARRAY_H__
#define __HHERO_ARRAY_H__

#include "PointerArray.h"
#include "Hero.h"

typedef unsigned long HHERO;

typedef struct _tag_hero_entry{
	HHERO heroId;
	CHero* phero;
} HERO_ENTRY,*PHERO_ENTRY;

class CHHEROArray  
{
public:
	CHHEROArray();
	virtual ~CHHEROArray();

	HHERO AllocateHero();
	CHero* GetHero(HHERO hHero);
	void KillHero(HHERO hHero);

private:
	void FreeArray();
	PHERO_ENTRY FindHero(HHERO hHero);
	HHERO CreateUniqueId();

private:
	CPointerArray m_arrHeros;

};

#endif /* __HHERO_ARRAY_H__ */
