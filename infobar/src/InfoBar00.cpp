// InfoBar00.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#include "InfoBar00.h"
#include "InfoBar00Dlg.h"
#include "InstDlg.h"
#include "GlobalFunc.h"		// �ʐM�X���b�h����уO���[�o���ϐ�

#include "InstFunc.h"
#include "CheckWinVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char  sMutexInstance[] = {"InfoBar00"};    //  ���d�N���h�~�̂��ߗ��p


/////////////////////////////////////////////////////////////////////////////
// CInfoBar00App

BEGIN_MESSAGE_MAP(CInfoBar00App, CWinApp)
	//{{AFX_MSG_MAP(CInfoBar00App)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoBar00App �N���X�̍\�z

CInfoBar00App::CInfoBar00App()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CInfoBar00App �I�u�W�F�N�g

CInfoBar00App theApp;



/////////////////////////////////////////////////////////////////////////////
// CInfoBar00App �N���X�̏�����

BOOL CInfoBar00App::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������������
	//  ��Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ��̂��폜����
	//  ���������B

	// ڼ޽�؂����ޯĂ��ȉ��̂悤�ɂ���
	// HKEY_USERS/.Default/Software/HI-soft/Cursor32 ...
//	m_pszAppName = "Cursor32";
	// ���\�[�X��AFX_IDS_APP_TITLE�ɃA�v���P�[�V��������ݒ肷�邱�� 
	// m_pszAppName = "compdel" �Ƃ��Ă͂����Ȃ��I ASSERTION �G���[�ƂȂ�
	SetRegistryKey((LPCTSTR)"hi soft");

	// �C���X�g�[���t���O�� FALSE �ɐݒ�
	this->bInstalled = FALSE;

	CInfoBar00Dlg dlg;


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

	// **********************************
	//  ���d�N���h�~
	// **********************************
	m_hMutex = ::CreateMutex(NULL, TRUE, sMutexInstance);
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{	// ���łɋN�����Ă���
//		::CloseHandle(m_hMutex);
		m_bMutexOwner = FALSE;
		return FALSE;
	}
	m_bMutexOwner = TRUE;


	// **********************************
	//  ���C���_�C�A���O�̕\��
	// **********************************
	if(this->bInstalled) dlg.tPrev = 1;	// 0�łȂ���΁A�l�b�g�ڑ�����
	else dlg.tPrev = 0;		// �l�b�g�ɐڑ������A�w���v���b�Z�[�W�𗬂�

	m_pMainWnd = &dlg;
	dlg.DoModal();

	RegConfigWrite(&dlg);


	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}


int CInfoBar00App::ExitInstance() 
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
//  ���W�X�g������ݒ�l��ǂݏo��
//
// **********************************
void CInfoBar00App::RegConfigRead(CInfoBar00Dlg *dlg)
{
	// ���[�h
	nMode = GetProfileInt("Settings","Mode",0);

	// URL
	sURL = GetProfileString("Settings","URL","headlines.yahoo.co.jp/hl?c=pol&t=l");
	// �|�[�g�ԍ�
	nPort = GetProfileInt("Settings","Port",80);
	// �v���L�V�E�T�[�o �i�W���ł͂Ȃ��F���ڐڑ��j
	sProxy = GetProfileString("Settings","Proxy","");
	// �؂蕪���w�b�_
	sPhHeader = GetProfileString("Settings","Header","<li>");
	// �؂蕪���w�b�_ B
	sPhHeaderB = GetProfileString("Settings","HeaderB","");
	// �؂蕪���w�b�_ C
	sPhHeaderC = GetProfileString("Settings","HeaderC","");
	// �擪��΂��� �i�W���͂O�j
	nPhSkip = GetProfileInt("Settings","skip",0);
	// ���ڎ擾�� �i�W���͂P�O�j
	nPhGetcount = GetProfileInt("Settings","getitems",10);
	// �o�b�N�O���E���h�J���[ �i�W���̓I�����W�j
	dlg->cBackColor = GetProfileInt("Settings","color_back",0x007ee5);
	// �t�H�A�O���E���h�J���[ �i�W���͍��j
	dlg->cForeColor = GetProfileInt("Settings","color_fore",0);
	// �^�C�}�[���荞�ݎ��� �i�P�P�O�~���b�j
	dlg->nTimer = GetProfileInt("Settings","timer",110);
	// �����\���X�s�[�h �i�W���͂U�h�b�g���j
	dlg->nMoveSpeed = GetProfileInt("Settings","flow_speed",6);
	// �l�b�g�ւ̃A�N�Z�X�Ԋu�i�R�O�O�b���Ɓj
	dlg->tInterval = GetProfileInt("Settings","interval",300);
	// �^�C�g��������
	sTitle = GetProfileString("Settings","title","");
	// �،��f�[�^�Ȃǂ̎w��Ɏg���A�C�e��
	sItems = GetProfileString("Settings","item","");
	// �؂�o����̕����񂩂�d������󔒕������폜���郂�[�h
	nDelSpace = GetProfileInt("Settings","del_space",0);
	// �S�f�[�^�P�s���i���s���󔒁j���[�h
	nCr2Spc = GetProfileInt("Settings","cr2spc",0);

	// �_�C�A���O�̂w���W
	dlg->nPosX = GetProfileInt("Settings","pos_x",100);
	// �_�C�A���O�̂x���W
	dlg->nPosY = GetProfileInt("Settings","pos_y",100);
	// �_�C�A���O�̕�
	dlg->nInfoWndWidth = GetProfileInt("Settings","siz_x",0);
	// �_�C�A���O�̍���
	dlg->nInfoWndHeight = GetProfileInt("Settings","siz_y",0);
	// �őO�ʂɕ\������
	dlg->bDispTopmost = GetProfileInt("Settings","topwin",1);

	// �t�H���g�̃|�C���g�� �i�O�ŃV�X�e���f�t�H���g�j
	dlg->nFontPoint = GetProfileInt("Settings","fnt_point",0);
	// �t�H���g��
	dlg->sFontName = GetProfileString("Settings","fnt_name","");

	// �C���X�g�[���t���O�̎擾
	this->bInstalled = GetProfileInt("Settings","installed",0);


}

// **********************************
//  ���W�X�g���֐ݒ�l����������
//
// **********************************
void CInfoBar00App::RegConfigWrite(CInfoBar00Dlg *dlg)
{
	// ���[�h
	if(nMode != (int)GetProfileInt("Settings","Mode",0))
		WriteProfileInt("Settings","Mode",nMode);

	// URL
	if(sURL != GetProfileString("Settings","URL","headlines.yahoo.co.jp/hl?c=pol&t=l"))
		WriteProfileString("Settings","URL",sURL);
	// �|�[�g�ԍ�
	if(nPort != GetProfileInt("Settings","Port",80))
		WriteProfileInt("Settings","Port",nPort);
	// �v���L�V�E�T�[�o �i�W���ł͂Ȃ��F���ڐڑ��j
	if(sProxy != GetProfileString("Settings","Proxy",""))
		WriteProfileString("Settings","Proxy",sProxy);
	// �؂蕪���w�b�_
	if(sPhHeader != GetProfileString("Settings","Header","<li>"))
		WriteProfileString("Settings","Header",sPhHeader);
	// �؂蕪���w�b�_ B
	if(sPhHeaderB != GetProfileString("Settings","HeaderB",""))
		WriteProfileString("Settings","HeaderB",sPhHeaderB);
	// �؂蕪���w�b�_ C
	if(sPhHeaderC != GetProfileString("Settings","HeaderC",""))
		WriteProfileString("Settings","HeaderC",sPhHeaderC);
	// �擪��΂��� �i�W���͂O�j
	if(nPhSkip != (int)GetProfileInt("Settings","skip",0))
		WriteProfileInt("Settings","skip",nPhSkip);
	// ���ڎ擾�� �i�W���͂P�O�j
	if(nPhGetcount != (int)GetProfileInt("Settings","getitems",10))
		WriteProfileInt("Settings","getitems",nPhGetcount);
	// �o�b�N�O���E���h�J���[ �i�W���̓I�����W�j
	if(dlg->cBackColor != GetProfileInt("Settings","color_back",0x007ee5))
		WriteProfileInt("Settings","color_back",dlg->cBackColor);
	// �t�H�A�O���E���h�J���[ �i�W���͍��j
	if(dlg->cForeColor != GetProfileInt("Settings","color_fore",0))
		WriteProfileInt("Settings","color_fore",dlg->cForeColor);
	// �^�C�}�[���荞�ݎ��� �i�P�P�O�~���b�j
	if(dlg->nTimer != GetProfileInt("Settings","timer",110))
		WriteProfileInt("Settings","timer",dlg->nTimer);
	// �����\���X�s�[�h �i�W���͂U�h�b�g���j
	if(dlg->nMoveSpeed != GetProfileInt("Settings","flow_speed",6))
		WriteProfileInt("Settings","flow_speed",dlg->nMoveSpeed);
	// �l�b�g�ւ̃A�N�Z�X�Ԋu�i�R�O�O�b���Ɓj
	if(dlg->tInterval != (int)GetProfileInt("Settings","interval",300))
		WriteProfileInt("Settings","interval",dlg->tInterval);
	// �^�C�g��������
	if(sTitle != GetProfileString("Settings","title",""))
		WriteProfileString("Settings","title",sTitle);
	// �،��f�[�^�Ȃǂ̎w��Ɏg���A�C�e��
	if(sItems != GetProfileString("Settings","item",""))
		WriteProfileString("Settings","item",sItems);
	// �؂�o����̕����񂩂�d������󔒕������폜���郂�[�h
	if(nDelSpace != (int)GetProfileInt("Settings","del_space",0))
		WriteProfileInt("Settings","del_space",nDelSpace);
	// �S�f�[�^�P�s���i���s���󔒁j���[�h
	if(nCr2Spc != (int)GetProfileInt("Settings","cr2spc",0))
		WriteProfileInt("Settings","cr2spc",nCr2Spc);

	// �_�C�A���O�̂w���W
	if(dlg->nPosX != GetProfileInt("Settings","pos_x",100))
		WriteProfileInt("Settings","pos_x",dlg->nPosX);
	// �_�C�A���O�̂x���W
	if(dlg->nPosY != GetProfileInt("Settings","pos_y",100))
		WriteProfileInt("Settings","pos_y",dlg->nPosY);
	// �_�C�A���O�̕�
	if(dlg->nInfoWndWidth != (int)GetProfileInt("Settings","siz_x",0))
		WriteProfileInt("Settings","siz_x",dlg->nInfoWndWidth);
	// �_�C�A���O�̍���
	if(dlg->nInfoWndHeight != (int)GetProfileInt("Settings","siz_y",0))
		WriteProfileInt("Settings","siz_y",dlg->nInfoWndHeight);
	// �őO�ʂɕ\������
	if(dlg->bDispTopmost != (int)GetProfileInt("Settings","topwin",1))
		WriteProfileInt("Settings","topwin",dlg->bDispTopmost);

	// �t�H���g�̃|�C���g�� �i�O�ŃV�X�e���f�t�H���g�j
	if(dlg->nFontPoint != (int)GetProfileInt("Settings","fnt_point",0))
		WriteProfileInt("Settings","fnt_point",dlg->nFontPoint);
	// �t�H���g��
	if(dlg->sFontName != GetProfileString("Settings","fnt_name",""))
		WriteProfileString("Settings","fnt_name",dlg->sFontName);


	// �C���X�g�[���t���O
	if(GetProfileInt("Settings","installed",0) != 1)
	WriteProfileInt("Settings","installed",1);

}

// **********************************
//  �C���X�g�[����� �ꎮ
//
//  �߂�l  TRUE:�C���X�g�[�������܂��͕s�v,  FALSE:���[�U�L�����Z��
// **********************************
BOOL CInfoBar00App::InstallSeq()
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
BOOL CInfoBar00App::UninstallSeq()
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

