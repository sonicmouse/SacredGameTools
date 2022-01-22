// ImageViewer.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemEdit.h"
#include "ImageViewer.h"
#include "PicFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageViewer

CImageViewer::CImageViewer()
{
	m_bEditable = TRUE;
}

CImageViewer::~CImageViewer()
{
}


BEGIN_MESSAGE_MAP(CImageViewer, CStatic)
	//{{AFX_MSG_MAP(CImageViewer)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageViewer message handlers

void CImageViewer::OnLButtonDown(UINT nFlags, CPoint point){

	if(!m_bEditable){ return; }

	CPicFileDlg fileDlg(TRUE,
		"",
		"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"All Supported Types |*.GIF;*.BMP;*.TGA|Windows Bitmap (*.BMP)|*.BMP|CompuServe GIF (*.GIF)|*.GIF|Targa TrueVision (*.TGA)|*.TGA||",
		GetParent());

	if(fileDlg.DoModal()==IDCANCEL){ return; }
	CString rc = fileDlg.GetPathName();

	if(!SetImageData(rc)){
		GetParent()->MessageBox("Unable to load picture file!","Error...", MB_ICONHAND|MB_OK);
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CImageViewer::ShrinkImage(CxImage* pImg){
	float wsmallerby = float(ITEM_IMAGE_SIZE_X) / float(pImg->GetWidth());
	float hsmallerby = float(ITEM_IMAGE_SIZE_Y) / float(pImg->GetHeight());
	if(wsmallerby > 1.0f){ wsmallerby = 1.0f; }
	if(hsmallerby > 1.0f){ hsmallerby = 1.0f; }
	float percentageDown = ((wsmallerby<hsmallerby)?wsmallerby:hsmallerby);
	int dbmpwidth = int(pImg->GetWidth() * percentageDown);
	int dbmpheight = int(pImg->GetHeight() * percentageDown);
	if(dbmpwidth != (int)pImg->GetWidth() || dbmpheight != (int)pImg->GetHeight()){
		pImg->Resample(dbmpwidth,dbmpheight,2);
	}
}

BOOL CImageViewer::SetImageData(PST_BUFFER pBuffer){
	m_Image.Destroy();
	Repaint();
	if(!pBuffer->buf || !pBuffer->size){ return FALSE; }
	if(m_Image.Decode(pBuffer->buf,pBuffer->size,GetImageType(pBuffer))){
		m_Image.SetTransIndex(-1);
		m_Image.IncreaseBpp(24);
		if(m_Image.GetWidth() > ITEM_IMAGE_SIZE_X || m_Image.GetHeight() > ITEM_IMAGE_SIZE_Y){
			ShrinkImage(&m_Image);
		}
		Repaint();
		return TRUE;
	}
	return FALSE;
}

BOOL CImageViewer::SetImageData(const CString& szPath){
	ST_BUFFER buf;
	FILE* f = fopen(szPath,READ_BINARY);
	if(f){
		fseek(f,0,SEEK_END);
		buf.size = ftell(f);
		rewind(f);
		buf.buf = new UCHAR[buf.size];
		fread(buf.buf,buf.size,1,f);
		fclose(f);
	}else{
		return FALSE;
	}
	BOOL rc = SetImageData(&buf);
	SAFE_DELETE(buf.buf);
	return rc;
}

void CImageViewer::DestroyImageData(){
	m_Image.Destroy();
	Repaint();
}

UINT CImageViewer::GetImageType(PST_BUFFER pBuffer){
	if(!pBuffer->buf || !pBuffer->size){ return -1; }
	if(pBuffer->buf[0] == 'G' &&
		pBuffer->buf[1] == 'I' &&
		pBuffer->buf[2] == 'F'){
		return CXIMAGE_FORMAT_GIF;
	}else if(pBuffer->buf[0] == 'B' &&
		pBuffer->buf[1] == 'M'){
		return CXIMAGE_FORMAT_BMP;
	}
	return CXIMAGE_FORMAT_TGA;
}

void CImageViewer::OnPaint(){
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(rc);

	// fill in with hatch brush
	CBrush hatchBrush;
	hatchBrush.CreateHatchBrush(HS_DIAGCROSS,RGB(200,200,200));
	
	COLORREF oldCol = dc.SetBkColor(RGB(255,255,255));
	dc.FillRect(rc,&hatchBrush);
	dc.SetBkColor(oldCol);
	hatchBrush.DeleteObject();

	// draw image
	if(m_Image.IsValid()){
		CRect rcImg;
		rcImg.left = rc.Width()/2-(m_Image.GetWidth()/2);
		rcImg.top = rc.Height()/2-(m_Image.GetHeight()/2);
		rcImg.right = rcImg.left + m_Image.GetWidth();
		rcImg.bottom = rcImg.top + m_Image.GetHeight();
		m_Image.Draw(dc.m_hDC,rcImg,NULL,true);
	}else{
		CFont* pOldFont = dc.SelectObject(GetParent()->GetFont());
		int oldBkMode = dc.SetBkMode(TRANSPARENT);
		COLORREF oldTxtCol = dc.SetTextColor(RGB(0,0,0));

		dc.DrawText((m_bEditable?"Click Here":"No Image"), rc, DT_VCENTER|DT_CENTER|DT_SINGLELINE);

		dc.SetTextColor(oldTxtCol);
		dc.SetBkMode(oldBkMode);
		dc.SelectObject(pOldFont);
	}
}

BOOL CImageViewer::GetImageData(PST_BUFFER pBuffer){
	pBuffer->buf = NULL;
	pBuffer->size = 0;
	if(!m_Image.IsValid()){ return FALSE; }
	return m_Image.Encode(pBuffer->buf,(long&)pBuffer->size,CXIMAGE_FORMAT_BMP);
}
