// Cursor32.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "Cursor32.h"
#include "InstDlg.h"
#include "global.h"

#include "InstFunc.h"
#include "CheckWinVer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char  sMutexInstance[] = {"Cursor32_free"};    //  多重起動防止のため利用


/////////////////////////////////////////////////////////////////////////////
// CCursor32App

BEGIN_MESSAGE_MAP(CCursor32App, CWinApp)
	//{{AFX_MSG_MAP(CCursor32App)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCursor32App クラスの構築

CCursor32App::CCursor32App()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CCursor32App オブジェクト

CCursor32App theApp;

/////////////////////////////////////////////////////////////////////////////
// CCursor32App クラスの初期化

BOOL CCursor32App::InitInstance()
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

	// ﾚｼﾞｽﾄﾘのﾀｰｹﾞｯﾄを以下のようにする
	// HKEY_USERS/.Default/Software/HI-soft/Cursor32 ...
//	m_pszAppName = "Cursor32";
	// リソースでAFX_IDS_APP_TITLEにアプリケーション名を設定すること 
	// m_pszAppName = "compdel" としてはいけない！ ASSERTION エラーとなる
	SetRegistryKey((LPCTSTR)"hi soft");


	// **********************************
	//  多重起動防止
	// **********************************
	m_hMutex = ::CreateMutex(NULL, TRUE, sMutexInstance);
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{	// すでに起動している
//		::CloseHandle(m_hMutex);
		m_bMutexOwner = FALSE;
		::MessageBox(NULL, "Cursor32 is already executed", "Cursor32", MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	m_bMutexOwner = TRUE;


	CCursor32Dlg dlg;

	// **********************************
	//  レジストリから全設定値を読み込む
	// **********************************
	RegConfigRead(&dlg);

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
	else if(!strcmpi(m_lpCmdLine, "/delete"))
	{	// アンインストール
		UninstallSeq();
		return FALSE;
	}

//	m_pMainWnd = &dlg;

	// ダイアログ自体の表示色を変更する
	if(dlg.nChangeDlgColor) SetDialogBkColor(dlg.DlgBackColor, dlg.DlgColor);

	dlg.DoModal(); // メインダイアログの表示

	RegConfigWrite(&dlg);

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}


int CCursor32App::ExitInstance() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if(m_bMutexOwner)
	{
		// **********************************
		//  多重起動防止の Mutex を閉じる
		// **********************************
		::ReleaseMutex(m_hMutex);
		::CloseHandle(m_hMutex);
	}
	
	return CWinApp::ExitInstance();
}


// **********************************
//  レジストリから設定値を読み込む
// **********************************
void CCursor32App::RegConfigRead(CCursor32Dlg *dlg)
{
	// 常に手前に表示
	dlg->isTopmost = GetProfileInt("Settings","top",1);

	// タスクバーに表示しない
	dlg->isNotTaskbar = GetProfileInt("Settings","ntask",1);

	// タイマー間隔
	dlg->timertick = GetProfileInt("Settings","timer",100);

	// カーソルアシスタンス
	dlg->IsAssist = GetProfileInt("Settings","assist",0);
	// 相対座標モード
	dlg->IsRelmode = GetProfileInt("Settings","relmode",0);
	// ホットキー
	dlg->hotkey1 = GetProfileInt("Settings","hotkey1",VK_CONTROL);
	dlg->hotkey2 = GetProfileInt("Settings","hotkey2",VK_SHIFT);
	dlg->hotkey_rel = GetProfileInt("Settings","hotkey_rel",VK_F12);

	// タイトル
	dlg->title = GetProfileString("Settings","title","Cursor32");

	// ウインドウ位置
	dlg->x_init = GetProfileInt("Settings","x",0);
	dlg->y_init = GetProfileInt("Settings","y",0);

	// XY表示色を変えるかどうか
	dlg->nChangeColor = GetProfileInt("Settings","color",1);
	// XY表示色
	dlg->TextColor = GetProfileInt("Settings","b",0)*0x10000L +
					GetProfileInt("Settings","g",0)*0x100L +
					GetProfileInt("Settings","r",128);

	// ダイアログの色を変えるかどうか
	dlg->nChangeDlgColor = GetProfileInt("Settings","colordlg",0);
	// ダイアログ前景色
	dlg->DlgColor = GetProfileInt("Settings","db",0)*0x10000L +
					GetProfileInt("Settings","dg",0)*0x100L +
					GetProfileInt("Settings","dr",0);
	// ダイアログ背景色
	dlg->DlgBackColor = GetProfileInt("Settings","dbb",255)*0x10000L +
					GetProfileInt("Settings","dbg",255)*0x100L +
					GetProfileInt("Settings","dbr",255);
}

// **********************************
//  レジストリに設定値を書き込む
// **********************************
void CCursor32App::RegConfigWrite(CCursor32Dlg *dlg)
{
	// 常に手前に表示
	if(dlg->isTopmost != (BOOL)GetProfileInt("Settings","top",1))
		WriteProfileInt("Settings","top",dlg->isTopmost);

	// タスクバーに表示しない
	if(dlg->isNotTaskbar != (BOOL)GetProfileInt("Settings","ntask",1))
		WriteProfileInt("Settings","ntask",dlg->isNotTaskbar);

	// タイマー間隔
	if(dlg->timertick != (int)GetProfileInt("Settings","timer",100))
		WriteProfileInt("Settings","timer",dlg->timertick);

	// カーソルアシスタンス
	if(dlg->IsAssist != (BOOL)GetProfileInt("Settings","assist",0))
		WriteProfileInt("Settings","assist",dlg->IsAssist);
	// 相対座標モード
	if(dlg->IsRelmode != (BOOL)GetProfileInt("Settings","relmode",0))
		WriteProfileInt("Settings","relmode",dlg->IsRelmode);
	// ホットキー
	if(dlg->hotkey1 != GetProfileInt("Settings","hotkey1",VK_CONTROL))
		WriteProfileInt("Settings","hotkey1",dlg->hotkey1);
	if(dlg->hotkey2 != GetProfileInt("Settings","hotkey2",VK_SHIFT))
		WriteProfileInt("Settings","hotkey2",dlg->hotkey2);
	if(dlg->hotkey_rel != GetProfileInt("Settings","hotkey_rel",VK_F12))
		WriteProfileInt("Settings","hotkey_rel",dlg->hotkey_rel);

	// タイトル
	if(dlg->title != GetProfileString("Settings","title","Cursor32"))
		WriteProfileString("Settings","title",dlg->title);

	// ウインドウ位置
	if(dlg->x_init != (int)GetProfileInt("Settings","x",0))
		WriteProfileInt("Settings","x",dlg->x_init);
	if(dlg->y_init != (int)GetProfileInt("Settings","y",0))
		WriteProfileInt("Settings","y",dlg->y_init);

	// XY表示色を変えるかどうか
	if(dlg->nChangeColor != (BOOL)GetProfileInt("Settings","color",1))
		WriteProfileInt("Settings","color",dlg->nChangeColor);
	// XY表示色
	if(dlg->TextColor != GetProfileInt("Settings","b",0)*0x10000L +
					GetProfileInt("Settings","g",0)*0x100L +
					GetProfileInt("Settings","r",128))
	{
		WriteProfileInt("Settings","b",(dlg->TextColor/0x10000L)&0xff);
		WriteProfileInt("Settings","g",(dlg->TextColor/0x100L)&0xff);
		WriteProfileInt("Settings","r", dlg->TextColor&0xff);
	}

	// ダイアログの色を変えるかどうか
	if(dlg->nChangeDlgColor != (BOOL)GetProfileInt("Settings","colordlg",0))
		WriteProfileInt("Settings","colordlg",dlg->nChangeDlgColor);
	// ダイアログ前景色
	if(dlg->DlgColor != GetProfileInt("Settings","db",0)*0x10000L +
					GetProfileInt("Settings","dg",0)*0x100L +
					GetProfileInt("Settings","dr",0))
	{
		WriteProfileInt("Settings","db",(dlg->DlgColor/0x10000L)&0xff);
		WriteProfileInt("Settings","dg",(dlg->DlgColor/0x100L)&0xff);
		WriteProfileInt("Settings","dr", dlg->DlgColor&0xff);
	}
	// ダイアログ背景色
	if(dlg->DlgBackColor != GetProfileInt("Settings","dbb",255)*0x10000L +
					GetProfileInt("Settings","dbg",255)*0x100L +
					GetProfileInt("Settings","dbr",255))
	{
		WriteProfileInt("Settings","dbb",(dlg->DlgBackColor/0x10000L)&0xff);
		WriteProfileInt("Settings","dbg",(dlg->DlgBackColor/0x100L)&0xff);
		WriteProfileInt("Settings","dbr", dlg->DlgBackColor&0xff);
	}

}

// **********************************
// ダイアログのカラーを変更する
//
// SetDialogBkColor が Protected 関数のため、外部から呼び出せないことに対する対策
// ダイアログ自体から呼び出して使う
// **********************************
void CCursor32App::SetDialogBkColor_Extern(COLORREF cback, COLORREF cfore)
{
	SetDialogBkColor(cback, cfore);
}

// **********************************
//  インストール作業 一式
//
//  戻り値  TRUE:インストール完了または不要,  FALSE:ユーザキャンセル
// **********************************
BOOL CCursor32App::InstallSeq()
{
	CInstDlg dlg;
	dlg.m_r_autoexec = 0;	// 自動起動しない
	dlg.m_chk_uninst_start = TRUE;	// スタートメニューにアンインストールアイコン


	// **********************************
	//  インストールフラグのチェック (HKLM/HKCU)
	// **********************************
	if(ChkHkcuInstReg() || ChkHklmInstReg()) return TRUE;

	if(dlg.DoModal() != IDOK) return FALSE;	// ユーザがインストールをキャンセル
	::MkHkcuInstReg();

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
	//  スタートメニューにアイコン登録
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
BOOL CCursor32App::UninstallSeq()
{
	CString sTmpMsg;

	sTmpMsg.LoadString(STR_MES_UNIN);
	if(::MessageBox(NULL, (LPCSTR)sTmpMsg, "Cursor32 Uninstall", MB_YESNO|MB_ICONQUESTION) != IDYES)
		return FALSE;	// ユーザがキャンセル

	::RmHkcuInstReg();
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


