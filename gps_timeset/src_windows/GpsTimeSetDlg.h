// GpsTimeSetDlg.h : ヘッダー ファイル
//
// ***********************
// メインダイアログ、スレッドの処理 ver 1.0.0.1
// 
// Copyright (C) 2008 INOUE. Hirokazu
//
// 
// http://www.opensource.jp/gpl/gpl.ja.html
// このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフトウェ
// ア財団によって発行された GNU 一般公衆利用許諾契約書(バージョン2か、希
// 望によってはそれ以降のバージョンのうちどれか)の定める条件の下で再頒布
// または改変することができます。
// 
// このプログラムは有用であることを願って頒布されますが、*全くの無保証* 
// です。商業可能性の保証や特定の目的への適合性は、言外に示されたものも含
// め全く存在しません。詳しくはGNU 一般公衆利用許諾契約書をご覧ください。
// 
// あなたはこのプログラムと共に、GNU 一般公衆利用許諾契約書の複製物を一部
// 受け取ったはずです。もし受け取っていなければ、フリーソフトウェア財団ま
// で請求してください(宛先は the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA 02111-1307 USA)。
//

#pragma once
#include "afxwin.h"


// CGpsTimeSetDlg ダイアログ
class CGpsTimeSetDlg : public CDialog
{
// コンストラクション
public:
	CGpsTimeSetDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_GPSTIMESET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
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
