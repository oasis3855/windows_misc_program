// PropertySheet.cpp : �C���v�������e�[�V���� �t�@�C��
// CPropAbout ����� CPropConf�̋L�q

#include "stdafx.h"
#include "Cursor32.h"
#include "PropertySheet.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropAbout �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CPropAbout, CPropertyPage)

CPropAbout::CPropAbout() : CPropertyPage(CPropAbout::IDD)
{
	//{{AFX_DATA_INIT(CPropAbout)
	m_ddx_odometer = _T("");
	//}}AFX_DATA_INIT
	m_uninstall=FALSE;
}

CPropAbout::~CPropAbout()
{
}

void CPropAbout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropAbout)
	DDX_Text(pDX, IDC_ODOMETER, m_ddx_odometer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAbout, CPropertyPage)
	//{{AFX_MSG_MAP(CPropAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropAbout ���b�Z�[�W �n���h��

// ���c�Z�[�W�n���h���Ȃ�

/////////////////////////////////////////////////////////////////////////////
// CPropConf �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CPropConf, CPropertyPage)

CPropConf::CPropConf() : CPropertyPage(CPropConf::IDD)
{
	//{{AFX_DATA_INIT(CPropConf)
	m_ddx_assist = FALSE;
	m_ddx_title = _T("");
	m_ddx_x = 0;
	m_ddx_y = 0;
	m_ddx_timertick = 0;
	m_ddx_hotkey1 = -1;
	m_ddx_hotkey2 = -1;
	m_ddx_changecolor = FALSE;
	m_ddx_isNotTaskbar = FALSE;
	m_ddx_isTopmost = FALSE;
	m_ddx_nChangeDlgColor = FALSE;
	//}}AFX_DATA_INIT
}

CPropConf::~CPropConf()
{
}

void CPropConf::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropConf)
	DDX_Control(pDX, IDC_PICKDLGCOL, m_pickdlgcol);
	DDX_Control(pDX, IDC_PICKDLGBACKCOL, m_pickdlgbackcol);
	DDX_Control(pDX, IDC_PICKCOLOR, m_pickcolor);
	DDX_Control(pDX, IDC_HOTKEY2, m_hotkey2);
	DDX_Control(pDX, IDC_HOTKEY1, m_hotkey1);
	DDX_Check(pDX, IDC_ASSIST, m_ddx_assist);
	DDX_Text(pDX, IDC_TITLE, m_ddx_title);
	DDV_MaxChars(pDX, m_ddx_title, 20);
	DDX_Text(pDX, IDC_INITX, m_ddx_x);
	DDX_Text(pDX, IDC_INITY, m_ddx_y);
	DDX_Text(pDX, IDC_TIMERTICK, m_ddx_timertick);
	DDV_MinMaxInt(pDX, m_ddx_timertick, 10, 2000);
	DDX_LBIndex(pDX, IDC_HOTKEY1, m_ddx_hotkey1);
	DDX_LBIndex(pDX, IDC_HOTKEY2, m_ddx_hotkey2);
	DDX_Check(pDX, IDC_CHANGECOLOR, m_ddx_changecolor);
	DDX_Check(pDX, IDC_DSPNOTTASK, m_ddx_isNotTaskbar);
	DDX_Check(pDX, IDC_DSPTOP, m_ddx_isTopmost);
	DDX_Check(pDX, IDC_CHANGEDLGCOLOR, m_ddx_nChangeDlgColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropConf, CPropertyPage)
	//{{AFX_MSG_MAP(CPropConf)
	ON_BN_CLICKED(IDC_ASSIST, OnAssist)
	ON_BN_CLICKED(IDC_PICKCOLOR, OnPickcolor)
	ON_BN_CLICKED(IDC_CHANGECOLOR, OnChangecolor)
	ON_BN_CLICKED(IDC_PICKDLGBACKCOL, OnPickdlgbackcol)
	ON_BN_CLICKED(IDC_PICKDLGCOL, OnPickdlgcol)
	ON_BN_CLICKED(IDC_CHANGEDLGCOLOR, OnChangedlgcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropConf ���b�Z�[�W �n���h��

// ���X�g�{�b�N�X�̓��e���Z�b�g����
BOOL CPropConf::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	char *keyname[] = {"Shift","Ctrl","Alt","Insert","Delete",
			"BackSpace","tab","F9","F10","F11","F12"};	// 11 key
	// �z�b�g�L�[�̖��O��ݒ�
	for(int i=0;i<11;i++)
	{
		m_hotkey1.AddString(keyname[i]);	
		m_hotkey2.AddString(keyname[i]);
	}
	// �z�b�g�L�[��I������
	m_hotkey1.SetCurSel(m_ddx_hotkey1);
	m_hotkey2.SetCurSel(m_ddx_hotkey2);
	// �A�V�X�^���g�������̎��̏����i�O���C�\���j
	if(!IsDlgButtonChecked(IDC_ASSIST))
	{
		m_hotkey1.EnableWindow(FALSE);
		m_hotkey2.EnableWindow(FALSE);
	}
	if(!IsDlgButtonChecked(IDC_CHANGECOLOR))
	{
		m_pickcolor.EnableWindow(FALSE);
	}
	if(!IsDlgButtonChecked(IDC_CHANGEDLGCOLOR))
	{
		m_pickdlgcol.EnableWindow(FALSE);
		m_pickdlgbackcol.EnableWindow(FALSE);
	}

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CPropConf::OnAssist() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	// �A�V�X�^���g�������̎��̏����i�O���C�\���j
	if(!IsDlgButtonChecked(IDC_ASSIST))
	{
		m_hotkey1.EnableWindow(FALSE);
		m_hotkey2.EnableWindow(FALSE);
	}
	else
	{
		m_hotkey1.EnableWindow(TRUE);
		m_hotkey2.EnableWindow(TRUE);
	}

	
}

void CPropConf::OnPickcolor() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CColorDialog dlg(m_TextColor, CC_FULLOPEN);
	if(dlg.DoModal()==IDOK)
		m_TextColor = dlg.GetColor();
}

void CPropConf::OnPickdlgbackcol() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CColorDialog dlg(m_DlgBackColor, CC_FULLOPEN);
	if(dlg.DoModal()==IDOK)
		m_DlgBackColor = dlg.GetColor();
	
}

void CPropConf::OnPickdlgcol() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CColorDialog dlg(m_DlgColor, CC_FULLOPEN);
	if(dlg.DoModal()==IDOK)
		m_DlgColor = dlg.GetColor();
	
}

void CPropConf::OnChangecolor() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if(!IsDlgButtonChecked(IDC_CHANGECOLOR))
	{
		m_pickcolor.EnableWindow(FALSE);
	}
	else
	{
		m_pickcolor.EnableWindow(TRUE);
	}
	
}


void CPropConf::OnChangedlgcolor() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if(!IsDlgButtonChecked(IDC_CHANGEDLGCOLOR))
	{
		m_pickdlgcol.EnableWindow(FALSE);
		m_pickdlgbackcol.EnableWindow(FALSE);
	}
	else
	{
		m_pickdlgcol.EnableWindow(TRUE);
		m_pickdlgbackcol.EnableWindow(TRUE);
	}
	
}


