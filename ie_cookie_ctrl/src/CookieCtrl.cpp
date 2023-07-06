// CookieCtrl.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "CookieCtrl.h"
#include "CookieCtrlDlg.h"
#include "InstDlg.h"
#include "GlobalFunc.h"
#include "InstFunc.h"
#include "CheckWinVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// グローバル変数
CString gstr_folder;		// Cookieフォルダ
CString gstr_ext;		// Cookie ファイル名
CString gstr_exclude;	// 除外するファイル文字列
int gint_sw_auto_folder;			// Cookie フォルダの自動取得スイッチ
int gint_days;			// 除外する期間
int gint_sw_exclude;		// 除外するファイル文字列のスイッチ
int gint_sw_days;		// 除外する期間のスイッチ
int gint_admin_inst;	// アドミニストレーター（最初のインストール者)
int gint_user_inst;		// 現在のユーザでインストールされているか


/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlApp

BEGIN_MESSAGE_MAP(CCookieCtrlApp, CWinApp)
	//{{AFX_MSG_MAP(CCookieCtrlApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlApp クラスの構築

CCookieCtrlApp::CCookieCtrlApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CCookieCtrlApp オブジェクト

CCookieCtrlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlApp クラスの初期化

BOOL CCookieCtrlApp::InitInstance()
{
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	CString str_tmp1, str_tmp2;		// ダイアログメッセージなどの一時格納

	// システムレジストリの使用位置を設定する
	// \HKEY_USERS\.Default\Software\hi soft\AFS_IDS_APP_TITLE\Settings
	SetRegistryKey((LPCTSTR)"hi soft");
	LoadStdProfileSettings(0);	// MRUリスト管理(0:しない)



	// **********************************
	//  レジストリから全設定値を読み込む
	// **********************************
	RegReadAll();

	// **********************************
	//  コマンドライン引数による分岐
	// **********************************
	if(!strcmpi(m_lpCmdLine, ""))
	{	// オプションなし
		// **********************************
		//  インストーラの起動。
		//  すでに、インストール済みと時は、単に TRUE を返すだけ
		// **********************************
		if(!InstallSeq())
			return FALSE;	// インストーラをユーザがキャンセルしたとき （プログラム終了）
	}
	else if(!strcmpi(m_lpCmdLine, "/start"))
	{	// 自動実行
		::DeleteCookie();
		return FALSE;		// （プログラム終了）
	}
	else if(!strcmpi(m_lpCmdLine, "/delete"))
	{	// アンインストール
		UninstallSeq();
		return FALSE;
	}


	// **********************************
	//  設定ダイアログの表示
	// **********************************
	CCookieCtrlDlg dlg;
	m_pMainWnd = &dlg;
	dlg.m_chk_auto_folder = ::gint_sw_auto_folder;
	dlg.m_str_folder = ::gstr_folder;
	dlg.m_str_ext = ::gstr_ext;
	dlg.m_str_exclude = ::gstr_exclude;
	dlg.m_int_days = ::gint_days;
	dlg.m_sw_exclude = ::gint_sw_exclude;
	dlg.m_sw_days = ::gint_sw_days;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//       記述してください。
		::gint_sw_auto_folder = dlg.m_chk_auto_folder;
		if(!::gint_sw_auto_folder) ::gstr_folder = dlg.m_str_folder;
		else ::gstr_folder = "";
		::gstr_ext = dlg.m_str_ext;
		::gstr_exclude = dlg.m_str_exclude;
		::gint_days = dlg.m_int_days;
		::gint_sw_exclude = dlg.m_sw_exclude;
		::gint_sw_days = dlg.m_sw_days;
		::gint_user_inst = 1;	// インストール済み
		RegWriteAll();
	}

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

void CCookieCtrlApp::RegReadAll()
{
	::gstr_folder = GetProfileString("Settings","folder","");
	::gstr_ext = GetProfileString("Settings","ext","*.txt;*.url");
	::gstr_exclude = GetProfileString("Settings","exclude","");
	::gint_sw_auto_folder = GetProfileInt("Settings","auto",1);
	::gint_days = GetProfileInt("Settings","days",0);
	::gint_sw_exclude = GetProfileInt("Settings","sw_exclude",0);
	::gint_sw_days = GetProfileInt("Settings","sw_days",0);
	::gint_admin_inst = GetProfileInt("Settings","admin_inst",0);
	::gint_user_inst = GetProfileInt("Settings","user_inst",0);

	if(::gstr_folder == "" || ::gint_sw_auto_folder)
	{
		HKEY hCU;
		DWORD lpType = REG_SZ;
		char str_tmp[MAX_PATH];
		unsigned long ulSize = MAX_PATH;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			RegQueryValueEx( hCU, "Cookies", NULL, &lpType, (unsigned char *)&str_tmp, &ulSize);
			RegCloseKey(hCU);
			::gstr_folder = str_tmp;
		}
	}
}

void CCookieCtrlApp::RegWriteAll()
{
	if(::gint_sw_auto_folder) ::gstr_folder = "";
	if(::gstr_folder != GetProfileString("Settings","folder",""))
		WriteProfileString("Settings","folder",::gstr_folder);
	if(::gstr_ext != GetProfileString("Settings","ext","*.txt;*.url"))
		WriteProfileString("Settings","ext",::gstr_ext);
	if(::gstr_exclude != GetProfileString("Settings","exclude",""))
		WriteProfileString("Settings","exclude",::gstr_exclude);
	if(::gint_sw_auto_folder != (int)GetProfileInt("Settings","auto",1))
		WriteProfileInt("Settings","auto",::gint_sw_auto_folder);
	if(::gint_days != (int)GetProfileInt("Settings","days",0))
		WriteProfileInt("Settings","days",::gint_days);
	if(::gint_sw_exclude != (int)GetProfileInt("Settings","sw_exclude",0))
		WriteProfileInt("Settings","sw_exclude",::gint_sw_exclude);
	if(::gint_sw_days != (int)GetProfileInt("Settings","sw_days",0))
		WriteProfileInt("Settings","sw_days",::gint_sw_days);
	if(::gint_user_inst != (int)GetProfileInt("Settings","user_inst",0))
		WriteProfileInt("Settings","user_inst",::gint_user_inst);

}

// **********************************
//  インストール作業 一式
//
//  戻り値  TRUE:インストール完了または不要,  FALSE:ユーザキャンセル
// **********************************
BOOL CCookieCtrlApp::InstallSeq()
{
	CInstDlg dlg;
	dlg.m_r_autoexec = 1;	// スタートメニューに登録
	dlg.m_chk_uninst_start = TRUE;	// スタートメニューにアンインストールアイコン
	dlg.m_IsAdminInst = TRUE;


	// **********************************
	//  インストールフラグのチェック (HKLM/HKCU)
	// **********************************
	if(ChkHkcuInstReg() || ChkHklmInstReg()) return TRUE;

	// **********************************
	//  Windows NT 系列とそれ以外で区別
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT のとき ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run 書き込み可のとき (Admin モード)
			if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
			if(dlg.m_r_autoexec == 3) ::MkHklmInstReg();	// Admin インストール
			else ::MkHkcuInstReg();	// Current User インストール
		}
		else
		{	// HKLM ... Run 書き込み不可のとき (ユーザ モード)
			dlg.m_IsAdminInst = FALSE;	
			if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
			::MkHkcuInstReg();
		}
	}
	else
	{	// ********************  WinNT以外 のとき ********************
		if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
		if(dlg.m_r_autoexec == 3) ::MkHklmInstReg();	// Admin インストール
		else ::MkHkcuInstReg();	// Current User インストール
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

// **********************************
//  アンインストール作業 一式
//
//  戻り値  TRUE:完了または不要,  FALSE:失敗またはユーザキャンセル
// **********************************
BOOL CCookieCtrlApp::UninstallSeq()
{
	CString sTmpMsg;

	// **********************************
	//  Windows NT 系列とそれ以外で区別
	// **********************************
	if(::IsWinNT())
	{	// ********************  WinNT のとき ********************
		if(::IsPossibleWriteHklmRun())
		{	// HKLM ... Run 書き込み可のとき (Admin モードかも？)
			if(::ChkHklmInstReg())
			{	// Admin モードでインストールされていたとき
				sTmpMsg.LoadString(STR_MES_UNIN_ADM);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ユーザがキャンセル
				::RmHklmInstReg();
			}
			else
			{	// カレント・ユーザ・モードのとき
				sTmpMsg.LoadString(STR_MES_UNIN_CUR);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ユーザがキャンセル
				::RmHkcuInstReg();
			}
		}
		else
		{	// HKLM ... Run 書き込み不可のとき (ユーザ モード)
			if(::ChkHklmInstReg())
			{	// Admin モードでインストールされていたとき
				sTmpMsg.LoadString(STR_MES_UNIN_NOT);
				::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall Error", MB_OK|MB_ICONSTOP);
			}
			else
			{	// カレント・ユーザ・モードのとき
				sTmpMsg.LoadString(STR_MES_UNIN_CUR);
				if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
					return FALSE;	// ユーザがキャンセル
				::RmHkcuInstReg();
			}
		}
	}
	else
	{	// ********************  WinNT以外 のとき ********************
		if(::ChkHklmInstReg())
		{	// Admin モードでインストールされていたとき
			sTmpMsg.LoadString(STR_MES_UNIN_ADM);
			if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
				return FALSE;	// ユーザがキャンセル
			::RmHklmInstReg();
		}
		else
		{	// カレント・ユーザ・モードのとき
			sTmpMsg.LoadString(STR_MES_UNIN_CUR);
			if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "CookieCtrl Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
				return FALSE;	// ユーザがキャンセル
			::RmHkcuInstReg();
		}
	}

	::RmHkcuInstReg();		// どちらにしても、これは消しておく
	::RmHklmLnk();
	::RmHkcuLnk();
	::RmStartMnu();
	::RmUserReg();

	::RmHkcuUninstMnu();
	::RmHklmUninstMnu();
	::RmUninstMnu();

	::RmProgramsMnu();

	return TRUE;

}
