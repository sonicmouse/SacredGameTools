// SITFile.cpp: implementation of the CSITFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "SITFile.h"
#include "ZlibWrapper.h"

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
	m_Picture.buf = NULL;
	m_Picture.size = 0;
	m_Item.buf = NULL;
	m_Item.size = 0;
	m_sizeInv.cx = 1;
	m_sizeInv.cy = 1;
}

CSITFile::~CSITFile()
{
	Reset();
}

//
void CSITFile::Reset(){
	SetItemPicture((PST_BUFFER)NULL);
	SetItem((PST_BUFFER)NULL);
	SetItemAuthor("");
	SetItemDescription("");
	SetItemName("");
}

BOOL CSITFile::SetItemPicture(const CString& szFile){
	SAFE_DELETE(m_Picture.buf);
	m_Picture.size = 0;
	bool bSuccess = true;
	if(szFile.GetLength()>0){
		FILE* f = fopen(szFile,READ_BINARY);
		if(f){
			ST_BUFFER buf;
			fseek(f,0,SEEK_END);
			buf.size = ftell(f);
			rewind(f);
			buf.buf = new UCHAR[buf.size];
			if(buf.buf){
				if(fread(buf.buf,buf.size,1,f)==1){
					bSuccess = (SetItemPicture(&buf)!=FALSE);
				}else{
					// fread failed
					bSuccess = false;
				}
				SAFE_DELETE(buf.buf);
			}else{
				// out of memory
				bSuccess = false;
			}
			fclose(f);
		}else{
			// fopen failed
			bSuccess = false;
		}
	}
	return bSuccess;
}

BOOL CSITFile::SetItemPicture(PST_BUFFER pPicture){
	SAFE_DELETE(m_Picture.buf);
	m_Picture.size = 0;
	if(pPicture){
		m_Picture.buf = new UCHAR[pPicture->size];
		if(!m_Picture.buf){
			return FALSE;
		}
		m_Picture.size = pPicture->size;
		memcpy(m_Picture.buf,pPicture->buf,pPicture->size);
	}
	return TRUE;
}

void CSITFile::SetItem(PST_BUFFER pItem){
	SAFE_DELETE(m_Item.buf);
	m_Item.size = 0;
	if(pItem){
		m_Item.size = pItem->size;
		m_Item.buf = new UCHAR[pItem->size];
		memcpy(m_Item.buf,pItem->buf,pItem->size);
	}
}

int CSITFile::ReadBufferShort(const UCHAR* pSrc, UCHAR** pBuffer){
	USHORT size = *(USHORT*)pSrc;
	pSrc += sizeof(USHORT);
	*pBuffer = new UCHAR[size+1];
	if(*pBuffer){
		memset(*pBuffer,0,size+1);
		memcpy(*pBuffer,pSrc,size);
		return size;
	}
	return -1;
}

int CSITFile::ReadBufferLong(const UCHAR* pSrc, UCHAR** pBuffer){
	ULONG size = *(ULONG*)pSrc;
	pSrc += sizeof(ULONG);
	*pBuffer = new UCHAR[size+1];
	if(*pBuffer){
		memset(*pBuffer,0,size+1);
		memcpy(*pBuffer,pSrc,size);
		return size;
	}
	return -1;
}

BOOL CSITFile::DecompileSITBuffer(PST_BUFFER pBuffer, ULONG ulOrigSize){
	UCHAR* tempBuff = new UCHAR[pBuffer->size + sizeof(USHORT)];
	*(USHORT*)tempBuff = ZLIB_HEADER;
	memcpy(&tempBuff[sizeof(USHORT)],pBuffer->buf,pBuffer->size);
	CZlibWrapper zlib(tempBuff,pBuffer->size + sizeof(USHORT),ulOrigSize,true);
	SAFE_DELETE(tempBuff);
	if(zlib.GetResult() != 0){ return FALSE; }
	PST_BUFFER pDecoded = zlib.GetBuffer();
	UCHAR* p = pDecoded->buf;

	char* szName = NULL;
	char* szDesc = NULL;
	char* szAuth = NULL;

	int rc;
	bool bGood = false;
	if((rc = ReadBufferShort(p,(UCHAR**)&szName)) != -1){
		p += sizeof(USHORT) + rc;
		if((rc = ReadBufferShort(p,(UCHAR**)&szDesc)) != -1){
			p += sizeof(USHORT) + rc;
			if((rc = ReadBufferShort(p,(UCHAR**)&szAuth)) != -1){
				p += sizeof(USHORT) + rc;
				
				m_sizeInv.cx = *(USHORT*)p;
				p += sizeof(USHORT);
				m_sizeInv.cy = *(USHORT*)p;
				p += sizeof(USHORT);

				if((rc = ReadBufferLong(p,&m_Picture.buf)) != -1){
					m_Picture.size = rc;
					p += sizeof(ULONG)+rc;
					if((rc = ReadBufferLong(p,&m_Item.buf)) != -1){
						m_Item.size = rc;
						// done...
						SetItemAuthor(szAuth);
						SetItemDescription(szDesc);
						SetItemName(szName);
						if(!m_Picture.size){
							SetItemPicture((PST_BUFFER)NULL);
						}
						if(!m_Item.size){
							SetItem((PST_BUFFER)NULL);
						}
						bGood = true;
					}else{
						// failed to read item
					}
				}else{
					// failed to read picture
				}
				SAFE_DELETE(szAuth);
			}else{
				// failed to read author
			}
			SAFE_DELETE(szDesc);
		}else{
			// failed to read desc
		}
		SAFE_DELETE(szName);
	}else{
		// failed to read name
	}
	return (bGood && (m_Item.size > 0));
}

BOOL CSITFile::IsValidSITFileQuickScan(const CString& szPath){
	FILE* f = fopen(szPath,READ_BINARY);
	bool bGood = false;
	if(f){
		fseek(f,0,SEEK_END);
		ULONG ulFileSize = ftell(f);
		rewind(f);
		ULONG ulHeader;
		if(fread(&ulHeader,sizeof(ULONG),1,f)==1){
			ulFileSize -= sizeof(ULONG);
			if(ulHeader==SIT_HEADER_ALL){
				fseek(f,sizeof(ULONG),SEEK_CUR);
				ulFileSize -= sizeof(ULONG);
				ULONG ulSizeLeft;
				if(fread(&ulSizeLeft,sizeof(ULONG),1,f)==1){
					ulFileSize -= sizeof(ULONG);
					if(!(ulFileSize - ulSizeLeft)){
						bGood = true;
					}
				}
			}
		}
		fclose(f);
	}
	return bGood;
}

BOOL CSITFile::SetSITFile(const CString& szPath){
	Reset();
	FILE* f = fopen(szPath,READ_BINARY);
	bool bGood = false;
	if(f){
		ULONG header;
		if(fread(&header,sizeof(ULONG),1,f)==1){
			if(header == SIT_HEADER_ALL){
				ULONG origSize;
				if(fread(&origSize,sizeof(ULONG),1,f)==1){
					ULONG toGo;
					if(fread(&toGo,sizeof(toGo),1,f)==1){
						ST_BUFFER buf;
						buf.size = toGo;
						buf.buf = new UCHAR[buf.size];
						if(buf.buf){
							if(fread(buf.buf,buf.size,1,f)==1){
								bGood = (DecompileSITBuffer(&buf,origSize)!=FALSE);
							}else{
								// fread guts failed
							}
							SAFE_DELETE(buf.buf);
						}else{
							// out of memory
						}
					}else{
						// couldnt read new size
					}
				}else{
					// couldnt read orig size
				}
			}else{
				// not a SIT file
			}
		}else{
			// fread header failed
		}
		fclose(f);
	}else{
		// fopen failed
	}
	return bGood;
}

BOOL CSITFile::CompileSITFile(const CString& szPath){

	// calc buffer size
	ST_BUFFER rawdata = {0,0};

	rawdata.size += sizeof(USHORT) + m_szItemName.GetLength();
	rawdata.size += sizeof(USHORT) + m_szItemDescription.GetLength();
	rawdata.size += sizeof(USHORT) + m_szItemAuthor.GetLength();
	rawdata.size += sizeof(USHORT) * 2;
	rawdata.size += sizeof(ULONG) + m_Picture.size;
	rawdata.size += sizeof(ULONG) + m_Item.size;

	// create it
	rawdata.buf = new UCHAR[rawdata.size];
	if(!rawdata.buf){ return FALSE; }

	// preserve pointer
	UCHAR* p = rawdata.buf;

	// name
	*(USHORT*)p = (USHORT)m_szItemName.GetLength();
	p += sizeof(USHORT);
	memcpy(p,(const char*)m_szItemName,m_szItemName.GetLength());
	p += m_szItemName.GetLength();

	// desc
	*(USHORT*)p = (USHORT)m_szItemDescription.GetLength();
	p += sizeof(USHORT);
	memcpy(p,(const char*)m_szItemDescription,m_szItemDescription.GetLength());
	p += m_szItemDescription.GetLength();

	// author
	*(USHORT*)p = (USHORT)m_szItemAuthor.GetLength();
	p += sizeof(USHORT);
	memcpy(p,(const char*)m_szItemAuthor,m_szItemAuthor.GetLength());
	p += m_szItemAuthor.GetLength();

	// inv size
	*(USHORT*)p = (USHORT)m_sizeInv.cx;
	p += sizeof(USHORT);
	*(USHORT*)p = (USHORT)m_sizeInv.cy;
	p += sizeof(USHORT);

	// picture
	*(ULONG*)p = m_Picture.size;
	p += sizeof(ULONG);
	if(m_Picture.buf && m_Picture.size){
		memcpy(p,m_Picture.buf,m_Picture.size);
		p += m_Picture.size;
	}

	// item data
	*(ULONG*)p = m_Item.size;
	p += sizeof(ULONG);
	memcpy(p,m_Item.buf,m_Item.size);

	// encode it with zlib
	CZlibWrapper zlib(rawdata.buf,rawdata.size,-1,false);
	SAFE_DELETE(rawdata.buf);

	if(zlib.GetResult() != 0){
		return FALSE;
	}

	// now write it with the header...
	PST_BUFFER pEncoded = zlib.GetBuffer();

	FILE* f = fopen(szPath,WRITE_BINARY);
	if(!f){
		return FALSE;
	}

	bool bSuccess = false;

	// write the file
	if(fwrite(SIT_HEADER,strlen(SIT_HEADER),1,f)==1){
		UCHAR version = SIT_VERSION;
		if(fwrite(&version,sizeof(UCHAR),1,f)==1){
			if(fwrite(&rawdata.size,sizeof(rawdata.size),1,f)==1){
				ULONG sizeEncoded = pEncoded->size-sizeof(USHORT);
				if(fwrite(&sizeEncoded,sizeof(ULONG),1,f)==1){
					if(fwrite(&pEncoded->buf[sizeof(USHORT)],pEncoded->size - sizeof(USHORT),1,f)==1){
						// good
						bSuccess = true;
					}else{
						// fwrite guts failed
					}
				}else{
					// fwrite new size failed
				}
			}else{
				// fwrite oldsize failed
			}
		}else{
			// fwrite version failed
		}
	}else{
		// fwrite header failed
	}
	// done
	fclose(f);

	return bSuccess;
}