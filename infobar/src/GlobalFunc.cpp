
#include "stdafx.h"

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <time.h>
//#include "resource.h"

#include "GlobalFunc.h"

#include <mbstring.h>		// ************** JIS コード変換用


//********************
// 文字列バッファのサイズ決定用
//********************
#define		RECVSIZE	5000		// 一度に受信できる最大サイズ
#define		URLSIZE		2048		// URL のサイズ
#define		TITLESIZE	1024		// タイトル文字列のサイズ
#define		ITEMSIZE	2048		// 証券データのコードリストのサイズ
#define		BUFSIZE		100*1024	// ＨＴＭＬ受信バッファのサイズ (100kbytes)
#define		TMPSIZE		2048		// 文字列取り出し用の一時バッファサイズ

#define		PADSIZE		128			// バッファがオーバーフローしないためのパディングサイズ

/*
 * WinSock 2 extension -- manifest constants for shutdown()
 */
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02


//********************
// グローバル変数
// 外部との変数受け渡し用
//********************
char _sTransBuf[BUFSIZE + PADSIZE];		// スレッドとダイアログの通信用
char _sURL[URLSIZE + PADSIZE];			// スレッドに渡す URL
char _sProxy[URLSIZE + PADSIZE];		// スレッドに渡す PROXY
char _sPhHeader[URLSIZE + PADSIZE];		// 切り分け用 ヘッダー文字列
char _sPhHeaderB[URLSIZE + PADSIZE];	// 切り分け用 ヘッダー文字列 B
char _sPhHeaderC[URLSIZE + PADSIZE];	// 切り分け用 ヘッダー文字列 C
char _sTitle[TITLESIZE + PADSIZE];		// タイトル
char _sItems[ITEMSIZE + PADSIZE];		// 証券データの指定などに使うアイテム

BOOL bUpdated;		// スレッドで更新が完了した通知
BOOL bInThread;		// スレッド動作中のフラグ
int nThreadError;	// スレッドで起こったエラーの種類を返す
int _nMode;			// 動作モード（通常受信・証券受信…）
UINT _nPort;		// スレッドに渡す ポート番号
int _nPhSkip;		// 切り分け用 スキップ個数
int _nPhGetcount;	// 切り分け用 取得数

BOOL bDebugMode;		// デバッグモード


//********************
// 内部のみで使う、関数群の宣言
//********************
int _inet_norm_process(char *sCompose, char *sHTML, char *szTemp, FILE **fo);
int _inet_stock_process(char *sCompose, char *sHTML, char *szTemp, FILE **fo);

BOOL receive_HTML(char *sRURL, char *sHTML, char *szTemp, FILE **fo);		// HTMLファイルの受信
int _str_find(char *szOrg, char *szKey, int seek);
void _conv_euc_to_sjis(char *buf);
void _add_title(char *sReturn, char *sHTML, char *sTitle, char *szTemp);


void _dbg_openfile(FILE **fo, char *szTemp);
void _dbg_write_init(FILE **fo, char *szTemp);
void _dbg_write_formated(FILE **fo, char *szBuf1, char *szBuf2);
void _dbg_write_formated_int(FILE **fo, char *szBuf1, int x);
void _dbg_bufferovrn_check(FILE **fo, char *szStr, int _sLen0, int _sPad, char *szTitle);

//********************
// スレッド関数
// (通信部分を担う）
//********************
void thread_main(void *pVoid)
{
	char		sHTML[BUFSIZE + PADSIZE];			// HTML 受信バッファ
	char		sCompose[BUFSIZE + PADSIZE];		// 解析後の文字列（一時保管）
	char		szTemp[TMPSIZE + PADSIZE];			// 一時バッファ
	FILE		*fo;

	// スレッドの重複起動は避ける
	if(bInThread == TRUE)
		_endthread();


	//***********************
	// 初期値の設定
	//***********************
	bInThread = TRUE;			// スレッド動作中
	bUpdated = FALSE;			// 更新が終わっていないことを示す

	memset(sHTML, NULL, BUFSIZE+PADSIZE);
	memset(sCompose, NULL, BUFSIZE+PADSIZE);
	memset(szTemp, NULL, TMPSIZE+PADSIZE);

	strcpy(sHTML, "");
	strcpy(sCompose, "");


	if(strlen(_sPhHeader) == 0)
	{
		strcpy(_sTransBuf, "Error : Pharse Key is NULL\n");
		bUpdated = TRUE;
		bInThread = FALSE;		// スレッド動作中
		_endthread();
	}

	if(bDebugMode)
	{
		fo = NULL;		// ファイルポインタに NULL を割り当てておく
		_dbg_openfile(&fo, szTemp);
		_dbg_write_init(&fo, szTemp);
	}

	if(strlen(_sURL) == 0)
	{
		strcpy(_sTransBuf, "Error : URL is NULL\n");
		bUpdated = TRUE;
		bInThread = FALSE;		// スレッド動作中
		if(bDebugMode && fo != NULL) fclose(fo);
		_endthread();
	}

	//***********************
	// タスクによって振り分け
	//***********************
	if(_nMode == 1)
	{	// 証券データ受信

		if(strlen(_sPhHeaderB) == 0 || strlen(_sPhHeaderC) == 0)
		{
			strcpy(_sTransBuf, "Error : Pharse Key (B,C) is NULL\n");
			bUpdated = TRUE;
			bInThread = FALSE;		// スレッド動作中
			if(bDebugMode && fo != NULL) fclose(fo);
			_endthread();
		}

		_inet_stock_process(sCompose, sHTML, szTemp, &fo);
	}
	else
	{	// 通常受信
		_inet_norm_process(sCompose, sHTML, szTemp, &fo);
	}

	if(bDebugMode)
	{
		_dbg_write_formated(&fo, "sCompose", sCompose);
	}

	if(strlen(_sTransBuf) != 0)
		bUpdated = TRUE;
	else
		bUpdated = FALSE;

	bInThread = FALSE;		// スレッド動作中

	//***********************
	// スレッドの終了
	//***********************
	if(bDebugMode)
	{
		_dbg_bufferovrn_check(&fo, sHTML, BUFSIZE, PADSIZE, "buffer over run check (sHTML)");
		_dbg_bufferovrn_check(&fo, sCompose, BUFSIZE, PADSIZE, "buffer over run check (sCompose)");
		_dbg_bufferovrn_check(&fo, szTemp, TMPSIZE, PADSIZE, "buffer over run check (szTemp)");
		_dbg_write_formated(&fo, "Thread Terminate", "debug file is closed");
		fclose(fo);
		bDebugMode = FALSE;
	}
	
	_endthread();
}


int _inet_norm_process(char *sCompose, char *sHTML, char *szTemp, FILE **fo)
{
	int			i ,j, k;			// 一時カウンタ
	int			_nSkip	= 0;		// 先頭スキップ数のカウント
	int			_nGetCt	= 0;		// 取得項目数のカウント
	BOOL		bSockResult;		// 

	//***********************
	// インターネットから受信
	//***********************
	// sURL のアドレスの HTML を受信し、 sHTML に内容を格納
	bSockResult = receive_HTML(_sURL, sHTML, szTemp, fo);

	//***********************
	// EUC を ShiftJIS に変換する
	//***********************
	_conv_euc_to_sjis(sHTML);

	if(bDebugMode)
	{
		_dbg_write_formated(fo, "sHTML", sHTML);
	}


	strcpy(sCompose, "");

	if(bSockResult)
	{	// 受信成功の場合
		if(strlen(sHTML) != 0)
		{
			//**************************************
			// 検索抽出
			//**************************************
			for(i=0,j=0 ; _nGetCt < _nPhGetcount ; _nSkip++)
			{
				i = _str_find(sHTML, _sPhHeader, j);	// 区切文字列を検索
				if(i<0) break;
				i += strlen(_sPhHeader);		// 区切文字列文だけインデックスを進める
				for(;;)
				{
					if(sHTML[i] == '<')
					{	// 直後に <...> が続いている場合
						// 次の ">" へ
						i = _str_find(sHTML, ">", i) + 1;
					}
					else if(_str_find(sHTML, "<", i) > _str_find(sHTML, ">", i))
					{	// 区切文字列が <...>の内部にあるとき
						// 次の ">" へ
						i = _str_find(sHTML, ">", i) + 1;
					}
					else break;	// 平文に差し掛かると、ループ抜ける
				}
				if(i<0) break;
				j = _str_find(sHTML, "<", i+1);	// 次の <...>の始まりを見つける
				if(j<0) break;
				if(_nPhSkip > _nSkip) continue;

				//**************************************
				// 抽出
				//**************************************
				strncpy(szTemp, sHTML + i, j-i < TMPSIZE ? j-i : TMPSIZE);
				szTemp[j-i < TMPSIZE ? j-i : TMPSIZE] = (char)NULL;		// 文末の NULL を付加

				if(strlen(szTemp) == 0 || !strcmp(szTemp, "\n")) continue;	// 空文字列スキップ
				strcat(sCompose, szTemp);
				if(szTemp[strlen(szTemp)] != '\n')
					strcat(sCompose, "\n");		// 最終文字が 改行 でない時は、改行をつける
				_nGetCt++;

			}

		}
		else
		{	// 受信バッファが空の場合
			strcat(sCompose, "Error : No HTML File\n");
		}
	}
	else
	{	// 受信失敗の場合 (bSockResult == FALSE)
		strcat(sCompose, "Network Error : ");
		strcat(sCompose, sHTML);
		strcat(sCompose, "\n");
	}

	if(strlen(sCompose) == 0)
	{	// ＨＴＭＬは受信されたが、データが取り出せなかった場合
		strcpy(sCompose, "Error : HTML received, but No target data (debug:head of HTML follows ...)\n");
		// 先頭５行をプリント
		for(i=0, j=0, k=0; i<5; i++)
		{
			k = _str_find(sHTML, "\n", j);
			if(k < 0) break;
			strncpy(szTemp, sHTML + j, k-j<TMPSIZE ? k-j : TMPSIZE-1);
			szTemp[k-j] = (char)NULL;
			strcat(sCompose, szTemp);
			strcat(sCompose, "\n");
			j = k+1;	// "\n" の次から始める
			if(j >= (int)strlen(sHTML)) break;
		}
	}

	//**************************************
	// タイトル付け
	//**************************************
	_add_title(_sTransBuf, sCompose, _sTitle, szTemp);

	return 0;
}

int _inet_stock_process(char *sCompose, char *sHTML, char *szTemp, FILE **fo)
{
	int			i, j, _i, _j;		// 一時カウンタ
	int			_nSkip	= 0;		// 先頭スキップ数のカウント
	int			_nGetCt	= 0;		// 取得項目数のカウント
	char		sComposeURL[URLSIZE + PADSIZE];		// コードを代入したURLの作成用
	BOOL		bSockResult;		// 

	strcpy(sCompose, "");

	for(_i=0,_j=0; ; )
	{
		_j = _str_find(_sItems, "\n", _i);
		if(_j<0)
			_j = strlen(_sItems) + 1;		// 次の行がない時（最後のNULLを指す)
		strncpy(szTemp, _sItems + _i, _j-_i-1);
		szTemp[_j-_i-1] = (char)NULL;

		if(strlen(szTemp) == 0 || !strcmp(szTemp, "\n"))
		{
			_i = _j+1;							// 改行の次の文字
			if(_j >= (int)strlen(_sItems)) break;	// 最終行を越えた
			continue;
		}

		// sURL の "%s" に szTemp のコードを挿入する
		sprintf(sComposeURL, _sURL, szTemp);

		//***********************
		// インターネットから受信
		//***********************
		// sURL のアドレスの HTML を受信し、 sHTML に内容を格納
		bSockResult = receive_HTML(sComposeURL, sHTML, szTemp, fo);
		//***********************
		// EUC を ShiftJIS に変換する
		//***********************
		_conv_euc_to_sjis(sHTML);

		if(bDebugMode)
		{
			_dbg_write_formated(fo, "sHTML", sHTML);
		}

		if(bSockResult)
		{	// 受信成功の場合
			if(strlen(sHTML) != 0)
			{
				//**************************************
				// 検索抽出
				//**************************************
				strcpy(szTemp, _sPhHeader);	// 最初の区切り文字
				_nGetCt = 0;
				for(i=0,j=0 ; _nGetCt < _nPhGetcount ; _nSkip++)
				{
					i = _str_find(sHTML, szTemp, j);		// 区切文字列を検索
					if(i<0) break;
					i += strlen(szTemp);		// 区切文字列文だけインデックスを進める
					for(;;)
					{
						if(sHTML[i] == '<')
						{	// 直後に <...> が続いている場合
							// 次の ">" へ
							i = _str_find(sHTML, ">", i) + 1;
						}
						else if(_str_find(sHTML, "<", i) > _str_find(sHTML, ">", i))
						{	// 区切文字列が <...>の内部にあるとき
							// 次の ">" へ
							i = _str_find(sHTML, ">", i) + 1;
						}
						else break;	// 平文に差し掛かると、ループ抜ける
					}
					if(i<0) break;
					j = _str_find(sHTML, "<", i+1);	// 次の <...>の始まりを見つける
					if(j<0) break;
					if(_nPhSkip > _nSkip) continue;

					//**************************************
					// 抽出
					//**************************************
					strncpy(szTemp, sHTML + i, j-i < TMPSIZE ? j-i : TMPSIZE);
					szTemp[j-i < TMPSIZE ? j-i : TMPSIZE] = (char)NULL;		// 文末の NULL を付加

					if(strlen(szTemp) == 0 || !strcmp(szTemp, "\n")) continue;	// 空文字列スキップ
					if(szTemp[strlen(szTemp)-1] == (char)"\n")
					{	// 行末の改行を削除する
						szTemp[strlen(szTemp)-1] = (char)NULL;
					}
					strcat(szTemp, "  ");	// 空白をひとつ付ける
					strcat(sCompose, szTemp);

					_nGetCt++;

					if(_nGetCt == 1)
						strcpy(szTemp, _sPhHeaderB);	// 2個目の区切り文字
					else if(_nGetCt == 2)
						strcpy(szTemp, _sPhHeaderC);	// 2個目の区切り文字
					else
						break;
				}

			}
			else
			{	// 証券情報（１項目）の受信に失敗
				strncpy(szTemp, _sItems + _i, _j-_i-1);
				szTemp[_j-_i-1] = (char)NULL;

				strcat(sCompose, "No HTML File : ");
				strcat(sCompose, szTemp);
			}

			if(_nGetCt <= 2)
			{	// 証券コードが間違っていた場合（規定の解析文字が現れなかった場合）
				strncpy(szTemp, _sItems + _i, _j-_i-1);
				szTemp[_j-_i-1] = (char)NULL;

				strcat(sCompose, "Wrong Code : ");
				strcat(sCompose, szTemp);
			}
		}
		else
		{	// 受信失敗の場合 (bSockResult == FALSE)
			strncpy(szTemp, _sItems + _i, _j-_i-1);
			szTemp[_j-_i-1] = (char)NULL;

			strcat(sCompose, "Network Error : ");
			strcat(sCompose, szTemp);
			strcat(sCompose, " : ");
			strcat(sCompose, sHTML);

		}

		// 改行をつける （１行終了）
		if(strlen(sCompose) != 0)
		{
			if(sCompose[strlen(sCompose)-1] != (char)'\n')
				strcat(sCompose, "\n");		// 最終文字が 改行 でない時は、改行をつける
		}

		_i = _j+1;							// 改行の次の文字
		if(_j >= (int)strlen(_sItems)) break;	// 最終行を越えた
	}

	//**************************************
	// タイトル付け
	//**************************************
	_add_title(_sTransBuf, sCompose, _sTitle, szTemp);

	return 0;

}


//********************
// 指定したＨＴＭＬを受信する
// 
//********************
BOOL receive_HTML(char *sRURL, char *sHTML, char *szTemp, FILE **fo)
{
	int			i, j;
	char		*psz;
	char		szBuf[RECVSIZE+1500];				// 受信バッファ (RWIN の分だけ安全のため加えておく）
	char		szHostName[URLSIZE + PADSIZE];		// 与えられたＵＲＬを分解して設定
	char		szConnHostName[URLSIZE + PADSIZE];	// 接続するホスト名（プロキシorダイレクト）
	char		szHtmlName[URLSIZE + PADSIZE];		// 与えられたＵＲＬを分解して設定
	SOCKET		sock = INVALID_SOCKET;
	LPCSTR		pszUserAgent = "Mozilla/4.0";		// User-Agent
	DWORD		dwTotalSize;
	LPHOSTENT	m_pHostEnt;
	SOCKADDR_IN	sai;					// inet アドレス
	in_addr		iadr;					// inet アドレス
	WSADATA _sock_wsa;

	//***********************
	// 初期値の設定
	//***********************
	dwTotalSize = 0L;			// ダウンロードしたデータのサイズ
	memset(szBuf, NULL, RECVSIZE+1500);
	memset(szHostName, NULL, URLSIZE + PADSIZE);
	memset(szConnHostName, NULL, URLSIZE + PADSIZE);
	memset(szHtmlName, NULL, URLSIZE + PADSIZE);

	//***********************
	// EUC -> SJIS 変換用
	//***********************
	BOOL		isSJIS = FALSE;			// Shift JIS エンコード時に TRUE
	BOOL		bInEuc = FALSE;
	//***********************

	// ====================================================================
	// ソケット作成
	// ====================================================================
	// バージョン 1.1 を要求する 
	// Ver 1.1 : Win95 , Ver 2.2 : Win98SE , 2000
	WORD wVersionRequested = MAKEWORD( 1, 1 );
	if(::WSAStartup( wVersionRequested, &_sock_wsa ))
	{
		sprintf(sHTML, "WSAStartup (error code : %d)", WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_CREAT);	// ネットワーク・エラー：Windowsソケットの作成失敗\n
		return FALSE;
	}
	if(bDebugMode)
	{
		sprintf(szTemp, "winsock required 0x%04X , max supported 0x%04X ,\r\ndesc :%s\r\nstat :%s", _sock_wsa.wVersion, _sock_wsa.wHighVersion, _sock_wsa.szDescription, _sock_wsa.szSystemStatus);
		_dbg_write_formated(fo, "WSAStartup", szTemp);
	}

	if(!::AfxSocketInit(NULL))	// WinSock の初期化
	{
		strcpy(sHTML, "AfxSocketInit (error code : ---)");
//		sHTML->LoadString(IDS_ERR_SOCK_CREAT);	// ネットワーク・エラー：Windowsソケットの作成失敗\n
		return FALSE;
	}
	if(bDebugMode)
	{
		_dbg_write_formated(fo, "AfxSocketInit", "");
	}

	if((sock=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
	{	// ソケット作成不能
		sprintf(sHTML, "socket create (error code : %d)", WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_CREAT);	// ネットワーク・エラー：Windowsソケットの作成失敗\n
		::WSACleanup();
		return FALSE;
	}
	if(bDebugMode)
	{
		sprintf(szTemp, "socket # :0x%X", (UINT)sock);
		_dbg_write_formated(fo, "socket (create)", szTemp);
	}

	// ====================================================================
	// URLの先頭に "http://"がついていた場合は削除する
	// ====================================================================
	if(!strncmp(sRURL, "http://", 7))
	{
		strncpy(szTemp, sRURL+7, strlen(sRURL)-7);
		szTemp[strlen(sRURL)-7] = (char)NULL;
		strcpy(sRURL, szTemp);
	}

	// ====================================================================
	// URLの分解
	// ====================================================================
	// aaa.bbb.ccc/xxx/yyy.htm のとき
	// szConnHostName = aaa.bbb.ccc
	// szHtmlName = /xxx/yyy.htm
	//
	i = _str_find(sRURL, "/", 0);
	if(i >0)
	{	// aaa.bbb.ccc/xxx の形のとき
		strncpy(szConnHostName, sRURL, i);
		szConnHostName[i] = (char)NULL;
		strncpy(szHtmlName, sRURL + i, strlen(sRURL)-i+1);		// スラッシュも含める
		szHtmlName[strlen(sRURL)-i+1] = (char)NULL;
	}
	else
	{	// aaa.bbb.ccc の形のとき
		strcpy(szConnHostName, sRURL);
		strcpy(szHtmlName, "/");			// スラッシュのみ
	}

	strcpy(szHostName, szConnHostName);		// HTTP - GET で使用するため

	// プロキシサーバを通すときのための処理
	if(strlen(_sProxy) != 0)
	{	// プロキシあり
		strcpy(szConnHostName, _sProxy);
		sprintf(szHtmlName, "http://%s", sRURL);
	}
	else
	{	// プロキシ無し
	}


	// ====================================================================
	// ホスト解決
	// ====================================================================

//	iadr.s_addr=inet_addr(szConnHostName);
	iadr.S_un.S_addr=inet_addr(szConnHostName);		// 999.999.999.999 形式の IP アドレスを long int に変換

//	if(iadr.s_addr==INADDR_NONE)
	if(iadr.S_un.S_addr==INADDR_NONE)
	{	// szConnHostName が 999.999.999.999 形式で与えられていなかった場合
		// 「ホスト名」を構造体に代入する
		m_pHostEnt=gethostbyname(szConnHostName);
	}
	else
	{
		// 999.999.999.999 形式から、「ホスト名」を得る
		// winsock ver 1 (windows 95) では失敗する
// この処理は、ホスト名を得るだけで、不必要なので抹消
//		m_pHostEnt=gethostbyaddr((const char *)&iadr.S_un.S_addr,sizeof(iadr.S_un.S_addr),AF_INET);
	}

	if(!m_pHostEnt && iadr.S_un.S_addr==INADDR_NONE)
	{	// 「ホスト名」形式で接続先が与えられたが、解析できなかった場合
		sprintf(sHTML, "DNS lookup : %s (error code : %d)", szConnHostName, WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_GETHOST);		// ネットワーク・エラー：Windowsソケットでホスト名の解決ができません\n
		shutdown(sock,SD_BOTH);
		closesocket(sock);
		::WSACleanup();
		return FALSE;
	}

	sai.sin_port=htons((u_short)_nPort);
	sai.sin_family=AF_INET;
	// ＩＰアドレスの代入
	if(iadr.S_un.S_addr == INADDR_NONE)
	{
		sai.sin_addr=*((LPIN_ADDR)*m_pHostEnt->h_addr_list);
	}
	else
	{
		sai.sin_addr.S_un.S_addr = iadr.S_un.S_addr;
		sai.sin_addr.S_un.S_un_b = iadr.S_un.S_un_b;
		sai.sin_addr.S_un.S_un_w.s_w1 = iadr.S_un.S_un_w.s_w1;
		sai.sin_addr.S_un.S_un_w.s_w2 = iadr.S_un.S_un_w.s_w2;
	}

	if(bDebugMode)
	{
		sprintf(szTemp, "address pharse : %s : %d.%d.%d.%d\r\n", szConnHostName, sai.sin_addr.S_un.S_un_b.s_b1, sai.sin_addr.S_un.S_un_b.s_b2, sai.sin_addr.S_un.S_un_b.s_b3, sai.sin_addr.S_un.S_un_b.s_b4);
		_dbg_write_formated(fo, "gethost", szTemp);
		sprintf(szTemp, "sin_family : %X\r\nsin_port : %X\r\nsin_addr.S_un.S_addr : %lX\r\nsin_addr.S_un.S_un_w.s_w1 : %X\r\nsin_addr.S_un.S_un_w.s_w2 : %X\r\nsin_addr.S_un.S_un_b.s_b1 : %X\r\nsin_addr.S_un.S_un_b.s_b2 : %X\r\nsin_addr.S_un.S_un_b.s_b3 : %X\r\nsin_addr.S_un.S_un_b.s_b4 : %X\r\n",
				sai.sin_family, sai.sin_port, sai.sin_addr.S_un.S_addr, sai.sin_addr.S_un.S_un_w.s_w1, sai.sin_addr.S_un.S_un_w.s_w2,sai.sin_addr.S_un.S_un_b.s_b1, sai.sin_addr.S_un.S_un_b.s_b2, sai.sin_addr.S_un.S_un_b.s_b3, sai.sin_addr.S_un.S_un_b.s_b4);
		_dbg_write_formated(fo, "SOCKADDR_IN info", szTemp);
	}



	// ====================================================================
	// 接続
	// ====================================================================
	if(connect(sock,(LPSOCKADDR)&sai,sizeof(sai)) == SOCKET_ERROR)
	{	// 接続できませんでした
		sprintf(sHTML, "socket connect : %s : %d.%d.%d.%d (port:%u) (error code : %d)", szConnHostName, sai.sin_addr.S_un.S_un_b.s_b1, sai.sin_addr.S_un.S_un_b.s_b2, sai.sin_addr.S_un.S_un_b.s_b3, sai.sin_addr.S_un.S_un_b.s_b4, _nPort, WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_CONN);	// ネットワーク・エラー：Windowsソケットの接続に失敗しました\n
		shutdown(sock,SD_BOTH);
		closesocket(sock);
		::WSACleanup();
		return FALSE;
	}

	if(bDebugMode)
	{
		sprintf(szTemp, "connect : %s : %d.%d.%d.%d", szConnHostName, sai.sin_addr.S_un.S_un_b.s_b1, sai.sin_addr.S_un.S_un_b.s_b2, sai.sin_addr.S_un.S_un_b.s_b3, sai.sin_addr.S_un.S_un_b.s_b4);
		_dbg_write_formated(fo, "socket (connect)", szTemp);
	}

	// ====================================================================
	// リクエスト （送信）
	// ====================================================================

	if(strlen(_sProxy) == 0)
	{	// ダイレクト接続の場合
		sprintf(
			szTemp,
			"GET %s HTTP/1.0\r\n"
			"Host: %s:%d\r\n"
			"User-Agent: %s\r\n\r\n",
			szHtmlName,szHostName,_nPort,pszUserAgent);
	}
	else
	{	// プロキシ経由接続の場合
		sprintf(
			szTemp,
			"GET %s HTTP/1.0\r\n"
			"Host: %s:%d\r\n"
			"User-Agent: %s\r\n\r\n",
			szHtmlName,szHostName,80,pszUserAgent);
	}
	psz=szTemp;

	while(i < lstrlen(psz))
	{
		i = send(sock, psz, lstrlen(psz), 0);	// 送信
		if(i == SOCKET_ERROR)
		{
			sprintf(sHTML, "socket send (error code : %d)", WSAGetLastError ());
//			sHTML->LoadString(IDS_ERR_SOCK_CONN);	// ネットワーク・エラー：Windowsソケットの接続に失敗しました\n
			shutdown(sock,SD_BOTH);
			closesocket(sock);
			::WSACleanup();
			return FALSE;
		}
		// 送信バイト数が i に格納される。
		psz += i;
	}
	if(bDebugMode)
	{
		_dbg_write_formated(fo, "socket (send)", szTemp);
	}

	// ====================================================================
	// 受信
	// ====================================================================

	strcpy(sHTML, "");
	for(j=0;;)
	{
		i = recv(sock, szBuf, RECVSIZE, 0);		// 受信
		// 受信バイト数が i に格納される
		if(i<=0 || i==SOCKET_ERROR) break;		// 受信終了
		j += i;

		szBuf[i] = (char)NULL;		// 文字列として完成させるため、末尾に NULL 追加

		if(strlen(sHTML) + i < BUFSIZE) strcat(sHTML, szBuf);

	}

	if(i == SOCKET_ERROR)
	{
		sprintf(sHTML, "socket recv (error code : %d)", WSAGetLastError ());
		shutdown(sock,SD_BOTH);
		closesocket(sock);
		::WSACleanup();
		return FALSE;
	}

	dwTotalSize = strlen(sHTML);

	if(bDebugMode)
	{
		sprintf(szTemp, "recv total size : %d, this program buffer %d\r\n", j, dwTotalSize);
		_dbg_write_formated(fo, "socket (recv) (size)", szTemp);
		_dbg_write_formated(fo, "socket (recv)", sHTML);
	}

	// ====================================================================
	// 通信終了
	// ====================================================================

	if(shutdown(sock,SD_BOTH))
	{
		sprintf(sHTML, "socket shutdown (error code : %d)", WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_SHUT);		// ネットワーク・エラー：Windowsソケットの解放に失敗\n
	}
	if(sock!=INVALID_SOCKET)
	{
		if(closesocket(sock))
		{
			sprintf(sHTML, "socket close (error code : %d)", WSAGetLastError ());
//			sHTML->LoadString(IDS_ERR_SOCK_CLOSE);	// ネットワーク・エラー：Windowsソケットの解放に失敗\n
		}
	}

	if(::WSACleanup())
	{
		sprintf(sHTML, "WSACleanup (error code : %d)", WSAGetLastError ());
//		sprintf(szTemp, "WSACleanup : error code : %d", WSAGetLastError ());
	}
	if(bDebugMode)
	{
		_dbg_write_formated(fo, "socket (close), WSACleanup", "");
		_dbg_bufferovrn_check(fo, szBuf, RECVSIZE, 1500, "buffer over run check (szBuf)");
		_dbg_bufferovrn_check(fo, szHostName, URLSIZE, PADSIZE, "buffer over run check (szHostName)");
		_dbg_bufferovrn_check(fo, szConnHostName, URLSIZE, PADSIZE, "buffer over run check (szConnHostName)");
		_dbg_bufferovrn_check(fo, szHtmlName, URLSIZE, PADSIZE, "buffer over run check (szHtmlName)");
	}

	return TRUE;
}

//********************
// 文字列を検索し、先頭からの文字数を返す
// 
// szOrg から szKey を探す。（スタート地点は、先頭より seek だけ進んだ位置）
// 戻り値：0 ～ ：見つかった文字列の先頭、-1：発見できなかった
//********************
int _str_find(char *szOrg, char *szKey, int seek)
{
	char *pszTmp;
	int nTemp;

	if((int)strlen(szOrg) <= seek) return -1;

	pszTmp = strstr(szOrg + seek, szKey);

	if(pszTmp == NULL) return -1;

	nTemp = pszTmp - szOrg;

	return nTemp;
}

//********************
// ＥＵＣをＳｈｉｆｔＪＩＳに変換する
// 
// sHTML で渡された元のバッファ自体を書き換える
// <meta> キー内で Shift-JIS が宣言されている場合は、変換をしない
//        それ以外は、すべて変換ルーチンを通るが、EUC以外の文字は影響を受けない
//********************
void _conv_euc_to_sjis(char *sHTML)
{
	int			i,j;
	BOOL		isSJIS = FALSE;			// Shift JIS エンコード時に TRUE
//	BOOL		bInEuc = FALSE;
	unsigned int nEucCode;
	char		szTemp[1024];			// 送信文字列の一時バッファ、と各種作業用バッファ
	long		dwTotalSize;

	dwTotalSize = strlen(sHTML);

	//***********************
	// META SJIS  判定
	//***********************
	isSJIS = FALSE;

	for(i=0,j=0;;)
	{
		i=_str_find(sHTML, "<META", j);
		if(i<0)
		{
			break;
		}
		j=_str_find(sHTML, ">", i+1);
		if(j<0) break;
		strncpy(szTemp, sHTML + (i+1), j-i);
		if(_str_find(szTemp,"SHIFT",0)!=-1 || _str_find(szTemp,"shift",0)!=-1 || _str_find(szTemp,"Shift",0)!=-1 ||
					_str_find(szTemp,"sjis",0)!=-1 || _str_find(szTemp,"Sjis",0)!=-1 || _str_find(szTemp,"SJIS",0)!=-1)
		{
			isSJIS = TRUE;
			break;
		}
	}		
	for(i=0,j=0;;)
	{
		i=_str_find(sHTML, "<meta", j);
		if(i<0)
		{
			break;
		}
		j=_str_find(sHTML, ">", i+1);
		if(j<0) break;
		strncpy(szTemp, sHTML + (i+1), j-i);
		if(_str_find(szTemp,"SHIFT",0)!=-1 || _str_find(szTemp,"shift",0)!=-1 || _str_find(szTemp,"Shift",0)!=-1 ||
					_str_find(szTemp,"sjis",0)!=-1 || _str_find(szTemp,"Sjis",0)!=-1 || _str_find(szTemp,"SJIS",0)!=-1)
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
			if((sHTML[i] >= (char)0xa1) && (sHTML[i] <= (char)0xfe))
			{
				if(i<(int)dwTotalSize-1)
				{	// 後ろに一文字ある場合
					nEucCode = (0xff00 & (sHTML[i] << 8)) | (0xff & sHTML[i+1]);
					nEucCode &= 0x7f7f;
					nEucCode = _mbcjistojms(nEucCode);
					if(nEucCode != 0)
					{
						sHTML[i] =  (char)((nEucCode & 0xff00) >> 8);
						sHTML[i+1] = (char)(nEucCode & 0xff);
					}
					i++;

				}
				else
				{	// 最後の文字の場合
				}
			}
		}
	}

}

//********************
// 与えられた文字列の先頭に、タイトル行（現在時刻入り）を付加する。
// 
// __sTitle + 日時 + "\n" + __sHTML → __sReturn
//
// __sTitle がＮＵＬＬ文字列の時は、何もしない
//********************
void _add_title(char *__sReturn, char *__sHTML, char *__sTitle, char *szTemp)
{
	struct tm *_tm_loc;	// ローカル時刻
	time_t	_tm_sys;	// システム時刻（long int)

	if(strlen(__sHTML) != 0 && strlen(__sTitle) != 0)
	{
		time(&_tm_sys);
		_tm_loc = localtime(&_tm_sys);
		strcpy(__sReturn, __sTitle);
		sprintf(szTemp, "     %4d/%4d/%4d  %2d:%02d\n", _tm_loc->tm_year+1900, _tm_loc->tm_mon+1, _tm_loc->tm_mday, _tm_loc->tm_hour, _tm_loc->tm_min);
		strcat(__sReturn, szTemp);
		strcat(__sReturn, __sHTML);
	}
	else
	{	// タイトル無しのとき
		strcpy(__sReturn, __sHTML);
	}

	return;
}

//********************
// デバッグモードで、新規ファイル保存の一連の作業を行う
// 
// 「ファイルを保存する」ダイアログを表示する
// ファイルを開き、ファイルポインタに設定する。
//********************
void _dbg_openfile(FILE **fo, char *szTemp)
{
	OPENFILENAME ofn;

	strcpy(szTemp, "");

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szTemp;
	ofn.nMaxFile = TMPSIZE;
	ofn.lpstrFilter = "Text file\0*.txt\0Data file\0*.dat\0All Files\n*.*\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = szTemp;
	ofn.nMaxFileTitle = TMPSIZE;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Save Debug Data";
	ofn.Flags = OFN_OVERWRITEPROMPT;

	// 「ファイルを保存する」ダイアログを表示する
	if(GetSaveFileName(&ofn) != 0)
	{
		*fo = fopen(ofn.lpstrFile, "wb");
	}
	else
	{
		bDebugMode = FALSE;
	}
}

//********************
// ファイルにデバック情報を書き込む
// 
// szBuf1 + "\n" + szBuf2 → ファイル
//********************
void _dbg_write_formated(FILE **fo, char *szBuf1, char *szBuf2)
{
	struct tm *_tm_loc;	// ローカル時刻
	time_t	_tm_sys;	// システム時刻（long int)
	clock_t _tm_clk;	// CPU クロック時間

	if(*fo == NULL) return;

	time(&_tm_sys);
	_tm_loc = localtime(&_tm_sys);
	_tm_clk = clock();
	fprintf(*fo, "==> %04d/%04d/%04d %02d:%02d:%02d (%08lu) \r\n", _tm_loc->tm_year+1900, _tm_loc->tm_mon+1, _tm_loc->tm_mday, _tm_loc->tm_hour, _tm_loc->tm_min, _tm_loc->tm_sec, _tm_clk);
	fprintf(*fo, "%s : \r\n%s\r\n", szBuf1, szBuf2);
	fprintf(*fo, "===============================================\r\n");

}

//********************
// ファイルにデバック情報を書き込む (数値を文字列に変換）
// 
// szBuf1 + "\n" + x → ファイル
//********************
void _dbg_write_formated_int(FILE **fo, char *szBuf1, int x)
{
	struct tm *_tm_loc;	// ローカル時刻
	time_t	_tm_sys;	// システム時刻（long int)
	clock_t _tm_clk;	// CPU クロック時間

	if(*fo == NULL) return;

	time(&_tm_sys);
	_tm_loc = localtime(&_tm_sys);
	_tm_clk = clock();
	fprintf(*fo, "==> %04d/%04d/%04d %02d:%02d:%02d (%08lu) \r\n", _tm_loc->tm_year+1900, _tm_loc->tm_mon+1, _tm_loc->tm_mday, _tm_loc->tm_hour, _tm_loc->tm_min, _tm_loc->tm_sec, _tm_clk);
	fprintf(*fo, "%s : \r\n%u\r\n", szBuf1, (UINT)x);
	fprintf(*fo, "===============================================\r\n");

}

//********************
// ファイルにデバック情報を書き込む （各種文字列の初期条件をまとめて書き込む）
// 
//********************
void _dbg_write_init(FILE **fo, char *szTemp)
{
	OSVERSIONINFO ver;
	LPHOSTENT _hostent;
	IN_ADDR in_addr;

	if(*fo == NULL) return;

	// OSのバージョン等
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((LPOSVERSIONINFO)&ver);

	sprintf(szTemp, "dwMajorVersion : %Lu\r\ndwMinorVersion : %Lu\r\ndwBuildNumber : %u\r\nszCSDVersion : %s\r\n", ver.dwMajorVersion, ver.dwMinorVersion,
			ver.dwBuildNumber, ver.szCSDVersion);
	_dbg_write_formated(fo, "Windows Version", szTemp);

	// このマシンのホスト名
	if(!gethostname(szTemp, TMPSIZE))
	{
		// ホスト名を書き出す
		_dbg_write_formated(fo, "gethostname (this unit)", szTemp);
		// ホストのＩＰアドレスを書き出す
		_hostent = gethostbyname(szTemp);
		in_addr = *((LPIN_ADDR)*_hostent->h_addr_list);

		sprintf(szTemp, "%d.%d.%d.%d\r\n", in_addr.S_un.S_un_b.s_b1, in_addr.S_un.S_un_b.s_b2,
					in_addr.S_un.S_un_b.s_b3, in_addr.S_un.S_un_b.s_b4);
		_dbg_write_formated(fo, "gethostbyname (this unit)", szTemp);
	}
	else
	{
		sprintf(szTemp, "Error : gethostname (error code : %d)\r\n", WSAGetLastError ());
		_dbg_write_formated(fo, "gethostname (this unit)", szTemp);
	}

	// 各種変数の初期値
	_dbg_write_formated(fo, "_sURL", _sURL);
	_dbg_write_formated(fo, "_sProxy", _sProxy);
	_dbg_write_formated_int(fo, "_nPort", _nPort);
	_dbg_write_formated(fo, "_sPhHeader", _sPhHeader);
	_dbg_write_formated(fo, "_sPhHeaderB", _sPhHeaderB);
	_dbg_write_formated(fo, "_sPhHeaderC", _sPhHeaderC);
	_dbg_write_formated(fo, "_sTitle", _sTitle);
	_dbg_write_formated(fo, "_sItems", _sItems);
	_dbg_write_formated_int(fo, "_nMode", _nMode);
	_dbg_write_formated_int(fo, "_nPhSkip", _nPhSkip);
	_dbg_write_formated_int(fo, "_nPhGetcount", _nPhGetcount);

}

//********************
// ファイルにデバック情報を書き込む（バッファオーバーラン用のパディング領域を書き込む）
// 
// szStr のサイズは (_sLen0 + _sPad)。パディング領域は _sPad
//********************
void _dbg_bufferovrn_check(FILE **fo, char *szStr, int _sLen0, int _sPad, char *szTitle)
{
	int i;

	if(*fo == NULL) return;

	fprintf(*fo, "%s :\r\n", szTitle);

	for(i=0; i<_sPad; i++)
	{
		fprintf(*fo, "%02X ", szStr[_sLen0+i]);
	}
	fprintf(*fo, "\r\n===============================================\r\n");

}
