// Server.h: interface for the CServer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SERVER_H__
#define __SERVER_H__

#include "SacredSocket.h"

class CServer  
{
public:
	CServer();
	virtual ~CServer();

	BOOL SetServerData(PSERVERPACKET pServerPacket, unsigned short usLocalPort);

	unsigned long GetID(){ return m_ulId; }
	char* GetName(){ return m_szName; }
	char* GetIP(){ return inet_ntoa(*(in_addr*)&m_ulIp); }
	unsigned short GetRemotePort(){ return m_usRemotePort; }
	unsigned short GetLocalPort(){ return m_usLocalPort; }
	CSacredSocket* GetSocketListen(){ return m_pSocketListen; }
	CSacredSocket* GetSocketServer(){ return m_pSocketServer; }
	CSacredSocket* GetSocketClient(){ return m_pSocketClient; }

private:
	unsigned long m_ulId;
	char m_szName[80];
	unsigned long m_ulIp;
	unsigned short m_usRemotePort;
	unsigned short m_usLocalPort;
	CSacredSocket* m_pSocketListen;
	CSacredSocket* m_pSocketServer;
	CSacredSocket* m_pSocketClient;

};

#endif /* __SERVER_H__ */
