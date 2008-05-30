// GpsTimeSetCEDlg.h : header file
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


#if !defined(AFX_GPSTIMESETCEDLG_H__C3CE8252_5729_49BD_A879_F914196919F0__INCLUDED_)
#define AFX_GPSTIMESETCEDLG_H__C3CE8252_5729_49BD_A879_F914196919F0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CGpsTimeSetCEDlg dialog

class CGpsTimeSetCEDlg : public CDialog
{
// Construction
public:
	CGpsTimeSetCEDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGpsTimeSetCEDlg)
	enum { IDD = IDD_GPSTIMESETCE_DIALOG };
	CComboBox	m_ctrlComboBaud;
	CComboBox	m_ctrlComboPortname;
	//}}AFX_DATA

	static UINT ThreadMain(LPVOID pParam);
	UINT ThreadDataReceive(LPVOID pParam);
	
	CWinThread * volatile m_pThread;
	volatile int m_ThreadActivate;
	volatile int m_ThreadTerminateCommand;
	volatile int m_nBaudRate;
	volatile char m_sPortName[7];
	volatile int m_ThreadWriteInterrupt;
	volatile int m_ThreadSetTime;

	BOOL NmeaChecksum(const char * sNmea);

	int GGA_Process(const char * sNmea, LPVOID pParam);
	int ZDA_Process(const char * sNmea, LPVOID pParam);
	void TimeSeparate(char * sTimeStr, int *nHour, int *nMinutes, int *nSecond);
	char * SplitString(char *sBuf, char *sSeparator);
	void PcTimeDisp(int nYear, int nMonth, int nDay, int nHour, int nMinutes, int nSecond, LPVOID pParam);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGpsTimeSetCEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGpsTimeSetCEDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnComopen();
	afx_msg void OnBtnSettime();
	afx_msg void OnBtnComclose();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_GPSTIMESETCEDLG_H__C3CE8252_5729_49BD_A879_F914196919F0__INCLUDED_)
