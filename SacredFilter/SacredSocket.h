// SacredSocket.h : header file
//
#ifndef __SACRED_SOCKET_H__
#define __SACRED_SOCKET_H__

/////////////////////////////////////////////////////////////////////////////
// CSacredSocket command target

class CSacredSocket : public CSocket
{
// Attributes
public:
	
// Operations
public:
	CSacredSocket(int nSocketType);
	virtual ~CSacredSocket();

	int GetSocketType(){ return m_nType; }

	BOOL CreateListener(UINT nPort);
	BOOL ConnectToServer(const CString& szServer, UINT nPort);
	BOOL SendPacket(PST_BUFFER pbufHeader, PST_BUFFER pbufBody);
	void CloseSocket();

	BOOL IsSocketInUse(){ return m_bIsInUse; }

	void SetUserData(unsigned long ulData){ m_ulUserData = ulData; }
	unsigned long GetUserData(){ return m_ulUserData; }

	PST_BUFFER GetCurrentHeader(){ return &m_bufHeader; }
	PST_BUFFER GetCurrentBody(){ return &m_bufBody; }

	void ManualSetInUse(BOOL bInUse){ m_bIsInUse = bInUse; }

	unsigned long GetTotalBytesSent(){ return m_ulTotalBytesSent; }
	unsigned long GetTotalBytesRecv(){ return m_ulTotalBytesRecved; }

// private methods
private:
	BOOL SendFull(const unsigned char* pBuf, unsigned long ulSize);
	int bufcat(PST_BUFFER pDst, PST_BUFFER pSrc);

// private varibles
private:
	int m_nType;

	unsigned long m_ulUserData;

	unsigned long m_ulTotalBytesSent;
	unsigned long m_ulTotalBytesRecved;

	BOOL m_bIsInUse;

	ST_BUFFER m_bufHeader;
	ST_BUFFER m_bufBody;

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSacredSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSacredSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* __SACRED_SOCKET_H__ */
