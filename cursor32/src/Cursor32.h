// Cursor32.h : CURSOR32 �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��
#include "Cursor32Dlg.h"



/////////////////////////////////////////////////////////////////////////////
// CCursor32App:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� Cursor32.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CCursor32App : public CWinApp
{
public:
	void SetDialogBkColor_Extern(COLORREF cback, COLORREF cfore);
	BOOL UninstallSeq(void);
	BOOL InstallSeq();
	void RegConfigRead(CCursor32Dlg *dlg);
	void RegConfigWrite(CCursor32Dlg *dlg);
	CCursor32App();

	HANDLE m_hMutex;
	BOOL m_bMutexOwner;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CCursor32App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CCursor32App)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

