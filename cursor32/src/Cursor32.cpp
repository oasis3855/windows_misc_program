// Cursor32.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#include "Cursor32.h"
#include "DlgInst.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

	CCursor32Dlg dlg;

	// ���W�X�g�����珉���l��ǂݍ���
	RegConfigRead(&dlg);

	// �R�}���h���C���̉��
	if(!strcmpi(m_lpCmdLine, "delete"))
	{	// �A���C���X�g�[��
		UninstallSeq();
	}
	else if(!strcmpi(m_lpCmdLine, "inst"))
	{	// �C���X�g�[��
		InstallSeq();
	}
	else if(m_lpCmdLine[0] != NULL)
	{
		::MessageBox(NULL, "�����ȃX�C�b�`���w�肳��܂���\n�L���ȃp�����[�^�[�́Astart,delete�ł�", "�X�^�[�g�A�b�v �^�X�N�X�P�W���[��",
			MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
	}
	else
	{	// �ʏ�̎��s
		if(!GetProfileInt("Global","inst",0))
		{	// �C���X�g�[���_�C�A���O�̕\���ƃC���X�g�[������
			InstallSeq();
		}
		else
		{
			m_pMainWnd = &dlg;

			// �_�C�A���O���̂̕\���F��ύX����
			if(dlg.nChangeDlgColor) SetDialogBkColor(dlg.DlgBackColor, dlg.DlgColor);

			dlg.DoModal(); // ���C���_�C�A���O�̕\��

			RegConfigWrite(&dlg);
		}
	}
	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}

///////////////////////////////////////// �h�L�������g�̏I���

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
	// �z�b�g�L�[
	dlg->hotkey1 = GetProfileInt("Settings","hotkey1",VK_CONTROL);
	dlg->hotkey2 = GetProfileInt("Settings","hotkey2",VK_SHIFT);

	// �^�C�g��
	dlg->title = GetProfileString("Settings","title","Cursor32");

	// �E�C���h�E�ʒu
	dlg->x_init = GetProfileInt("Settings","x",0);
	dlg->y_init = GetProfileInt("Settings","y",0);

	// XY�\���F
	dlg->nChangeColor = GetProfileInt("Settings","color",1);
	dlg->TextColor = GetProfileInt("Settings","b",0)*0x10000L +
					GetProfileInt("Settings","g",0)*0x100L +
					GetProfileInt("Settings","r",128);

	// �_�C�A���O�w�i�F
	dlg->nChangeDlgColor = GetProfileInt("Settings","colordlg",0);
	dlg->DlgColor = GetProfileInt("Settings","db",0)*0x10000L +
					GetProfileInt("Settings","dg",0)*0x100L +
					GetProfileInt("Settings","dr",0);
	dlg->DlgBackColor = GetProfileInt("Settings","dbb",255)*0x10000L +
					GetProfileInt("Settings","dbg",179)*0x100L +
					GetProfileInt("Settings","dbr",179);
}

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
	// �z�b�g�L�[
	if(dlg->hotkey1 != GetProfileInt("Settings","hotkey1",VK_CONTROL))
		WriteProfileInt("Settings","hotkey1",dlg->hotkey1);
	if(dlg->hotkey2 != GetProfileInt("Settings","hotkey2",VK_SHIFT))
		WriteProfileInt("Settings","hotkey2",dlg->hotkey2);

	// �^�C�g��
	if(dlg->title != GetProfileString("Settings","title","Cursor32"))
		WriteProfileString("Settings","title",dlg->title);

	// �E�C���h�E�ʒu
	if(dlg->x_init != (int)GetProfileInt("Settings","x",0))
		WriteProfileInt("Settings","x",dlg->x_init);
	if(dlg->y_init != (int)GetProfileInt("Settings","y",0))
		WriteProfileInt("Settings","y",dlg->y_init);

	// XY�\���F
	if(dlg->nChangeColor != (BOOL)GetProfileInt("Settings","color",1))
		WriteProfileInt("Settings","color",dlg->nChangeColor);
	if(dlg->TextColor != GetProfileInt("Settings","b",0)*0x10000L +
					GetProfileInt("Settings","g",0)*0x100L +
					GetProfileInt("Settings","r",128))
		WriteProfileInt("Settings","b",(dlg->TextColor/0x10000L)&0xff);
		WriteProfileInt("Settings","g",(dlg->TextColor/0x100L)&0xff);
		WriteProfileInt("Settings","r", dlg->TextColor&0xff);

	// �_�C�A���O�w�i�F
	if(dlg->nChangeDlgColor != (BOOL)GetProfileInt("Settings","colordlg",0))
		WriteProfileInt("Settings","colordlg",dlg->nChangeDlgColor);
	if(dlg->DlgColor != GetProfileInt("Settings","db",255)*0x10000L +
					GetProfileInt("Settings","dg",179)*0x100L +
					GetProfileInt("Settings","dr",179))
		WriteProfileInt("Settings","db",(dlg->DlgColor/0x10000L)&0xff);
		WriteProfileInt("Settings","dg",(dlg->DlgColor/0x100L)&0xff);
		WriteProfileInt("Settings","dr", dlg->DlgColor&0xff);
	if(dlg->DlgBackColor != GetProfileInt("Settings","dbb",0)*0x10000L +
					GetProfileInt("Settings","dbg",0)*0x100L +
					GetProfileInt("Settings","dbr",0))
		WriteProfileInt("Settings","dbb",(dlg->DlgBackColor/0x10000L)&0xff);
		WriteProfileInt("Settings","dbg",(dlg->DlgBackColor/0x100L)&0xff);
		WriteProfileInt("Settings","dbr", dlg->DlgBackColor&0xff);

}

void CCursor32App::InstallSeq()
{
	HKEY hCU;
	DWORD lpType, dw;
	char szProg[MAX_PATH];
	char szLink[MAX_PATH];
	CString msg;

	CDlgInst dlg;
	dlg.m_logon = 0;
	dlg.m_manual = 1;
	dlg.m_uninst = 1;

	if(dlg.DoModal() == TRUE)
	{
		if(dlg.m_uninst)
		{	// �A���C���X�g�[�����
			if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return;
			strcat(szProg, " delete");
			if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Cursor32",
				0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hCU, &dw) == ERROR_SUCCESS)
			{
				RegSetValueEx( hCU, "UninstallString", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
				strcpy(szProg, "Cursor32");
				RegSetValueEx( hCU, "DisplayName", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
				RegCloseKey(hCU);
			}
		}
		if(dlg.m_logon)
		{	// ���O�I�����Ɏ������s
			// �������s�A�C�R���̓o�^
			::GetModuleFileName(NULL, szProg, MAX_PATH);
			ULONG ulSize = MAX_PATH;
				if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
				0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
			{
				RegQueryValueEx( hCU, "Startup", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
				RegCloseKey(hCU);
			}
			strcat(szLink, "\\Cursor32.LNK");
			::CreateShellLink(szProg, szLink, "Cursor32");
		}
		if(dlg.m_manual)
		{	// �蓮���s
			::GetModuleFileName(NULL, szProg, MAX_PATH);
			ULONG ulSize = MAX_PATH;
			if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
				0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
			{
				RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
				RegCloseKey(hCU);
			}
			strcat(szLink, "\\Cursor32.LNK");
			::CreateShellLink(szProg, szLink, "Cursor32");
		}
		WriteProfileInt("Global","inst",1);
	}
}

void CCursor32App::UninstallSeq()
{
	if(::MessageBox(NULL, "Cursor32���A���C���X�g�[�����܂�\n\n\n���s�A�C�R���Ɛݒ���e�����ׂď������܂�",
		"Cursor32 Uninstall", MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL )
		!= IDYES) return;

	HKEY hCU;
	char szLink[MAX_PATH];
	DWORD lpType;
	ULONG ulSize = MAX_PATH;

	// ���W�X�g���̐ݒ��������
	::RegDeleteKey(HKEY_USERS,".Default\\Software\\hi soft\\Cursor32");
	// �X�^�[�g���j���[�̐ݒ�A�C�R���𖕎E
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, "\\Cursor32.LNK");
	::remove(szLink);
	// �X�^�[�g���j���[�̎������s�A�C�R���𖕎E
	ulSize = MAX_PATH;	// �������͌��ɖ߂��Ď��s���邱��
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Startup", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, "\\Cursor32.LNK");
	::remove(szLink);
	// �폜�f�[�^�̍폜
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteKey( hCU, "Cursor32");
		RegCloseKey(hCU);
	}
	::MessageBox(NULL, "Cursor32���A���C���X�g�[�����܂���\n\n\n�v���O�����̂���t�H���_�̊֘A�t�@�C���͎蓮�ō폜���Ă�������",
		"Cursor32", MB_YESNO|MB_ICONINFORMATION|MB_APPLMODAL );


}
