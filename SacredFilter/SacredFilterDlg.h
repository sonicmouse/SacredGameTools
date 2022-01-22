// SacredFilterDlg.h : header file
//

#ifndef __SACRED_FILTER_DIALOG_H__
#define __SACRED_FILTER_DIALOG_H__

/////////////////////////////////////////////////////////////////////////////
// CSacredFilterDlg dialog

#include "SacredSocket.h"
#include "LobbyManager.h"
#include "FLGBox.h"
#include "StaticInfo.h"
#include "ScriptManager.h"
#include "PluginsDlg.h"

class CSacredFilterDlg : public CDialog
{
// Construction
public:
	CSacredFilterDlg(CWnd* pParent = NULL);	// standard constructor

	// socket communication
	void OnSocketConnectionRequest(CSacredSocket* pSocket);
	void OnSocketRecvData(CSacredSocket* pSocket);
	void OnSocketClose(CSacredSocket* pSocket);

	void ResetLobbySockets();
	void ResetGameSockets();

private:
	CLobbyManager m_LobbyManager;
	CScriptManager m_ScriptManager;
	CPluginsDlg* m_pPluginsDlg;

// Dialog Data
	//{{AFX_DATA(CSacredFilterDlg)
	enum { IDD = IDD_SACREDFILTER_DIALOG };
	CFLGBox	m_FrameGC;
	CFLGBox	m_FrameCtrl;
	CStaticInfo	m_InfoSentG;
	CStaticInfo	m_InfoRecvG;
	CStaticInfo	m_InfoConnectedG;
	CStaticInfo	m_InfoRecv;
	CStaticInfo	m_InfoSent;
	CStaticInfo	m_InfoConnected;
	CFLGBox	m_FrameFP;
	CFLGBox	m_FrameFLC;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSacredFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSacredFilterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonPlugins();
	afx_msg void OnButtonDisconnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* __SACRED_FILTER_DIALOG_H__ */
