
#ifndef __GLOBAL_FUNC
#define __GLOBAL_FUNC


extern char _sTransBuf[];		// スレッドとダイアログの通信用
extern char _sURL[];			// スレッドに渡す URL
extern char _sProxy[];			// スレッドに渡す PROXY
extern char _sPhHeader[];		// 切り分け用 ヘッダー文字列
extern char _sPhHeaderB[];			// 切り分け用 ヘッダー文字列 B
extern char _sPhHeaderC[];			// 切り分け用 ヘッダー文字列 C
extern char _sTitle[];				// タイトル
extern char _sItems[];				// 証券データの指定などに使うアイテム

extern BOOL bUpdated;		// スレッドで更新が完了した通知
extern BOOL bInThread;		// スレッド動作中のフラグ
extern int nThreadError;	// スレッドで起こったエラーの種類を返す
extern int _nMode;			// 動作モード（通常受信・証券受信…）
extern UINT _nPort;			// スレッドに渡す ポート番号
extern int _nPhSkip;		// 切り分け用 スキップ個数
extern int _nPhGetcount;	// 切り分け用 取得数
extern int _nDelSpace;		// 重複空白文字の削除モード		2003/06/01
extern int _nCr2Spc;		// 全データ１行化（改行→空白）モード		2003/06/02

extern BOOL bDebugMode;		// デバッグモード


void thread_main(void *pVoid);	// スレッド関数

#endif
