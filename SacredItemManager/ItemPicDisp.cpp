// ItemPicDisp.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "ItemPicDisp.h"
#include "MemDC.h"
#include "PicFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemPicDisp

CItemPicDisp::CItemPicDisp()
{
	m_bEditable = TRUE;
}

CItemPicDisp::~CItemPicDisp()
{
}


BEGIN_MESSAGE_MAP(CItemPicDisp, CStatic)
	//{{AFX_MSG_MAP(CItemPicDisp)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemPicDisp message handlers

BOOL CItemPicDisp::SetImage(const CString& szPath){
	m_Image.Destroy();
	if(!szPath.GetLength()){ Repaint(); return TRUE; }
	bool bSuccess = false;
	FILE* f = fopen(szPath,"rb");
	if(f){
		ST_BUFFER picBuf = {0,0};
		fseek(f,0,SEEK_END);
		picBuf.size = ftell(f);
		rewind(f);
		picBuf.buf = new UCHAR[picBuf.size];
		if(picBuf.buf){
			if(fread(picBuf.buf,picBuf.size,1,f)==1){
				if(m_Image.Decode(picBuf.buf,picBuf.size,GetImageType(&picBuf))){
					// great
					m_Image.IncreaseBpp(24);
					m_Image.SetTransIndex(-1);
					ShrinkImage(&m_Image);
					bSuccess = TRUE;
				}else{
					// failed to decode picture
				}
			}else{
				// failed to read picture
			}
			SAFE_DELETE(picBuf.buf);
		}else{
			// failed to alloc memory
		}
		fclose(f);
	}else{
		// failed to open file
	}
	Repaint();
	return bSuccess;
}

UINT CItemPicDisp::GetImageType(PST_BUFFER pBuffer){
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

void CItemPicDisp::OnPaint() 
{
	CPaintDC origdc(this);
	CRect rc;
	GetClientRect(rc);
	CMemDC mdc(&origdc,&rc);
	CDC& dc = mdc;

	// fill in with hatch brush
	CBrush hatchBrush;
	hatchBrush.CreateHatchBrush(HS_DIAGCROSS,RGB(200,200,200));
	
	COLORREF oldCol = dc.SetBkColor(RGB(255,255,255));
	dc.FillRect(rc,&hatchBrush);
	dc.SetBkColor(oldCol);
	hatchBrush.DeleteObject();

	// image
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

void CItemPicDisp::ShrinkImage(CxImage* pImg){
	if((pImg->GetWidth() <= ITEM_IMAGE_SIZE_X) && (pImg->GetHeight() <= ITEM_IMAGE_SIZE_Y)){ return; }
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

void CItemPicDisp::OnLButtonDown(UINT nFlags, CPoint point){
	if(!m_bEditable){ return; }

	CPicFileDlg fileDlg(TRUE,
		"",
		"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"All Supported Types |*.GIF;*.BMP;*.TGA|Windows Bitmap (*.BMP)|*.BMP|CompuServe GIF (*.GIF)|*.GIF|Targa TrueVision (*.TGA)|*.TGA||",
		GetParent());

	if(fileDlg.DoModal()==IDCANCEL){ return; }
	CString rc = fileDlg.GetPathName();

	if(!SetImage(rc)){
		GetParent()->MessageBox("Unable to load picture file!","Error...", MB_ICONHAND|MB_OK);
	}
	CStatic::OnLButtonDown(nFlags, point);
}
