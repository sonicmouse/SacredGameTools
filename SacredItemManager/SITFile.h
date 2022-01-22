// SITFile.h: interface for the CSITFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SIT_FILE_H__
#define __SIT_FILE_H__

#define SIT_FILE_HEADER "AAH\001"
#define SIT_FILE_HEADER_SIZE (4)
#define ZLIB_HEADER 0xDA78

#include "Item.h"

class CSITFile  
{
public:
	CSITFile();
	virtual ~CSITFile();

	CString* GetItemName(){ return &m_szItemName; }
	CString* GetItemDesc(){ return &m_szItemDesc; }
	CString* GetItemAuthor(){ return &m_szItemAuthor; }
	CxImage* GetItemPicture(){ return &m_myImage; }

	CItem* GetItem(){ return &m_Item; }
	void SetItem(CItem* pItem){ m_Item = *pItem; }
	
	BOOL DecompileSitFile(const CString& szPath);
	BOOL CompileSitFile(const CString& szToPath, BOOL bIncludeSlotted);
	
private:
	void ReadShortString(UCHAR** pBuffer, CString* pToString);
	void ReadLongBuffer(UCHAR** pBuffer, PST_BUFFER pToBuffer);

private:
	CItem m_Item;
	CxImage m_myImage;

	CString m_szItemName;
	CString m_szItemDesc;
	CString m_szItemAuthor;
};

#endif /* __SIT_FILE_H__ */
