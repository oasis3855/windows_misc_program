// InfoBar00.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "InfoBar00.h"
#include "InfoBar00Dlg.h"
#include "InstDlg.h"
#include "GlobalFunc.h"		// 通信スレッドおよびグローバル変数

#include "InstFunc.h"
#include "CheckWinVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char  sMutexInstance[] = {"InfoBar00"};    //  多重起動防止のため利用


/////////////////////////////////////////////////////////////////////////////
// CInfoBar00App

BEGIN_MESSAGE_MAP(CInfoBar00App, CWinApp)
	//{{AFX_MSG_MAP(CInfoBar00App)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoBar00App クラスの構築

CInfoBar00App::CInfoBar00App()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CInfoBar00App オブジェクト

CInfoBar00App theApp;



/////////////////////////////////////////////////////////////////////////////
// CInfoBar00App クラスの初期化

BOOL CInfoBar00App::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

	// ﾚｼﾞｽﾄﾘのﾀｰｹﾞｯﾄを以下のようにする
	// HKEY_USERS/.Default/Software/HI-soft/Cursor32 ...
//	m_pszAppName = "Cursor32";
	// リソースでAFX_IDS_APP_TITLEにアプリケーション名を設定すること 
	// m_pszAppName = "compdel" としてはいけない！ ASSERTION エラーとなる
	SetRegistryKey((LPCTSTR)"hi soft");

	// インストールフラグを FALSE に設定
	this->bInstalled = FALSE;

	CInfoBar00Dlg dlg;


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

	// **********************************
	//  多重起動防止
	// **********************************
	m_hMutex = ::CreateMutex(NULL, TRUE, sMutexInstance);
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{	// すでに起動している
//		::CloseHandle(m_hMutex);
		m_bMutexOwner = FALSE;
		return FALSE;
	}
	m_bMutexOwner = TRUE;


	// **********************************
	//  メインダイアログの表示
	// **********************************
	if(this->bInstalled) dlg.tPrev = 1;	// 0でなければ、ネット接続する
	else dlg.tPrev = 0;		// ネットに接続せず、ヘルプメッセージを流す

	m_pMainWnd = &dlg;
	dlg.DoModal();

	RegConfigWrite(&dlg);


	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}


int CInfoBar00App::ExitInstance() 
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
//  レジストリから設定値を読み出す
//
// **********************************
void CInfoBar00App::RegConfigRead(CInfoBar00Dlg *dlg)
{
	// モード
	nMode = GetProfileInt("Settings","Mode",0);

	// URL
	sURL = GetProfileString("Settings","URL","headlines.yahoo.co.jp/hl?c=pol&t=l");
	// ポート番号
	nPort = GetProfileInt("Settings","Port",80);
	// プロキシ・サーバ （標準ではなし：直接接続）
	sProxy = GetProfileString("Settings","Proxy","");
	// 切り分けヘッダ
	sPhHeader = GetProfileString("Settings","Header","<li>");
	// 切り分けヘッダ B
	sPhHeaderB = GetProfileString("Settings","HeaderB","");
	// 切り分けヘッダ C
	sPhHeaderC = GetProfileString("Settings","HeaderC","");
	// 先頭飛ばし個数 （標準は０個）
	nPhSkip = GetProfileInt("Settings","skip",0);
	// 項目取得数 （標準は１０個）
	nPhGetcount = GetProfileInt("Settings","getitems",10);
	// バックグラウンドカラー （標準はオレンジ）
	dlg->cBackColor = GetProfileInt("Settings","color_back",0x007ee5);
	// フォアグラウンドカラー （標準は黒）
	dlg->cForeColor = GetProfileInt("Settings","color_fore",0);
	// タイマー割り込み時間 （１１０ミリ秒）
	dlg->nTimer = GetProfileInt("Settings","timer",110);
	// 文字表示スピード （標準は６ドットずつ）
	dlg->nMoveSpeed = GetProfileInt("Settings","flow_speed",6);
	// ネットへのアクセス間隔（３００秒ごと）
	dlg->tInterval = GetProfileInt("Settings","interval",300);
	// タイトル文字列
	sTitle = GetProfileString("Settings","title","");
	// 証券データなどの指定に使うアイテム
	sItems = GetProfileString("Settings","item","");
	// 切り出し後の文字列から重複する空白文字を削除するモード
	nDelSpace = GetProfileInt("Settings","del_space",0);
	// 全データ１行化（改行→空白）モード
	nCr2Spc = GetProfileInt("Settings","cr2spc",0);

	// ダイアログのＸ座標
	dlg->nPosX = GetProfileInt("Settings","pos_x",100);
	// ダイアログのＹ座標
	dlg->nPosY = GetProfileInt("Settings","pos_y",100);
	// ダイアログの幅
	dlg->nInfoWndWidth = GetProfileInt("Settings","siz_x",0);
	// ダイアログの高さ
	dlg->nInfoWndHeight = GetProfileInt("Settings","siz_y",0);
	// 最前面に表示する
	dlg->bDispTopmost = GetProfileInt("Settings","topwin",1);

	// フォントのポイント数 （０でシステムデフォルト）
	dlg->nFontPoint = GetProfileInt("Settings","fnt_point",0);
	// フォント名
	dlg->sFontName = GetProfileString("Settings","fnt_name","");

	// インストールフラグの取得
	this->bInstalled = GetProfileInt("Settings","installed",0);


}

// **********************************
//  レジストリへ設定値を書き込む
//
// **********************************
void CInfoBar00App::RegConfigWrite(CInfoBar00Dlg *dlg)
{
	// モード
	if(nMode != (int)GetProfileInt("Settings","Mode",0))
		WriteProfileInt("Settings","Mode",nMode);

	// URL
	if(sURL != GetProfileString("Settings","URL","headlines.yahoo.co.jp/hl?c=pol&t=l"))
		WriteProfileString("Settings","URL",sURL);
	// ポート番号
	if(nPort != GetProfileInt("Settings","Port",80))
		WriteProfileInt("Settings","Port",nPort);
	// プロキシ・サーバ （標準ではなし：直接接続）
	if(sProxy != GetProfileString("Settings","Proxy",""))
		WriteProfileString("Settings","Proxy",sProxy);
	// 切り分けヘッダ
	if(sPhHeader != GetProfileString("Settings","Header","<li>"))
		WriteProfileString("Settings","Header",sPhHeader);
	// 切り分けヘッダ B
	if(sPhHeaderB != GetProfileString("Settings","HeaderB",""))
		WriteProfileString("Settings","HeaderB",sPhHeaderB);
	// 切り分けヘッダ C
	if(sPhHeaderC != GetProfileString("Settings","HeaderC",""))
		WriteProfileString("Settings","HeaderC",sPhHeaderC);
	// 先頭飛ばし個数 （標準は０個）
	if(nPhSkip != (int)GetProfileInt("Settings","skip",0))
		WriteProfileInt("Settings","skip",nPhSkip);
	// 項目取得数 （標準は１０個）
	if(nPhGetcount != (int)GetProfileInt("Settings","getitems",10))
		WriteProfileInt("Settings","getitems",nPhGetcount);
	// バックグラウンドカラー （標準はオレンジ）
	if(dlg->cBackColor != GetProfileInt("Settings","color_back",0x007ee5))
		WriteProfileInt("Settings","color_back",dlg->cBackColor);
	// フォアグラウンドカラー （標準は黒）
	if(dlg->cForeColor != GetProfileInt("Settings","color_fore",0))
		WriteProfileInt("Settings","color_fore",dlg->cForeColor);
	// タイマー割り込み時間 （１１０ミリ秒）
	if(dlg->nTimer != GetProfileInt("Settings","timer",110))
		WriteProfileInt("Settings","timer",dlg->nTimer);
	// 文字表示スピード （標準は６ドットずつ）
	if(dlg->nMoveSpeed != GetProfileInt("Settings","flow_speed",6))
		WriteProfileInt("Settings","flow_speed",dlg->nMoveSpeed);
	// ネットへのアクセス間隔（３００秒ごと）
	if(dlg->tInterval != (int)GetProfileInt("Settings","interval",300))
		WriteProfileInt("Settings","interval",dlg->tInterval);
	// タイトル文字列
	if(sTitle != GetProfileString("Settings","title",""))
		WriteProfileString("Settings","title",sTitle);
	// 証券データなどの指定に使うアイテム
	if(sItems != GetProfileString("Settings","item",""))
		WriteProfileString("Settings","item",sItems);
	// 切り出し後の文字列から重複する空白文字を削除するモード
	if(nDelSpace != (int)GetProfileInt("Settings","del_space",0))
		WriteProfileInt("Settings","del_space",nDelSpace);
	// 全データ１行化（改行→空白）モード
	if(nCr2Spc != (int)GetProfileInt("Settings","cr2spc",0))
		WriteProfileInt("Settings","cr2spc",nCr2Spc);

	// ダイアログのＸ座標
	if(dlg->nPosX != GetProfileInt("Settings","pos_x",100))
		WriteProfileInt("Settings","pos_x",dlg->nPosX);
	// ダイアログのＹ座標
	if(dlg->nPosY != GetProfileInt("Settings","pos_y",100))
		WriteProfileInt("Settings","pos_y",dlg->nPosY);
	// ダイアログの幅
	if(dlg->nInfoWndWidth != (int)GetProfileInt("Settings","siz_x",0))
		WriteProfileInt("Settings","siz_x",dlg->nInfoWndWidth);
	// ダイアログの高さ
	if(dlg->nInfoWndHeight != (int)GetProfileInt("Settings","siz_y",0))
		WriteProfileInt("Settings","siz_y",dlg->nInfoWndHeight);
	// 最前面に表示する
	if(dlg->bDispTopmost != (int)GetProfileInt("Settings","topwin",1))
		WriteProfileInt("Settings","topwin",dlg->bDispTopmost);

	// フォントのポイント数 （０でシステムデフォルト）
	if(dlg->nFontPoint != (int)GetProfileInt("Settings","fnt_point",0))
		WriteProfileInt("Settings","fnt_point",dlg->nFontPoint);
	// フォント名
	if(dlg->sFontName != GetProfileString("Settings","fnt_name",""))
		WriteProfileString("Settings","fnt_name",dlg->sFontName);


	// インストールフラグ
	if(GetProfileInt("Settings","installed",0) != 1)
	WriteProfileInt("Settings","installed",1);

}

// **********************************
//  インストール作業 一式
//
//  戻り値  TRUE:インストール完了または不要,  FALSE:ユーザキャンセル
// **********************************
BOOL CInfoBar00App::InstallSeq()
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
BOOL CInfoBar00App::UninstallSeq()
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

