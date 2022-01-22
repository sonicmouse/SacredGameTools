// ZlibWrapper.h: interface for the CZlibWrapper class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ZLIB_WRAPPER_H__
#define __ZLIB_WRAPPER_H__

#ifndef _DEBUG
#pragma comment(lib,"../zlib122\\projects\\visualc6\\Win32_LIB_Release\\zlib.lib")
#else
#pragma comment(lib,"../zlib122\\projects\\visualc6\\Win32_LIB_Debug\\zlibd.lib")
#endif

class CZlibWrapper  
{
public:
	CZlibWrapper(unsigned char* pBuffer, unsigned long buffSize, unsigned long inflatedSize = ~0, bool inflate = true);
	virtual ~CZlibWrapper();

	void Reset();

	int GetBuffer(unsigned char** pBuffer){
		*pBuffer = m_pBuffer;
		return m_ulBufferSize;
	}
	unsigned long GetResult(){ return m_ulResult; }
	
private:
	unsigned char* m_pBuffer;
	unsigned long m_ulBufferSize;

	unsigned long m_ulResult;
};

#endif /* __ZLIB_WRAPPER_H__ */
