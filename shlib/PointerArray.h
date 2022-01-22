// PointerArray.h: interface for the CPointerArray class.
//////////////////////////////////////////////////////////////////////
// About:	A Class that acts like MFC's CPtrArray.
//			Good for *nix, CGIs, or Win32 API apps which dont support MFC.
//			Basically, a glorified linked list class.
// Author:	Andrew Heinlein [Mouse]
// Web:		www.mouseindustries.com
// EMail:	andy@mouseindustries.com
//////////////////////////////////////////////////////////////////////

#ifndef __GENERIC_POINTER_ARRAY_H__
#define __GENERIC_POINTER_ARRAY_H__

#ifndef GPA_SAFE_DELETE
#define GPA_SAFE_DELETE(p){{if(p){delete(p);(p)=0;}}}
#endif /* !GPA_SAFE_DELETE */

// linked list node definition
typedef struct _gpa_node{
	const void* pvdata;
	_gpa_node* pnextnode;
} gpa_node, *pgpa_node;

// our Pointer Array class
class CPointerArray {
	// construction / destruction
public:
	CPointerArray();
	virtual ~CPointerArray();

	// public methods
public:
	bool Add(const void* ptr); // adds a node to the end of our list
	bool SetAt(int nIndex, const void* ptr); // sets a value at an index
	void RemoveAll(); // kills our whole linked list
	bool RemoveAt(int nIndex); // removes an item at an index
	void* GetAt(int nIndex); // retrieves an item at an index
	int GetSize(){ return m_nCount; } // returns the total items in the list

	// private attributes
private:
	int m_nCount;				// count of items in our list
	pgpa_node m_pgpanodeStart;	// base node of our list
};

#endif /* __GENERIC_POINTER_ARRAY_H__ */
