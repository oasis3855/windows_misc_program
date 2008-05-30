// GpsTimeSetDlg.h : �w�b�_�[ �t�@�C��
//
// ***********************
// ���C���_�C�A���O�A�X���b�h�̏��� ver 1.0.0.1
// 
// Copyright (C) 2008 INOUE. Hirokazu
//
// 
// http://www.opensource.jp/gpl/gpl.ja.html
// ���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t�g�E�F
// �A���c�ɂ���Ĕ��s���ꂽ GNU ��ʌ��O���p�����_��(�o�[�W����2���A��
// �]�ɂ���Ă͂���ȍ~�̃o�[�W�����̂����ǂꂩ)�̒�߂�����̉��ōĔЕz
// �܂��͉��ς��邱�Ƃ��ł��܂��B
// 
// ���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖��ۏ�* 
// �ł��B���Ɖ\���̕ۏ؂����̖ړI�ւ̓K�����́A���O�Ɏ����ꂽ���̂���
// �ߑS�����݂��܂���B�ڂ�����GNU ��ʌ��O���p�����_�񏑂��������������B
// 
// ���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU ��ʌ��O���p�����_�񏑂̕��������ꕔ
// �󂯎�����͂��ł��B�����󂯎���Ă��Ȃ���΁A�t���[�\�t�g�E�F�A���c��
// �Ő������Ă�������(����� the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA 02111-1307 USA)�B
//

#pragma once
#include "afxwin.h"


// CGpsTimeSetDlg �_�C�A���O
class CGpsTimeSetDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CGpsTimeSetDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_GPSTIMESET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnComopen();
	afx_msg void OnBnClickedBtnComclose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSettime();

	static UINT ThreadMain(LPVOID pParam);
	UINT ThreadDataReceive(LPVOID pParam);

	CWinThread * volatile m_pThread;
	volatile int m_ThreadActivate;
	volatile int m_ThreadTerminateCommand;
	volatile int m_ThreadSetTime;
	volatile int m_nBaudRate;
	volatile char m_sPortName[7];

	CComboBox m_ctrlComboPortname;
	CComboBox m_ctrlComboBaud;

	BOOL NmeaChecksum(const char * sNmea);

	int GGA_Process(const char * sNmea, LPVOID pParam);
	int ZDA_Process(const char * sNmea, LPVOID pParam);
	void TimeSeparate(char * sTimeStr, int *nHour, int *nMinutes, int *nSecond);
	char * SplitString(char *sBuf, char *sSeparator);
	void PcTimeDisp(int nYear, int nMonth, int nDay, int nHour, int nMinutes, int nSecond, LPVOID pParam);
};
