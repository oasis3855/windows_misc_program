// InfoBar00.h : INFOBAR00 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_INFOBAR00_H__525BBCAD_48E3_4E79_A054_992BBD7B7892__INCLUDED_)
#define AFX_INFOBAR00_H__525BBCAD_48E3_4E79_A054_992BBD7B7892__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��
#include "InfoBar00Dlg.h"

/////////////////////////////////////////////////////////////////////////////
// CInfoBar00App:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� InfoBar00.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CInfoBar00App : public CWinApp
{
public:
	BOOL m_bMutexOwner;
	HANDLE m_hMutex;
	BOOL bInstalled;
	BOOL UninstallSeq(void);
	BOOL InstallSeq(void);
	void RegConfigWrite(CInfoBar00Dlg *dlg);
	void RegConfigRead(CInfoBar00Dlg *dlg);
	CInfoBar00App();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CInfoBar00App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CInfoBar00App)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_INFOBAR00_H__525BBCAD_48E3_4E79_A054_992BBD7B7892__INCLUDED_)
