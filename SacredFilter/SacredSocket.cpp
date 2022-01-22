// SacredSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SacredFilter.h"
#include "SacredSocket.h"

#include "SacredFilterDlg.h"
#include "SacredCRC32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSacredSocket

CSacredSocket::CSacredSocket(int nSocketType)
{
	m_nType = nSocketType;
	m_ulTotalBytesSent = 0;
	m_ulTotalBytesRecved = 0;
	m_bIsInUse = FALSE;
	m_bufHeader.buf = NULL;
	m_bufHeader.size = 0;
	m_bufBody.buf = NULL;
	m_bufBody.size = 0;
	m_ulUserData = 0;
}

CSacredSocket::~CSacredSocket()
{
	CloseSocket();
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSacredSocket, CSocket)
	//{{AFX_MSG_MAP(CSacredSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSacredSocket member functions

//
// creates a listening socket
//
BOOL CSacredSocket::CreateListener(UINT nPort){

#ifdef _DEBUG
	ASSERT( (m_nType == SOCKETTYPE_LOBBY_LISTEN) || (m_nType == SOCKETTYPE_GAME_LISTEN) );
#endif

	CloseSocket();
	if(Create(nPort)){
		m_bIsInUse = Listen();
	}
	return m_bIsInUse;
}

//
// connects to another server
//
BOOL CSacredSocket::ConnectToServer(const CString& szServer, UINT nPort){

#ifdef _DEBUG
	ASSERT( (m_nType != SOCKETTYPE_LOBBY_LISTEN) && (m_nType != SOCKETTYPE_GAME_LISTEN) );
#endif

	CloseSocket();
	if(Create()){
		m_bIsInUse = Connect(szServer, nPort);
	}
	return m_bIsInUse;
}

//
// sends a sacred style packet
//
BOOL CSacredSocket::SendPacket(PST_BUFFER pbufHeader, PST_BUFFER pbufBody){

#ifdef _DEBUG
	ASSERT( (m_nType != SOCKETTYPE_LOBBY_LISTEN) && (m_nType != SOCKETTYPE_GAME_LISTEN) );
	ASSERT( IsSocketInUse() );
#endif

	// recalculate the CRC checksum
	*(unsigned long*)&pbufHeader->buf[HEADER_SIZE-sizeof(unsigned long)] = CSacredCRC32::Compute(pbufBody->buf,pbufBody->size);
	
	// send it off
	return (SendFull(pbufHeader->buf,pbufHeader->size)&&SendFull(pbufBody->buf,pbufBody->size));
}

//
// closes and resets the socket
//
void CSacredSocket::CloseSocket(){
	Close();

	m_ulTotalBytesSent = 0;
	m_ulTotalBytesRecved = 0;

	m_bIsInUse = FALSE;

	SAFE_DELETE(m_bufHeader.buf);
	SAFE_DELETE(m_bufBody.buf);

	m_bufHeader.size = 0;
	m_bufBody.size = 0;
}

//
// private - makes sure the full packet sends
//
BOOL CSacredSocket::SendFull(const unsigned char* pBuf, unsigned long ulSize){
	int nSent;
	while( (nSent = Send(pBuf,ulSize)) > 0 ){
		m_ulTotalBytesSent += nSent;
		ulSize -= nSent;
		pBuf += nSent;
		if(!ulSize){ break; }
	}
	return (ulSize==0);
}

//
// private - concatinates one st_buffer to another st_buffer
//
int CSacredSocket::bufcat(PST_BUFFER pDst, PST_BUFFER pSrc){
	if(!pDst->size && !pDst->buf){
		pDst->buf = new unsigned char[pSrc->size];
		pDst->size = pSrc->size;
		memcpy(pDst->buf,pSrc->buf,pSrc->size);
		return (int)pDst->size;
	}
	ST_BUFFER tmp;
	tmp.size = pDst->size + pSrc->size;
	tmp.buf = new unsigned char[tmp.size];
	memcpy(tmp.buf,pDst->buf,pDst->size);
	memcpy(&tmp.buf[pDst->size],pSrc->buf,pSrc->size);
	SAFE_DELETE(pDst->buf);
	pDst->buf = tmp.buf;
	pDst->size = tmp.size;
	return (int)pDst->size;
}

/////////////////////////////////////////////////////////////
// over-ridden methods of CSocket
/////////////////////////////////////////////////////////////

//
// something is trying to connect to us
//
void CSacredSocket::OnAccept(int nErrorCode){
#ifdef _DEBUG
	ASSERT( (m_nType == SOCKETTYPE_LOBBY_LISTEN) || (m_nType == SOCKETTYPE_GAME_LISTEN) );
	ASSERT( IsSocketInUse() );
#endif
	((CSacredFilterDlg*)AfxGetMainWnd())->OnSocketConnectionRequest(this);
	CSocket::OnAccept(nErrorCode);
}

//
// we were closed remotely
//
void CSacredSocket::OnClose(int nErrorCode){

#ifdef _DEBUG
	ASSERT( (m_nType != SOCKETTYPE_LOBBY_LISTEN) && (m_nType != SOCKETTYPE_GAME_LISTEN) );
	ASSERT( IsSocketInUse() );
#endif

	CloseSocket();
	((CSacredFilterDlg*)AfxGetMainWnd())->OnSocketClose(this);
	CSocket::OnClose(nErrorCode);
}

//
// data is ready to be recved
//
void CSacredSocket::OnReceive(int nErrorCode){

#ifdef _DEBUG
	ASSERT( (m_nType != SOCKETTYPE_LOBBY_LISTEN) && (m_nType != SOCKETTYPE_GAME_LISTEN) );
	ASSERT( IsSocketInUse() );
#endif

	if(m_bufHeader.size < HEADER_SIZE){
		int nHeaderToGo = HEADER_SIZE - m_bufHeader.size;
		unsigned char* pTemp = new unsigned char[nHeaderToGo];
		int rc = Receive(pTemp,nHeaderToGo);
		if(rc>0){
			m_ulTotalBytesRecved += rc;
			ST_BUFFER bufNew;
			bufNew.buf = pTemp;
			bufNew.size = rc;
			bufcat(&m_bufHeader,&bufNew);
		}else{
			// recv error
		}
		SAFE_DELETE(pTemp);
	}else{
		unsigned long ulSizeBody = BODYSIZE(m_bufHeader.buf);
		int nBodyToGo = ulSizeBody - m_bufBody.size;
		unsigned char* pTemp = new unsigned char[nBodyToGo];
		int rc = Receive(pTemp,nBodyToGo);
		if(rc>0){
			m_ulTotalBytesRecved += rc;
			ST_BUFFER bufNew;
			bufNew.buf = pTemp;
			bufNew.size = rc;
			bufcat(&m_bufBody,&bufNew);
		}else{
			// recv error
		}
		SAFE_DELETE(pTemp);
	}

	// do we have everything?
	if(( m_bufHeader.size == HEADER_SIZE) &&
		(m_bufBody.size == BODYSIZE(m_bufHeader.buf)) ){
		// done recving..
		((CSacredFilterDlg*)AfxGetMainWnd())->OnSocketRecvData(this);
		// reset everything
		SAFE_DELETE(m_bufHeader.buf);
		m_bufHeader.size = 0;
		SAFE_DELETE(m_bufBody.buf);
		m_bufBody.size = 0;
	}

	CSocket::OnReceive(nErrorCode);
}

/* eof */
