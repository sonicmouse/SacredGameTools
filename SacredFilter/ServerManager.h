// ServerManager.h: interface for the CServerManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

#include "Server.h"

class CServerManager  
{
public:
	CServerManager();
	virtual ~CServerManager();

	BOOL ServerFound(PSERVERPACKET pServer, BOOL bRemove);

	void ClearAllServers(BOOL bForceAll);
	void ClearAllInUseServers();

	BOOL OnConnectionAttempt(unsigned long ulId);

	BOOL OnDataFromServer(unsigned long ulId);
	BOOL OnDataFromClient(unsigned long ulId);

private:
	CPtrArray m_arrServers;

};

#endif /* __SERVER_MANAGER_H__ */
