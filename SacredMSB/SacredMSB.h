// SacredMSB.h : main header file for the SACREDMSB application
//

#if !defined(AFX_SACREDMSB_H__86045400_BA86_4DDF_AFB8_4C4C869E5DEF__INCLUDED_)
#define AFX_SACREDMSB_H__86045400_BA86_4DDF_AFB8_4C4C869E5DEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "SHLib.h"

/////////////////////////////////////////////////////////////////////////////
// CSMSBApp:
// See SacredMSB.cpp for the implementation of this class
//

class CSMSBApp : public CWinApp
{
public:
	CSMSBApp();

	CSHLib* GetSHLib(){ return &m_shLib; }

private:
	CSHLib m_shLib;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSBApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSMSBApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SACREDMSB_H__86045400_BA86_4DDF_AFB8_4C4C869E5DEF__INCLUDED_)
