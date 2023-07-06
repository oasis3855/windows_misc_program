// CookieCtrl.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#include "CookieCtrl.h"
#include "CookieCtrlDlg.h"
#include "InstDlg.h"
#include "GlobalFunc.h"
#include "InstFunc.h"
#include "CheckWinVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// �O���[�o���ϐ�
CString gstr_folder;		// Cookie�t�H���_
CString gstr_ext;		// Cookie �t�@�C����
CString gstr_exclude;	// ���O����t�@�C��������
int gint_sw_auto_folder;			// Cookie �t�H���_�̎����擾�X�C�b�`
int gint_days;			// ���O�������
int gint_sw_exclude;		// ���O����t�@�C��������̃X�C�b�`
int gint_sw_days;		// ���O������Ԃ̃X�C�b�`
int gint_admin_inst;	// �A�h�~�j�X�g���[�^�[�i�ŏ��̃C���X�g�[����)
int gint_user_inst;		// ���݂̃��[�U�ŃC���X�g�[������Ă��邩


/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlApp

BEGIN_MESSAGE_MAP(CCookieCtrlApp, CWinApp)
	//{{AFX_MSG_MAP(CCookieCtrlApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlApp �N���X�̍\�z

CCookieCtrlApp::CCookieCtrlApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CCookieCtrlApp �I�u�W�F�N�g

CCookieCtrlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlApp �N���X�̏�����

BOOL CCookieCtrlApp::InitInstance()
{
	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������������
	//  ��Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ��̂��폜����
	//  ���������B

#ifdef _AFXDLL
	Enable3dControls();			// ���L DLL ���� MFC ���g���ꍇ�͂������R�[�����Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N����ꍇ�͂������R�[�����Ă��������B
#endif

	CString str_tmp1, str_tmp2;		// �_�C�A���O���b�Z�[�W�Ȃǂ̈ꎞ�i�[

	// �V�X�e�����W�X�g���̎g�p�ʒu��ݒ肷��
	// \HKEY_USERS\.Default\Software\hi soft\AFS_IDS_APP_TITLE\Settings
	SetRegistryKey((LPCTSTR)"hi soft");
	LoadStdProfileSettings(0);	// MRU���X�g�Ǘ�(0:���Ȃ�)



	// **********************************
	//  ���W�X�g������S�ݒ�l��ǂݍ���
	// **********************************
	RegReadAll();

	// **********************************
	//  �R�}���h���C�������ɂ�镪��
	// **********************************
	if(!strcmpi(m_lpCmdLine, ""))
	{	// �I�v�V�����Ȃ�
		// **********************************
		//  �C���X�g�[���̋N���B
		//  ���łɁA�C���X�g�[���ς݂Ǝ��́A�P�� TRUE ��Ԃ�����
		// **********************************
		if(!InstallSeq())
			return FALSE;	// �C���X�g�[�������[�U���L�����Z�������Ƃ� �i�v���O�����I���j
	}
	else if(!strcmpi(m_lpCmdLine, "/start"))
	{	// �������s
		::DeleteCookie();
		return FALSE;		// �i�v���O�����I���j
	}
	else if(!strcmpi(m_lpCmdLine, "/delete"))
	{	// �A���C���X�g�[��
		UninstallSeq();
		return FALSE;
	}


	// **********************************
	//  �ݒ�_�C�A���O�̕\��
	// **********************************
	CCookieCtrlDlg dlg;
	m_pMainWnd = &dlg;
	dlg.m_chk_auto_folder = ::gint_sw_auto_folder;
	dlg.m_str_folder = ::gstr_folder;
	dlg.m_str_ext = ::gstr_ext;
	dlg.m_str_exclude = ::gstr_exclude;
	dlg.m_int_days = ::gint_days;
	dlg.m_sw_exclude = ::gint_sw_exclude;
	dlg.m_sw_days = ::gint_sw_days;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
		::gint_sw_auto_folder = dlg.m_chk_auto_folder;
		if(!::gint_sw_auto_folder) ::gstr_folder = dlg.m_str_folder;
		else ::gstr_folder = "";
		::gstr_ext = dlg.m_str_ext;
		::gstr_exclude = dlg.m_str_exclude;
		::gint_days = dlg.m_int_days;
		::gint_sw_exclude = dlg.m_sw_exclude;
		::gint_sw_days = dlg.m_sw_days;
		::gint_user_inst = 1;	// �C���X�g�[���ς�
		RegWriteAll();
	}

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}

void CCookieCtrlApp::RegReadAll()
{
	::gstr_folder = GetProfileString("Settings","folder","");
	::gstr_ext = GetProfileString("Settings","ext","*.txt;*.url");
	::gstr_exclude = GetProfileString("Settings","exclude","");
	::gint_sw_auto_folder = GetProfileInt("Settings","auto",1);
	::gint_days = GetProfileInt("Settings","days",0);
	::gint_sw_exclude = GetProfileInt("Settings","sw_exclude",0);
	::gint_sw_days = GetProfileInt("Settings","sw_days",0);
	::gint_admin_inst = GetProfileInt("Settings","admin_inst",0);
	::gint_user_inst = GetProfileInt("Settings","user_inst",0);

	if(::gstr_folder == "" || ::gint_sw_auto_folder)
	{
		HKEY hCU;
		DWORD lpType = REG_SZ;
		char str_tmp[MAX_PATH];
		unsigned long ulSize = MAX_PATH;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegQueryValueEx( hCU, "Cookies", NULL, &lpType, (unsigned char *)&str_tmp, &ulSize);
			RegCloseKey(hCU);
			::gstr_folder = str_tmp;
		}
	}
}

void CCookieCtrlApp::RegWriteAll()
{
	if(::gint_sw_auto_folder) ::gstr_folder = "";
	if(::gstr_folder != GetProfileString("Settings","folder",""))
		WriteProfileString("Settings","folder",::gstr_folder);
	if(::gstr_ext != GetProfileString("Settings","ext","*.txt;*.url"))
		WriteProfileString("Settings","ext",::gstr_ext);
	if(::gstr_exclude != GetProfileString("Settings","exclude",""))
		WriteProfileString("Settings","exclude",::gstr_exclude);
	if(::gint_sw_auto_folder != (int)GetProfileInt("Settings","auto",1))
		WriteProfileInt("Settings","auto",::gint_sw_auto_folder);
	if(::gint_days != (int)GetProfileInt("Settings","days",0))
		WriteProfileInt("Settings","days",::gint_days);
	if(::gint_sw_exclude != (int)GetProfileInt("Settings","sw_exclude",0))
		WriteProfileInt("Settings","sw_exclude",::gint_sw_exclude);
	if(::gint_sw_days != (int)GetProfileInt("Settings","sw_days",0))
		WriteProfileInt("Settings","sw_days",::gint_sw_days);
	if(::gint_user_inst != (int)GetProfileInt("Settings","user_inst",0))
		WriteProfileInt("Settings","user_inst",::gint_user_inst);

}

// **********************************
//  �C���X�g�[����� �ꎮ
//
//  �߂�l  TRUE:�C���X�g�[�������܂��͕s�v,  FALSE:���[�U�L�����Z��
// **********************************
BOOL CCookieCtrlApp::InstallSeq()
{
	CInstDlg dlg;
	dlg.m_r_autoexec = 1;	// �X�^�[�g���j���[�ɓo�^
	dlg.m_chk_uninst_start = TRUE;	// �X�^�[�g���j���[�ɃA���C���X�g�[���A�C�R��
	dlg.m_IsAdminInst = TRUE;


	// **********************************
	//  �C���X�g�[���t���O�̃`�F�b�N (HKLM/HKCU)
	// **********************************
	if(ChkHkcuInstReg() || ChkHklmInstReg()) return TRUE;

	// **********************************
	//  Windows NT �n��Ƃ���ȊO�ŋ��
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT �̂Ƃ� ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run �������݉̂Ƃ� (Admin ���[�h)
			if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
			if(dlg.m_r_autoexec == 3) ::MkHklmInstReg();	// Admin �C���X�g�[��
			else ::MkHkcuInstReg();	// Current User �C���X�g�[��
		}
		else
		{	// HKLM ... Run �������ݕs�̂Ƃ� (���[�U ���[�h)
			dlg.m_IsAdminInst = FALSE;	
			if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
			::MkHkcuInstReg();
		}
	}
	else
	{	// ********************  WinNT�ȊO �̂Ƃ� ********************
		if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
		if(dlg.m_r_autoexec == 3) ::MkHklmInstReg();	// Admin �C���X�g�[��
		else ::MkHkcuInstReg();	// Current User �C���X�g�[��
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

// **********************************
//  �A���C���X�g�[����� �ꎮ
//
//  �߂�l  TRUE:�����܂��͕s�v,  FALSE:���s�܂��̓��[�U�L�����Z��
// **********************************
BOOL CCookieCtrlApp::UninstallSeq()
{
	CString sTmpMsg;

	// **********************************
	//  Windows NT �n��Ƃ���ȊO�ŋ��
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT �̂Ƃ� ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run �������݉̂Ƃ� (Admin ���[�h�����H)
			if(::ChkHklmInstReg())
			{	// Admin ���[�h�ŃC���X�g�[������Ă����Ƃ�
				sTmpMsg.LoadString(STR_MES_UNIN_ADM);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ���[�U���L�����Z��
				::RmHklmInstReg();
			}
			else
			{	// �J�����g�E���[�U�E���[�h�̂Ƃ�
				sTmpMsg.LoadString(STR_MES_UNIN_CUR);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ���[�U���L�����Z��
				::RmHkcuInstReg();
			}
		}
		else
		{	// HKLM ... Run �������ݕs�̂Ƃ� (���[�U ���[�h)
			if(::ChkHklmInstReg())
			{	// Admin ���[�h�ŃC���X�g�[������Ă����Ƃ�
				sTmpMsg.LoadString(STR_MES_UNIN_NOT);
				::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall Error", MB_OK|MB_ICONSTOP);
			}
			else
			{	// �J�����g�E���[�U�E���[�h�̂Ƃ�
				sTmpMsg.LoadString(STR_MES_UNIN_CUR);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ���[�U���L�����Z��
				::RmHkcuInstReg();
			}
		}
	}
	else
	{	// ********************  WinNT�ȊO �̂Ƃ� ********************
		if(::ChkHklmInstReg())
		{	// Admin ���[�h�ŃC���X�g�[������Ă����Ƃ�
			sTmpMsg.LoadString(STR_MES_UNIN_ADM);
			if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
				return FALSE;	// ���[�U���L�����Z��
			::RmHklmInstReg();
		}
		else
		{	// �J�����g�E���[�U�E���[�h�̂Ƃ�
			sTmpMsg.LoadString(STR_MES_UNIN_CUR);
			if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
				return FALSE;	// ���[�U���L�����Z��
			::RmHkcuInstReg();
		}
	}

	::RmHkcuInstReg();		// �ǂ���ɂ��Ă��A����͏����Ă���
	::RmHklmLnk();
	::RmHkcuLnk();
	::RmStartMnu();
	::RmUserReg();

	::RmHkcuUninstMnu();
	::RmHklmUninstMnu();
	::RmUninstMnu();

	::RmProgramsMnu();

	return TRUE;

}
