// Section.h: interface for the CSection class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __HERO_SECTION_H__
#define __HERO_SECTION_H__

class CSection  
{
public:
	CSection(PSECTION_DESC pSectionDesc, PST_BUFFER pBuffer, BOOL bInflated);
	virtual ~CSection();

	BOOL WasInflated(){ return m_bInflated; }
	PSECTION_DESC GetSectionDescriptor(){ return &m_SectionDesc; }
	PST_BUFFER GetBuffer(){ return &m_Buffer; }

protected:
	ST_BUFFER m_Buffer;

private:
	SECTION_DESC m_SectionDesc;
	BOOL m_bInflated;
};

#endif /* __HERO_SECTION_H__ */
