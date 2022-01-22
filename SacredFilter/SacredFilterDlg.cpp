// SacredFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredFilter.h"
#include "SacredFilterDlg.h"
#include "ScriptEngine.h"
#include "SacredCRC32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSacredFilterDlg dialog

CSacredFilterDlg::CSacredFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSacredFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSacredFilterDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pPluginsDlg = NULL;
}

void CSacredFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSacredFilterDlg)
	DDX_Control(pDX, IDC_STATIC_FGC, m_FrameGC);
	DDX_Control(pDX, IDC_STATIC_FCTRL, m_FrameCtrl);
	DDX_Control(pDX, IDC_STATIC_INFO_SENT_G, m_InfoSentG);
	DDX_Control(pDX, IDC_STATIC_INFO_RECV_G, m_InfoRecvG);
	DDX_Control(pDX, IDC_STATIC_INFO_CONNECTED_G, m_InfoConnectedG);
	DDX_Control(pDX, IDC_STATIC_INFO_RECV, m_InfoRecv);
	DDX_Control(pDX, IDC_STATIC_INFO_SENT, m_InfoSent);
	DDX_Control(pDX, IDC_STATIC_INFO_CONNECTED, m_InfoConnected);
	DDX_Control(pDX, IDC_STATIC_FLC, m_FrameFLC);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSacredFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CSacredFilterDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_PLUGINS, OnButtonPlugins)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSacredFilterDlg message handlers

//
// WM_INITDIALOG
//
BOOL CSacredFilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//CSacredCRC32::ExtractCRC32Array("E:\\SacredGame\\tincat2.dll","c:\\test.txt",0x4A8D8);

	
	// TODO: Add extra initialization here

	CScriptEngine a("E:\\proj32\\SacredFilter\\Debug\\plugins\\base_script.vbs");
	char* test = NULL;
	if(a.Run_GetPluginDescription(&test) && test){
		// has a description!
		delete test;
	}
	if(a.Run_OnPluginInit()){
		// success
	}
	if(a.Run_OnPluginTerminate()){
		// success
	}

	// create plugins dlg
	m_pPluginsDlg = new CPluginsDlg(this);
	m_pPluginsDlg->Create(IDD_PLUGINS_DIALOG,this);
	m_pPluginsDlg->UpdateWindow();
	m_pPluginsDlg->CenterWindow();
	m_pPluginsDlg->ShowWindow(SW_HIDE);

	// set the script/plugins directory and load them
	char myPath[MAX_PATH];
	::GetModuleFileName(NULL,myPath,MAX_PATH);
	strrchr(myPath,'\\')[1]=0;
	strcat(myPath,"plugins\\");
	if(!m_ScriptManager.SetScriptDirectory(myPath)){
		AfxMessageBox("There are no plugins in your plugin directory!",MB_OK|MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
	}

	m_InfoConnected.SetColorInfo(RGB(0,0,255));
	m_InfoConnected.SetColorTitle(RGB(0,0,0));
	m_InfoConnected.SetSplitSize(90);
	m_InfoConnected.SetInfo("Socket Status: ","Listening");

	m_InfoSent.SetColorInfo(RGB(0,0,255));
	m_InfoSent.SetColorTitle(RGB(0,0,0));
	m_InfoSent.SetSplitSize(90);
	m_InfoSent.SetInfo("Data Sent: ","0 kb.");

	m_InfoRecv.SetColorInfo(RGB(0,0,255));
	m_InfoRecv.SetColorTitle(RGB(0,0,0));
	m_InfoRecv.SetSplitSize(90);
	m_InfoRecv.SetInfo("Data Received: ","0 kb.");

	m_InfoConnectedG.SetColorInfo(RGB(0,0,255));
	m_InfoConnectedG.SetColorTitle(RGB(0,0,0));
	m_InfoConnectedG.SetSplitSize(90);
	m_InfoConnectedG.SetInfo("Socket Status: ","Inactive");

	m_InfoSentG.SetColorInfo(RGB(0,0,255));
	m_InfoSentG.SetColorTitle(RGB(0,0,0));
	m_InfoSentG.SetSplitSize(90);
	m_InfoSentG.SetInfo("Data Sent: ","0 kb.");

	m_InfoRecvG.SetColorInfo(RGB(0,0,255));
	m_InfoRecvG.SetColorTitle(RGB(0,0,0));
	m_InfoRecvG.SetSplitSize(90);
	m_InfoRecvG.SetInfo("Data Received: ","0 kb.");

	ResetLobbySockets();
	ResetGameSockets();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//
// reset lobby sockets
//
void CSacredFilterDlg::ResetLobbySockets(){
	if(!m_LobbyManager.ResetVirtualLobby(VIRTUAL_NETWORK_LOBBYPORT,
										 SACRED_NETWORK_LOBBY,
										 SACRED_NETWORK_LOBBYPORT)){
		m_InfoConnected.SetInfo("Socket Status: ","Failed");
		MessageBox("Failed to initiate the Virtual Lobby","Error...", MB_ICONHAND|MB_OK);
	}else{
		m_InfoConnected.SetInfo("Socket Status: ","Listening");
	}
}

//
// reset server sockets
//
void CSacredFilterDlg::ResetGameSockets(){
	m_InfoConnectedG.SetInfo("Socket Status: ","Inactive");
	m_LobbyManager.GetServerManager()->ClearAllServers(TRUE);
}

//
// WM_DESTROY
//
void CSacredFilterDlg::OnDestroy(){

	if(m_pPluginsDlg){
		m_pPluginsDlg->DestroyWindow();
		delete m_pPluginsDlg;
	}

	DestroyIcon(m_hIcon);
	CDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////////////
// Central point for socket routines - start
//////////////////////////////////////////////////////////////////////////////////

//
// someone is trying to connect to this socket
//
void CSacredFilterDlg::OnSocketConnectionRequest(CSacredSocket* pSocket){
	switch(pSocket->GetSocketType()){
		case SOCKETTYPE_LOBBY_LISTEN:
			m_InfoConnected.SetInfo("Socket Status: ","Connected");
			m_InfoConnectedG.SetInfo("Socket Status: ","Inactive");
			if(!m_LobbyManager.AcceptConnectionRequest()){
				ResetLobbySockets();
			}
			break;
		case SOCKETTYPE_GAME_LISTEN:
			m_InfoConnectedG.SetInfo("Socket Status: ","Connected");
			if(!m_LobbyManager.GetServerManager()->OnConnectionAttempt(pSocket->GetUserData())){
				m_LobbyManager.GetServerManager()->ClearAllInUseServers();
				m_InfoConnectedG.SetInfo("Socket Status: ","Inactive");
			}
			// todo: plugin? OnConnectToServer(strServer, intPort)
			break;
		default:
#ifdef _DEBUG
			ASSERT(FALSE);
#endif
			break;
	}
}

//
// full packets have been recved on this socket
//
void CSacredFilterDlg::OnSocketRecvData(CSacredSocket* pSocket){

	CString temp; temp.Format("%0.2f kb.", pSocket->GetTotalBytesRecv()/1024.0f);

	switch(pSocket->GetSocketType()){
		case SOCKETTYPE_LOBBY_SERVER:
			if(!m_LobbyManager.OnDataFromServer()){
				ResetLobbySockets();
			}else{
				m_InfoRecv.SetInfo("Data Received: ",temp);
			}
			break;
		case SOCKETTYPE_LOBBY_CLIENT:
			if(!m_LobbyManager.OnDataFromClient()){
				ResetLobbySockets();
			}else{
				m_InfoSent.SetInfo("Data Sent: ",temp);
			}
			break;
		case SOCKETTYPE_GAME_SERVER:
			// TODO: OnDataFromServer(strData)
			if(!m_LobbyManager.GetServerManager()->OnDataFromServer(pSocket->GetUserData())){
				m_LobbyManager.GetServerManager()->ClearAllInUseServers();
				m_InfoConnectedG.SetInfo("Socket Status: ","Inactive");
			}else{
				m_InfoRecvG.SetInfo("Data Received: ",temp);
			}
			break;
		case SOCKETTYPE_GAME_CLIENT:
			// TODO: OnDataFromClient(strData)
			if(!m_LobbyManager.GetServerManager()->OnDataFromClient(pSocket->GetUserData())){
				m_LobbyManager.GetServerManager()->ClearAllInUseServers();
				m_InfoConnectedG.SetInfo("Socket Status: ","Inactive");
			}else{
				m_InfoSentG.SetInfo("Data Sent: ",temp);
			}
			break;
		default:
#ifdef _DEBUG
			ASSERT(FALSE);
#endif
			break;
	}
}

//
// this socket has closed remotely
//
void CSacredFilterDlg::OnSocketClose(CSacredSocket* pSocket){
	switch(pSocket->GetSocketType()){
		case SOCKETTYPE_LOBBY_SERVER:
		case SOCKETTYPE_LOBBY_CLIENT:
			ResetLobbySockets();
			break;
		case SOCKETTYPE_GAME_SERVER:
		case SOCKETTYPE_GAME_CLIENT:
			// todo: plugin? OnDisconnectFromServer()
			m_LobbyManager.GetServerManager()->ClearAllInUseServers();
			m_InfoConnectedG.SetInfo("Socket Status: ","Inactive");
			break;
		default:
#ifdef _DEBUG
			ASSERT(FALSE);
#endif
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Central point for socket routines - end
//////////////////////////////////////////////////////////////////////////////////

//
// show/hide plugins dlg
//
void CSacredFilterDlg::OnButtonPlugins(){
#ifdef _DEBUG
	ASSERT(m_pPluginsDlg!=NULL);
#endif
	if(m_pPluginsDlg){
		BOOL b = m_pPluginsDlg->IsWindowVisible();
		m_pPluginsDlg->ShowWindow((b?SW_HIDE:SW_SHOW));
	}
}

//
// reset all sockets
//
void CSacredFilterDlg::OnButtonDisconnect(){
	ResetLobbySockets();
	ResetGameSockets();
}
