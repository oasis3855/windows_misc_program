#if !defined(AFX_NETACCESSTHREAD_H__A53AE552_DEBC_4CCE_B3CE_937A7D4450E4__INCLUDED_)
#define AFX_NETACCESSTHREAD_H__A53AE552_DEBC_4CCE_B3CE_937A7D4450E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetAccessThread.h : ヘッダー ファイル
//



/////////////////////////////////////////////////////////////////////////////
// CNetAccessThread スレッド

class CNetAccessThread : public CWinThread
{
	DECLARE_DYNCREATE(CNetAccessThread)
protected:
public:
	CNetAccessThread();           // 動的生成に使用されるプロテクト コンストラクタ

// アトリビュート
protected:
//	CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
//	CString		_sTmp_MidBuf;	// sTransBuf に渡す最終結果格納用
//	CString		_sPhHeaderCur;	// 現在の区切り文字列
//	CTime		_tm_trd;

public:
/*
BOOL *bUpdated;		// スレッドで更新が完了した通知
BOOL *bInThread;	// スレッド動作中のフラグ
int nMode;			// 動作モード（通常受信・証券受信…）
//int nThreadError;	// スレッドで起こったエラーの種類を返す
CString *sTransBuf;			// スレッドとダイアログの通信用
CString sURL;				// スレッドに渡す URL
CString sProxy;				// スレッドに渡す PROXY
UINT nPort;		// スレッドに渡す ポート番号
CString sPhHeader;			// 切り分け用 ヘッダー文字列
CString sPhHeaderB;			// 切り分け用 ヘッダー文字列 B
CString sPhHeaderC;			// 切り分け用 ヘッダー文字列 C
int nPhSkip;		// 切り分け用 スキップ個数
int nPhGetcount;	// 切り分け用 取得数
CString sTitle;				// タイトル
CString sItems;				// 証券データの指定などに使うアイテム

char _sURL[2048];
char _sProxy[1024];
char _sPhHeader[1024];
char _sPhHeaderB[1024];
char _sPhHeaderC[1024];
char _sTitle[1024];
char _sItems[2048];
char *_sTransBuf;
*/


// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNetAccessThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
public:
	BOOL receive_HTML(CString *sRURL, CString *sHTML);
	void Receive_Mode_01(void);
	virtual ~CNetAccessThread();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CNetAccessThread)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NETACCESSTHREAD_H__A53AE552_DEBC_4CCE_B3CE_937A7D4450E4__INCLUDED_)
