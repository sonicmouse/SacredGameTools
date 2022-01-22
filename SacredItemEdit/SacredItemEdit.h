// SacredItemEdit.h : main header file for the SACREDITEMEDIT application
//

#ifndef __SACRED_ITEM_EDIT_APP_H__
#define __SACRED_ITEM_EDIT_APP_H__

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "FileDlg.h"
#include "ximage.h"

/////////////////////////////////////////////////////////////////////////////
// CSIEApp:
// See SacredItemEdit.cpp for the implementation of this class
//

#ifdef _DEBUG
#pragma comment(lib,"./CxImage\\Debug\\CxImage.lib")
#else
#pragma comment(lib,"./CxImage\\Release\\CxImage.lib")
#endif

#define SACRED_VERSION "Sacred Plus version 1.8.36"

#define ITEM_FILLER_IS_NOT_IMPORTANT (TRUE) /* recompile inv data WITHOUT(TRUE) or WITH(FALSE) pointless filler */

#define HERO_HEADER7 0x07484D41
#define COMPRESSED_CODE 0xBAADC0DE

#define NULL_ITEM 0xBAADF00D
#define NULL_ITEM_SIZE (sizeof(ULONG)*5)
#define DEFAULT_ITEM_PAYLOAD_SIZE 0x82
#define DEFAULT_ITEM_PAYLOAD_SIZE_LARGE 0x379
#define ITEM_TYPE 0xFEEDF00D
#define TYPE_385_SIZE_INCLUDED 33554432

#define SECTION_INVENTORY 0xC8
#define SECTION_PLACEMENT_BP 0xCA
#define SECTION_PLACEMENT_CH 0xCB
#define SECTION_HEROATAGLANCE 0xC3
#define SECTION_HERO 0xC7

#define SLOT_COLOR_NONE 0
#define SLOT_COLOR_BRONZE 1
#define SLOT_COLOR_SILVER 2
#define SLOT_COLOR_GOLD 3
#define SLOT_COLOR_GREEN 4
#define SLOT_COLOR_PLATINUM 5

#define HERO_NAME_SIZE (128/2)

#define ITEM_IMAGE_SIZE_X 128
#define ITEM_IMAGE_SIZE_Y 128

#define SAFE_DELETE(p){{if(p){delete(p);p=NULL;}}}

#define READ_BINARY "rb"
#define WRITE_BINARY "wb"

// 1 byte alignment
#pragma pack(push,1)

// general buffer storage
typedef struct _st_buffer{
	ULONG size;
	UCHAR* buf;
} ST_BUFFER,*PST_BUFFER;

// hero section description header
typedef struct _section_desc{
	ULONG type;
	ULONG offset;
	ULONG sizeinflated;
} SECTION_DESC,*PSECTION_DESC;

// sacred item header
typedef struct _st_item_header{
	ULONG itemid;
	ULONG feedfood;
	USHORT itemtype;
	ULONG sixtyfour;
	ULONG slotindex;
	ULONG itemid2;
	USHORT usageid2;
	USHORT usageid3;
	UCHAR fluffbuffer1[0x0E];
	USHORT isused;
	UCHAR fluffbuffer2[0x0A];
	USHORT slotid2;
	ULONG flag1;
	UCHAR fluffbuffer3[0x05];
	ULONG slotid3;
	UCHAR randgarbage[0xBC];
	UCHAR slotcount;
} ST_ITEM_HEADER, *PST_ITEM_HEADER;

// sacred item data (SIT)
typedef struct _st_item_data{
	ULONG itemid;
	ST_BUFFER payload;
} ST_ITEM_DATA,*PST_ITEM_DATA;

// full sacred item
typedef struct _st_item{
	ST_ITEM_HEADER header;
	CPtrArray itemdataarr; /* ptrarray of _st_item_data stucts */
	ULONG tail1;
	ULONG tail2;
	ULONG tail3;
} ST_ITEM,*PST_ITEM;

// placement page file description
typedef struct _st_placement{
	USHORT unknown1;
	ULONG unknown2;
	USHORT unknown3;
	USHORT width;
	USHORT height;
	UCHAR fluff[1024];
	ST_BUFFER placement;
	ST_BUFFER tail;
} ST_PLACEMENT,*PST_PLACEMENT;

// sacred item placement
typedef struct _st_placement_item{
	ULONG index;
	UCHAR slotused;
	UCHAR width;
	UCHAR height;
	UCHAR unknown;
	ULONG itemid;
} ST_PLACEMENT_ITEM,*PST_PLACEMENT_ITEM;
// 8 byte alignment
#pragma pack(pop)

class CSIEApp : public CWinApp
{
public:
	CSIEApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSIEApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // /* __SACRED_ITEM_EDIT_APP_H__ */
