// Cursor32.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#include "Cursor32.h"
#include "InstDlg.h"
#include "global.h"

#include "InstFunc.h"
#include "CheckWinVer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char  sMutexInstance[] = {"Cursor32_free"};    //  ���d�N���h�~�̂��ߗ��p


/////////////////////////////////////////////////////////////////////////////
// CCursor32App

BEGIN_MESSAGE_MAP(CCursor32App, CWinApp)
	//{{AFX_MSG_MAP(CCursor32App)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCursor32App �N���X�̍\�z

CCursor32App::CCursor32App()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CCursor32App �I�u�W�F�N�g

CCursor32App theApp;

/////////////////////////////////////////////////////////////////////////////
// CCursor32App �N���X�̏�����

BOOL CCursor32App::InitInstance()
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

	// ڼ޽�؂����ޯĂ��ȉ��̂悤�ɂ���
	// HKEY_USERS/.Default/Software/HI-soft/Cursor32 ...
//	m_pszAppName = "Cursor32";
	// ���\�[�X��AFX_IDS_APP_TITLE�ɃA�v���P�[�V��������ݒ肷�邱�� 
	// m_pszAppName = "compdel" �Ƃ��Ă͂����Ȃ��I ASSERTION �G���[�ƂȂ�
	SetRegistryKey((LPCTSTR)"hi soft");


	// **********************************
	//  ���d�N���h�~
	// **********************************
	m_hMutex = ::CreateMutex(NULL, TRUE, sMutexInstance);
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{	// ���łɋN�����Ă���
//		::CloseHandle(m_hMutex);
		m_bMutexOwner = FALSE;
		::MessageBox(NULL, "Cursor32 is already executed", "Cursor32", MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	m_bMutexOwner = TRUE;


	CCursor32Dlg dlg;

	// **********************************
	//  ���W�X�g������S�ݒ�l��ǂݍ���
	// **********************************
	RegConfigRead(&dlg);

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
	else if(!strcmpi(m_lpCmdLine, "/delete"))
	{	// �A���C���X�g�[��
		UninstallSeq();
		return FALSE;
	}

//	m_pMainWnd = &dlg;

	// �_�C�A���O���̂̕\���F��ύX����
	if(dlg.nChangeDlgColor) SetDialogBkColor(dlg.DlgBackColor, dlg.DlgColor);

	dlg.DoModal(); // ���C���_�C�A���O�̕\��

	RegConfigWrite(&dlg);

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}


int CCursor32App::ExitInstance() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

	if(m_bMutexOwner)
	{
		// **********************************
		//  ���d�N���h�~�� Mutex �����
		// **********************************
		::ReleaseMutex(m_hMutex);
		::CloseHandle(m_hMutex);
	}
	
	return CWinApp::ExitInstance();
}


// **********************************
//  ���W�X�g������ݒ�l��ǂݍ���
// **********************************
void CCursor32App::RegConfigRead(CCursor32Dlg *dlg)
{
	// ��Ɏ�O�ɕ\��
	dlg->isTopmost = GetProfileInt("Settings","top",1);

	// �^�X�N�o�[�ɕ\�����Ȃ�
	dlg->isNotTaskbar = GetProfileInt("Settings","ntask",1);

	// �^�C�}�[�Ԋu
	dlg->timertick = GetProfileInt("Settings","timer",100);

	// �J�[�\���A�V�X�^���X
	dlg->IsAssist = GetProfileInt("Settings","assist",0);
	// ���΍��W���[�h
	dlg->IsRelmode = GetProfileInt("Settings","relmode",0);
	// �z�b�g�L�[
	dlg->hotkey1 = GetProfileInt("Settings","hotkey1",VK_CONTROL);
	dlg->hotkey2 = GetProfileInt("Settings","hotkey2",VK_SHIFT);
	dlg->hotkey_rel = GetProfileInt("Settings","hotkey_rel",VK_F12);

	// �^�C�g��
	dlg->title = GetProfileString("Settings","title","Cursor32");

	// �E�C���h�E�ʒu
	dlg->x_init = GetProfileInt("Settings","x",0);
	dlg->y_init = GetProfileInt("Settings","y",0);

	// XY�\���F��ς��邩�ǂ���
	dlg->nChangeColor = GetProfileInt("Settings","color",1);
	// XY�\���F
	dlg->TextColor = GetProfileInt("Settings","b",0)*0x10000L +
					GetProfileInt("Settings","g",0)*0x100L +
					GetProfileInt("Settings","r",128);

	// �_�C�A���O�̐F��ς��邩�ǂ���
	dlg->nChangeDlgColor = GetProfileInt("Settings","colordlg",0);
	// �_�C�A���O�O�i�F
	dlg->DlgColor = GetProfileInt("Settings","db",0)*0x10000L +
					GetProfileInt("Settings","dg",0)*0x100L +
					GetProfileInt("Settings","dr",0);
	// �_�C�A���O�w�i�F
	dlg->DlgBackColor = GetProfileInt("Settings","dbb",255)*0x10000L +
					GetProfileInt("Settings","dbg",255)*0x100L +
					GetProfileInt("Settings","dbr",255);
}

// **********************************
//  ���W�X�g���ɐݒ�l����������
// **********************************
void CCursor32App::RegConfigWrite(CCursor32Dlg *dlg)
{
	// ��Ɏ�O�ɕ\��
	if(dlg->isTopmost != (BOOL)GetProfileInt("Settings","top",1))
		WriteProfileInt("Settings","top",dlg->isTopmost);

	// �^�X�N�o�[�ɕ\�����Ȃ�
	if(dlg->isNotTaskbar != (BOOL)GetProfileInt("Settings","ntask",1))
		WriteProfileInt("Settings","ntask",dlg->isNotTaskbar);

	// �^�C�}�[�Ԋu
	if(dlg->timertick != (int)GetProfileInt("Settings","timer",100))
		WriteProfileInt("Settings","timer",dlg->timertick);

	// �J�[�\���A�V�X�^���X
	if(dlg->IsAssist != (BOOL)GetProfileInt("Settings","assist",0))
		WriteProfileInt("Settings","assist",dlg->IsAssist);
	// ���΍��W���[�h
	if(dlg->IsRelmode != (BOOL)GetProfileInt("Settings","relmode",0))
		WriteProfileInt("Settings","relmode",dlg->IsRelmode);
	// �z�b�g�L�[
	if(dlg->hotkey1 != GetProfileInt("Settings","hotkey1",VK_CONTROL))
		WriteProfileInt("Settings","hotkey1",dlg->hotkey1);
	if(dlg->hotkey2 != GetProfileInt("Settings","hotkey2",VK_SHIFT))
		WriteProfileInt("Settings","hotkey2",dlg->hotkey2);
	if(dlg->hotkey_rel != GetProfileInt("Settings","hotkey_rel",VK_F12))
		WriteProfileInt("Settings","hotkey_rel",dlg->hotkey_rel);

	// �^�C�g��
	if(dlg->title != GetProfileString("Settings","title","Cursor32"))
		WriteProfileString("Settings","title",dlg->title);

	// �E�C���h�E�ʒu
	if(dlg->x_init != (int)GetProfileInt("Settings","x",0))
		WriteProfileInt("Settings","x",dlg->x_init);
	if(dlg->y_init != (int)GetProfileInt("Settings","y",0))
		WriteProfileInt("Settings","y",dlg->y_init);

	// XY�\���F��ς��邩�ǂ���
	if(dlg->nChangeColor != (BOOL)GetProfileInt("Settings","color",1))
		WriteProfileInt("Settings","color",dlg->nChangeColor);
	// XY�\���F
	if(dlg->TextColor != GetProfileInt("Settings","b",0)*0x10000L +
					GetProfileInt("Settings","g",0)*0x100L +
					GetProfileInt("Settings","r",128))
	{
		WriteProfileInt("Settings","b",(dlg->TextColor/0x10000L)&0xff);
		WriteProfileInt("Settings","g",(dlg->TextColor/0x100L)&0xff);
		WriteProfileInt("Settings","r", dlg->TextColor&0xff);
	}

	// �_�C�A���O�̐F��ς��邩�ǂ���
	if(dlg->nChangeDlgColor != (BOOL)GetProfileInt("Settings","colordlg",0))
		WriteProfileInt("Settings","colordlg",dlg->nChangeDlgColor);
	// �_�C�A���O�O�i�F
	if(dlg->DlgColor != GetProfileInt("Settings","db",0)*0x10000L +
					GetProfileInt("Settings","dg",0)*0x100L +
					GetProfileInt("Settings","dr",0))
	{
		WriteProfileInt("Settings","db",(dlg->DlgColor/0x10000L)&0xff);
		WriteProfileInt("Settings","dg",(dlg->DlgColor/0x100L)&0xff);
		WriteProfileInt("Settings","dr", dlg->DlgColor&0xff);
	}
	// �_�C�A���O�w�i�F
	if(dlg->DlgBackColor != GetProfileInt("Settings","dbb",255)*0x10000L +
					GetProfileInt("Settings","dbg",255)*0x100L +
					GetProfileInt("Settings","dbr",255))
	{
		WriteProfileInt("Settings","dbb",(dlg->DlgBackColor/0x10000L)&0xff);
		WriteProfileInt("Settings","dbg",(dlg->DlgBackColor/0x100L)&0xff);
		WriteProfileInt("Settings","dbr", dlg->DlgBackColor&0xff);
	}

}

// **********************************
// �_�C�A���O�̃J���[��ύX����
//
// SetDialogBkColor �� Protected �֐��̂��߁A�O������Ăяo���Ȃ����Ƃɑ΂���΍�
// �_�C�A���O���̂���Ăяo���Ďg��
// **********************************
void CCursor32App::SetDialogBkColor_Extern(COLORREF cback, COLORREF cfore)
{
	SetDialogBkColor(cback, cfore);
}

// **********************************
//  �C���X�g�[����� �ꎮ
//
//  �߂�l  TRUE:�C���X�g�[�������܂��͕s�v,  FALSE:���[�U�L�����Z��
// **********************************
BOOL CCursor32App::InstallSeq()
{
	CInstDlg dlg;
	dlg.m_r_autoexec = 0;	// �����N�����Ȃ�
	dlg.m_chk_uninst_start = TRUE;	// �X�^�[�g���j���[�ɃA���C���X�g�[���A�C�R��


	// **********************************
	//  �C���X�g�[���t���O�̃`�F�b�N (HKLM/HKCU)
	// **********************************
	if(ChkHkcuInstReg() || ChkHklmInstReg()) return TRUE;

	if(dlg.DoModal() != IDOK) return FALSE;	// ���[�U���C���X�g�[�����L�����Z��
	::MkHkcuInstReg();

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
	//  �X�^�[�g���j���[�ɃA�C�R���o�^
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
BOOL CCursor32App::UninstallSeq()
{
	CString sTmpMsg;

	sTmpMsg.LoadString(STR_MES_UNIN);
	if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "Cursor32 Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
		return FALSE;	// ���[�U���L�����Z��

	::RmHkcuInstReg();
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


