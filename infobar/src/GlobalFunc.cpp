
#include "stdafx.h"

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "resource.h"

#include <mbstring.h>		// ************** JIS コード変換用

//********************
// グローバル変数
// 外部との変数受け渡し用
//********************
volatile BOOL bUpdated;		// スレッドで更新が完了した通知
volatile BOOL bInThread;	// スレッド動作中のフラグ
volatile int nThreadError;	// スレッドで起こったエラーの種類を返す
CString sTransBuf;			// スレッドとダイアログの通信用
CString sURL;				// スレッドに渡す URL
CString sProxy;				// スレッドに渡す PROXY
volatile UINT nPort;		// スレッドに渡す ポート番号
CString sPhHeader;			// 切り分け用 ヘッダー文字列
volatile int nPhSkip;		// 切り分け用 スキップ個数
volatile int nPhGetcount;	// 切り分け用 取得数
CString sTitle;				// タイトル


//********************
// このファイルの中だけで使う変数
// (スレッド中で宣言すると、バッファオーバーフローするため）
//********************

CString		_sTmp_trd, _sTmp2_trd;
CTime		_tm_trd;
//********************

#define		RECVSIZE	5000		// 一度に受信できる最大サイズ


//********************
// スレッド関数
// (通信部分を担う）
//********************
void thread_main(void *pVoid)
{
	int			i,j;
	char		*psz,*pDest;
	char		pszBuf[RECVSIZE+1500];	// 受信バッファ (RWIN の分だけ安全のため加えておく）
	char		szURL[1024];			// URL (与えられたURLをコピーしてここに格納）
	char		szHostName[1024];		// 与えられたＵＲＬを分解して設定
	char		szConnHostName[1024];	// 接続するホスト名（プロキシorダイレクト）
	char		szHtmlName[1024];		// 与えられたＵＲＬを分解して設定
	char		szCmd[1024];
	SOCKET		sock = INVALID_SOCKET;
	LPCSTR		pszUserAgent = "Mozilla/4.0";		// User-Agent
	DWORD		dwTotalSize;
	LPHOSTENT	m_pHostEnt;
	SOCKADDR_IN	sai;					// inet アドレス
	in_addr		iadr;					// inet アドレス

//	WSADATA _sock_wsa;

	// スレッドの重複起動は避ける
	if(bInThread == TRUE)
		_endthread();

	//***********************
	// 初期値の設定
	//***********************
	bInThread = TRUE;			// スレッド動作中
	bUpdated = FALSE;			// 更新が終わっていないことを示す
	_sTmp_trd = "";				// 受信した HTML を一時格納
	dwTotalSize = 0L;			// ダウンロードしたデータのサイズ

	//***********************
	// EUC -> SJIS 変換用
	//***********************
	BOOL		isSJIS = FALSE;			// Shift JIS エンコード時に TRUE
	BOOL		bInEuc = FALSE;
	unsigned int nEucCode;
	//***********************
	



	// ====================================================================
	// ソケット作成
	// ====================================================================
	if((sock=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
	{	// ソケット作成不能
		sTransBuf.LoadString(IDS_ERR_SOCK_CREAT);	// ネットワーク・エラー：Windowsソケットの作成失敗\n
		//****************
		// スレッド中断処理
		bUpdated = TRUE;		// 情報更新
		bInThread = FALSE;
		_endthread();
		//****************
	}
	// ====================================================================
	// URLの先頭に "http://"がついていた場合は削除する
	// ====================================================================
	if(!strncmp((LPCSTR)sURL, "http://", 7))
	{
		_sTmp_trd = sURL.Mid(7, sURL.GetLength()-7);
		sURL = _sTmp_trd;
	}

	// ====================================================================
	// URLの分解
	// ====================================================================

	memset(szHostName,'\0',sizeof(szHostName));
	memset(szHtmlName,'\0',sizeof(szHtmlName));

	strcpy(szURL, sURL);
	psz=szURL;

	// ホスト名のみを取り出す www.yahoo.co.jp
	pDest=szHostName;
	while(*psz)
	{
		if(*psz=='/')
			break;
		*pDest=*psz;
		pDest++;
		psz++;
	}
	if(sProxy != "")
	{	// プロキシありのとき
		strcpy(szConnHostName, sProxy);
	}
	else
	{	// プロキシ無しのとき
		strcpy(szConnHostName, szHostName);
	}

	// フォルダ名のみを取り出す /index.html
	pDest=szHtmlName;
	if(*psz)
	{
		while(*psz)
		{
			*pDest=*psz;
			pDest++;
			psz++;
		}
	}
	if(szHtmlName[0]=='\0')
		lstrcpy(szHtmlName,"/");	// 先頭に スラッシュをつける

	if(sProxy != "")
	{	// プロキシありのときは、取得するHTML名を取得アドレス全てとする
		lstrcpy(szHtmlName, "http://");
		lstrcat(szHtmlName,sURL);
	}

	// ====================================================================
	// ホスト解決
	// ====================================================================

	iadr.s_addr=inet_addr(szConnHostName);

	if(iadr.s_addr==INADDR_NONE)
		m_pHostEnt=gethostbyname(szConnHostName);
	else
		m_pHostEnt=gethostbyaddr((const char *)&iadr,sizeof(iadr),AF_INET);

	if(!m_pHostEnt)
	{	// ホストを検出できません
		sTransBuf.LoadString(IDS_ERR_SOCK_GETHOST);		// ネットワーク・エラー：Windowsソケットでホスト名の解決ができません\n
		//****************
		// スレッド中断処理
		bUpdated = TRUE;		// 情報更新
		bInThread = FALSE;
		shutdown(sock,0x2);
		closesocket(sock);
		_endthread();
		//****************
	}

	// ====================================================================
	// 接続
	// ====================================================================

	sai.sin_port=htons((u_short)nPort);
	sai.sin_family=AF_INET;
	sai.sin_addr=*((LPIN_ADDR)*m_pHostEnt->h_addr_list);

	if(connect(sock,(LPSOCKADDR)&sai,sizeof(sai)))
	{	// 接続できませんでした
		sTransBuf.LoadString(IDS_ERR_SOCK_CONN);	// ネットワーク・エラー：Windowsソケットの接続に失敗しました\n
		//****************
		// スレッド中断処理
		bUpdated = TRUE;		// 情報更新
		bInThread = FALSE;
		shutdown(sock,0x2);
		closesocket(sock);
		_endthread();
		//****************
	}

	// ====================================================================
	// リクエスト （送信）
	// ====================================================================

	if(sProxy=="")
	{	// ダイレクト接続の場合
		wsprintf(
			szCmd,
			"GET %s HTTP/1.0\r\n"
			"Host: %s:%d\r\n"
			"User-Agent: %s\r\n\r\n",
//			"Connection: Keep-Alive\r\n\r\n",
			szHtmlName,szHostName,nPort,pszUserAgent);
	}
	else
	{	// プロキシ経由接続の場合
	wsprintf(
			szCmd,
			"GET %s HTTP/1.0\r\n"
			"Host: %s:%d\r\n"
			"User-Agent: %s\r\n\r\n",
			//"Proxy-Connection: Keep-Alive\r\n\r\n",
			szHtmlName,szHostName,80,pszUserAgent);
	}
	psz=szCmd;

	while(i < lstrlen(psz))
	{
		i = send(sock, psz, lstrlen(psz), 0);	// 送信
		if(i == SOCKET_ERROR)
		{
			sTransBuf.LoadString(IDS_ERR_SOCK_CONN);	// ネットワーク・エラー：Windowsソケットの接続に失敗しました\n
			//****************
			// スレッド中断処理
			bInThread = FALSE;
			bUpdated = TRUE;		// 情報更新
			shutdown(sock,0x2);
			closesocket(sock);
			_endthread();
			//****************
		}
		// 送信バイト数が i に格納される。
		psz += i;
	}

	// ====================================================================
	// 受信
	// ====================================================================

	for(;;)
	{
		i = recv(sock, pszBuf, RECVSIZE, 0);		// 受信
		// 受信バイト数が i に格納される
		if(i<=0 || i==SOCKET_ERROR) break;		// 受信終了

		pszBuf[i] = (char)NULL;		// 文字列として完成させるため、末尾に NULL 追加

		_sTmp_trd += (LPCSTR)pszBuf;

	}
	dwTotalSize = _sTmp_trd.GetLength();

	// ====================================================================
	// 通信終了
	// ====================================================================
	if(shutdown(sock,0x2))
	{
		_sTmp2_trd.LoadString(IDS_ERR_SOCK_SHUT);		// ネットワーク・エラー：Windowsソケットの解放に失敗\n
		sTransBuf = _sTmp2_trd + sTransBuf;
	}
	if(sock!=INVALID_SOCKET)
	{
		if(closesocket(sock))
		{
			_sTmp2_trd.LoadString(IDS_ERR_SOCK_CLOSE);	// ネットワーク・エラー：Windowsソケットの解放に失敗\n
			sTransBuf = _sTmp2_trd + sTransBuf;
		}
	}

	//***********************
	// META SJIS  判定
	//***********************
	for(i=0,j=0;;)
	{
		i=_sTmp_trd.Find("<META", j);
		if(i<0)
		{
			i=_sTmp_trd.Find("<meta", j);
			if(i<0) break;
		}
		j=_sTmp_trd.Find(">", i+1);
		if(j<0) break;
		_sTmp2_trd = _sTmp_trd.Mid(i+1, j-i);
		if(_sTmp2_trd.Find("SHIFT",0) || _sTmp2_trd.Find("shift",0) || _sTmp2_trd.Find("Shift",0) ||
					_sTmp2_trd.Find("sjis",0) || _sTmp2_trd.Find("Sjis",0) || _sTmp2_trd.Find("SJIS",0))
		{
			isSJIS = TRUE;
			break;
		}
	}		


	//***********************
	// EUC -> SJIS 変換用
	//***********************
	if(!isSJIS)
	{
		for(i=0; i<(int)dwTotalSize; i++)
		{
			if((_sTmp_trd.GetAt(i) >= (char)0xa1) && (_sTmp_trd.GetAt(i) <= (char)0xfe))
			{
				if(i<(int)_sTmp_trd.GetLength()-1)
				{	// 後ろに一文字ある場合
					nEucCode = (0xff00 & (_sTmp_trd.GetAt(i) << 8)) | (0xff & _sTmp_trd.GetAt(i+1));
					nEucCode &= 0x7f7f;
					nEucCode = _mbcjistojms(nEucCode);
					if(nEucCode != 0)
					{
						_sTmp_trd.SetAt(i, (char)((nEucCode & 0xff00) >> 8));
						_sTmp_trd.SetAt(i+1, (char)(nEucCode & 0xff));
					}
					i++;

				}
				else
				{	// 最後の文字の場合
				}
			}
		}
	}


	int _nSkip=0;		// 先頭スキップ数のカウント
	int _nGetCt=0;		// 取得項目数のカウント
	sTransBuf = "";

	if(_sTmp_trd != "")
	{
		//**************************************
		// 検索抽出
		//**************************************
		for(i=0,j=0 ; _nGetCt < nPhGetcount ; _nSkip++)
		{
			i = _sTmp_trd.Find(sPhHeader,j);	// 区切文字列を検索
			if(i<0) break;
			i += sPhHeader.GetLength();		// 区切文字列文だけインデックスを進める
			for(;;)
			{
				if(_sTmp_trd[i+1] == '<') i = _sTmp_trd.Find(">", i+1);		// 直後に <...> が続いている場合
				else if(_sTmp_trd.Find("<", i+1) > _sTmp_trd.Find(">", i+1))
				{	// 区切文字列が <...>の内部にあるとき
					i = _sTmp_trd.Find(">", i+1);
				}
				else break;	// 平文に差し掛かると、ループ抜ける
			}
			if(i<0) break;
			j = _sTmp_trd.Find("<", i+1);	// 次の <...>の始まりを見つける
			if(j<0) break;
			if(nPhSkip > _nSkip) continue;
			sTransBuf += _sTmp_trd.Mid(i+1, j-i-1);
			sTransBuf += "\n";
			_nGetCt++;
		}

		bUpdated = TRUE;		// 情報更新
	}

	//**************************************
	// タイトル付け
	//**************************************
	if(sTransBuf != "" && sTitle != "")
	{
		_tm_trd = CTime::GetCurrentTime();
		_sTmp_trd.LoadString(IDS_STR_TITLEFMT);		// %s     %2d月%2d日%02d時%02d分 現在\n
		_sTmp2_trd.Format(_sTmp_trd, sTitle, _tm_trd.GetMonth(), _tm_trd.GetDay(), _tm_trd.GetHour(), _tm_trd.GetMinute());
		sTransBuf = _sTmp2_trd + sTransBuf;
	}

	bInThread = FALSE;		// スレッド動作中

	_endthread();
}
