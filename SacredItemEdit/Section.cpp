// Section.cpp: implementation of the CSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "Section.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSection::CSection(PSECTION_DESC pSectionDesc, PST_BUFFER pBuffer, BOOL bInflated)
{
	m_bInflated = bInflated;

	m_SectionDesc.offset = pSectionDesc->offset;
	m_SectionDesc.sizeinflated = pSectionDesc->sizeinflated;
	m_SectionDesc.type = pSectionDesc->type;

	m_Buffer.buf = new unsigned char[pBuffer->size];
	m_Buffer.size = pBuffer->size;
	memcpy(m_Buffer.buf,pBuffer->buf,pBuffer->size);
}

CSection::~CSection()
{
	SAFE_DELETE(m_Buffer.buf);
}
