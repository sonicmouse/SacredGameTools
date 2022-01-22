// SITFile.cpp: implementation of the CSITFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemManager.h"
#include "SITFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSITFile::CSITFile()
{

}

CSITFile::~CSITFile()
{

}

BOOL CSITFile::CompileSitFile(const CString& szToPath, BOOL bIncludeSlotted){

	// compile picture.. if there is one...
	ST_BUFFER bufPic = {0,0};
	if(m_myImage.IsValid()){
		if(!m_myImage.Encode(bufPic.buf,(long&)bufPic.size,CXIMAGE_FORMAT_BMP)){
			bufPic.buf = NULL;
			bufPic.size = 0;
		}
	}

	// calc buffer size
	ST_BUFFER rawdata = {0,0};

	rawdata.size += sizeof(ULONG); // desc id
	rawdata.size += sizeof(USHORT) + m_szItemName.GetLength();
	rawdata.size += sizeof(USHORT) + m_szItemDesc.GetLength();
	rawdata.size += sizeof(USHORT) + m_szItemAuthor.GetLength();
	rawdata.size += sizeof(UCHAR) * 2; // inv width/height
	rawdata.size += sizeof(USHORT); // item type
	rawdata.size += sizeof(ULONG) + bufPic.size;
	rawdata.size += sizeof(ULONG) + m_Item.GetItemData()->size;
	rawdata.size += sizeof(UCHAR); // total slot items
	if(bIncludeSlotted){
		for(int i = 0; i < m_Item.GetSlottedArray()->GetSize(); i++){
			rawdata.size += sizeof(ULONG); // slotted item size
			rawdata.size += ((CItem*)m_Item.GetSlottedArray()->GetAt(i))->GetItemData()->size;
		}
	}

	// create it
	rawdata.buf = new UCHAR[rawdata.size];
	if(!rawdata.buf){
		m_myImage.FreeMemory(bufPic.buf);
		return FALSE;
	}

	// preserve pointer
	UCHAR* p = rawdata.buf;

	// desc id
	*(ULONG*)p = *m_Item.GetItemDescId();
	p += sizeof(ULONG);
	// name
	*(USHORT*)p = (USHORT)m_szItemName.GetLength();
	p += sizeof(USHORT);
	memcpy(p,(const char*)m_szItemName,m_szItemName.GetLength());
	p += m_szItemName.GetLength();
	// desc
	*(USHORT*)p = (USHORT)m_szItemDesc.GetLength();
	p += sizeof(USHORT);
	memcpy(p,(const char*)m_szItemDesc,m_szItemDesc.GetLength());
	p += m_szItemDesc.GetLength();
	// author
	*(USHORT*)p = (USHORT)m_szItemAuthor.GetLength();
	p += sizeof(USHORT);
	memcpy(p,(const char*)m_szItemAuthor,m_szItemAuthor.GetLength());
	p += m_szItemAuthor.GetLength();
	// inv size width
	*(UCHAR*)p = (UCHAR)m_Item.GetItemInvSize()->cx;
	p += sizeof(UCHAR);
	// inv size height
	*(UCHAR*)p = (UCHAR)m_Item.GetItemInvSize()->cy;
	p += sizeof(UCHAR);
	// type
	*(USHORT*)p = *m_Item.GetItemType();
	p += sizeof(USHORT);
	// picture
	*(ULONG*)p = bufPic.size;
	p += sizeof(ULONG);
	if(bufPic.buf && bufPic.size){
		memcpy(p,bufPic.buf,bufPic.size);
		p += bufPic.size;
	}
	// item data
	*(ULONG*)p = m_Item.GetItemData()->size;
	p += sizeof(ULONG);
	memcpy(p,m_Item.GetItemData()->buf,m_Item.GetItemData()->size);
	p += m_Item.GetItemData()->size;
	// total slots
	*(UCHAR*)p = (bIncludeSlotted?(UCHAR)m_Item.GetSlottedArray()->GetSize():0);
	p += sizeof(UCHAR);
	// slotted items
	if(bIncludeSlotted){
		for(int i = 0; i < m_Item.GetSlottedArray()->GetSize(); i++){
			CItem* pSlotted = (CItem*)m_Item.GetSlottedArray()->GetAt(i);
			*(ULONG*)p = pSlotted->GetItemData()->size;
			p += sizeof(ULONG);
			memcpy(p,pSlotted->GetItemData()->buf,pSlotted->GetItemData()->size);
			p += pSlotted->GetItemData()->size;
		}
	}

	// done with picture data
	if(bufPic.buf){
		m_myImage.FreeMemory(bufPic.buf);
	}

	// compress it...
	ST_BUFFER compr;
	compr.buf = new UCHAR[rawdata.size*2+12];
	if(!compr.buf){
		SAFE_DELETE(rawdata.buf);
		return FALSE;
	}
	compr.size = rawdata.size*2+12;

	CSIMApp* pApp = (CSIMApp*)AfxGetApp();
	int rc = pApp->GetSHLib()->Compress(compr.buf,&compr.size,rawdata.buf,rawdata.size,9);

	// done with raw data
	SAFE_DELETE(rawdata.buf);

	if(rc != 0){
		SAFE_DELETE(compr.buf);
		return FALSE;
	}

	// write it...
	bool bSuccess = false;
	FILE* f = fopen(szToPath,"wb");
	if(f){
		// write header
		UCHAR* pHeader = (UCHAR*)SIT_FILE_HEADER;
		if(fwrite(pHeader,SIT_FILE_HEADER_SIZE,1,f)==1){
			// write uncompressed size...
			if(fwrite(&rawdata.size,sizeof(rawdata.size),1,f)==1){
				// write compressed data (skip over ushort ZLIB header)
				if(fwrite(&compr.buf[sizeof(USHORT)],compr.size-sizeof(USHORT),1,f)==1){
					// done!
					bSuccess = true;
				}
			}else{
				// failed to write size
			}
		}else{
			// failed to write header
		}
		fflush(f);
		fclose(f);
	}else{
		// failed to open dest file
	}

	SAFE_DELETE(compr.buf);

	return bSuccess;
}

BOOL CSITFile::DecompileSitFile(const CString& szPath){
	m_Item.Reset();
	m_myImage.Destroy();
	if(!szPath.GetLength()){ return FALSE; }

	ST_BUFFER guts = {0,0};
	FILE* f = fopen(szPath,"rb");
	if(f){
		fseek(f,0,SEEK_END);
		int fileLen = ftell(f);
		rewind(f);
		unsigned char header[SIT_FILE_HEADER_SIZE];
		if(fread(header,SIT_FILE_HEADER_SIZE,1,f)==1){
			if(memcmp(header,SIT_FILE_HEADER,SIT_FILE_HEADER_SIZE)==0){
				ULONG ulSize;
				if(fread(&ulSize,sizeof(ulSize),1,f)==1){
					guts.size = fileLen-ftell(f);
					guts.buf = new UCHAR[guts.size];
					if(guts.buf){
						if(fread(guts.buf,guts.size,1,f)==1){
							ST_BUFFER tempBuf = {0,0};
							tempBuf.buf = new UCHAR[ulSize];
							if(tempBuf.buf){
								tempBuf.size = ulSize;
								// we have to tack on the header to guts.buf
								UCHAR* pTempArr = new UCHAR[guts.size+sizeof(USHORT)];
								*(USHORT*)pTempArr = (USHORT)ZLIB_HEADER;
								memcpy(&pTempArr[sizeof(USHORT)],guts.buf,guts.size);
								guts.size += sizeof(USHORT);
								SAFE_DELETE(guts.buf);
								guts.buf = pTempArr;

								CSIMApp* pApp = (CSIMApp*)AfxGetApp();
								int rc = pApp->GetSHLib()->Uncompress(tempBuf.buf,&tempBuf.size,guts.buf,guts.size);
								SAFE_DELETE(guts.buf);
								guts.buf = tempBuf.buf;
								guts.size = tempBuf.size;

								if(rc == 0){
									// success!
								}else{
									// no good...
									SAFE_DELETE(guts.buf);
								}
							}else{
								// failed to allocate temp buffer
							}
						}else{
							SAFE_DELETE(guts.buf);
						}
					}else{
						// failed to alloc memory
					}
				}else{
					// failed to read size
				}
			}else{
				// wrong header
			}
		}else{
			// failed to read header...
		}
		fclose(f);
	}else{
		// failed to open file
	}
	if(!guts.buf){
		return FALSE;
	}

	UCHAR* p = guts.buf;

	*m_Item.GetItemDescId() = *(ULONG*)p;
	p += sizeof(ULONG);

	ReadShortString(&p,&m_szItemName);
	ReadShortString(&p,&m_szItemDesc);
	ReadShortString(&p,&m_szItemAuthor);

	m_Item.GetItemInvSize()->cx = __min(p[0],8);
	m_Item.GetItemInvSize()->cy = __min(p[1],8);
	p += sizeof(UCHAR)*2;

	*m_Item.GetItemType() = *(USHORT*)p;
	p += sizeof(USHORT);

	ST_BUFFER bufPic = {0,0};
	ReadLongBuffer(&p,&bufPic);
	if(bufPic.buf && bufPic.size){
		m_myImage.Decode(bufPic.buf,bufPic.size,CXIMAGE_FORMAT_BMP);
		SAFE_DELETE(bufPic.buf);
	}

	ST_BUFFER bufItem = {0,0};
	ReadLongBuffer(&p,&bufItem);
	if(bufItem.buf && bufItem.size){
		m_Item.GetItemData()->buf = bufItem.buf;
		m_Item.GetItemData()->size = bufItem.size;
	}

	// total items...
	int totalSlotted = *(UCHAR*)p;
	p += sizeof(UCHAR);
	// read in each slotted
	for(int i = 0; i < totalSlotted; i++){
		ST_BUFFER pItemBuff = {0,0};
		ReadLongBuffer(&p,&pItemBuff);

		CItem* pNewItem = new CItem();
		pNewItem->GetItemData()->buf = pItemBuff.buf;
		pNewItem->GetItemData()->size = pItemBuff.size;
		*pNewItem->GetItemDescId() = *(ULONG*)pItemBuff.buf;
		*pNewItem->GetItemParent() = (ULONG)&m_Item;

		m_Item.GetSlottedArray()->Add(pNewItem);
		++*m_Item.GetItemSlotCount();
	}

	SAFE_DELETE(guts.buf);

	return TRUE;
}

void CSITFile::ReadShortString(UCHAR** pBuffer, CString* pToString){
	USHORT strLen = *(USHORT*)*pBuffer;
	*pBuffer += sizeof(USHORT);
	char* pTempStr = new char[strLen+1];
	pTempStr[strLen]=0;
	strncpy(pTempStr,(char*)*pBuffer,strLen);
	*pBuffer += strLen;
	*pToString = pTempStr;
	SAFE_DELETE(pTempStr);
}

void CSITFile::ReadLongBuffer(UCHAR** pBuffer, PST_BUFFER pToBuffer){
	pToBuffer->size = *(ULONG*)*pBuffer;
	*pBuffer += sizeof(ULONG);
	if(pToBuffer->size){
		pToBuffer->buf = new UCHAR[pToBuffer->size];
		memcpy(pToBuffer->buf,*pBuffer,pToBuffer->size);
		*pBuffer += pToBuffer->size;
	}else{
		pToBuffer->buf = NULL;
	}
}
