// GpsTimeSetCEDlg.h : header file
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
// Microsoft eMbedded Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_GPSTIMESETCEDLG_H__C3CE8252_5729_49BD_A879_F914196919F0__INCLUDED_)
