// DlgSysConf.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "infobar00.h"
#include "DlgSysConf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysConf �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CDlgSysConf, CPropertyPage)

CDlgSysConf::CDlgSysConf() : CPropertyPage(CDlgSysConf::IDD)
{
	//{{AFX_DATA_INIT(CDlgSysConf)
	m_nSpeed = 0;
	m_nTimer = 0;
	m_tInterval = 0;
	m_bTopmost = FALSE;
	//}}AFX_DATA_INIT
}

CDlgSysConf::~CDlgSysConf()
{
}

void CDlgSysConf::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysConf)
	DDX_Control(pDX, IDC_AREA_FORE_COLOR, m_ctrl_area_fore_color);
	DDX_Control(pDX, IDC_AREA_BACK_COLOR, m_ctrl_area_back_color);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_nSpeed);
	DDV_MinMaxUInt(pDX, m_nSpeed, 1, 20);
	DDX_Text(pDX, IDC_EDIT_TIMER, m_nTimer);
	DDV_MinMaxUInt(pDX, m_nTimer, 10, 1000);
	DDX_Text(pDX, IDC_EDIT_REFRESH, m_tInterval);
	DDV_MinMaxUInt(pDX, m_tInterval, 0, 65536);
	DDX_Check(pDX, IDC_CHK_TOPMOST, m_bTopmost);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysConf, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgSysConf)
	ON_BN_CLICKED(IDC_BTN_BACK_COLOR, OnBtnBackColor)
	ON_BN_CLICKED(IDC_BTN_FORE_COLOR, OnBtnForeColor)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_FONT, OnBtnFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysConf ���b�Z�[�W �n���h��

void CDlgSysConf::OnBtnBackColor() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CColorDialog dlg;
	CDC *pDC;
	CRect rect;

	dlg.m_cc.Flags |= CC_RGBINIT;
	dlg.m_cc.rgbResult = m_cBackColor;

	if(dlg.DoModal() == IDOK)
	{
		pDC = m_ctrl_area_back_color.GetDC();
		m_ctrl_area_back_color.GetWindowRect(&rect);
		pDC->FillSolidRect(1,1,rect.right-rect.left-2, rect.bottom-rect.top-2,dlg.GetColor());

		m_cBackColor = dlg.GetColor();

		ReleaseDC(pDC);
	}
}

void CDlgSysConf::OnBtnForeColor() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CColorDialog dlg;
	CDC *pDC;
	CRect rect;

	dlg.m_cc.Flags |= CC_RGBINIT;
	dlg.m_cc.rgbResult = m_cForeColor;

	if(dlg.DoModal() == IDOK)
	{
		pDC = m_ctrl_area_fore_color.GetDC();
		m_ctrl_area_fore_color.GetWindowRect(&rect);
		pDC->FillSolidRect(1,1,rect.right-rect.left-2, rect.bottom-rect.top-2,dlg.GetColor());

		m_cForeColor = dlg.GetColor();

		ReleaseDC(pDC);
	}
	
}


void CDlgSysConf::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	CDC *pDC;
	CRect rect;

	pDC = m_ctrl_area_back_color.GetDC();
	m_ctrl_area_back_color.GetWindowRect(&rect);
	pDC->FillSolidRect(1,1,rect.right-rect.left-2, rect.bottom-rect.top-2,m_cBackColor);
	ReleaseDC(pDC);

	pDC = m_ctrl_area_fore_color.GetDC();
	m_ctrl_area_fore_color.GetWindowRect(&rect);
	pDC->FillSolidRect(1,1,rect.right-rect.left-2, rect.bottom-rect.top-2,m_cForeColor);
	ReleaseDC(pDC);
	
	// �`��p���b�Z�[�W�Ƃ��� CPropertyPage::OnPaint() ���Ăяo���Ă͂����܂���
}


void CDlgSysConf::OnBtnFont() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CDC *pDC;
	CFont fnt;		// �b��t�H���g�쐬�p
	CFont *pfnt;	// DC�Ɋ��蓖�Ă��Ă��錻�݂̃t�H���g�𓾂�Ƃ��Ɏg��
	LOGFONT lf, lf2;
	CString sTmp;

	if(nFontPoint <= 0)
	{	// �t�H���g�����܂��w�肳��Ă��Ȃ��Ƃ�
		//*******************
		// �Ƃ肠�����K���ȃt�H���g�����݂̃f�o�C�X�R���e�L�X�g�̊���t��
		// ���̂Ƃ��Ԃ��Ă���A���݂̃t�H���g���𓾂�
		//*******************
		pDC = this->GetDC();				// �f�o�C�X�R���e�L�X�g�𓾂�
		this->GetFont()->GetLogFont(&lf);	// ���̃_�C�A���O�̌��݂̃t�H���g
		fnt.CreateFontIndirect(&lf);		// �b��t�H���g�쐬
		pfnt = pDC->SelectObject(&fnt);		// �b��t�H���g�̊��t�ƁA���݃t�H���g�̎擾
		pfnt->GetLogFont(&lf2);				// ���݃t�H���g�� LOGFONT �\���̂�
											// �P��̎g�p�ŁApfnt �͔j�󂳂��

		pDC->SelectStockObject(DEVICE_DEFAULT_FONT);	// �V�X�e���t�H���g������t��
		fnt.DeleteObject();					// �b��t�H���g�̉��
		ReleaseDC(pDC);						// DC�̉��
	}
	else
	{
		fnt.CreatePointFont(nFontPoint, sFontName);
		fnt.GetLogFont(&lf2);
		fnt.DeleteObject();
	}

	
	CFontDialog dlg(&lf2);

//	dlg.m_cf.iPointSize = 120;		// 12pt
	dlg.m_cf.rgbColors  = m_cForeColor;


	if(dlg.DoModal() == IDOK)
	{
		nFontPoint = dlg.m_cf.iPointSize;
		sFontName = dlg.m_lf.lfFaceName;

	sTmp.Format("%s %.1f point", sFontName, nFontPoint/10.0);

		SetDlgItemText(IDC_TXT_FONT,sTmp);
	}
}

BOOL CDlgSysConf::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CString sTmp;

	sTmp.Format("%s %.1f point", sFontName, nFontPoint/10.0);

	SetDlgItemText(IDC_TXT_FONT,sTmp);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}
