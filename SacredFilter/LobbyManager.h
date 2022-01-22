// LobbyManager.h: interface for the CLobbyManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __LOBBY_MANAGER_H__
#define __LOBBY_MANAGER_H__

#include "ServerManager.h"

class CLobbyManager  
{
public:
	CLobbyManager();
	virtual ~CLobbyManager();

	BOOL ResetVirtualLobby(UINT nPort, const CString& szLobbyServer, UINT nLobbyPort);
	void CloseDownVirtualLobby();

	BOOL AcceptConnectionRequest();

	BOOL OnDataFromServer();
	BOOL OnDataFromClient();

	CServerManager* GetServerManager(){ return &m_ServerManager; }

private:
	CSacredSocket* m_pSocketListen;
	CSacredSocket* m_pSocketServer;
	CSacredSocket* m_pSocketClient;
	
	CServerManager m_ServerManager;

	CString m_szLobbyServer;
	UINT m_nLobbyPort;
};

#endif /* __LOBBY_MANAGER_H__ */
