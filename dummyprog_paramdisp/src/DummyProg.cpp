// *******************************************************************
// �v���O�����ɓn����������\������v���O����
//
// *******************************************************************
#include "stdafx.h"
#include "DummyProg.h"
#include "DummyProgDlg.h"
#include "PharseCmdline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDummyProgApp

BEGIN_MESSAGE_MAP(CDummyProgApp, CWinApp)
	//{{AFX_MSG_MAP(CDummyProgApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyProgApp �N���X�̍\�z

CDummyProgApp::CDummyProgApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CDummyProgApp �I�u�W�F�N�g

CDummyProgApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDummyProgApp �N���X�̏�����

#include<direct.h>

BOOL CDummyProgApp::InitInstance()
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

	char szTmp[MAX_PATH];
	OSVERSIONINFO ver;


	CDummyProgDlg dlg;
	m_pMainWnd = &dlg;

	// �v���O�����{�̂̃t���p�X
	::GetModuleFileName(NULL, szTmp, MAX_PATH);
	dlg.m_edit_thisprog = szTmp;

	// ����
//	dlg.m_edit_param = m_lpCmdLine;
	CPharseCmdline PharseCmdline;
	PharseCmdline.SetOrgStr(m_lpCmdLine);
	int i = PharseCmdline.CountParam();
	dlg.m_edit_param.Format("�n���ꂽ������ %d ��\r\n", i);
	CString sTmp1;
	do{
		sTmp1 = PharseCmdline.GetNextParam();
		if(sTmp1 == "") break;
		dlg.m_edit_param = dlg.m_edit_param + sTmp1 + "\r\n";
	}while(1);

	// �J�����g��f�B���N�g��
	::_getcwd(szTmp, sizeof(szTmp));
	dlg.m_edit_curdir = szTmp;

	// OS�o�[�W����
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((LPOSVERSIONINFO)&ver);

	dlg.m_edit_winver = "Windows ";
	switch(ver.dwPlatformId)
	{
		case VER_PLATFORM_WIN32s :
				dlg.m_edit_winver += "3.1 Win32s";
				break;
		case VER_PLATFORM_WIN32_WINDOWS :
			if(ver.dwMinorVersion == 0)
			{
				if((ver.dwBuildNumber&0xffff) < 1000)
				{
					dlg.m_edit_winver += "95";
				}
				else dlg.m_edit_winver += "95 OSR2";
				break;
			}
			else
			{
				if(ver.dwBuildNumber&0xffff < 2000)
					dlg.m_edit_winver += "98";
				else dlg.m_edit_winver += "98SE";
				break;
			}
		case VER_PLATFORM_WIN32_NT :
			dlg.m_edit_winver += "NT, 2000";
			break;
		default : dlg.m_edit_winver += "Unknown"; break;
	}
	sprintf(szTmp, "  Version %Lu.%Lu.%u %s", ver.dwMajorVersion, ver.dwMinorVersion,
			ver.dwBuildNumber, ver.szCSDVersion);
	dlg.m_edit_winver += szTmp;


	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <��ݾ�> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
