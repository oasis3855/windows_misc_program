// Cursor32.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "Cursor32.h"
#include "DlgInst.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

	CCursor32Dlg dlg;

	// レジストリから初期値を読み込む
	RegConfigRead(&dlg);

	// コマンドラインの解析
	if(!strcmpi(m_lpCmdLine, "delete"))
	{	// アンインストール
		UninstallSeq();
	}
	else if(!strcmpi(m_lpCmdLine, "inst"))
	{	// インストール
		InstallSeq();
	}
	else if(m_lpCmdLine[0] != NULL)
	{
		::MessageBox(NULL, "無効なスイッチが指定されました\n有効なパラメーターは、start,deleteです", "スタートアップ タスクスケジューラ",
			MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
	}
	else
	{	// 通常の実行
		if(!GetProfileInt("Global","inst",0))
		{	// インストールダイアログの表示とインストール処理
			InstallSeq();
		}
		else
		{
			m_pMainWnd = &dlg;

			// ダイアログ自体の表示色を変更する
			if(dlg.nChangeDlgColor) SetDialogBkColor(dlg.DlgBackColor, dlg.DlgColor);

			dlg.DoModal(); // メインダイアログの表示

			RegConfigWrite(&dlg);
		}
	}
	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

///////////////////////////////////////// ドキュメントの終わり

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
	// ホットキー
	dlg->hotkey1 = GetProfileInt("Settings","hotkey1",VK_CONTROL);
	dlg->hotkey2 = GetProfileInt("Settings","hotkey2",VK_SHIFT);

	// タイトル
	dlg->title = GetProfileString("Settings","title","Cursor32");

	// ウインドウ位置
	dlg->x_init = GetProfileInt("Settings","x",0);
	dlg->y_init = GetProfileInt("Settings","y",0);

	// XY表示色
	dlg->nChangeColor = GetProfileInt("Settings","color",1);
	dlg->TextColor = GetProfileInt("Settings","b",0)*0x10000L +
					GetProfileInt("Settings","g",0)*0x100L +
					GetProfileInt("Settings","r",128);

	// ダイアログ背景色
	dlg->nChangeDlgColor = GetProfileInt("Settings","colordlg",0);
	dlg->DlgColor = GetProfileInt("Settings","db",0)*0x10000L +
					GetProfileInt("Settings","dg",0)*0x100L +
					GetProfileInt("Settings","dr",0);
	dlg->DlgBackColor = GetProfileInt("Settings","dbb",255)*0x10000L +
					GetProfileInt("Settings","dbg",179)*0x100L +
					GetProfileInt("Settings","dbr",179);
}

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
	// ホットキー
	if(dlg->hotkey1 != GetProfileInt("Settings","hotkey1",VK_CONTROL))
		WriteProfileInt("Settings","hotkey1",dlg->hotkey1);
	if(dlg->hotkey2 != GetProfileInt("Settings","hotkey2",VK_SHIFT))
		WriteProfileInt("Settings","hotkey2",dlg->hotkey2);

	// タイトル
	if(dlg->title != GetProfileString("Settings","title","Cursor32"))
		WriteProfileString("Settings","title",dlg->title);

	// ウインドウ位置
	if(dlg->x_init != (int)GetProfileInt("Settings","x",0))
		WriteProfileInt("Settings","x",dlg->x_init);
	if(dlg->y_init != (int)GetProfileInt("Settings","y",0))
		WriteProfileInt("Settings","y",dlg->y_init);

	// XY表示色
	if(dlg->nChangeColor != (BOOL)GetProfileInt("Settings","color",1))
		WriteProfileInt("Settings","color",dlg->nChangeColor);
	if(dlg->TextColor != GetProfileInt("Settings","b",0)*0x10000L +
					GetProfileInt("Settings","g",0)*0x100L +
					GetProfileInt("Settings","r",128))
		WriteProfileInt("Settings","b",(dlg->TextColor/0x10000L)&0xff);
		WriteProfileInt("Settings","g",(dlg->TextColor/0x100L)&0xff);
		WriteProfileInt("Settings","r", dlg->TextColor&0xff);

	// ダイアログ背景色
	if(dlg->nChangeDlgColor != (BOOL)GetProfileInt("Settings","colordlg",0))
		WriteProfileInt("Settings","colordlg",dlg->nChangeDlgColor);
	if(dlg->DlgColor != GetProfileInt("Settings","db",255)*0x10000L +
					GetProfileInt("Settings","dg",179)*0x100L +
					GetProfileInt("Settings","dr",179))
		WriteProfileInt("Settings","db",(dlg->DlgColor/0x10000L)&0xff);
		WriteProfileInt("Settings","dg",(dlg->DlgColor/0x100L)&0xff);
		WriteProfileInt("Settings","dr", dlg->DlgColor&0xff);
	if(dlg->DlgBackColor != GetProfileInt("Settings","dbb",0)*0x10000L +
					GetProfileInt("Settings","dbg",0)*0x100L +
					GetProfileInt("Settings","dbr",0))
		WriteProfileInt("Settings","dbb",(dlg->DlgBackColor/0x10000L)&0xff);
		WriteProfileInt("Settings","dbg",(dlg->DlgBackColor/0x100L)&0xff);
		WriteProfileInt("Settings","dbr", dlg->DlgBackColor&0xff);

}

void CCursor32App::InstallSeq()
{
	HKEY hCU;
	DWORD lpType, dw;
	char szProg[MAX_PATH];
	char szLink[MAX_PATH];
	CString msg;

	CDlgInst dlg;
	dlg.m_logon = 0;
	dlg.m_manual = 1;
	dlg.m_uninst = 1;

	if(dlg.DoModal() == TRUE)
	{
		if(dlg.m_uninst)
		{	// アンインストール情報
			if(!::GetModuleFileName(NULL, szProg, MAX_PATH)) return;
			strcat(szProg, " delete");
			if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Cursor32",
				0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hCU, &dw) == ERROR_SUCCESS)
			{
				RegSetValueEx( hCU, "UninstallString", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
				strcpy(szProg, "Cursor32");
				RegSetValueEx( hCU, "DisplayName", 0, REG_SZ, (const unsigned char *)szProg, strlen(szProg));
				RegCloseKey(hCU);
			}
		}
		if(dlg.m_logon)
		{	// ログオン時に自動実行
			// 自動実行アイコンの登録
			::GetModuleFileName(NULL, szProg, MAX_PATH);
			ULONG ulSize = MAX_PATH;
				if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
				0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
			{
				RegQueryValueEx( hCU, "Startup", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
				RegCloseKey(hCU);
			}
			strcat(szLink, "\\Cursor32.LNK");
			::CreateShellLink(szProg, szLink, "Cursor32");
		}
		if(dlg.m_manual)
		{	// 手動実行
			::GetModuleFileName(NULL, szProg, MAX_PATH);
			ULONG ulSize = MAX_PATH;
			if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
				0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
			{
				RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
				RegCloseKey(hCU);
			}
			strcat(szLink, "\\Cursor32.LNK");
			::CreateShellLink(szProg, szLink, "Cursor32");
		}
		WriteProfileInt("Global","inst",1);
	}
}

void CCursor32App::UninstallSeq()
{
	if(::MessageBox(NULL, "Cursor32をアンインストールします\n\n\n実行アイコンと設定内容をすべて消去します",
		"Cursor32 Uninstall", MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL )
		!= IDYES) return;

	HKEY hCU;
	char szLink[MAX_PATH];
	DWORD lpType;
	ULONG ulSize = MAX_PATH;

	// レジストリの設定情報を消去
	::RegDeleteKey(HKEY_USERS,".Default\\Software\\hi soft\\Cursor32");
	// スタートメニューの設定アイコンを抹殺
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Programs", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, "\\Cursor32.LNK");
	::remove(szLink);
	// スタートメニューの自動実行アイコンを抹殺
	ulSize = MAX_PATH;	// 文字数は元に戻して実行すること
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, "Startup", NULL, &lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
	}
	strcat(szLink, "\\Cursor32.LNK");
	::remove(szLink);
	// 削除データの削除
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
			0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		RegDeleteKey( hCU, "Cursor32");
		RegCloseKey(hCU);
	}
	::MessageBox(NULL, "Cursor32をアンインストールしました\n\n\nプログラムのあるフォルダの関連ファイルは手動で削除してください",
		"Cursor32", MB_YESNO|MB_ICONINFORMATION|MB_APPLMODAL );


}
