#if !defined(AFX_DLGNETSTOCK_H__DCAB6613_BD04_46D4_9BA3_FED0BF2832EA__INCLUDED_)
#define AFX_DLGNETSTOCK_H__DCAB6613_BD04_46D4_9BA3_FED0BF2832EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNetStock.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNetStock ダイアログ

class CDlgNetStock : public CDialog
{
// コンストラクション
public:
	CDlgNetStock(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgNetStock)
	enum { IDD = IDD_DLG_NET_STOCK };
	CComboBox	m_cmb_preset_ctrl;
	CString	m_sURL;
	CString	m_sItems;
	CString	m_sPhHeader;
	CString	m_sPhHeaderB;
	CString	m_sPhHeaderC;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgNetStock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgNetStock)
	afx_msg void OnSelchangeCmbPreset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGNETSTOCK_H__DCAB6613_BD04_46D4_9BA3_FED0BF2832EA__INCLUDED_)
