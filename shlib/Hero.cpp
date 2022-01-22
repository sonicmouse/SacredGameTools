// Hero.cpp: implementation of the CHero class.
//
//////////////////////////////////////////////////////////////////////

#include "Hero.h"
#include "ZLibWrapper.h"
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHero::CHero()
{

}

CHero::~CHero()
{
	ResetSections();
}

bool CHero::SetHeroSection(unsigned long ulSectionType, const unsigned char* lpData, int nDataSize){
	for(int i = 0; i < m_arrSections.GetSize(); i++){
		PSECTION pSection = (PSECTION)m_arrSections.GetAt(i);
		if(pSection->sd.type == ulSectionType){
			if(pSection->pdata){
				delete pSection->pdata;
			}
			pSection->datalen = 0;
			pSection->pdata = new unsigned char[nDataSize];
			if(pSection->pdata && lpData && nDataSize){
				pSection->datalen = nDataSize;
				memcpy(pSection->pdata,lpData,nDataSize);
				return true;
			}
			return false;
		}
	}
	return false;
}

int CHero::GetHeroSection(unsigned long ulSectionType, unsigned char** lpBuffer){
	for(int i = 0; i < m_arrSections.GetSize(); i++){
		PSECTION pSection = (PSECTION)m_arrSections.GetAt(i);
		if(pSection->sd.type == ulSectionType){
			if(lpBuffer && *lpBuffer){
				memcpy(*lpBuffer,pSection->pdata,pSection->datalen);
			}
			return pSection->datalen;
		}
	}
	return -1;
}

int CHero::GetSectionsList(unsigned long* lpulArray){
	int nFound = 0;
	for(int i = 0; i < m_arrSections.GetSize(); i++){
		PSECTION pSection = (PSECTION)m_arrSections.GetAt(i);
		if(pSection->sd.offset){
			++nFound;
			if(lpulArray){
				lpulArray[nFound-1] = pSection->sd.type;
			}
		}
	}
	return nFound;
}

int CHero::DecompileHero(const char* szPath){

	int le = ERROR_SHLIB_SUCCESS;

	ResetSections();

	unsigned long ulTotalSections = 0;

	FILE* f = fopen(szPath,"rb");
	if(f){
		if(fread(&m_ulHeroVersion,sizeof(m_ulHeroVersion),1,f)==1){
			if((HERO_HEADER27 == m_ulHeroVersion) || (HERO_HEADER7 == m_ulHeroVersion)){
				if(fread(&ulTotalSections,sizeof(ulTotalSections),1,f)==1){
					if(fread(m_ucFluffHeader,sizeof(m_ucFluffHeader),1,f)==1){
						for(unsigned long i = 0; i < ulTotalSections; i++){

							PSECTION pSection = new SECTION;
							if(pSection){
								pSection->compressed = false;
								pSection->datalen = 0;
								pSection->pdata = NULL;
								pSection->sd.offset = 0;
								pSection->sd.sizeinflated = 0;
								pSection->sd.type = 0;

								m_arrSections.Add(pSection);

								if(fread(&pSection->sd,sizeof(pSection->sd),1,f)==1){
									if(pSection->sd.offset){
										// we have an offset, move up to it, save old
										unsigned long ulOriginalOffset = ftell(f);
										// move up
										fseek(f,pSection->sd.offset,SEEK_SET);
										// we make an exception for type 0xC4
										// which is 0x40 bytes long
										if(pSection->sd.type == 0xC4){
											pSection->datalen = 0x40;
											pSection->pdata = new unsigned char[0x40];
											if(pSection->pdata){
												if(fread(pSection->pdata,0x40,1,f)==1){
													// done with oddball
												}else{
													// failed to read in oddball
												}
											}else{
												// failed to allocate oddball section
											}
										}else{
											// regular section
											unsigned long secDesc;
											if(fread(&secDesc,sizeof(secDesc),1,f)==1){
												if(secDesc == COMPRESSED_CODE){
													// compressed
													pSection->compressed = true;
													unsigned long sizeData;
													if(fread(&sizeData,sizeof(sizeData),1,f)==1){
														fseek(f,0x18,SEEK_CUR);
														unsigned char* pCompressed = new unsigned char[sizeData];
														if(pCompressed){
															if(fread(pCompressed,sizeData,1,f)==1){
																CZlibWrapper zlib(pCompressed,sizeData,pSection->sd.sizeinflated);
																if(zlib.GetResult()==0){
																	unsigned char* pTempBuffer = NULL;
																	pSection->datalen = zlib.GetBuffer(&pTempBuffer);
																	pSection->pdata = new unsigned char[pSection->datalen];
																	if(pSection->pdata){
																		memcpy(pSection->pdata,pTempBuffer,pSection->datalen);
																	}else{
																		// failed to allocate pData
																	}
																}else{
																	// zlib failed
																}
															}else{
																// failed to read in compressed data
															}
															delete pCompressed;
														}else{
															// failed to alloc pCompressed
														}
													}else{
														// failed to read in size
													}
												}else{
													// not compressed
													fseek(f,pSection->sd.offset,SEEK_SET);
													pSection->pdata = new unsigned char[pSection->sd.sizeinflated];
													if(pSection->pdata){
														pSection->datalen = pSection->sd.sizeinflated;
														if(fread(pSection->pdata,pSection->datalen,1,f)==1){
															// done with non-compressed data
														}else{
															// failed to read in data
														}
													}else{
														// failed to allocate pdata
													}
												}
											}else{
												// failed to read in secDesc
											}
										}
										// move back to sections
										fseek(f,ulOriginalOffset,SEEK_SET);
									}else{
										// no offset...
									}
								}else{
									// failed to read in section
								}
							}else{
								// failed to allocate new section
							}
						} // end (for)
					}else{
						// failed to read fluff header
					}
				}else{
					// failed to read in total sections
				}
			}else{
				// wrong header version
				le = ERROR_SHLIB_WRONGVERSION;
			}
		}else{
			// failed to read header
		}
		fclose(f);
	}else{
		// failed to open hero file
	}

	if((ulTotalSections!=(unsigned long)m_arrSections.GetSize()) && (ERROR_SHLIB_SUCCESS==le)){
		ResetSections();
		le = ERROR_SHLIB_FILE;
	}

	return le;
}

void CHero::ResetSections(){
	for(int i = 0; i < m_arrSections.GetSize(); i++){
		PSECTION pSection = (PSECTION)m_arrSections.GetAt(i);
		if(pSection->pdata){
			delete pSection->pdata;
		}
		delete pSection;
	}
	m_arrSections.RemoveAll();
}

bool CHero::CompileHero(const char* szPath){
	FILE* f = fopen(szPath,"wb");
	if(f){
		//
		// write the front end of the file
		//
		bool bFrontEndWrote = false;
		if(fwrite(&m_ulHeroVersion,sizeof(m_ulHeroVersion),1,f)==1){
			unsigned long numSections = m_arrSections.GetSize();
			if(fwrite(&numSections,sizeof(numSections),1,f)==1){
				if(fwrite(m_ucFluffHeader,sizeof(m_ucFluffHeader),1,f)==1){
					unsigned long sdSize = numSections*sizeof(SECTION_DESC);
					unsigned char* tempBuffer = new unsigned char[sdSize];
					if(tempBuffer){
						if(fwrite(tempBuffer,sdSize,1,f)==1){
							bFrontEndWrote = true;
						}else{
							// fwrite() sections buffer failed
						}
						delete tempBuffer;
					}
				}else{
					// failed to write fluff header
				}
			}else{
				// failed to write number of sections
			}
		}else{
			// failed to write header
		}
		if(!bFrontEndWrote){
			fflush(f);
			fclose(f);
			_unlink(szPath);
			return false;
		}
		//
		// write the sections
		//
		unsigned long totalWrote = 0;
		for(int i = 0; i < m_arrSections.GetSize(); i++){
			PSECTION pSection = (PSECTION)m_arrSections.GetAt(i);
			if(pSection->compressed){
				// write it compressed
				pSection->sd.offset = ftell(f);
				unsigned long header = COMPRESSED_CODE;
				if(fwrite(&header,sizeof(header),1,f)==1){
					CZlibWrapper zlib(pSection->pdata,pSection->datalen,~0,false);
					if(zlib.GetResult()==0){
						unsigned char* compBuff = NULL;
						unsigned long compLen = zlib.GetBuffer(&compBuff);
						if(fwrite(&compLen,sizeof(compLen),1,f)==1){
							const unsigned char nullBuffer[0x18] = {0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0};
							if(fwrite(nullBuffer,sizeof(nullBuffer),1,f)==1){
								if(fwrite(compBuff,compLen,1,f)==1){
									pSection->sd.sizeinflated = pSection->datalen;
									++totalWrote;
								}else{
									// failed to write compressed buffer
								}
							}else{
								// failed to write nullBuffer
							}
						}else{
							// failed to write compLen
						}
					}else{
						// zlib failed
					}
				}else{
					// failed to write header
				}
			}else{
				// write it uncompressed
				if(pSection->datalen>0){
					pSection->sd.offset = ftell(f);
					pSection->sd.sizeinflated = pSection->datalen;
					if(fwrite(pSection->pdata,pSection->datalen,1,f)==1){
						++totalWrote;
					}
				}else{
					pSection->sd.offset = 0;
					pSection->sd.sizeinflated = 0;
					++totalWrote;
				}
			}
		}// end (for)
		if(totalWrote != (unsigned long)m_arrSections.GetSize()){
			fflush(f);
			fclose(f);
			_unlink(szPath);
			return false;
		}
		//
		// write index
		//
		fseek(f,0x100,SEEK_SET);
		totalWrote = 0;
		for(i = 0; i < m_arrSections.GetSize(); i++){
			PSECTION pSection = (PSECTION)m_arrSections.GetAt(i);
			if(fwrite(&pSection->sd,sizeof(pSection->sd),1,f)==1){
				++totalWrote;
			}else{
				// failed to write section header
			}
		}
		fflush(f);
		fclose(f);
		if(totalWrote != (unsigned long)m_arrSections.GetSize()){
			_unlink(szPath);
			return false;
		}
	}else{
		// failed to open the file
		return false;
	}
	return true;
}
