// DlgAbout.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "infobar00.h"
#include "DlgAbout.h"
#include "InstFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgAbout, CPropertyPage)

CDlgAbout::CDlgAbout() : CPropertyPage(CDlgAbout::IDD)
{
	//{{AFX_DATA_INIT(CDlgAbout)
		// メモ - ClassWizard はこの位置にメンバの初期化処理を追加します。
	//}}AFX_DATA_INIT
}

CDlgAbout::~CDlgAbout()
{
}

void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAbout)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAbout, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgAbout)
	ON_BN_CLICKED(IDC_BTN_MAKEUNIN, OnBtnMakeunin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout メッセージ ハンドラ

// **********************************
//  アンインストール・アイコンの作成
//
// **********************************
void CDlgAbout::OnBtnMakeunin() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::MkUninstMnu();

	MessageBox("Uninstall shortcut is made on start menu .", "InfoBar", MB_OK|MB_ICONINFORMATION);
	
}
