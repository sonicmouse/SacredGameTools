#if !defined(AFX_IMAGEVIEWER_H__C2507097_3747_457D_BE1E_97D3E4F299FE__INCLUDED_)
#define AFX_IMAGEVIEWER_H__C2507097_3747_457D_BE1E_97D3E4F299FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageViewer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageViewer window

class CImageViewer : public CStatic
{
// Construction
public:
	CImageViewer();

	BOOL GetImageData(PST_BUFFER pBuffer);

	BOOL SetImageData(PST_BUFFER pBuffer);
	BOOL SetImageData(const CString& szPath);
	void DestroyImageData();

	void Repaint(){ Invalidate(); }

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
	//{{AFX_VIRTUAL(CImageViewer)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageViewer();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageViewer)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEVIEWER_H__C2507097_3747_457D_BE1E_97D3E4F299FE__INCLUDED_)
