#if !defined(AFX_INSTDLG_H__56D0E476_4F9A_493C_84AF_DB6520ACD0BF__INCLUDED_)
#define AFX_INSTDLG_H__56D0E476_4F9A_493C_84AF_DB6520ACD0BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InstDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CInstDlg ダイアログ

class CInstDlg : public CDialog
{
// コンストラクション
public:
	BOOL m_IsAdminInst;
	CInstDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CInstDlg)
	enum { IDD = IDD_DLG_INST };
	CButton	m_r_autoexec_ctrl1;
	CButton	m_r_autoexec_ctrl2;
	CButton	m_r_autoexec_ctrl3;
	CButton	m_r_autoexec_ctrl4;
	int		m_r_autoexec;
	BOOL	m_chk_uninst_cpl;
	BOOL	m_chk_uninst_start;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CInstDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CInstDlg)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_INSTDLG_H__56D0E476_4F9A_493C_84AF_DB6520ACD0BF__INCLUDED_)
