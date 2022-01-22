// Hero.cpp: implementation of the CHero class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "Hero.h"

#include "ZLibWrapper.h"
#include "Section.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHero::CHero(const CString& szHeroFile)
{
	m_szHeroFile = szHeroFile;
	m_bLoaded = DecompileHeroFile();
	m_bSectionInvLoaded = FALSE;
	m_bPlacementLoadedBP = FALSE;
	m_bPlacementLoadedCH = FALSE;
}

CHero::~CHero()
{
	ClearSections();
}

BOOL CHero::DecompileHeroFile(){
	FILE* f = fopen(m_szHeroFile,READ_BINARY);
	ULONG totalItems = 0;
	if(f){
		ULONG ulHeader;
		if(fread(&ulHeader,sizeof(ulHeader),1,f)==1){
			if(ulHeader == HERO_HEADER7){
				if(fread(&totalItems,sizeof(totalItems),1,f)==1){
					if(fread(m_ucFluffHeader,sizeof(m_ucFluffHeader),1,f)==1){
						for(ULONG i = 0; i < totalItems; i++){
							SECTION_DESC sd;
							if(fread(&sd,sizeof(sd),1,f)==1){
								if(sd.offset>0){
									ULONG offsetOld = ftell(f);
									fseek(f,sd.offset,SEEK_SET);
									switch(sd.type){
										case 0xC7: /* compressed hero details */
										case 0xC8: /* compressed inventory */
										case 0xCA: /* compressed placement BP */
										case 0xCB: /* compressed placement CH */
										case 0xCD: /* compressed unknown (missions?) */
										case 0xC3: /* name offset (size is in offset_desc) */
										case 0xCE: /* unknown tail chunk (size is in offset_desc) */
											{
												ULONG desc;
												if(fread(&desc,sizeof(desc),1,f)==1){
													if(desc==COMPRESSED_CODE){
														ULONG sz;
														if(fread(&sz,sizeof(sz),1,f)==1){
															fseek(f,0x18,SEEK_CUR);
															unsigned char* p = new unsigned char[sz];
															if(fread(p,sz,1,f)==1){
																CZlibWrapper zlib(p,sz,sd.sizeinflated);
																if(zlib.GetResult()==0){
																	if(sd.type == SECTION_HERO){
																		// TODO (lvl and gold)
																		//UCHAR* pBuffer = zlib.GetBuffer()->buf;
																		//pBuffer = &pBuffer[0x000003D3];
																		//*(ULONG*)pBuffer = 0x7FFFFFF0;
																	}
																	if(sd.type == SECTION_INVENTORY){
																		m_bSectionInvLoaded = m_Inv.SetInventoryData(zlib.GetBuffer());
																	}
																	if(sd.type == SECTION_PLACEMENT_BP){
																		m_bPlacementLoadedBP = m_Inv.SetPlacementDataBP(zlib.GetBuffer());
																	}
																	if(sd.type == SECTION_PLACEMENT_CH){
																		m_bPlacementLoadedCH = m_Inv.SetPlacementDataCH(zlib.GetBuffer());
																	}
																	CSection* pSection = new CSection(&sd,zlib.GetBuffer(),true);
																	m_arrSections.Add(pSection);
																}else{
																	// inflate failed
																}
															}else{
																// fread() compressed code failed
															}
															SAFE_DELETE(p);
														}else{
															// fread() sz failed
														}
													}else{
														// move back to front
														fseek(f,sd.offset,SEEK_SET);
														ST_BUFFER buf;
														buf.buf = new unsigned char[sd.sizeinflated];
														buf.size = sd.sizeinflated;
														if(fread(buf.buf,buf.size,1,f)==1){
															if(sd.type == SECTION_HEROATAGLANCE){
																SetMyName(&buf);
															}
															CSection* pSection = new CSection(&sd,&buf,false);
															m_arrSections.Add(pSection);
														}else{
															// fread() data failed
														}
														SAFE_DELETE(buf.buf);
													}
												}else{
													// fread() desc failed
												}
											}
											break;
										case 0xC4: /* oddball offset (const 0x40 bytes long) */
											{
												unsigned char oddBuffer[0x40];
												ST_BUFFER ob;
												ob.buf = &oddBuffer[0];
												ob.size = sizeof(oddBuffer);
												if(fread(oddBuffer,sizeof(oddBuffer),1,f)==1){
													CSection* pSection = new CSection(&sd,&ob,false);
													m_arrSections.Add(pSection);
												}else{
													// fread oddBuffer failed
												}
											}
											break;
										default:
											ASSERT(FALSE);
									}
									fseek(f,offsetOld,SEEK_SET);
								}else{
									// no offset, add null section anyway
									ST_BUFFER nullBuffer={0,0};
									CSection* pSection = new CSection(&sd,&nullBuffer,false);
									m_arrSections.Add(pSection);
								}
							}else{
								// fread() sd failed
							}
						}
					}else{
						// fread() m_ucFluffHeader failed
					}
				}else{
					// fread() totalItems failed
				}
			}else{
				// wrong header version
			}
		}else{
			// fread() header failed
		}
		fclose(f);
	}else{
		// fopen() failed
	}

	if( (totalItems!=(ULONG)m_arrSections.GetSize()) ||
		!totalItems ||
		!m_bSectionInvLoaded ||
		!m_bPlacementLoadedBP ||
		!m_bPlacementLoadedCH){

		ClearSections();
		return FALSE;

	}

	return TRUE;
}

void CHero::ClearSections(){
	while(m_arrSections.GetSize()>0){
		CSection* p = (CSection*)m_arrSections.GetAt(0);
		SAFE_DELETE(p);
		m_arrSections.RemoveAt(0);
	}
}

BOOL CHero::ReplaceSection(ULONG ulSection, PST_BUFFER pBuffer){
	for(int i = 0; i < m_arrSections.GetSize(); i++){
		CSection* pSection = (CSection*)m_arrSections.GetAt(i);
		if(pSection->GetSectionDescriptor()->type == ulSection){

			SECTION_DESC sectionCopy;
			sectionCopy.offset = pSection->GetSectionDescriptor()->offset;
			sectionCopy.sizeinflated = pBuffer->size;
			sectionCopy.type = pSection->GetSectionDescriptor()->type;
			BOOL wasInflated = pSection->WasInflated();

			SAFE_DELETE(pSection);
			pSection = new CSection(&sectionCopy,pBuffer,wasInflated);

			m_arrSections.SetAt(i,pSection);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CHero::CompileHeroFile(const CString& szFile){
	
	bool bRecompileSuccess = false;

	// compile inventory...
	ST_BUFFER tmpBuffer;
	if(m_Inv.GetInventoryData(&tmpBuffer,ITEM_FILLER_IS_NOT_IMPORTANT)){
		if(ReplaceSection(SECTION_INVENTORY,&tmpBuffer)){
			bRecompileSuccess = true;
		}else{
			// failed to replace inv data
		}
		SAFE_DELETE(tmpBuffer.buf);
	}else{
		// failed to compile inv data
	}
	if(!bRecompileSuccess){ return FALSE; }

	// compile placement BP
	bRecompileSuccess = false;
	if(m_Inv.GetPlacementBP()->CompilePlacement(&tmpBuffer)){
		if(ReplaceSection(SECTION_PLACEMENT_BP,&tmpBuffer)){
			bRecompileSuccess = true;
		}else{
			// failed to replace bp section
		}
		SAFE_DELETE(tmpBuffer.buf);
	}else{
		// failed to compile bp placement
	}
	if(!bRecompileSuccess){ return FALSE; }

	// compile placement CH
	bRecompileSuccess = false;
	if(m_Inv.GetPlacementCH()->CompilePlacement(&tmpBuffer)){
		if(ReplaceSection(SECTION_PLACEMENT_CH,&tmpBuffer)){
			bRecompileSuccess = true;
		}else{
			// failed to replace ch section
		}
		SAFE_DELETE(tmpBuffer.buf);
	}else{
		// failed to compile ch placement
	}
	if(!bRecompileSuccess){ return FALSE; }

	FILE* f = fopen(szFile,WRITE_BINARY);
	// first we put together the front end of the file...
	if(f){
		bool bFrontEndWrote = false;
		ULONG header = HERO_HEADER7;
		if(fwrite(&header,sizeof(header),1,f)==1){
			ULONG numSections = m_arrSections.GetSize();
			if(fwrite(&numSections,sizeof(numSections),1,f)==1){
				if(fwrite(m_ucFluffHeader,sizeof(m_ucFluffHeader),1,f)==1){
					ULONG sdSize = numSections*sizeof(SECTION_DESC);
					unsigned char* tempBuffer = new unsigned char[sdSize];
					if(fwrite(tempBuffer,sdSize,1,f)==1){
						bFrontEndWrote = true;
					}else{
						// fwrite() sections buffer failed
					}
					SAFE_DELETE(tempBuffer);
				}else{
					// fwrite() m_ucFluffHeader failed
				}
			}else{
				// fwrite() numSections failed
			}
		}else{
			// fwrite() header failed
		}
		if(!bFrontEndWrote){
			fclose(f);
			DeleteFile(szFile);
			return FALSE;
		}
		// now we write in the actual data...
		ULONG totalWrote = 0;
		ULONG* pOffsetArr = new ULONG[m_arrSections.GetSize()];
		for(int i = 0; i < m_arrSections.GetSize(); i++){
			CSection* pSection = (CSection*)m_arrSections.GetAt(i);
			if(pSection->WasInflated()){
				ULONG header = COMPRESSED_CODE;
				pOffsetArr[i] = ftell(f);
				if(fwrite(&header,sizeof(header),1,f)==1){
					PST_BUFFER pBuffer = pSection->GetBuffer();
					CZlibWrapper zlib(pBuffer->buf,pBuffer->size,-1,false);
					if(zlib.GetResult()==0){
						pBuffer = zlib.GetBuffer();
						if(fwrite(&pBuffer->size,sizeof(pBuffer->size),1,f)==1){
							const unsigned char nullBuffer[0x18] = {0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0};
							if(fwrite(nullBuffer,sizeof(nullBuffer),1,f)==1){
								if(fwrite(pBuffer->buf,pBuffer->size,1,f)==1){
									++totalWrote;
								}else{
									// fwrite buffer failed
								}	
							}else{
								// fwrite null buffer failed
							}
						}else{
							// fwrite data failed
						}
					}else{
						// zlib failed
					}
				}else{
					// failed to write header
				}
			}else{
				PST_BUFFER pBuffer = pSection->GetBuffer();
				if(pBuffer->size>0){
					pOffsetArr[i]=ftell(f);
					if(fwrite(pBuffer->buf,pBuffer->size,1,f)==1){
						++totalWrote;
					}else{
						// failed to write buffer

					}
				}else{
					// nothing to write...
					++totalWrote;
					pOffsetArr[i]=0;
				}
			}
		}

		if((int)totalWrote != m_arrSections.GetSize()){
			SAFE_DELETE(pOffsetArr);
			fclose(f);
			DeleteFile(szFile);
			return FALSE;
		}

		// now we go back and fill in section details
		fseek(f,0x100,SEEK_SET);
		totalWrote = 0;
		for(i = 0; i < m_arrSections.GetSize(); i++){
			CSection* pSection = (CSection*)m_arrSections.GetAt(i);

			PSECTION_DESC pDesc = pSection->GetSectionDescriptor();
			PST_BUFFER pBuff = pSection->GetBuffer();

			SECTION_DESC newSd;
			newSd.offset = pOffsetArr[i];
			newSd.sizeinflated = pBuff->size;
			newSd.type = pDesc->type;

			if(fwrite(&newSd,sizeof(newSd),1,f)==1){
				++totalWrote;
			}else{
				// failed to write description
			}
		}

		SAFE_DELETE(pOffsetArr);

		fflush(f);
		fclose(f);

		if((int)totalWrote != m_arrSections.GetSize()){
			DeleteFile(szFile);
			return FALSE;
		}
	}

	return TRUE;
}

ULONG CHero::GetItemCount(BOOL bItemsOnly/* = FALSE*/){
	if(!bItemsOnly){
		return m_Inv.GetItemCount();
	}
	ULONG itemCount = 0;
	for(int i = 0; i < m_Inv.GetItemCount(); i++){
		PST_ITEM pItem = m_Inv.GetItem(i);
		if(pItem->header.itemid != NULL_ITEM && pItem->header.itemid != 0){
			++itemCount;
			itemCount += pItem->header.slotcount;
		}
	}
	return itemCount;
}

PST_ITEM CHero::GetItem(int nIndex){
	return m_Inv.GetItem(nIndex);
}

PST_PLACEMENT_ITEM CHero::GetItemPlacement(int nInvType, CPoint pt){
	CSectionPlacement* pPlacement = NULL;
	switch(nInvType){
		case SECTION_PLACEMENT_BP:
			pPlacement = m_Inv.GetPlacementBP();
			break;
		case SECTION_PLACEMENT_CH:
			pPlacement = m_Inv.GetPlacementCH();
			break;
		default:
			ASSERT(FALSE);
	}
	return pPlacement->GetItemPlacement(pt.x,pt.y);
}

PST_PLACEMENT_ITEM CHero::GetItemPlacement(int nInvType, ULONG index){
	CSectionPlacement* pPlacement = NULL;
	switch(nInvType){
		case SECTION_PLACEMENT_BP:
			pPlacement = m_Inv.GetPlacementBP();
			break;
		case SECTION_PLACEMENT_CH:
			pPlacement = m_Inv.GetPlacementCH();
			break;
		default:
			ASSERT(FALSE);
	}
	return pPlacement->GetItemPlacement(index);
}

CSize CHero::GetItemInvSize(ULONG index){
	CSize rc;
	rc = m_Inv.GetPlacementBP()->GetItemInvSize(index);
	if(rc == CSize(0,0)){
		rc = m_Inv.GetPlacementCH()->GetItemInvSize(index);
	}
	return rc;
}

CSize CHero::GetRowSpan(int nInvType){
	CSectionPlacement* pPlacement = NULL;
	switch(nInvType){
		case SECTION_PLACEMENT_BP:
			pPlacement = m_Inv.GetPlacementBP();
			break;
		case SECTION_PLACEMENT_CH:
			pPlacement = m_Inv.GetPlacementCH();
			break;
		default:
			ASSERT(FALSE);
	}
	return pPlacement->GetRowSpan();
}

void CHero::SetMyName(PST_BUFFER pBuffer){
	// clear name
	m_szName.Empty();
	// move up to name
	UCHAR* p = &pBuffer->buf[sizeof(ULONG)*2];
	// create a name buffer
	char nameBuff[HERO_NAME_SIZE];
	// convert name buffer from unicode to ansi
	int rc = WideCharToMultiByte(CP_ACP,0,(WCHAR*)p,-1,nameBuff,HERO_NAME_SIZE,NULL,NULL);
	// if it failed, give up
	if(!rc){
		m_szName = "Unknown";
	}
	// go thru and kill special chars
	for(int i = 0; i < rc-1; i++){
		if(nameBuff[i]=='\\'){
			if(nameBuff[i+1]=='c'){
				// check for 1 ulong
				if(rc-i >= 11){ // 8 + 3
					i += 9;
				}
			}else if(nameBuff[i+1]=='g'){
				// check for 2 ULONGs 
				if(rc-i >= 19){ // 16 + 3
					i += 17;
				}
			}else{
				// tack it on
				m_szName += nameBuff[i];
			}
		}else{
			// tack it on
			m_szName += nameBuff[i];
		}
	}
}
