// ServerManager.cpp: implementation of the CServerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SacredFilter.h"
#include "ServerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerManager::CServerManager()
{

}

CServerManager::~CServerManager()
{
	ClearAllServers(TRUE);
}

//
//
//
BOOL CServerManager::ServerFound(PSERVERPACKET pServer, BOOL bRemove){
	// find if this server exists...
	unsigned short ulMaxLocalPort = VIRTUAL_NETWORK_GAMEPORT_BASE;
	for(int i = 0; i < m_arrServers.GetSize(); i++){
		CServer* p = (CServer*)m_arrServers.GetAt(i);
		ulMaxLocalPort = __max(p->GetLocalPort(),ulMaxLocalPort);
		if(p->GetID() == pServer->id){
			p->SetServerData(pServer,p->GetLocalPort());
			if(bRemove){
				// remove it
				SAFE_DELETE(p);
				m_arrServers.RemoveAt(i);
			}
			return TRUE;
		}
	}
	// why do we have to do this? we shouldnt get here.
	if(bRemove){ return TRUE; }

	// make sure this port works
	while(true){
		CSocket sockDummy;
		sockDummy.Create(++ulMaxLocalPort);
		if(sockDummy.Listen()){
			sockDummy.Close();
			break;
		}
	}

	// create a server entry
	CServer* pNewServer = new CServer();
	if(!pNewServer->SetServerData(pServer,ulMaxLocalPort)){
		SAFE_DELETE(pNewServer);
		return FALSE;
	}

	// add it to our array
	m_arrServers.Add(pNewServer);

	// outta here
	return TRUE;
}

void CServerManager::ClearAllServers(BOOL bForceAll){
	for(int i = 0; i < m_arrServers.GetSize(); i++){
		CServer* p = (CServer*)m_arrServers.GetAt(i);
		if(bForceAll){
			SAFE_DELETE(p);
		}else{
			if(p->GetSocketClient() && p->GetSocketClient()->IsSocketInUse()){
				// let this one go
			}else{
				SAFE_DELETE(p);
				m_arrServers.RemoveAt(i);
				--i;
			}
		}
	}
	if(bForceAll){
		m_arrServers.RemoveAll();
	}
}

void CServerManager::ClearAllInUseServers(){
	for(int i = 0; i < m_arrServers.GetSize(); i++){
		CServer* p = (CServer*)m_arrServers.GetAt(i);
		if(p->GetSocketClient() && p->GetSocketClient()->IsSocketInUse()){
			p->GetSocketClient()->CloseSocket();
		}
		if(p->GetSocketServer() && p->GetSocketServer()->IsSocketInUse()){
			p->GetSocketServer()->CloseSocket();
		}
	}
}

BOOL CServerManager::OnConnectionAttempt(unsigned long ulId){
	for(int i = 0; i < m_arrServers.GetSize(); i++){
		CServer* p = (CServer*)m_arrServers.GetAt(i);
		if(p->GetID() == ulId){
			if(p->GetSocketServer()->ConnectToServer(p->GetIP(),p->GetRemotePort())){
				BOOL rc = p->GetSocketListen()->Accept(*p->GetSocketClient());
				if(rc){
					p->GetSocketClient()->ManualSetInUse(TRUE);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CServerManager::OnDataFromServer(unsigned long ulId){
	for(int i = 0; i < m_arrServers.GetSize(); i++){
		CServer* p = (CServer*)m_arrServers.GetAt(i);
		if((p->GetID() == ulId) &&
			p->GetSocketClient() &&
			p->GetSocketClient()->IsSocketInUse() &&
			p->GetSocketServer() &&
			p->GetSocketServer()->IsSocketInUse()){

			PST_BUFFER pbufHeader = p->GetSocketServer()->GetCurrentHeader();
			PST_BUFFER pbufBody = p->GetSocketServer()->GetCurrentBody();
			return p->GetSocketClient()->SendPacket(pbufHeader,pbufBody);
		}
	}
	return FALSE;
}

BOOL CServerManager::OnDataFromClient(unsigned long ulId){
	for(int i = 0; i < m_arrServers.GetSize(); i++){
		CServer* p = (CServer*)m_arrServers.GetAt(i);
		if((p->GetID() == ulId) &&
			p->GetSocketClient() &&
			p->GetSocketClient()->IsSocketInUse() &&
			p->GetSocketServer() &&
			p->GetSocketServer()->IsSocketInUse()){

			PST_BUFFER pbufHeader = p->GetSocketClient()->GetCurrentHeader();
			PST_BUFFER pbufBody = p->GetSocketClient()->GetCurrentBody();
			return p->GetSocketServer()->SendPacket(pbufHeader,pbufBody);
		}
	}
	return FALSE;
}
