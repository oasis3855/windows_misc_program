#if !defined(AFX_DLGSYSCONF_H__0D33F4EA_9101_4F9F_B2AA_91AC267403C3__INCLUDED_)
#define AFX_DLGSYSCONF_H__0D33F4EA_9101_4F9F_B2AA_91AC267403C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysConf.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSysConf ダイアログ

class CDlgSysConf : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgSysConf)

// コンストラクション
public:
	COLORREF m_cForeColor, m_cBackColor;
	CDlgSysConf();
	~CDlgSysConf();

// ダイアログ データ
	//{{AFX_DATA(CDlgSysConf)
	enum { IDD = IDD_DLG_SYSCONF };
	CStatic	m_ctrl_area_fore_color;
	CStatic	m_ctrl_area_back_color;
	UINT	m_nSpeed;
	UINT	m_nTimer;
	UINT	m_tInterval;
	BOOL	m_bTopmost;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDlgSysConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgSysConf)
	afx_msg void OnBtnBackColor();
	afx_msg void OnBtnForeColor();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGSYSCONF_H__0D33F4EA_9101_4F9F_B2AA_91AC267403C3__INCLUDED_)
