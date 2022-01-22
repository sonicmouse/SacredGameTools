// HHEROArray.cpp: implementation of the CHHEROArray class.
//
//////////////////////////////////////////////////////////////////////

#include "HHEROArray.h"
#include <stdlib.h> // for rand()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHHEROArray::CHHEROArray()
{

}

CHHEROArray::~CHHEROArray()
{
	FreeArray();
}

HHERO CHHEROArray::AllocateHero(){
	// create an entry
	PHERO_ENTRY pEntry = new HERO_ENTRY;
	if(!pEntry){ return 0; }
	// create a hero
	pEntry->phero = new CHero();
	if(!pEntry->phero){
		delete pEntry;
		return 0;
	}
	// create an id
	pEntry->heroId = CreateUniqueId();
	// add it to array
	m_arrHeros.Add(pEntry);
	// return id
	return pEntry->heroId;
}

void CHHEROArray::KillHero(HHERO hHero){
	for(int i = 0; i < m_arrHeros.GetSize(); i++){
		PHERO_ENTRY pEntry = (PHERO_ENTRY)m_arrHeros.GetAt(i);
		if(pEntry->heroId == hHero){
			delete pEntry->phero;
			delete pEntry;
			m_arrHeros.RemoveAt(i);
			break;
		}
	}
}

CHero* CHHEROArray::GetHero(HHERO hHero){
	PHERO_ENTRY pEntry = FindHero(hHero);
	return (pEntry?pEntry->phero:0);
}

void CHHEROArray::FreeArray(){
	while(m_arrHeros.GetSize()){
		PHERO_ENTRY pEntry = (PHERO_ENTRY)m_arrHeros.GetAt(0);
		delete pEntry->phero;
		delete pEntry;
		m_arrHeros.RemoveAt(0);
	}
}

PHERO_ENTRY CHHEROArray::FindHero(HHERO hHero){
	for(int i = 0; i < m_arrHeros.GetSize(); i++){
		PHERO_ENTRY pEntry = (PHERO_ENTRY)m_arrHeros.GetAt(i);
		if(pEntry->heroId == hHero){
			return pEntry;
		}
	}
	return 0;
}

HHERO CHHEROArray::CreateUniqueId(){
	HHERO hId = 0;
	while(true){
		hId = ((rand()%RAND_MAX) << 16) + rand()%RAND_MAX;
		bool bFound = false;
		if(hId != 0){
			for(int i = 0; i < m_arrHeros.GetSize(); i++){
				PHERO_ENTRY pEntry = (PHERO_ENTRY)m_arrHeros.GetAt(i);
				if(pEntry->heroId == hId){
					bFound = true;
					break;
				}
			}
		}
		if(!bFound){ break; }
	}
	return hId;
}
