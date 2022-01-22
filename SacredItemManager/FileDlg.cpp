// FileDlg.cpp: implementation of the CFileDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredItemManager.h"
#include "FileDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileDlg::CFileDlg(BOOL bOpen,
				   const CString& szTitle,
				   HWND hWndOwner /* = NULL */,
				   const CString& szFilter/* = "All Files (*.*)|*.*||"*/,
				   const CString& szDefaultExt /* = "" */,
				   const CString& szDefaultFileTitle/* = ""*/)
{
	m_bOpen = bOpen;
	m_szTitle = szTitle;
	m_hWndOwner = hWndOwner;
	m_szFilter = szFilter;
	m_szExt = szDefaultExt;
	m_szFileTitle = szDefaultFileTitle;
}

CFileDlg::~CFileDlg()
{

}

char* CFileDlg::ConvertFilter(){
	char* str = new char[m_szFilter.GetLength()+1];
	for(int i = 0; i < m_szFilter.GetLength(); i++){
		char c = m_szFilter.GetAt(i);
		str[i] = (c=='|'?0:c);
	}
	str[i] = 0;
	return str;
}

CString CFileDlg::GetResult(){
	char fileBuffer[MAX_PATH];
	fileBuffer[0] = 0;

	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));

	char* pFilter = ConvertFilter();

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWndOwner;
	ofn.lpstrFilter = pFilter;
	ofn.lpstrFile = fileBuffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = m_szTitle;
	ofn.lpstrDefExt = m_szExt;

	if(m_bOpen){
		ofn.Flags = OFN_HIDEREADONLY;
		if(GetOpenFileName(&ofn)==0){
			SAFE_DELETE(pFilter);
			return "";
		}
	}else{
		strcpy(fileBuffer,m_szFileTitle);
		ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		if(GetSaveFileName(&ofn)==0){
			SAFE_DELETE(pFilter);
			return "";
		}
	}

	SAFE_DELETE(pFilter);

	return fileBuffer;
}
