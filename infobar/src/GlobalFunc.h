


extern volatile BOOL bUpdated;			// スレッドで更新が完了した通知
extern volatile BOOL bInThread;			// スレッド動作中のフラグ
extern volatile int nThreadError;		// スレッドで起こったエラーの種類を返す
extern CString sTransBuf;		// スレッドとダイアログの通信用
extern CString sURL;			// スレッドに渡す URL
extern CString sProxy;			// スレッドに渡す PROXY
extern volatile UINT nPort;				// スレッドに渡す ポート番号
extern CString sPhHeader;		// 切り分け用 ヘッダー文字列
extern volatile int nPhSkip;	// 切り分け用 スキップ個数
extern volatile int nPhGetcount;	// 切り分け用 取得数
extern CString sTitle;				// タイトル


void thread_main(void *pVoid);	// スレッド関数
