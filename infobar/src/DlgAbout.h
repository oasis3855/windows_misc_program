#if !defined(AFX_DLGABOUT_H__3D816F1E_4671_43F6_BAA0_4664CAACA02B__INCLUDED_)
#define AFX_DLGABOUT_H__3D816F1E_4671_43F6_BAA0_4664CAACA02B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAbout.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout ダイアログ

class CDlgAbout : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgAbout)

// コンストラクション
public:
	CDlgAbout();
	~CDlgAbout();

// ダイアログ データ
	//{{AFX_DATA(CDlgAbout)
	enum { IDD = IDD_DLG_ABOUT };
		// メモ - ClassWizard はこの位置にデータ メンバを追加します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDlgAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgAbout)
	afx_msg void OnBtnMakeunin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGABOUT_H__3D816F1E_4671_43F6_BAA0_4664CAACA02B__INCLUDED_)
