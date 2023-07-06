// CookieCtrlDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "CookieCtrl.h"
#include "CookieCtrlDlg.h"
#include "DlgAbout.h"
#include "InstDlg.h"
#include <io.h>			// findfirst
#include "GlobalFunc.h"
#include "InstFunc.h"
#include "CheckWinVer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlDlg ダイアログ

CCookieCtrlDlg::CCookieCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCookieCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCookieCtrlDlg)
	m_str_folder = _T("");
	m_str_ext = _T("");
	m_str_exclude = _T("");
	m_int_days = 0;
	m_sw_days = FALSE;
	m_sw_exclude = FALSE;
	m_chk_auto_folder = FALSE;
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCookieCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCookieCtrlDlg)
	DDX_Control(pDX, IDC_CHECK_DAYS, m_sw_days_ctrl);
	DDX_Control(pDX, IDC_BTN_FOLDER_BRW, m_btn_folder_brw);
	DDX_Control(pDX, IDC_BTN_FOLDER_AUTO, m_btn_folder_auto);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_str_folder_ctrl);
	DDX_Control(pDX, IDC_STR_DAYS, m_str_days_ctrl);
	DDX_Control(pDX, IDC_SCR_DAYS, m_scroll_days_ctrl);
	DDX_Control(pDX, IDC_EDIT_DAYS, m_int_days_ctrl);
	DDX_Control(pDX, IDC_EDIT_EXCLUDE, m_str_exclude_ctrl);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_str_folder);
	DDV_MaxChars(pDX, m_str_folder, 255);
	DDX_Text(pDX, IDC_EDIT_EXT, m_str_ext);
	DDV_MaxChars(pDX, m_str_ext, 255);
	DDX_Text(pDX, IDC_EDIT_EXCLUDE, m_str_exclude);
	DDV_MaxChars(pDX, m_str_exclude, 255);
	DDX_Text(pDX, IDC_EDIT_DAYS, m_int_days);
	DDV_MinMaxInt(pDX, m_int_days, 0, 30);
	DDX_Check(pDX, IDC_CHECK_DAYS, m_sw_days);
	DDX_Check(pDX, IDC_CHECK_EXCLUDE, m_sw_exclude);
	DDX_Check(pDX, IDC_CHECK_AUTO_FOLDER, m_chk_auto_folder);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCookieCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CCookieCtrlDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FOLDER_BRW, OnBtnFolderBrw)
	ON_BN_CLICKED(IDC_BTN_FOLDER_AUTO, OnBtnFolderAuto)
	ON_BN_CLICKED(IDC_BTN_EXT, OnBtnExt)
	ON_BN_CLICKED(IDC_CHECK_EXCLUDE, OnCheckExclude)
	ON_BN_CLICKED(IDC_CHECK_DAYS, OnCheckDays)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BTN_RUN, OnBtnRun)
	ON_BN_CLICKED(IDC_BTN_STARTUP, OnBtnStartup)
	ON_BN_CLICKED(IDC_BTN_VERSION, OnBtnVersion)
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_CHECK_AUTO_FOLDER, OnCheckAutoFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlDlg メッセージ ハンドラ

BOOL CCookieCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。

	// チェックボックスの状態により、コントロールの有効・無効を切り替える
	OnCheckExclude();
	OnCheckDays();
	OnCheckAutoFolder();

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CCookieCtrlDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CCookieCtrlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// **********************************
// Cookie フォルダの参照
// **********************************
void CCookieCtrlDlg::OnBtnFolderBrw() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char str_tmp[MAX_PATH];
	CString sTmpMsg;

	sTmpMsg.LoadString(STR_MES_SEL_COKFOL);
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpszTitle = (LPCSTR)sTmpMsg;
	bi.hwndOwner = m_hWnd;

	pidl = ::SHBrowseForFolder(&bi);
	if(pidl == NULL) return;
	SHGetPathFromIDList(pidl, str_tmp);


	// ファイル入力ボックスをアップデートする
	SetDlgItemText(IDC_EDIT_FOLDER, str_tmp);
	
}

// **********************************
// レジストリより標準のCookieフォルダを代入
// **********************************
void CCookieCtrlDlg::OnBtnFolderAuto() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HKEY hCU;
	DWORD lpType = REG_SZ;
	char str_tmp[MAX_PATH];
	unsigned long ulSize = MAX_PATH;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
		0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Cookies", NULL, &lpType, (unsigned char *)&str_tmp, &ulSize);
		RegCloseKey(hCU);
		// ファイル入力ボックスをアップデートする
		SetDlgItemText(IDC_EDIT_FOLDER, str_tmp);
	}
	
}

// **********************************
// 標準の対象ファイル名を代入
// **********************************
void CCookieCtrlDlg::OnBtnExt() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// ファイル入力ボックスをアップデートする
	SetDlgItemText(IDC_EDIT_EXT, "*.txt;*.url");
	
}

// **********************************
// チェックボックスの状況による、非アクティブ項目の変化
// **********************************
void CCookieCtrlDlg::OnCheckExclude() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString sTmpMsg;

	if(!IsDlgButtonChecked(IDC_CHECK_EXCLUDE))
	{
		m_str_exclude_ctrl.EnableWindow(FALSE);
		sTmpMsg.LoadString(STR_CTL_CHK_DAYS_0);
	}
	else
	{
		m_str_exclude_ctrl.EnableWindow(TRUE);
		sTmpMsg.LoadString(STR_CTL_CHK_DAYS_1);
	}
	m_sw_days_ctrl.SetWindowText((LPCSTR)sTmpMsg);

}

// **********************************
// チェックボックスの状況による、非アクティブ項目の変化
// **********************************
void CCookieCtrlDlg::OnCheckDays() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_CHECK_DAYS))
	{
		m_int_days_ctrl.EnableWindow(FALSE);
		m_scroll_days_ctrl.EnableWindow(FALSE);
		m_str_days_ctrl.EnableWindow(FALSE);
	}
	else
	{
		m_int_days_ctrl.EnableWindow(TRUE);
		m_scroll_days_ctrl.EnableWindow(TRUE);
		m_str_days_ctrl.EnableWindow(TRUE);
	}
	
}

// **********************************
// チェックボックスの状況による、非アクティブ項目の変化
// **********************************
void CCookieCtrlDlg::OnCheckAutoFolder() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(!IsDlgButtonChecked(IDC_CHECK_AUTO_FOLDER))
	{
		m_str_folder_ctrl.EnableWindow(TRUE);
		m_btn_folder_brw.EnableWindow(TRUE);
		m_btn_folder_auto.EnableWindow(TRUE);
		
	}
	else
	{
		m_str_folder_ctrl.EnableWindow(FALSE);
		m_btn_folder_brw.EnableWindow(FALSE);
		m_btn_folder_auto.EnableWindow(FALSE);
	}
	
}

// **********************************
// 日数のスクロールバーが押された場合の処理
// **********************************
//
// CScrollBar は、何にもしてくれないので、CWndがスクロールバーが押されたことを検知して
// 処理をしないといけない。
void CCookieCtrlDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	int days;
	
	days = GetDlgItemInt(IDC_EDIT_DAYS);

	if(pScrollBar->GetDlgCtrlID() == IDC_SCR_DAYS)
	{
		switch(nSBCode)
		{
			case SB_LINEUP :
				if(days<30) days++;
				break;
			case SB_LINEDOWN :
				if(days>0) days--;
				break;
			default : break;
		}
		SetDlgItemInt(IDC_EDIT_DAYS, days);
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

// **********************************
//  今すぐ実行
// **********************************
void CCookieCtrlDlg::OnBtnRun() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	// **********************************
	//  ダイアログ項目の取得し、グローバル変数へ
	// **********************************
	GetDlgItemText(IDC_EDIT_FOLDER, ::gstr_folder);
	GetDlgItemText(IDC_EDIT_EXT, ::gstr_ext);
	if(IsDlgButtonChecked(IDC_CHECK_EXCLUDE))
		::gint_sw_exclude = 1;
	else
		::gint_sw_exclude = 0;
	GetDlgItemText(IDC_EDIT_EXCLUDE, ::gstr_exclude);
	if(IsDlgButtonChecked(IDC_CHECK_DAYS))
		::gint_sw_days = 1;
	else
		::gint_sw_days = 0;
	::gint_days = GetDlgItemInt(IDC_EDIT_DAYS);

	::DeleteCookie();

	// **********************************
	//  レジストリの内容を復元（グローバル変数に）
	//  ダイアログの状況はそのまま
	// **********************************
	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->RegReadAll();


}

// **********************************
//  再インストールダイアログを表示
// **********************************
void CCookieCtrlDlg::OnBtnStartup() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	ReInstallSeq();

}

// **********************************
//  バージョンダイアログを表示
// **********************************
void CCookieCtrlDlg::OnBtnVersion() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CDlgAbout dlg;
	dlg.DoModal();
}

// **********************************
//  ヘルプを表示
// **********************************
void CCookieCtrlDlg::OnBtnHelp() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->WinHelp(0);

}

// **********************************
//  F1 キーが押された場合、ヘルプを表示
// **********************************
BOOL CCookieCtrlDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	// 「ボタン」の方を使うように警告する
	CString str_tmp1, str_tmp2;
	str_tmp1.LoadString(STR_MES_F1MES);
	str_tmp2.LoadString(STR_MES_F1CAP);
	::MessageBox(this->m_hWnd, str_tmp1, str_tmp2, MB_OK|MB_ICONEXCLAMATION);

	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->WinHelp(0);
	
	return 0;
}

// **********************************
//  再インストール作業 一式
//
//  戻り値  TRUE:インストール完了または不要,  FALSE:ユーザキャンセル
// **********************************
BOOL CCookieCtrlDlg::ReInstallSeq()
{
	CInstDlg dlg;
	dlg.m_r_autoexec = 1;	// スタートメニューに登録
	dlg.m_chk_uninst_start = TRUE;	// スタートメニューにアンインストールアイコン
	dlg.m_IsAdminInst = TRUE;
	CString sTmpMsg;


	// **********************************
	//  インストール状況のチェックと設定
	// **********************************
	
	if(ChkHklmInstReg()) dlg.m_r_autoexec = 3;

	// **********************************
	//  Windows NT 系列とそれ以外で区別
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT のとき ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run 書き込み可のとき (Admin モード)
			if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
			if(dlg.m_r_autoexec == 3)
			{
				::MkHklmInstReg();	// Admin インストール
				::RmHkcuInstReg();
			}
			else
			{
				::MkHkcuInstReg();	// Current User インストール
				::RmHklmInstReg();
			}
		}
		else
		{	// HKLM ... Run 書き込み不可のとき (ユーザ モード)
			if(ChkHklmInstReg())
			{	// 再インストールは不可
				sTmpMsg.LoadString(STR_MES_REIN_NOT);
				::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Re-Install Error", MB_OK|MB_ICONSTOP);
				return FALSE;
			}
			else
			{
				dlg.m_IsAdminInst = FALSE;	
				if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
				::MkHkcuInstReg();
			}
		}
	}
	else
	{	// ********************  WinNT以外 のとき ********************
		if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
		if(dlg.m_r_autoexec == 3)
		{
			::MkHklmInstReg();	// Admin インストール
			::RmHkcuInstReg();
		}
		else
		{
			::MkHkcuInstReg();	// Current User インストール
			::RmHklmInstReg();
		}
	}

	// **********************************
	//  インストールダイアログでの選択項目により、インストール作業
	// **********************************
	//  自動実行オプション
	//  選択されたオプション以外の「自動実行」を削除することも同時に行う
	// **********************************
	switch(dlg.m_r_autoexec)
	{
	case 0 :	// 自動実行しない
		::RmHklmLnk();
		::RmHkcuLnk();
		::RmStartMnu();
		break;
	case 1 :	// スタートメニュー
		::RmHklmLnk();
		::RmHkcuLnk();
		::MkStartMnu();
		break;
	case 2 :	// HKCU に自動実行
		::RmHklmLnk();
		::MkHkcuLnk();
		::RmStartMnu();
		break;
	case 3 :	// HKLM に自動実行
		::MkHklmLnk();
		::RmHkcuLnk();
		::RmStartMnu();
		break;
	}

	// **********************************
	//  スタートメニューに設定アイコン登録
	// **********************************
	::MkProgramsMnu();

	// **********************************
	//  コンパネからアンインストールする設定
	// **********************************
	if(dlg.m_chk_uninst_cpl)
	{
		if(::IsWinNT()) ::MkHkcuUninstMnu();
		else ::MkHklmUninstMnu();
	}

	// **********************************
	//  スタートメニューからアンインストールする設定
	// **********************************
	if(dlg.m_chk_uninst_start)
		::MkUninstMnu();

	return TRUE;

}
