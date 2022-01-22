#ifndef __TREE_CTRL_EX_H__
#define __TREE_CTRL_EX_H__

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////
class CTreeCtrlEx : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlEx)

public:
					CTreeCtrlEx();
	virtual			~CTreeCtrlEx();

	void			SetItemFont(HTREEITEM, LOGFONT&);
	void			SetItemBold(HTREEITEM, BOOL);
	void			SetItemColor(HTREEITEM, COLORREF);
	BOOL			GetItemFont(HTREEITEM, LOGFONT *);
	BOOL			GetItemBold(HTREEITEM);
	COLORREF		GetItemColor(HTREEITEM);

protected:

	struct Color_Font {
		COLORREF color;
		LOGFONT  logfont;
	};

	CMap <void*, void*, Color_Font, Color_Font&> m_mapColorFont;

	protected:
	//{{AFX_MSG(CTreeCtrlEx)
		afx_msg void OnPaint();
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif /* __TREE_CTRL_EX_H__ */
