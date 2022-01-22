// ZlibWrapper.cpp: implementation of the CZlibWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "ZlibWrapper.h"
#include "zlib.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZlibWrapper::CZlibWrapper(unsigned char* pBuffer, unsigned long buffSize, unsigned long inflatedSize/* = ~0*/,bool inflate/* = true*/){
	m_pBuffer = 0;
	m_ulBufferSize = 0;
	m_ulResult = -10;


	if(inflate){
		m_ulBufferSize = (inflatedSize!=~0?inflatedSize:buffSize*20);
		m_pBuffer = new unsigned char[m_ulBufferSize];
		if(m_pBuffer){
			m_ulResult = uncompress(m_pBuffer, &m_ulBufferSize, pBuffer, buffSize);
		}else{
			m_ulBufferSize = 0;
		}
	}else{
		unsigned long dstSize = buffSize*2+12;
		unsigned char* dstBuffer = new unsigned char[dstSize];
		if(dstBuffer){
			m_ulResult = compress2( dstBuffer, &dstSize, pBuffer, buffSize, (inflatedSize==~0?9:inflatedSize) );
			if(m_ulResult == 0){
				m_pBuffer = new unsigned char[dstSize];
				if(m_pBuffer){
					m_ulBufferSize = dstSize;
					memcpy(m_pBuffer,dstBuffer,dstSize);
				}
			}else{
				// deflate failed
			}
			delete dstBuffer;
		}
	}
}

CZlibWrapper::~CZlibWrapper()
{
	Reset();
}

void CZlibWrapper::Reset(){
	if(m_pBuffer){
		delete m_pBuffer;
	}
	m_ulBufferSize = 0;
	m_ulResult = -10;
}
