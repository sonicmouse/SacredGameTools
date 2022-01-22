// SITFile.h: interface for the CSITFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SIT_FILE_H__
#define __SIT_FILE_H__

#define SIT_HEADER "SIT"
#define SIT_VERSION 1
#define SIT_HEADER_ALL 0x01544953
#define ZLIB_HEADER 0xDA78

class CSITFile  
{
public:
	CSITFile();
	virtual ~CSITFile();

	CString GetItemName(){ return m_szItemName; }
	CString GetItemDescription(){ return m_szItemDescription; }
	CString GetItemAuthor(){ return m_szItemAuthor; }
	CSize GetItemInvSize(){ return m_sizeInv; }
	PST_BUFFER GetItemPicture(){ return &m_Picture; }
	PST_BUFFER GetItem(){ return &m_Item; }

	void SetItemName(const CString& szName){ m_szItemName = szName; }
	void SetItemDescription(const CString& szDesc){ m_szItemDescription = szDesc; }
	void SetItemAuthor(const CString& szAuthor){ m_szItemAuthor = szAuthor; }
	BOOL SetItemPicture(const CString& szFile);
	BOOL SetItemPicture(PST_BUFFER pPicture);
	void SetItem(PST_BUFFER pItem);
	void SetItemInvSize(CSize sz){m_sizeInv=sz;}

	BOOL CompileSITFile(const CString& szPath);
	BOOL SetSITFile(const CString& szPath);
	static BOOL IsValidSITFileQuickScan(const CString& szPath);

	void Reset();

private:
	BOOL DecompileSITBuffer(PST_BUFFER pBuffer, ULONG ulOrigSize);
	int ReadBufferShort(const UCHAR* pSrc, UCHAR** pBuffer);
	int ReadBufferLong(const UCHAR* pSrc, UCHAR** pBuffer);

private:
	CString m_szItemName;
	CString m_szItemDescription;
	CString m_szItemAuthor;
	CSize m_sizeInv;
	ST_BUFFER m_Picture;
	ST_BUFFER m_Item;
};

#endif /* __SIT_FILE_H__ */
