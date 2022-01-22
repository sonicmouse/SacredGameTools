// PointerArray.cpp: implementation of the CPointerArray class.
// Andrew Heinlein [Mouse]
//////////////////////////////////////////////////////////////////////

#include "PointerArray.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointerArray::CPointerArray()
{
	m_pgpanodeStart = 0;
	m_nCount = 0;
}

CPointerArray::~CPointerArray()
{
	RemoveAll();
}

//
// Adds a node to the tail of our linked list
//
bool CPointerArray::Add(const void* ptr){
	// if we have no base node, allocate it
	if(!m_pgpanodeStart){
		m_pgpanodeStart = new gpa_node;
		if(!m_pgpanodeStart){ return false; }
		m_pgpanodeStart->pnextnode = 0;
	}
	if(!m_nCount){
		// set the first node and die
		m_pgpanodeStart->pvdata = ptr;
	}else{
		// move up to last node
		pgpa_node ptempnode = m_pgpanodeStart;
		while(ptempnode->pnextnode){ ptempnode = ptempnode->pnextnode; }
		// create a new node
		if(!(ptempnode->pnextnode = new gpa_node)){ return false; }
		ptempnode->pnextnode->pnextnode = 0;
		ptempnode->pnextnode->pvdata = ptr;
	}
	// if we make it here, we have successfully added
	// another node.. update count.
	++m_nCount;
	// success
	return true;
}

//
// removes a node from our linked list
//
bool CPointerArray::RemoveAt(int nIndex){
	if((nIndex >= m_nCount)||(m_nCount <= 0)){ return false; }
	// our counter
	int nCount = 0;
	// our starting node
	pgpa_node ptempnode = m_pgpanodeStart;
	// our previous node to keep track of last found
	pgpa_node pprevnode = 0;
	while(ptempnode){
		if(nCount == nIndex){ // found the index
			// save where its next node is
			pgpa_node pnextnode = ptempnode->pnextnode;
			// kill it off completly
			GPA_SAFE_DELETE(ptempnode);
			// count now goes down
			--m_nCount;
			if(nIndex == 0){ // if its the base node
				// set the next node as the base node
				m_pgpanodeStart = pnextnode;
			}else if(nIndex == m_nCount){ // if its the last node
				// set the previous node to null since it is now the tail
				pprevnode->pnextnode = 0;
			}else{ // if it is inbetween
				// set the previous node to point at the next node in line.
				pprevnode->pnextnode = pnextnode;
			}
			// get out of here, we are done.
			break;
		}
		// if we get here, it wasnt that index...
		// set previous to tempnode
		pprevnode = ptempnode;
		// move tempnode up to the next in line
		ptempnode = ptempnode->pnextnode;
		// move the index count up
		++nCount;
	}
	return true;
}

//
// gets the value at some node with index nIndex
//
void* CPointerArray::GetAt(int nIndex){
	pgpa_node ptempnode = m_pgpanodeStart;
	int nCount = 0;
	while(ptempnode){
		if(nIndex == nCount++){ return (void*)ptempnode->pvdata; }
		ptempnode = ptempnode->pnextnode;
	}
	return 0;
}

//
// destroys all node in our linked list (except base node)
//
void CPointerArray::RemoveAll(){
	while(m_nCount){ RemoveAt(0); }
}

//
// sets the pvdata at a node to ptr
//
bool CPointerArray::SetAt(int nIndex, const void* ptr){
	if((nIndex >= m_nCount)||(m_nCount <= 0)){ return false; }
	pgpa_node ptempnode = m_pgpanodeStart;
	int nCount = 0;
	while(ptempnode){
		if(nCount++ == nIndex){
			ptempnode->pvdata = ptr;
			return true;
		}
		ptempnode = ptempnode->pnextnode;
	}
	return false;
}
