// SacredItemManager.h : main header file for the SACREDITEMMANAGER application
//

#ifndef __SACRED_ITEM_MANAGER_H__
#define __SACRED_ITEM_MANAGER_H__

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//
// includes
//
#include "resource.h"		// main symbols
#include "SHLib.h"
#include "ximage.h"

//
// libs
//
#ifdef _DEBUG
	#pragma comment(lib,"./CxImage\\Debug\\CxImage.lib")
#else
	#pragma comment(lib,"./CxImage\\Release\\CxImage.lib")
#endif

//
// defines
//
#define SAFE_DELETE(p) { if(p){ delete(p); p=NULL; } }

#define ITEM_IMAGE_SIZE_X 128
#define ITEM_IMAGE_SIZE_Y 128

#define SIM_ITEM_ATTR_FILE "\\item_attr.dat"

//
// structs
//

// 1 byte alignment
#pragma pack(push,1)

// buffer struct...
// easy portable buffer and buffer size
typedef struct _tag_st_buffer{
	unsigned long size;
	unsigned char* buf;
} ST_BUFFER,*PST_BUFFER,*LPST_BUFFER;

// back to 8 byte alignment
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// CSIMApp:
// See SacredItemManager.cpp for the implementation of this class
//

class CSIMApp : public CWinApp
{
public:
	CSIMApp();

	CSHLib* GetSHLib(){ return &m_libSh; }

private:
	CSHLib m_libSh;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSIMApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* __SACRED_ITEM_MANAGER_H__ */
