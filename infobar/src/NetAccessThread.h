#if !defined(AFX_NETACCESSTHREAD_H__A53AE552_DEBC_4CCE_B3CE_937A7D4450E4__INCLUDED_)
#define AFX_NETACCESSTHREAD_H__A53AE552_DEBC_4CCE_B3CE_937A7D4450E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetAccessThread.h : �w�b�_�[ �t�@�C��
//



/////////////////////////////////////////////////////////////////////////////
// CNetAccessThread �X���b�h

class CNetAccessThread : public CWinThread
{
	DECLARE_DYNCREATE(CNetAccessThread)
protected:
public:
	CNetAccessThread();           // ���I�����Ɏg�p�����v���e�N�g �R���X�g���N�^

// �A�g���r���[�g
protected:
//	CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
//	CString		_sTmp_MidBuf;	// sTransBuf �ɓn���ŏI���ʊi�[�p
//	CString		_sPhHeaderCur;	// ���݂̋�؂蕶����
//	CTime		_tm_trd;

public:
/*
BOOL *bUpdated;		// �X���b�h�ōX�V�����������ʒm
BOOL *bInThread;	// �X���b�h���쒆�̃t���O
int nMode;			// ���샂�[�h�i�ʏ��M�E�،���M�c�j
//int nThreadError;	// �X���b�h�ŋN�������G���[�̎�ނ�Ԃ�
CString *sTransBuf;			// �X���b�h�ƃ_�C�A���O�̒ʐM�p
CString sURL;				// �X���b�h�ɓn�� URL
CString sProxy;				// �X���b�h�ɓn�� PROXY
UINT nPort;		// �X���b�h�ɓn�� �|�[�g�ԍ�
CString sPhHeader;			// �؂蕪���p �w�b�_�[������
CString sPhHeaderB;			// �؂蕪���p �w�b�_�[������ B
CString sPhHeaderC;			// �؂蕪���p �w�b�_�[������ C
int nPhSkip;		// �؂蕪���p �X�L�b�v��
int nPhGetcount;	// �؂蕪���p �擾��
CString sTitle;				// �^�C�g��
CString sItems;				// �،��f�[�^�̎w��ȂǂɎg���A�C�e��

char _sURL[2048];
char _sProxy[1024];
char _sPhHeader[1024];
char _sPhHeaderB[1024];
char _sPhHeaderC[1024];
char _sTitle[1024];
char _sItems[2048];
char *_sTransBuf;
*/


// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CNetAccessThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
public:
	BOOL receive_HTML(CString *sRURL, CString *sHTML);
	void Receive_Mode_01(void);
	virtual ~CNetAccessThread();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CNetAccessThread)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_NETACCESSTHREAD_H__A53AE552_DEBC_4CCE_B3CE_937A7D4450E4__INCLUDED_)
