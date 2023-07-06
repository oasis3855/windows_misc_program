// CookieCtrlDlg.h : ヘッダー ファイル
//

#if !defined(AFX_COOKIECTRLDLG_H__01212946_0903_11D5_AD9A_0090CCA1066A__INCLUDED_)
#define AFX_COOKIECTRLDLG_H__01212946_0903_11D5_AD9A_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlDlg ダイアログ

class CCookieCtrlDlg : public CDialog
{
// 構築
public:
	BOOL ReInstallSeq(void);
	CCookieCtrlDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CCookieCtrlDlg)
	enum { IDD = IDD_COOKIECTRL_DIALOG };
	CButton	m_sw_days_ctrl;
	CButton	m_btn_folder_brw;
	CButton	m_btn_folder_auto;
	CEdit	m_str_folder_ctrl;
	CStatic	m_str_days_ctrl;
	CScrollBar	m_scroll_days_ctrl;
	CEdit	m_int_days_ctrl;
	CEdit	m_str_exclude_ctrl;
	CString	m_str_folder;
	CString	m_str_ext;
	CString	m_str_exclude;
	int		m_int_days;
	BOOL	m_sw_days;
	BOOL	m_sw_exclude;
	BOOL	m_chk_auto_folder;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCookieCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCookieCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnFolderBrw();
	afx_msg void OnBtnFolderAuto();
	afx_msg void OnBtnExt();
	afx_msg void OnCheckExclude();
	afx_msg void OnCheckDays();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnRun();
	afx_msg void OnBtnStartup();
	afx_msg void OnBtnVersion();
	afx_msg void OnBtnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnCheckAutoFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_COOKIECTRLDLG_H__01212946_0903_11D5_AD9A_0090CCA1066A__INCLUDED_)
