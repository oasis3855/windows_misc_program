#if !defined(AFX_INSTDLG_H__2DA7246E_5DAA_4E72_97CD_740D3216DACC__INCLUDED_)
#define AFX_INSTDLG_H__2DA7246E_5DAA_4E72_97CD_740D3216DACC__INCLUDED_

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
	CInstDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CInstDlg)
	enum { IDD = IDD_DLG_INST };
	BOOL	m_chk_uninst_start;
	int		m_r_autoexec;
	BOOL	m_chk_uninst_cpl;
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
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_INSTDLG_H__2DA7246E_5DAA_4E72_97CD_740D3216DACC__INCLUDED_)
