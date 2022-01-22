// SacredFilter.h : main header file for the SACREDFILTER application
//

#ifndef __SACRED_FILTER_H__
#define __SACRED_FILTER_H__

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//
// includes
//
#include "resource.h"		// main symbols
#include "ScriptObject.h"

//
// socket types that CSacredSocket's can be
//
#define SOCKETTYPE_LOBBY_LISTEN 0x01
#define SOCKETTYPE_LOBBY_SERVER 0x02
#define SOCKETTYPE_LOBBY_CLIENT 0x03
#define SOCKETTYPE_GAME_LISTEN 0x04
#define SOCKETTYPE_GAME_SERVER 0x05
#define SOCKETTYPE_GAME_CLIENT 0x06

//
// general defines
//

#define LOBBY_MSG "Sacred Filter [BETA 1.0]\n"

// sacred protocol defines
#define HEADER_SIZE 28
#define BODYSIZE(p) ( *(unsigned long*)&p[20] )

// delete helper
#define SAFE_DELETE(p){ if(p){ delete(p); (p)=NULL; } }

// static address' and ports
#define SACRED_NETWORK_LOBBYPORT 7066
#define SACRED_NETWORK_LOBBY "sacredenu.ascaron-net.com"
#define VIRTUAL_NETWORK_LOBBYPORT 13763
#define VIRTUAL_NETWORK_GAMEPORT_BASE 15763

//
// typedef structs
//

#pragma pack(push,1)

// buffer
typedef struct _tag_st_buffer{
	unsigned long size;
	unsigned char* buf;
} ST_BUFFER, *PST_BUFFER;

// server packet
typedef struct _tag_server_packet{
	unsigned char header[18];	// header of packet
	char name[80];				// name of server
	unsigned long ip1;			// internal ip?
	unsigned long ip2;			// externam ip
	unsigned short port;		// port to server
	unsigned char unknown[10];	// dont care yet
	unsigned long id;			// id of server from lobby
	unsigned char unknown2[12];	// dont care yet
} SERVERPACKET, *PSERVERPACKET;

#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// CSacredFilterApp:
// See SacredFilter.cpp for the implementation of this class
//
class CSacredFilterApp : public CWinApp
{
public:
	CSacredFilterApp();

private:
	bool m_bAlreadyOpen;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSacredFilterApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSacredFilterApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /*__SACRED_FILTER_H__*/
