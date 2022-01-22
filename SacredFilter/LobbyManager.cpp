// LobbyManager.cpp: implementation of the CLobbyManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredFilter.h"
#include "LobbyManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLobbyManager::CLobbyManager()
{
	m_pSocketListen = NULL;
	m_pSocketServer = NULL;
	m_pSocketClient = NULL;
	m_nLobbyPort = 0;
}

CLobbyManager::~CLobbyManager()
{
	CloseDownVirtualLobby();
}

//
//
//
BOOL CLobbyManager::ResetVirtualLobby(UINT nPort, const CString& szLobbyServer, UINT nLobbyPort){
	// clear all the servers... but keep
	// any that are working.  since we are disconnecting, we could
	// be just starting a game server
	m_ServerManager.ClearAllServers(FALSE);
	// close down all lobby sockets
	CloseDownVirtualLobby();
	// create a new lobby listener
	m_pSocketListen = new CSacredSocket(SOCKETTYPE_LOBBY_LISTEN);
	if(!m_pSocketListen){ return FALSE; }
	BOOL bRc = m_pSocketListen->CreateListener(nPort);
	if(!bRc){
		CloseDownVirtualLobby();
	}else{
		m_szLobbyServer = szLobbyServer;
		m_nLobbyPort = nLobbyPort;
	}
	return bRc;
}

//
//
//
void CLobbyManager::CloseDownVirtualLobby(){
	m_szLobbyServer.Empty();
	m_nLobbyPort = 0;
	SAFE_DELETE(m_pSocketListen);
	SAFE_DELETE(m_pSocketServer);
	SAFE_DELETE(m_pSocketClient);
}

//
//
//
BOOL CLobbyManager::AcceptConnectionRequest(){
	// check to see if we are already connected as lobby
	if(m_pSocketServer || m_pSocketClient){
		// we are, just accept and ditch this connection
		CSocket sockDummy;
		m_pSocketListen->Accept(sockDummy);
		sockDummy.Close();
		return TRUE;
	}
	// clear all game servers.. if we are
	// just connecting to the lobby, then no games should be running
	m_ServerManager.ClearAllServers(TRUE);
	// if we get here, we start a new connection
	m_pSocketServer = new CSacredSocket(SOCKETTYPE_LOBBY_SERVER);
	m_pSocketClient = new CSacredSocket(SOCKETTYPE_LOBBY_CLIENT);
	if(!m_pSocketClient || !m_pSocketServer){
		// out of memory...
		CloseDownVirtualLobby();
		return FALSE;
	}
	// try to connect to sacred's real lobby server
	if(!m_pSocketServer->ConnectToServer(m_szLobbyServer, m_nLobbyPort)){
		CloseDownVirtualLobby();
		return FALSE;
	}
	// accept sacred.exe's connection
	if(!m_pSocketListen->Accept(*m_pSocketClient)){
		CloseDownVirtualLobby();
		return FALSE;
	}
	m_pSocketClient->ManualSetInUse(TRUE);
	// if we get here, we have successfully conjoined sacred.exe to
	// the real lobby server via local proxy
	return TRUE;
}

//
// data has been recved from lobby server, analyse it and then
// send it to sacred.exe
//
BOOL CLobbyManager::OnDataFromServer(){
	PST_BUFFER pbufHeader = m_pSocketServer->GetCurrentHeader();
	PST_BUFFER pbufBody = m_pSocketServer->GetCurrentBody();

	// grab format from header
	unsigned long ulPacketFormat = *(unsigned long*)&pbufHeader->buf[12];

	// avoid oddball packet formats
	if(ulPacketFormat == 5){
		return m_pSocketClient->SendPacket(pbufHeader,pbufBody);
	}

	// get the type of packet
	unsigned short usPacketType = *(unsigned short*)&pbufBody->buf[2];

	// catch the welcome message...
	/*
	if(usPacketType == 56){
		*(unsigned long*)&pbufBody->buf[20] = strlen(LOBBY_MSG);
		memcpy(&pbufBody->buf[150],LOBBY_MSG,strlen(LOBBY_MSG));
	}
	*/

	// catch all server modifying packets
	if(usPacketType == 12 || usPacketType == 13 || usPacketType == 14 || usPacketType == 21){
		// pass it to the server manager
		if(!m_ServerManager.ServerFound((PSERVERPACKET)pbufBody->buf, (usPacketType == 14))){
			// trouble with this server, dont send it to client
			return TRUE;
		}
	}

	// send packet to the client
	return m_pSocketClient->SendPacket(pbufHeader, pbufBody);
}

//
// data has been recved from sacred.exe, analyse it and then
// send it to the lobby server
//
BOOL CLobbyManager::OnDataFromClient(){
	PST_BUFFER pbufHeader = m_pSocketClient->GetCurrentHeader();
	PST_BUFFER pbufBody = m_pSocketClient->GetCurrentBody();

	//
	// nothing to do here yet... just pass it along...
	//

	// send packet to the server
	return m_pSocketServer->SendPacket(pbufHeader, pbufBody);
}
