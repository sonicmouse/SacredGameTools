#if !defined(AFX_ITEMPICDISP_H__02166C8F_E83C_4072_BD02_D99391373E2D__INCLUDED_)
#define AFX_ITEMPICDISP_H__02166C8F_E83C_4072_BD02_D99391373E2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemPicDisp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemPicDisp window

class CItemPicDisp : public CStatic
{
// Construction
public:
	CItemPicDisp();

	void Repaint(){ Invalidate(FALSE); }

	BOOL SetImage(CxImage* pImage){
		bool bSuccess = false;
		m_Image.Destroy();
		if(pImage){
			if(pImage->IsValid()){
				bSuccess = true;
				m_Image.Copy(*pImage);
				m_Image.IncreaseBpp(24);
				m_Image.SetTransIndex(-1);
				ShrinkImage(&m_Image);
			}
		}
		Repaint();
		return bSuccess;
	}
	BOOL SetImage(const CString& szPath);

	CxImage* GetImage(){ return &m_Image; }

	void SetEditable(BOOL bEditable){ m_bEditable = bEditable; }

private:
	UINT GetImageType(PST_BUFFER pBuffer);
	void ShrinkImage(CxImage* pImg);

private:
	CxImage m_Image;
	BOOL m_bEditable;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemPicDisp)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CItemPicDisp();

	// Generated message map functions
protected:
	//{{AFX_MSG(CItemPicDisp)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMPICDISP_H__02166C8F_E83C_4072_BD02_D99391373E2D__INCLUDED_)
