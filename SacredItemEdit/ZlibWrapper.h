// ZlibWrapper.h: interface for the CZlibWrapper class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ZLIB_WRAPPER_H__
#define __ZLIB_WRAPPER_H__

class CZlibWrapper  
{
public:
	CZlibWrapper(unsigned char* pBuffer, ULONG buffSize, ULONG inflatedSize = -1, bool inflate = true);
	virtual ~CZlibWrapper();

	void Reset();

	PST_BUFFER GetBuffer(){ return &m_stBuffer; }
	ULONG GetResult(){ return m_ulResult; }
	
private:
	ST_BUFFER m_stBuffer;
	ULONG m_ulResult;
};

#endif /* __ZLIB_WRAPPER_H__ */
