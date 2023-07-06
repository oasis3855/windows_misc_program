// CookieCtrlDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "CookieCtrl.h"
#include "CookieCtrlDlg.h"
#include "DlgAbout.h"
#include "InstDlg.h"
#include <io.h>			// findfirst
#include "GlobalFunc.h"
#include "InstFunc.h"
#include "CheckWinVer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlDlg �_�C�A���O

CCookieCtrlDlg::CCookieCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCookieCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCookieCtrlDlg)
	m_str_folder = _T("");
	m_str_ext = _T("");
	m_str_exclude = _T("");
	m_int_days = 0;
	m_sw_days = FALSE;
	m_sw_exclude = FALSE;
	m_chk_auto_folder = FALSE;
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCookieCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCookieCtrlDlg)
	DDX_Control(pDX, IDC_CHECK_DAYS, m_sw_days_ctrl);
	DDX_Control(pDX, IDC_BTN_FOLDER_BRW, m_btn_folder_brw);
	DDX_Control(pDX, IDC_BTN_FOLDER_AUTO, m_btn_folder_auto);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_str_folder_ctrl);
	DDX_Control(pDX, IDC_STR_DAYS, m_str_days_ctrl);
	DDX_Control(pDX, IDC_SCR_DAYS, m_scroll_days_ctrl);
	DDX_Control(pDX, IDC_EDIT_DAYS, m_int_days_ctrl);
	DDX_Control(pDX, IDC_EDIT_EXCLUDE, m_str_exclude_ctrl);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_str_folder);
	DDV_MaxChars(pDX, m_str_folder, 255);
	DDX_Text(pDX, IDC_EDIT_EXT, m_str_ext);
	DDV_MaxChars(pDX, m_str_ext, 255);
	DDX_Text(pDX, IDC_EDIT_EXCLUDE, m_str_exclude);
	DDV_MaxChars(pDX, m_str_exclude, 255);
	DDX_Text(pDX, IDC_EDIT_DAYS, m_int_days);
	DDV_MinMaxInt(pDX, m_int_days, 0, 30);
	DDX_Check(pDX, IDC_CHECK_DAYS, m_sw_days);
	DDX_Check(pDX, IDC_CHECK_EXCLUDE, m_sw_exclude);
	DDX_Check(pDX, IDC_CHECK_AUTO_FOLDER, m_chk_auto_folder);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCookieCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CCookieCtrlDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FOLDER_BRW, OnBtnFolderBrw)
	ON_BN_CLICKED(IDC_BTN_FOLDER_AUTO, OnBtnFolderAuto)
	ON_BN_CLICKED(IDC_BTN_EXT, OnBtnExt)
	ON_BN_CLICKED(IDC_CHECK_EXCLUDE, OnCheckExclude)
	ON_BN_CLICKED(IDC_CHECK_DAYS, OnCheckDays)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BTN_RUN, OnBtnRun)
	ON_BN_CLICKED(IDC_BTN_STARTUP, OnBtnStartup)
	ON_BN_CLICKED(IDC_BTN_VERSION, OnBtnVersion)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_CHECK_AUTO_FOLDER, OnCheckAutoFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlDlg ���b�Z�[�W �n���h��

BOOL CCookieCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B

	// �`�F�b�N�{�b�N�X�̏�Ԃɂ��A�R���g���[���̗L���E������؂�ւ���
	OnCheckExclude();
	OnCheckDays();
	OnCheckAutoFolder();

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CCookieCtrlDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CCookieCtrlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// **********************************
// Cookie �t�H���_�̎Q��
// **********************************
void CCookieCtrlDlg::OnBtnFolderBrw() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	char str_tmp[MAX_PATH];
	CString sTmpMsg;

	sTmpMsg.LoadString(STR_MES_SEL_COKFOL);
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)sTmpMsg;
	bi.hwndOwner = m_hWnd;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, str_tmp);


	// �t�@�C�����̓{�b�N�X���A�b�v�f�[�g����
	SetDlgItemText(IDC_EDIT_FOLDER, str_tmp);
	
}

// **********************************
// ���W�X�g�����W����Cookie�t�H���_����
// **********************************
void CCookieCtrlDlg::OnBtnFolderAuto() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	HKEY hCU;
	DWORD lpType = REG_SZ;
	char str_tmp[MAX_PATH];
	unsigned long ulSize = MAX_PATH;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
		0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Cookies", NULL, &lpType, (unsigned char *)&str_tmp, &ulSize);
		RegCloseKey(hCU);
		// �t�@�C�����̓{�b�N�X���A�b�v�f�[�g����
		SetDlgItemText(IDC_EDIT_FOLDER, str_tmp);
	}
	
}

// **********************************
// �W���̑Ώۃt�@�C��������
// **********************************
void CCookieCtrlDlg::OnBtnExt() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	// �t�@�C�����̓{�b�N�X���A�b�v�f�[�g����
	SetDlgItemText(IDC_EDIT_EXT, "*.txt;*.url");
	
}

// **********************************
// �`�F�b�N�{�b�N�X�̏󋵂ɂ��A��A�N�e�B�u���ڂ̕ω�
// **********************************
void CCookieCtrlDlg::OnCheckExclude() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString sTmpMsg;

	if(!IsDlgButtonChecked(IDC_CHECK_EXCLUDE))
	{
		m_str_exclude_ctrl.EnableWindow(FALSE);
		sTmpMsg.LoadString(STR_CTL_CHK_DAYS_0);
	}
	else
	{
		m_str_exclude_ctrl.EnableWindow(TRUE);
		sTmpMsg.LoadString(STR_CTL_CHK_DAYS_1);
	}
	m_sw_days_ctrl.SetWindowText((LPCSTR)sTmpMsg);

}

// **********************************
// �`�F�b�N�{�b�N�X�̏󋵂ɂ��A��A�N�e�B�u���ڂ̕ω�
// **********************************
void CCookieCtrlDlg::OnCheckDays() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if(!IsDlgButtonChecked(IDC_CHECK_DAYS))
	{
		m_int_days_ctrl.EnableWindow(FALSE);
		m_scroll_days_ctrl.EnableWindow(FALSE);
		m_str_days_ctrl.EnableWindow(FALSE);
	}
	else
	{
		m_int_days_ctrl.EnableWindow(TRUE);
		m_scroll_days_ctrl.EnableWindow(TRUE);
		m_str_days_ctrl.EnableWindow(TRUE);
	}
	
}

// **********************************
// �`�F�b�N�{�b�N�X�̏󋵂ɂ��A��A�N�e�B�u���ڂ̕ω�
// **********************************
void CCookieCtrlDlg::OnCheckAutoFolder() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if(!IsDlgButtonChecked(IDC_CHECK_AUTO_FOLDER))
	{
		m_str_folder_ctrl.EnableWindow(TRUE);
		m_btn_folder_brw.EnableWindow(TRUE);
		m_btn_folder_auto.EnableWindow(TRUE);
		
	}
	else
	{
		m_str_folder_ctrl.EnableWindow(FALSE);
		m_btn_folder_brw.EnableWindow(FALSE);
		m_btn_folder_auto.EnableWindow(FALSE);
	}
	
}

// **********************************
// �����̃X�N���[���o�[�������ꂽ�ꍇ�̏���
// **********************************
//
// CScrollBar �́A���ɂ����Ă���Ȃ��̂ŁACWnd���X�N���[���o�[�������ꂽ���Ƃ����m����
// ���������Ȃ��Ƃ����Ȃ��B
void CCookieCtrlDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	int days;
	
	days = GetDlgItemInt(IDC_EDIT_DAYS);

	if(pScrollBar->GetDlgCtrlID() == IDC_SCR_DAYS)
	{
		switch(nSBCode)
		{
			case SB_LINEUP :
				if(days<30) days++;
				break;
			case SB_LINEDOWN :
				if(days>0) days--;
				break;
			default : break;
		}
		SetDlgItemInt(IDC_EDIT_DAYS, days);
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

// **********************************
//  ���������s
// **********************************
void CCookieCtrlDlg::OnBtnRun() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	// **********************************
	//  �_�C�A���O���ڂ̎擾���A�O���[�o���ϐ���
	// **********************************
	GetDlgItemText(IDC_EDIT_FOLDER, ::gstr_folder);
	GetDlgItemText(IDC_EDIT_EXT, ::gstr_ext);
	if(IsDlgButtonChecked(IDC_CHECK_EXCLUDE))
		::gint_sw_exclude = 1;
	else
		::gint_sw_exclude = 0;
	GetDlgItemText(IDC_EDIT_EXCLUDE, ::gstr_exclude);
	if(IsDlgButtonChecked(IDC_CHECK_DAYS))
		::gint_sw_days = 1;
	else
		::gint_sw_days = 0;
	::gint_days = GetDlgItemInt(IDC_EDIT_DAYS);

	::DeleteCookie();

	// **********************************
	//  ���W�X�g���̓��e�𕜌��i�O���[�o���ϐ��Ɂj
	//  �_�C�A���O�̏󋵂͂��̂܂�
	// **********************************
	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->RegReadAll();


}

// **********************************
//  �ăC���X�g�[���_�C�A���O��\��
// **********************************
void CCookieCtrlDlg::OnBtnStartup() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	ReInstallSeq();

}

// **********************************
//  �o�[�W�����_�C�A���O��\��
// **********************************
void CCookieCtrlDlg::OnBtnVersion() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CDlgAbout dlg;
	dlg.DoModal();
}

// **********************************
//  �w���v��\��
// **********************************
void CCookieCtrlDlg::OnBtnHelp() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->WinHelp(0);

}

// **********************************
//  F1 �L�[�������ꂽ�ꍇ�A�w���v��\��
// **********************************
BOOL CCookieCtrlDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	// �u�{�^���v�̕����g���悤�Ɍx������
	CString str_tmp1, str_tmp2;
	str_tmp1.LoadString(STR_MES_F1MES);
	str_tmp2.LoadString(STR_MES_F1CAP);
	::MessageBox(this->m_hWnd, str_tmp1, str_tmp2, MB_OK|MB_ICONEXCLAMATION);

	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->WinHelp(0);
	
	return 0;
}

// **********************************
//  �ăC���X�g�[����� �ꎮ
//
//  �߂�l  TRUE:�C���X�g�[�������܂��͕s�v,  FALSE:���[�U�L�����Z��
// **********************************
BOOL CCookieCtrlDlg::ReInstallSeq()
{
	CInstDlg dlg;
	dlg.m_r_autoexec = 1;	// �X�^�[�g���j���[�ɓo�^
	dlg.m_chk_uninst_start = TRUE;	// �X�^�[�g���j���[�ɃA���C���X�g�[���A�C�R��
	dlg.m_IsAdminInst = TRUE;
	CString sTmpMsg;


	// **********************************
	//  �C���X�g�[���󋵂̃`�F�b�N�Ɛݒ�
	// **********************************
	
	if(ChkHklmInstReg()) dlg.m_r_autoexec = 3;

	// **********************************
	//  Windows NT �n��Ƃ���ȊO�ŋ��
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT �̂Ƃ� ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run �������݉̂Ƃ� (Admin ���[�h)
			if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
			if(dlg.m_r_autoexec == 3)
			{
				::MkHklmInstReg();	// Admin �C���X�g�[��
				::RmHkcuInstReg();
			}
			else
			{
				::MkHkcuInstReg();	// Current User �C���X�g�[��
				::RmHklmInstReg();
			}
		}
		else
		{	// HKLM ... Run �������ݕs�̂Ƃ� (���[�U ���[�h)
			if(ChkHklmInstReg())
			{	// �ăC���X�g�[���͕s��
				sTmpMsg.LoadString(STR_MES_REIN_NOT);
				::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Re-Install Error", MB_OK|MB_ICONSTOP);
				return FALSE;
			}
			else
			{
				dlg.m_IsAdminInst = FALSE;	
				if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
				::MkHkcuInstReg();
			}
		}
	}
	else
	{	// ********************  WinNT�ȊO �̂Ƃ� ********************
		if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
		if(dlg.m_r_autoexec == 3)
		{
			::MkHklmInstReg();	// Admin �C���X�g�[��
			::RmHkcuInstReg();
		}
		else
		{
			::MkHkcuInstReg();	// Current User �C���X�g�[��
			::RmHklmInstReg();
		}
	}

	// **********************************
	//  �C���X�g�[���_�C�A���O�ł̑I�����ڂɂ��A�C���X�g�[�����
	// **********************************
	//  �������s�I�v�V����
	//  �I�����ꂽ�I�v�V�����ȊO�́u�������s�v���폜���邱�Ƃ������ɍs��
	// **********************************
	switch(dlg.m_r_autoexec)
	{
	case 0 :	// �������s���Ȃ�
		::RmHklmLnk();
		::RmHkcuLnk();
		::RmStartMnu();
		break;
	case 1 :	// �X�^�[�g���j���[
		::RmHklmLnk();
		::RmHkcuLnk();
		::MkStartMnu();
		break;
	case 2 :	// HKCU �Ɏ������s
		::RmHklmLnk();
		::MkHkcuLnk();
		::RmStartMnu();
		break;
	case 3 :	// HKLM �Ɏ������s
		::MkHklmLnk();
		::RmHkcuLnk();
		::RmStartMnu();
		break;
	}

	// **********************************
	//  �X�^�[�g���j���[�ɐݒ�A�C�R���o�^
	// **********************************
	::MkProgramsMnu();

	// **********************************
	//  �R���p�l����A���C���X�g�[������ݒ�
	// **********************************
	if(dlg.m_chk_uninst_cpl)
	{
		if(::IsWinNT()) ::MkHkcuUninstMnu();
		else ::MkHklmUninstMnu();
	}

	// **********************************
	//  �X�^�[�g���j���[����A���C���X�g�[������ݒ�
	// **********************************
	if(dlg.m_chk_uninst_start)
		::MkUninstMnu();

	return TRUE;

}
