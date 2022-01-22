// ItemEditHelperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SacredItemManager.h"
#include "ItemEditHelperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemEditHelperDlg dialog


CItemEditHelperDlg::CItemEditHelperDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemEditHelperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemEditHelperDlg)
	//}}AFX_DATA_INIT
	m_pItem = NULL;
}


void CItemEditHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemEditHelperDlg)
	DDX_Control(pDX, IDC_SPIN_ATTR, m_SpinAttr);
	DDX_Control(pDX, IDC_COMBO_SLOTS, m_ComboSlots);
	DDX_Control(pDX, IDC_COMBO_ATTRIBUTE, m_ComboAttrNum);
	DDX_Control(pDX, IDC_COMBO_ITEM_ATTRS, m_ComboAttr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemEditHelperDlg, CDialog)
	//{{AFX_MSG_MAP(CItemEditHelperDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_ATTRIBUTE, OnSelchangeComboAttribute)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_ATTRS, OnSelchangeComboItemAttrs)
	ON_EN_CHANGE(IDC_EDIT_ATTR_ID, OnChangeEditAttrId)
	ON_BN_CLICKED(IDC_RADIO_ATTR_NUM, OnRadioAttrNum)
	ON_BN_CLICKED(IDC_RADIO_ATTR_TEXT, OnRadioAttrText)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_ATTR_CHANGE, OnButtonApplyAttrChange)
	ON_CBN_SELCHANGE(IDC_COMBO_SLOTS, OnSelchangeComboSlots)
	ON_BN_CLICKED(IDC_RADIO_SLOT_NONE, OnRadioSlotNone)
	ON_BN_CLICKED(IDC_RADIO_SLOT_BRONZE, OnRadioSlotBronze)
	ON_BN_CLICKED(IDC_RADIO_SLOT_GOLD, OnRadioSlotGold)
	ON_BN_CLICKED(IDC_RADIO_SLOT_GREEN, OnRadioSlotGreen)
	ON_BN_CLICKED(IDC_RADIO_SLOT_PLAT, OnRadioSlotPlat)
	ON_BN_CLICKED(IDC_RADIO_SLOT_SILVER, OnRadioSlotSilver)
	ON_BN_CLICKED(IDC_BUTTON_RELOADATTRDB, OnButtonReloadattrdb)
	ON_BN_CLICKED(IDC_BUTTON_EDITATTRDB, OnButtonEditattrdb)
	ON_EN_CHANGE(IDC_EDIT_ATTR_VALUE, OnChangeEditAttrValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemEditHelperDlg message handlers

BOOL CItemEditHelperDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ASSERT(m_pItem!=NULL);

	LoadItemAttributesData();

	m_ComboAttrNum.SetCurSel(0);
	OnSelchangeComboAttribute();

	GetDlgItem(IDC_BUTTON_APPLY_ATTR_CHANGE)->EnableWindow(CurrentAttrDiffrent());

	m_SpinAttr.SetBuddy(GetDlgItem(IDC_EDIT_ATTR_VALUE));
	m_SpinAttr.SetRange(short(0x8000),short(0x7fff));

	m_ComboSlots.SetCurSel(0);
	OnSelchangeComboSlots();

	// set up class ident
	unsigned char ci = m_pItem->GetItemData()->buf[0x04];
	CheckDlgButton(IDC_CHECK_CR_SERA,((ci&0x01)==0x01));
	CheckDlgButton(IDC_CHECK_CR_GLADI,((ci&0x02)==0x02));
	CheckDlgButton(IDC_CHECK_CR_BM,((ci&0x04)==0x04));
	CheckDlgButton(IDC_CHECK_CR_DE,((ci&0x08)==0x08));
	CheckDlgButton(IDC_CHECK_CR_WE,((ci&0x10)==0x10));
	CheckDlgButton(IDC_CHECK_CR_VAMP,((ci&0x20)==0x20));

	// resistances
	SetDlgItemInt(IDC_EDIT_RESIST_FIRE,*(short*)&m_pItem->GetItemData()->buf[0x34]);
	SetDlgItemInt(IDC_EDIT_RESIST_MAGIC,*(short*)&m_pItem->GetItemData()->buf[0x36]);
	SetDlgItemInt(IDC_EDIT_RESIST_POISON,*(short*)&m_pItem->GetItemData()->buf[0x38]);
	SetDlgItemInt(IDC_EDIT_RESIST_PHYSICAL,*(short*)&m_pItem->GetItemData()->buf[0x32]);

	// damage
	SetDlgItemInt(IDC_EDIT_DAMAGE_PHYSICAL_FROM,*(short*)&m_pItem->GetItemData()->buf[0x1C]);
	SetDlgItemInt(IDC_EDIT_DAMAGE_PHYSICAL_TO,*(short*)&m_pItem->GetItemData()->buf[0x24]);

	SetDlgItemInt(IDC_EDIT_DAMAGE_MAGIC_FROM,*(short*)&m_pItem->GetItemData()->buf[0x20]);
	SetDlgItemInt(IDC_EDIT_DAMAGE_MAGIC_TO,*(short*)&m_pItem->GetItemData()->buf[0x28]);

	SetDlgItemInt(IDC_EDIT_DAMAGE_FIRE_FROM,*(short*)&m_pItem->GetItemData()->buf[0x1E]);
	SetDlgItemInt(IDC_EDIT_DAMAGE_FIRE_TO,*(short*)&m_pItem->GetItemData()->buf[0x26]);

	SetDlgItemInt(IDC_EDIT_DAMAGE_POISON_FROM,*(short*)&m_pItem->GetItemData()->buf[0x22]);
	SetDlgItemInt(IDC_EDIT_DAMAGE_POISON_TO,*(short*)&m_pItem->GetItemData()->buf[0x2A]);

	// attack/defense
	SetDlgItemInt(IDC_EDIT_IATTACK,*(short*)&m_pItem->GetItemData()->buf[0x2C]);
	SetDlgItemInt(IDC_EDIT_IDEFENSE,*(short*)&m_pItem->GetItemData()->buf[0x2E]);

	// level/minlevel
	SetDlgItemInt(IDC_EDIT_ILEVEL,m_pItem->GetItemData()->buf[0x19]);
	SetDlgItemInt(IDC_EDIT_IMINLEVEL,m_pItem->GetItemData()->buf[0x14]);


	return TRUE;
}

void CItemEditHelperDlg::LoadItemAttributesData(){

	m_ComboAttr.ResetContent();

	char myPath[MAX_PATH];
	GetModuleFileName(NULL,myPath,MAX_PATH);
	strrchr(myPath,'\\')[0] = 0;
	strcat(myPath,SIM_ITEM_ATTR_FILE);

	FILE* f = fopen(myPath,"rb");
	if(f){
		fseek(f,0,SEEK_END);
		ULONG flen = ftell(f);
		rewind(f);
		char* data = new char[flen+1];
		data[flen]=0;
		if(data){
			if(fread(data,flen,1,f)==1){
				char* tok = strtok(data,"\n");
				while(tok){
					if(!isdigit(tok[0])){
						// comment line
					}else{
						char* mid = strchr(tok,'=');
						if(mid){
							mid[0] = 0;
							++mid;
							CString attrNum(tok);
							CString attrData(mid);
							attrNum.TrimLeft(); attrNum.TrimRight();
							attrData.TrimLeft(); attrData.TrimRight();
							int nInd = m_ComboAttr.AddString(attrData);
							m_ComboAttr.SetItemData(nInd,DWORD(atoi(attrNum)));
						}else{
							// not a valid line
						}
					}
					tok = strtok(NULL,"\n");
				}
			}else{
				// failed to read data
			}
			SAFE_DELETE(data);
		}else{
			// failed to alloc data
		}
		fclose(f);
	}else{
		// failed to open file
	}
}

// user as selected a diffrent attr number
void CItemEditHelperDlg::OnSelchangeComboAttribute() 
{
	int nAttr = m_ComboAttrNum.GetCurSel();

	ULONG attrId = *(ULONG*)&m_pItem->GetItemData()->buf[0x4A+(nAttr*sizeof(ULONG))];
	short attrVal = *(short*)&m_pItem->GetItemData()->buf[0x6A+(nAttr*sizeof(short))];

	bool bFound = false;
	for(int i = 0; i < m_ComboAttr.GetCount(); i++){
		if(m_ComboAttr.GetItemData(i)==attrId){
			m_ComboAttr.SetCurSel(i);
			bFound = true;
			break;
		}
	}

	//
	if(bFound){
		CheckDlgButton(IDC_RADIO_ATTR_TEXT,1);
		CheckDlgButton(IDC_RADIO_ATTR_NUM,0);
		GetDlgItem(IDC_EDIT_ATTR_ID)->EnableWindow(FALSE);
		m_ComboAttr.EnableWindow();
	}else{
		CheckDlgButton(IDC_RADIO_ATTR_TEXT,0);
		CheckDlgButton(IDC_RADIO_ATTR_NUM,1);
		GetDlgItem(IDC_EDIT_ATTR_ID)->EnableWindow(TRUE);
		m_ComboAttr.SetCurSel(-1);
		m_ComboAttr.EnableWindow(FALSE);
	}

	SetDlgItemInt(IDC_EDIT_ATTR_ID,attrId,FALSE);
	SetDlgItemInt(IDC_EDIT_ATTR_VALUE,attrVal);

	GetDlgItem(IDC_BUTTON_APPLY_ATTR_CHANGE)->EnableWindow(CurrentAttrDiffrent());
}

//
// user chose a diffrent attribute from text list
//
void CItemEditHelperDlg::OnSelchangeComboItemAttrs(){
	ULONG val = m_ComboAttr.GetItemData(m_ComboAttr.GetCurSel());
	SetDlgItemInt(IDC_EDIT_ATTR_ID,val,FALSE);

	GetDlgItem(IDC_BUTTON_APPLY_ATTR_CHANGE)->EnableWindow(CurrentAttrDiffrent());
}

void CItemEditHelperDlg::OnChangeEditAttrId(){
	ULONG val = GetDlgItemInt(IDC_EDIT_ATTR_ID,NULL,FALSE);

	bool bFound = false;
	for(int i = 0; i < m_ComboAttr.GetCount(); i++){
		if(m_ComboAttr.GetItemData(i)==val){
			m_ComboAttr.SetCurSel(i);
			bFound = true;
			break;
		}
	}
	if(!bFound){
		m_ComboAttr.SetCurSel(-1);
	}

	GetDlgItem(IDC_BUTTON_APPLY_ATTR_CHANGE)->EnableWindow(CurrentAttrDiffrent());
}

void CItemEditHelperDlg::OnRadioAttrNum(){
	GetDlgItem(IDC_EDIT_ATTR_ID)->EnableWindow(TRUE);
	m_ComboAttr.EnableWindow(FALSE);
}

void CItemEditHelperDlg::OnRadioAttrText(){
	GetDlgItem(IDC_EDIT_ATTR_ID)->EnableWindow(FALSE);
	m_ComboAttr.EnableWindow(TRUE);
}

//
// applies an attribute change
//
void CItemEditHelperDlg::OnButtonApplyAttrChange(){
	int nAttrIndex = m_ComboAttrNum.GetCurSel();
	ULONG ulAttrId = (ULONG)GetDlgItemInt(IDC_EDIT_ATTR_ID,NULL,FALSE);
	short usAttrVal = (short)GetDlgItemInt(IDC_EDIT_ATTR_VALUE);

	*(ULONG*)&m_pItem->GetItemData()->buf[0x4A+(nAttrIndex*sizeof(ULONG))] = ulAttrId;
	*(short*)&m_pItem->GetItemData()->buf[0x6A+(nAttrIndex*sizeof(short))] = usAttrVal;

	GetDlgItem(IDC_BUTTON_APPLY_ATTR_CHANGE)->EnableWindow(CurrentAttrDiffrent());
}

BOOL CItemEditHelperDlg::CurrentAttrDiffrent(){
	int nAttrIndex = m_ComboAttrNum.GetCurSel();
	ULONG ulAttrId = (ULONG)GetDlgItemInt(IDC_EDIT_ATTR_ID,NULL,FALSE);
	short usAttrVal = (short)GetDlgItemInt(IDC_EDIT_ATTR_VALUE);

	ULONG oldAttrId = *(ULONG*)&m_pItem->GetItemData()->buf[0x4A+(nAttrIndex*sizeof(ULONG))];
	short oldAttrVal = *(short*)&m_pItem->GetItemData()->buf[0x6A+(nAttrIndex*sizeof(short))];

	if(oldAttrId==ulAttrId && oldAttrVal==usAttrVal){
		// same
		return FALSE;
	}
	return TRUE;
}

void CItemEditHelperDlg::OnSelchangeComboSlots(){
	int nIndex = m_ComboSlots.GetCurSel();

	char slotVal = m_pItem->GetItemData()->buf[0x0C+nIndex];

	CheckDlgButton(IDC_RADIO_SLOT_NONE,0);
	CheckDlgButton(IDC_RADIO_SLOT_BRONZE,0);
	CheckDlgButton(IDC_RADIO_SLOT_SILVER,0);
	CheckDlgButton(IDC_RADIO_SLOT_GOLD,0);
	CheckDlgButton(IDC_RADIO_SLOT_GREEN,0);
	CheckDlgButton(IDC_RADIO_SLOT_PLAT,0);

	switch(slotVal){
		case 0:
			CheckDlgButton(IDC_RADIO_SLOT_NONE,1);
			break;
		case 1:
			CheckDlgButton(IDC_RADIO_SLOT_BRONZE,1);
			break;
		case 2:
			CheckDlgButton(IDC_RADIO_SLOT_SILVER,1);
			break;
		case 3:
			CheckDlgButton(IDC_RADIO_SLOT_GOLD,1);
			break;
		case 4:
			CheckDlgButton(IDC_RADIO_SLOT_GREEN,1);
			break;
		default:
			CheckDlgButton(IDC_RADIO_SLOT_PLAT,1);
			break;
	}

}

void CItemEditHelperDlg::OnRadioSlotNone(){
	int nIndex = m_ComboSlots.GetCurSel();
	m_pItem->GetItemData()->buf[0x0C+nIndex] = 0;
}

void CItemEditHelperDlg::OnRadioSlotBronze(){
	int nIndex = m_ComboSlots.GetCurSel();
	m_pItem->GetItemData()->buf[0x0C+nIndex] = 1;
}

void CItemEditHelperDlg::OnRadioSlotSilver(){
	int nIndex = m_ComboSlots.GetCurSel();
	m_pItem->GetItemData()->buf[0x0C+nIndex] = 2;
}

void CItemEditHelperDlg::OnRadioSlotGold(){
	int nIndex = m_ComboSlots.GetCurSel();
	m_pItem->GetItemData()->buf[0x0C+nIndex] = 3;
}

void CItemEditHelperDlg::OnRadioSlotGreen(){
	int nIndex = m_ComboSlots.GetCurSel();
	m_pItem->GetItemData()->buf[0x0C+nIndex] = 4;
}

void CItemEditHelperDlg::OnRadioSlotPlat(){
	int nIndex = m_ComboSlots.GetCurSel();
	m_pItem->GetItemData()->buf[0x0C+nIndex] = 5;
}

void CItemEditHelperDlg::OnButtonReloadattrdb(){
	LoadItemAttributesData();
	OnSelchangeComboAttribute();
	GetDlgItem(IDC_BUTTON_APPLY_ATTR_CHANGE)->EnableWindow(CurrentAttrDiffrent());
}

void CItemEditHelperDlg::OnButtonEditattrdb(){
	char myPath[MAX_PATH];
	GetModuleFileName(NULL,myPath,MAX_PATH);
	strrchr(myPath,'\\')[0] = 0;
	strcat(myPath,SIM_ITEM_ATTR_FILE);
	
	// see if it exists...
	FILE* f = fopen(myPath,"r");
	if(!f){
		MessageBox("You currently have no Item Attributes data file in the working directory.\nI will create a template now.","Error...",MB_ICONINFORMATION|MB_OK);
		f = fopen(myPath,"wt");
		fprintf(f,"# Entries have one line a piece\n");
		fprintf(f,"# Comments have the `#` sign infront of them\n");
		fprintf(f,"# Format of an entry is <attr id>=<attr text>\n");
		fprintf(f,"# Examples:\n\n");
		fprintf(f,"1080=Hard Hit\n");
		fprintf(f,"1081=Attack\n\n");
		fflush(f);	
	}
	fclose(f);

	if((int)ShellExecute(GetSafeHwnd(),"open","notepad.exe",myPath,NULL,SW_SHOWNORMAL)<=32){
		MessageBox("Failed to open the file for editing.\nPlease find it yourself and open it in notepad.","Error...",MB_ICONHAND|MB_OK);
	}
}

void CItemEditHelperDlg::OnChangeEditAttrValue(){
	GetDlgItem(IDC_BUTTON_APPLY_ATTR_CHANGE)->EnableWindow(CurrentAttrDiffrent());
}

void CItemEditHelperDlg::OnOK(){
	// class restrictions
	unsigned char bSera = (IsDlgButtonChecked(IDC_CHECK_CR_SERA)?0x01:0x00);
	unsigned char bGladi = (IsDlgButtonChecked(IDC_CHECK_CR_GLADI)?0x02:0x00);
	unsigned char bBM = (IsDlgButtonChecked(IDC_CHECK_CR_BM)?0x04:0x00);
	unsigned char bDE = (IsDlgButtonChecked(IDC_CHECK_CR_DE)?0x08:0x00);
	unsigned char bWE = (IsDlgButtonChecked(IDC_CHECK_CR_WE)?0x10:0x00);
	unsigned char bVamp = (IsDlgButtonChecked(IDC_CHECK_CR_VAMP)?0x20:0x00);
	m_pItem->GetItemData()->buf[0x04] = bSera|bGladi|bBM|bDE|bWE|bVamp;

	// resistances
	*(short*)&m_pItem->GetItemData()->buf[0x34] = (short)GetDlgItemInt(IDC_EDIT_RESIST_FIRE);
	*(short*)&m_pItem->GetItemData()->buf[0x36] = (short)GetDlgItemInt(IDC_EDIT_RESIST_MAGIC);
	*(short*)&m_pItem->GetItemData()->buf[0x38] = (short)GetDlgItemInt(IDC_EDIT_RESIST_POISON);
	*(short*)&m_pItem->GetItemData()->buf[0x32] = (short)GetDlgItemInt(IDC_EDIT_RESIST_PHYSICAL);

	// damage
	*(short*)&m_pItem->GetItemData()->buf[0x1E] = (short)GetDlgItemInt(IDC_EDIT_DAMAGE_FIRE_FROM);
	*(short*)&m_pItem->GetItemData()->buf[0x26] = (short)GetDlgItemInt(IDC_EDIT_DAMAGE_FIRE_TO);
	*(short*)&m_pItem->GetItemData()->buf[0x1C] = (short)GetDlgItemInt(IDC_EDIT_DAMAGE_PHYSICAL_FROM);
	*(short*)&m_pItem->GetItemData()->buf[0x24] = (short)GetDlgItemInt(IDC_EDIT_DAMAGE_PHYSICAL_TO);
	*(short*)&m_pItem->GetItemData()->buf[0x20] = (short)GetDlgItemInt(IDC_EDIT_DAMAGE_MAGIC_FROM);
	*(short*)&m_pItem->GetItemData()->buf[0x28] = (short)GetDlgItemInt(IDC_EDIT_DAMAGE_MAGIC_TO);
	*(short*)&m_pItem->GetItemData()->buf[0x22] = (short)GetDlgItemInt(IDC_EDIT_DAMAGE_POISON_FROM);
	*(short*)&m_pItem->GetItemData()->buf[0x2A] = (short)GetDlgItemInt(IDC_EDIT_DAMAGE_POISON_TO);

	// attack/defense
	*(short*)&m_pItem->GetItemData()->buf[0x2C] = (short)GetDlgItemInt(IDC_EDIT_IATTACK);
	*(short*)&m_pItem->GetItemData()->buf[0x2E] = (short)GetDlgItemInt(IDC_EDIT_IDEFENSE);

	// level/minlevel
	m_pItem->GetItemData()->buf[0x19] = (unsigned char)GetDlgItemInt(IDC_EDIT_ILEVEL);
	m_pItem->GetItemData()->buf[0x14] = (unsigned char)GetDlgItemInt(IDC_EDIT_IMINLEVEL);

	CDialog::OnOK();
}
