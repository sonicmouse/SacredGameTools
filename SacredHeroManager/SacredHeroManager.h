// SacredHeroManager.h : main header file for the SACREDHEROMANAGER application
//

#if !defined(AFX_SACREDHEROMANAGER_H__E772F578_9C19_4E49_8636_7AADD7203CAE__INCLUDED_)
#define AFX_SACREDHEROMANAGER_H__E772F578_9C19_4E49_8636_7AADD7203CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "SHLib.h"

/////////////////////////////////////////////////////////////////////////////
// CSHMApp:
// See SacredHeroManager.cpp for the implementation of this class
//

class CSHMApp : public CWinApp
{
public:
	CSHMApp();

	CSHLib* GetSHLib(){ return &m_shLib; }

private:
	CSHLib m_shLib;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSHMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSHMApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SACREDHEROMANAGER_H__E772F578_9C19_4E49_8636_7AADD7203CAE__INCLUDED_)
