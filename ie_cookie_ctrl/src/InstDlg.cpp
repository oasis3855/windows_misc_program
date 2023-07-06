// InstDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "cookiectrl.h"
#include "InstDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstDlg �_�C�A���O


CInstDlg::CInstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInstDlg)
	m_r_autoexec = -1;
	m_chk_uninst_cpl = FALSE;
	m_chk_uninst_start = FALSE;
	//}}AFX_DATA_INIT
}


void CInstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInstDlg)
	DDX_Control(pDX, IDC_R_AUTOEXEC, m_r_autoexec_ctrl1);	// �蓮�Œǉ�
	DDX_Control(pDX, IDC_R_AUTOEXEC2, m_r_autoexec_ctrl2);	// �蓮�Œǉ�
	DDX_Control(pDX, IDC_R_AUTOEXEC3, m_r_autoexec_ctrl3);	// �蓮�Œǉ�
	DDX_Control(pDX, IDC_R_AUTOEXEC4, m_r_autoexec_ctrl4);	// �蓮�Œǉ�
	DDX_Radio(pDX, IDC_R_AUTOEXEC, m_r_autoexec);
	DDX_Check(pDX, IDC_CHECK_UNINST_CPL, m_chk_uninst_cpl);
	DDX_Check(pDX, IDC_CHECK_UNINST_START, m_chk_uninst_start);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInstDlg, CDialog)
	//{{AFX_MSG_MAP(CInstDlg)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstDlg ���b�Z�[�W �n���h��

BOOL CInstDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->WinHelp(0);

	return 0;
}

BOOL CInstDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	// *******************
	// HKLM...run �ɏ������݌����̖����Ƃ��́A�A�h�~�j�X�g���[�^�C���X�g�[���𖳌���
	// DDX_Control(pDX, IDC_R_AUTOEXEC, m_r_autoexec_ctrl); ��  IDC_R_AUTOEXEC4 �Ə���������
	// *******************
	if(!m_IsAdminInst) m_r_autoexec_ctrl4.EnableWindow(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// **********************************
//  �w���v�̕\��
// **********************************
void CInstDlg::OnBtnHelp() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->WinHelp(0);
	
}
