// PropertySheet.cpp : インプリメンテーション ファイル
// CPropAbout および CPropConfの記述

#include "stdafx.h"
#include "Cursor32.h"
#include "PropertySheet.h"
#include "global.h"
#include "InstFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropAbout プロパティ ページ

IMPLEMENT_DYNCREATE(CPropAbout, CPropertyPage)

CPropAbout::CPropAbout() : CPropertyPage(CPropAbout::IDD)
{
	//{{AFX_DATA_INIT(CPropAbout)
	m_ddx_odometer = _T("");
	//}}AFX_DATA_INIT
}

CPropAbout::~CPropAbout()
{
}

void CPropAbout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropAbout)
	DDX_Text(pDX, IDC_ODOMETER, m_ddx_odometer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAbout, CPropertyPage)
	//{{AFX_MSG_MAP(CPropAbout)
	ON_BN_CLICKED(IDC_BTN_MK_UNIN, OnBtnMkUnin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropAbout メッセージ ハンドラ

// メツセージハンドラなし

/////////////////////////////////////////////////////////////////////////////
// CPropConf プロパティ ページ

IMPLEMENT_DYNCREATE(CPropConf, CPropertyPage)

CPropConf::CPropConf() : CPropertyPage(CPropConf::IDD)
{
	//{{AFX_DATA_INIT(CPropConf)
	m_ddx_assist = FALSE;
	m_ddx_title = _T("");
	m_ddx_x = 0;
	m_ddx_y = 0;
	m_ddx_timertick = 0;
	m_ddx_hotkey1 = -1;
	m_ddx_hotkey2 = -1;
	m_ddx_changecolor = FALSE;
	m_ddx_isNotTaskbar = FALSE;
	m_ddx_isTopmost = FALSE;
	m_ddx_nChangeDlgColor = FALSE;
	m_ddx_hotkey_rel = -1;
	m_ddx_relmode = FALSE;
	//}}AFX_DATA_INIT
}

CPropConf::~CPropConf()
{
}

void CPropConf::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropConf)
	DDX_Control(pDX, IDC_HOTKEY_REL, m_hotkey_rel);
	DDX_Control(pDX, IDC_PICKDLGCOL, m_pickdlgcol);
	DDX_Control(pDX, IDC_PICKDLGBACKCOL, m_pickdlgbackcol);
	DDX_Control(pDX, IDC_PICKCOLOR, m_pickcolor);
	DDX_Control(pDX, IDC_HOTKEY2, m_hotkey2);
	DDX_Control(pDX, IDC_HOTKEY1, m_hotkey1);
	DDX_Check(pDX, IDC_ASSIST, m_ddx_assist);
	DDX_Text(pDX, IDC_TITLE, m_ddx_title);
	DDV_MaxChars(pDX, m_ddx_title, 20);
	DDX_Text(pDX, IDC_INITX, m_ddx_x);
	DDX_Text(pDX, IDC_INITY, m_ddx_y);
	DDX_Text(pDX, IDC_TIMERTICK, m_ddx_timertick);
	DDV_MinMaxInt(pDX, m_ddx_timertick, 10, 2000);
	DDX_LBIndex(pDX, IDC_HOTKEY1, m_ddx_hotkey1);
	DDX_LBIndex(pDX, IDC_HOTKEY2, m_ddx_hotkey2);
	DDX_Check(pDX, IDC_CHANGECOLOR, m_ddx_changecolor);
	DDX_Check(pDX, IDC_DSPNOTTASK, m_ddx_isNotTaskbar);
	DDX_Check(pDX, IDC_DSPTOP, m_ddx_isTopmost);
	DDX_Check(pDX, IDC_CHANGEDLGCOLOR, m_ddx_nChangeDlgColor);
	DDX_LBIndex(pDX, IDC_HOTKEY_REL, m_ddx_hotkey_rel);
	DDX_Check(pDX, IDC_CHK_RELMODE, m_ddx_relmode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropConf, CPropertyPage)
	//{{AFX_MSG_MAP(CPropConf)
	ON_BN_CLICKED(IDC_ASSIST, OnAssist)
	ON_BN_CLICKED(IDC_PICKCOLOR, OnPickcolor)
	ON_BN_CLICKED(IDC_CHANGECOLOR, OnChangecolor)
	ON_BN_CLICKED(IDC_PICKDLGBACKCOL, OnPickdlgbackcol)
	ON_BN_CLICKED(IDC_PICKDLGCOL, OnPickdlgcol)
	ON_BN_CLICKED(IDC_CHANGEDLGCOLOR, OnChangedlgcolor)
	ON_BN_CLICKED(IDC_CHK_RELMODE, OnChkRelmode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropConf メッセージ ハンドラ

// **********************************
// リストボックスの内容をセットする
// **********************************
BOOL CPropConf::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	char *keyname[] = {"Shift","Ctrl","Alt","Insert","Delete",
			"BackSpace","tab","F9","F10","F11","F12"};	// 11 key
	// ホットキーの名前を設定
	for(int i=0;i<11;i++)
	{
		m_hotkey1.AddString(keyname[i]);	
		m_hotkey2.AddString(keyname[i]);
		m_hotkey_rel.AddString(keyname[i]);
	}
	// ホットキーを選択する
	m_hotkey1.SetCurSel(m_ddx_hotkey1);
	m_hotkey2.SetCurSel(m_ddx_hotkey2);
	m_hotkey_rel.SetCurSel(m_ddx_hotkey_rel);
	// アシスタントが無効の時の処理（グレイ表示）
	if(!IsDlgButtonChecked(IDC_ASSIST))
	{
		m_hotkey1.EnableWindow(FALSE);
		m_hotkey2.EnableWindow(FALSE);
	}
	// 相対座標モードが無効の時の処理（グレイ表示）
	if(!IsDlgButtonChecked(IDC_CHK_RELMODE))
	{
		m_hotkey_rel.EnableWindow(FALSE);
	}
	if(!IsDlgButtonChecked(IDC_CHANGECOLOR))
	{
		m_pickcolor.EnableWindow(FALSE);
	}
	if(!IsDlgButtonChecked(IDC_CHANGEDLGCOLOR))
	{
		m_pickdlgcol.EnableWindow(FALSE);
		m_pickdlgbackcol.EnableWindow(FALSE);
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// **********************************
//  カーソルアシスタント チェックボックスによるホットキー選択の有効・無効
// **********************************
void CPropConf::OnAssist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// アシスタントが無効の時の処理（グレイ表示）
	if(!IsDlgButtonChecked(IDC_ASSIST))
	{
		m_hotkey1.EnableWindow(FALSE);
		m_hotkey2.EnableWindow(FALSE);
	}
	else
	{
		m_hotkey1.EnableWindow(TRUE);
		m_hotkey2.EnableWindow(TRUE);
	}

	
}


// **********************************
//  相対座標モード チェックボックスによるホットキー選択の有効・無効
// **********************************
void CPropConf::OnChkRelmode() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// 相対座標モードが無効の時の処理（グレイ表示）
	if(!IsDlgButtonChecked(IDC_CHK_RELMODE))
	{
		m_hotkey_rel.EnableWindow(FALSE);
	}
	else
	{
		m_hotkey_rel.EnableWindow(TRUE);
	}
	
}


// **********************************
//  座標文字のカラー変更
// **********************************
void CPropConf::OnPickcolor() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CColorDialog dlg(m_TextColor, CC_FULLOPEN);
	if(dlg.DoModal()==IDOK)
		m_TextColor = dlg.GetColor();
}

// **********************************
//  ダイアログのカラー変更（背景）
// **********************************
void CPropConf::OnPickdlgbackcol() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CColorDialog dlg(m_DlgBackColor, CC_FULLOPEN);
	if(dlg.DoModal()==IDOK)
		m_DlgBackColor = dlg.GetColor();
	
}

// **********************************
//  ダイアログのカラー変更
// **********************************
void CPropConf::OnPickdlgcol() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CColorDialog dlg(m_DlgColor, CC_FULLOPEN);
	if(dlg.DoModal()==IDOK)
		m_DlgColor = dlg.GetColor();
	
}

// **********************************
//  スイッチボックスによるボタンの有効・無効の表示切替
// **********************************
void CPropConf::OnChangecolor() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_CHANGECOLOR))
	{
		m_pickcolor.EnableWindow(FALSE);
	}
	else
	{
		m_pickcolor.EnableWindow(TRUE);
	}
	
}

// **********************************
//  スイッチボックスによるボタンの有効・無効の表示切替
// **********************************
void CPropConf::OnChangedlgcolor() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_CHANGEDLGCOLOR))
	{
		m_pickdlgcol.EnableWindow(FALSE);
		m_pickdlgbackcol.EnableWindow(FALSE);
	}
	else
	{
		m_pickdlgcol.EnableWindow(TRUE);
		m_pickdlgbackcol.EnableWindow(TRUE);
	}
	
}

// **********************************
//  アンインストール用のショートカット作成
// **********************************
void CPropAbout::OnBtnMkUnin() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::MkUninstMnu();

	MessageBox("Uninstall shortcut is made on start menu", "Cursor32 installer", MB_ICONINFORMATION|MB_OK);
}

