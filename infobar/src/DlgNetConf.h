#if !defined(AFX_DLGNETCONF_H__77914E7B_ADCD_4144_BE44_E191F0AD8F39__INCLUDED_)
#define AFX_DLGNETCONF_H__77914E7B_ADCD_4144_BE44_E191F0AD8F39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNetConf.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNetConf ダイアログ

class CDlgNetConf : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgNetConf)

// コンストラクション
public:
	CDlgNetConf();
	~CDlgNetConf();

// ダイアログ データ
	//{{AFX_DATA(CDlgNetConf)
	enum { IDD = IDD_DLG_NETCONF };
	CComboBox	m_ctrl_cmb_qlist;
	CString	m_sURL;
	UINT	m_nPort;
	CString	m_sPhHeader;
	UINT	m_nPhSkip;
	UINT	m_nPhGetcount;
	CString	m_sProxy;
	CString	m_sTitle;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgNetConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgNetConf)
	afx_msg void OnSelchangeComboQlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGNETCONF_H__77914E7B_ADCD_4144_BE44_E191F0AD8F39__INCLUDED_)
