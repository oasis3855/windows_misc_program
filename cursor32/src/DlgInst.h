#if !defined(AFX_DLGINST_H__DF5260A2_321B_11D3_9DA0_0090CCA1066A__INCLUDED_)
#define AFX_DLGINST_H__DF5260A2_321B_11D3_9DA0_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInst.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInst ダイアログ

class CDlgInst : public CDialog
{
// コンストラクション
public:
	CDlgInst(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgInst)
	enum { IDD = IDD_INSTDLG };
	BOOL	m_logon;
	BOOL	m_manual;
	BOOL	m_uninst;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgInst)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgInst)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGINST_H__DF5260A2_321B_11D3_9DA0_0090CCA1066A__INCLUDED_)
