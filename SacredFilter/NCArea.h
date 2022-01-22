// NCArea.h: interface for the CNCArea class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __NC_AREA_H__
#define __NC_AREA_H__

//
// defines are missing from visual studio 6.0 SP6!
//
#ifndef WM_NCMOUSEHOVER
#define WM_NCMOUSEHOVER 0x02A0
#endif
#ifndef WM_NCMOUSELEAVE
#define WM_NCMOUSELEAVE 0x02A2
#endif
#ifndef WM_NCXBUTTONDOWN
#define WM_NCXBUTTONDOWN 0x00AB
#endif
#ifndef WM_NCXBUTTONUP
#define WM_NCXBUTTONUP 0x00AC
#endif
#ifndef WM_NCXBUTTONDBLCLK
#define WM_NCXBUTTONDBLCLK 0x00AD
#endif

//
// if you dont want flat system menus then
// change DFCS_FLAT to 0
//
//#define FLAT_BUTTONS DFCS_FLAT
#define FLAT_BUTTONS 0

#define ICON_WIDTH 16
#define ICON_HEIGHT 16

#define TOTAL_SYS_BUTTONS 3
#define SYS_BUTTON_NONE -1
#define SYS_BUTTON_CLOSE 0
#define SYS_BUTTON_MIN 1
#define SYS_BUTTON_PIN 2

#define SC_STAYONTOP 0x0010

class CNCArea  
{
public:
	CNCArea();
	virtual ~CNCArea();

	BOOL HandleNCArea(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void SetIcon(HICON hIcon){ m_hIcon = hIcon; }

private:
	BOOL OnNCLButtonDown(HWND hWnd, CPoint pt);
	BOOL OnNCLButtonUp(HWND hWnd, CPoint pt);
	BOOL OnNCRButtonDown(HWND hWnd, CPoint pt);
	BOOL OnNCRButtonUp(HWND hWnd, CPoint pt);
	void OnLButtonUp(HWND hWnd, LPARAM lParam);

	void CaptureMouse(HWND hWnd){ SetCapture(hWnd); m_bMouseCaptured = TRUE; }
	void ReleaseMouse(){ m_bMouseCaptured = FALSE; ReleaseCapture(); }
	BOOL MouseCaptured(){ return m_bMouseCaptured; }

private:
	void RepaintNonClientArea(HWND hWnd){ SendMessage(hWnd,WM_NCPAINT,0,0); }
	BOOL HasIcon(){ return (m_hIcon!=NULL); }

	void DrawTitleBar(CDC* pDC, HWND hWnd);
	void DrawFrame(CDC* pDC, HWND hWnd);
	void DrawPinButton(CDC* pDC, CRect rc, BOOL bStuck, BOOL bPushed);

	void FillGradientRect(CDC* pDC, CRect rc, COLORREF colStart, COLORREF colEnd);

	CRect GetIconRect(HWND hWnd);
	CRect GetSysButtonRect(int nIndex, HWND hWnd);
	CRect GetTitleBarRect(HWND hWnd);
	CRect GetFullWindowRect(HWND hWnd);
	CRect GetFullVisibleWindowRect(HWND hWnd);
	CRect GetFullClientRect(HWND hWnd);

	CPoint ConvertPointsToCPoint(HWND hWnd, unsigned long points);
	POINTS ConvertCPointToPoints(HWND hWnd, CPoint pt);

private:
	int m_nCyTitleBar;

	int m_nCxFrame;
	int m_nCyFrame;

	int m_nCxSysButton;
	int m_nCySysButton;
	int m_nCxSysButtonSpacing;

	HICON m_hIcon;

	BOOL m_bMouseCaptured;
	int m_nButtonDown;

	BOOL m_bIsOnTop;

	CFont m_Font;
};

#endif /* __NC_AREA_H__ */
