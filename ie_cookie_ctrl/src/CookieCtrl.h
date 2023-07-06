// CookieCtrl.h : COOKIECTRL アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_COOKIECTRL_H__01212944_0903_11D5_AD9A_0090CCA1066A__INCLUDED_)
#define AFX_COOKIECTRL_H__01212944_0903_11D5_AD9A_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// グローバル変数
extern CString gstr_folder;		// Cookieフォルダ
extern CString gstr_ext;		// Cookie ファイル名
extern CString gstr_exclude;	// 除外するファイル文字列
extern int gint_days;			// 除外する期間
extern int gint_sw_exclude;		// 除外するファイル文字列のスイッチ
extern int gint_sw_days;		// 除外する期間のスイッチ
extern int gint_admin_inst;	// アドミニストレーター（最初のインストール者)
extern int gint_user_inst;		// 現在のユーザでインストールされているか




/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlApp:
// このクラスの動作の定義に関しては CookieCtrl.cpp ファイルを参照してください。
//

class CCookieCtrlApp : public CWinApp
{
public:
	BOOL UninstallSeq(void);
	BOOL InstallSeq(void);
	void RegWriteAll(void);
	void RegReadAll(void);
	CCookieCtrlApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCookieCtrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CCookieCtrlApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_COOKIECTRL_H__01212944_0903_11D5_AD9A_0090CCA1066A__INCLUDED_)
