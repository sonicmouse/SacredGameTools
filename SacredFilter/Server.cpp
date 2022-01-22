// Server.cpp: implementation of the CServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredFilter.h"
#include "Server.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServer::CServer()
{
	m_ulId = ~0;
	m_ulIp = ~0;
	m_usRemotePort = ~0;
	m_usLocalPort = ~0;
	m_szName[0] = 0;
	m_pSocketListen = NULL;
	m_pSocketServer = NULL;
	m_pSocketClient = NULL;
}

CServer::~CServer()
{
	SAFE_DELETE(m_pSocketListen);
	SAFE_DELETE(m_pSocketServer);
	SAFE_DELETE(m_pSocketClient);
}

//
//
//
BOOL CServer::SetServerData(PSERVERPACKET pServerPacket, unsigned short usLocalPort){

	if(m_usLocalPort != usLocalPort){
		SAFE_DELETE(m_pSocketListen);
		m_pSocketListen = new CSacredSocket(SOCKETTYPE_GAME_LISTEN);
		if(!m_pSocketListen || !m_pSocketListen->CreateListener(usLocalPort)){
			return FALSE;
		}
	}

	if(!m_pSocketServer){
		m_pSocketServer = new CSacredSocket(SOCKETTYPE_GAME_SERVER);
	}
	if(!m_pSocketClient){
		m_pSocketClient = new CSacredSocket(SOCKETTYPE_GAME_CLIENT);
	}

	if(!m_pSocketServer || !m_pSocketClient){
		return FALSE;
	}

	m_pSocketListen->SetUserData(pServerPacket->id);
	m_pSocketServer->SetUserData(pServerPacket->id);
	m_pSocketClient->SetUserData(pServerPacket->id);

	m_ulId = pServerPacket->id;
	strcpy(m_szName,pServerPacket->name);
	m_ulIp = pServerPacket->ip2;
	m_usRemotePort = pServerPacket->port;
	m_usLocalPort = usLocalPort;

	unsigned long ulLocalHost = inet_addr("127.0.0.1");
	pServerPacket->ip1 = ulLocalHost;
	pServerPacket->ip2 = ulLocalHost;
	pServerPacket->port = usLocalPort;

	return TRUE;
}
