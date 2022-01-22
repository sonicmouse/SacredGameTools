// SacredSugarDaddy.h : main header file for the SACREDSUGARDADDY application
//

#if !defined(AFX_SACREDSUGARDADDY_H__248D4960_640D_4777_A755_FB9D4C0F30AC__INCLUDED_)
#define AFX_SACREDSUGARDADDY_H__248D4960_640D_4777_A755_FB9D4C0F30AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ShLib.h"

/////////////////////////////////////////////////////////////////////////////
// CSSDApp:
// See SacredSugarDaddy.cpp for the implementation of this class
//

class CSSDApp : public CWinApp
{
public:
	CSSDApp();

	CSHLib* GetSHLib(){ return &m_shLib; }

private:
	CSHLib m_shLib;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSSDApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SACREDSUGARDADDY_H__248D4960_640D_4777_A755_FB9D4C0F30AC__INCLUDED_)
