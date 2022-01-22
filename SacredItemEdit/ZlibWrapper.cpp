// ZlibWrapper.cpp: implementation of the CZlibWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "ZlibWrapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef ULONG (CALLBACK* __ZLIB_UNCOMPRESS)(unsigned char* dst,PULONG pdstsize,const unsigned char* src,ULONG srcsize);
typedef ULONG (CALLBACK* __ZLIB_COMPRESS2)(unsigned char* dst,PULONG pdstsize,const unsigned char* src,ULONG srcsize,int level);

CZlibWrapper::CZlibWrapper(unsigned char* pBuffer, ULONG buffSize, ULONG inflatedSize/* = -1*/,bool inflate/* = true*/)
{
	m_stBuffer.buf = NULL;
	m_stBuffer.size = 0;
	m_ulResult = -10;

	HMODULE hZlib = LoadLibrary("zlib.dll");
	if(hZlib){
		if(inflate){
			__ZLIB_UNCOMPRESS p = (__ZLIB_UNCOMPRESS)GetProcAddress(hZlib,"uncompress");
			if(p){
				m_stBuffer.size = (inflatedSize!=-1?inflatedSize:buffSize*20);
				m_stBuffer.buf = new unsigned char[m_stBuffer.size];
				m_ulResult = (*p)( m_stBuffer.buf, &m_stBuffer.size, pBuffer, buffSize );
			}else{
				// no uncompress proc
			}
		}else{
			__ZLIB_COMPRESS2 p = (__ZLIB_COMPRESS2)GetProcAddress(hZlib,"compress2");
			if(p){
				ULONG dstSize = buffSize*2+12;
				unsigned char* dstBuffer = new unsigned char[dstSize];
				m_ulResult = (*p)( dstBuffer, &dstSize, pBuffer, buffSize, (inflatedSize==-1?9:inflatedSize) );
				if(m_ulResult == 0){
					m_stBuffer.size = dstSize;
					m_stBuffer.buf = new unsigned char[dstSize];
					memcpy(m_stBuffer.buf,dstBuffer,dstSize);
				}else{
					// deflate failed
				}
				SAFE_DELETE(dstBuffer);
			}else{
				// no compress2 proc
			}
		}
		FreeLibrary(hZlib);
	}else{
		// no zlib library
	}
}

CZlibWrapper::~CZlibWrapper()
{
	Reset();
}

void CZlibWrapper::Reset(){
	SAFE_DELETE(m_stBuffer.buf);
	m_stBuffer.size = 0;
	m_ulResult = -10;
}
