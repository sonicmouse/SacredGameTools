// SacredRAW.h : main header file for the SACREDRAW application
//

#if !defined(AFX_SACREDRAW_H__2A3D145C_9670_4A26_A236_0098E9DD977E__INCLUDED_)
#define AFX_SACREDRAW_H__2A3D145C_9670_4A26_A236_0098E9DD977E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ShLib.h"

/////////////////////////////////////////////////////////////////////////////
// CSRApp:
// See SacredRAW.cpp for the implementation of this class
//

class CSRApp : public CWinApp
{
public:
	CSRApp();

	CSHLib* GetSHLib(){ return &m_shLib; }

private:
	CSHLib m_shLib;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSRApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSRApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SACREDRAW_H__2A3D145C_9670_4A26_A236_0098E9DD977E__INCLUDED_)
