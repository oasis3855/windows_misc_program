// CookieCtrl.h : COOKIECTRL �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_COOKIECTRL_H__01212944_0903_11D5_AD9A_0090CCA1066A__INCLUDED_)
#define AFX_COOKIECTRL_H__01212944_0903_11D5_AD9A_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// �O���[�o���ϐ�
extern CString gstr_folder;		// Cookie�t�H���_
extern CString gstr_ext;		// Cookie �t�@�C����
extern CString gstr_exclude;	// ���O����t�@�C��������
extern int gint_days;			// ���O�������
extern int gint_sw_exclude;		// ���O����t�@�C��������̃X�C�b�`
extern int gint_sw_days;		// ���O������Ԃ̃X�C�b�`
extern int gint_admin_inst;	// �A�h�~�j�X�g���[�^�[�i�ŏ��̃C���X�g�[����)
extern int gint_user_inst;		// ���݂̃��[�U�ŃC���X�g�[������Ă��邩




/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� CookieCtrl.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CCookieCtrlApp : public CWinApp
{
public:
	BOOL UninstallSeq(void);
	BOOL InstallSeq(void);
	void RegWriteAll(void);
	void RegReadAll(void);
	CCookieCtrlApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCookieCtrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CCookieCtrlApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_COOKIECTRL_H__01212944_0903_11D5_AD9A_0090CCA1066A__INCLUDED_)
