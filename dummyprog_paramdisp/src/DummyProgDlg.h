// DummyProgDlg.h : ヘッダー ファイル
//

#if !defined(AFX_DUMMYPROGDLG_H__BC6E13D2_A988_47F2_B911_87312C6D37F8__INCLUDED_)
#define AFX_DUMMYPROGDLG_H__BC6E13D2_A988_47F2_B911_87312C6D37F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDummyProgDlg ダイアログ

class CDummyProgDlg : public CDialog
{
// 構築
public:
	CDummyProgDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDummyProgDlg)
	enum { IDD = IDD_DUMMYPROG_DIALOG };
	CString	m_edit_param;
	CString	m_edit_thisprog;
	CString	m_edit_curdir;
	CString	m_edit_winver;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDummyProgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDummyProgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DUMMYPROGDLG_H__BC6E13D2_A988_47F2_B911_87312C6D37F8__INCLUDED_)
