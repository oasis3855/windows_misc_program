// NetAccessThread.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "infobar00.h"
#include "NetAccessThread.h"
//#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 * WinSock 2 extension -- manifest constants for shutdown()
 */
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02

#define		RECVSIZE	5000		// 一度に受信できる最大サイズ

//extern CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
//extern CString		_sTmp_MidBuf;	// sTransBuf に渡す最終結果格納用
//extern CString		_sPhHeaderCur;	// 現在の区切り文字列
//extern CTime		_tm_trd;


/////////////////////////////////////////////////////////////////////////////
// CNetAccessThread

IMPLEMENT_DYNCREATE(CNetAccessThread, CWinThread)

CNetAccessThread::CNetAccessThread()
{
}

CNetAccessThread::~CNetAccessThread()
{
}

BOOL CNetAccessThread::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。
#if 0
	CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
	CString		_sTmp_MidBuf;	// sTransBuf に渡す最終結果格納用
	CString		_sPhHeaderCur;	// 現在の区切り文字列
	CTime		_tm_trd;
	
	int			i,j;
	BOOL		bSockResult;
	int _nSkip=0;		// 先頭スキップ数のカウント
	int _nGetCt=0;		// 取得項目数のカウント

	// スレッドの重複起動は避ける
	if(*bInThread == TRUE)
		return TRUE;

	sURL = _sURL;
	sProxy = _sProxy;
	sPhHeader = _sPhHeader;
	sPhHeaderB = _sPhHeaderB;
	sPhHeaderC = _sPhHeaderC;
	sTitle = _sTitle;
	sItems = _sItems;


	//***********************
	// 初期値の設定
	//***********************
	*bInThread = TRUE;			// スレッド動作中
	*bUpdated = FALSE;			// 更新が終わっていないことを示す
	_sTmp_trd = "";				// 受信した HTML を一時格納
	_sTmp_MidBuf = "";

	if(nMode == 1)
	{	// Yahoo の証券データを受信する

		Receive_Mode_01();		// 受信関数のメイン

//		if(*sTransBuf != "")
		if(strlen(_sTransBuf))
			*bUpdated = TRUE;		// 情報更新

		*bInThread = FALSE;		// スレッド動作中

		return TRUE;
	}



	// sURL のアドレスの HTML を受信し、 _sTmp3_trd に内容を格納
	bSockResult = receive_HTML(&sURL, &_sTmp3_trd);

	_sTmp_trd = _sTmp3_trd;		// 一時バッファにコピー

	if(_sTmp_trd != "" && bSockResult)
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
				if(_sTmp_trd[i+1] == '<')
				{	// 直後に <...> が続いている場合
					// 次の ">" へ
					i = _sTmp_trd.Find(">", i+1) + 1;	// ">"の分の 1 加えておく
				}
				else if(_sTmp_trd.Find("<", i+1) > _sTmp_trd.Find(">", i+1))
				{	// 区切文字列が <...>の内部にあるとき
					// 次の ">" へ
					i = _sTmp_trd.Find(">", i+1) + 1;	// ">"の分の 1 加えておく
				}
				else break;	// 平文に差し掛かると、ループ抜ける
			}
			if(i<0) break;
			j = _sTmp_trd.Find("<", i+1);	// 次の <...>の始まりを見つける
			if(j<0) break;
			if(nPhSkip > _nSkip) continue;

			_sTmp2_trd = _sTmp_trd.Mid(i, j-i);		// １行抽出

			if(_sTmp2_trd == "" || _sTmp2_trd == "\n") continue;	// 空文字列スキップ
			_sTmp_MidBuf += _sTmp2_trd;		// バッファに追加
			if(_sTmp_MidBuf[_sTmp_MidBuf.GetLength()-1] != '\n')
				_sTmp_MidBuf += "\n";		// 最終文字が 改行 でない時は、改行をつける
			_nGetCt++;

		}

	}



	//**************************************
	// タイトル付け
	//**************************************
	if(_sTmp_MidBuf != "" && sTitle != "" && bSockResult)
	{
		_tm_trd = CTime::GetCurrentTime();
		_sTmp_trd.LoadString(IDS_STR_TITLEFMT);		// %s     %2d月%2d日%02d時%02d分 現在\n
		_sTmp2_trd.Format(_sTmp_trd, sTitle, _tm_trd.GetMonth(), _tm_trd.GetDay(), _tm_trd.GetHour(), _tm_trd.GetMinute());
		_sTmp_MidBuf = _sTmp2_trd + _sTmp_MidBuf;
	}

	if(!bSockResult)
	{
		_sTmp_MidBuf = _sTmp_trd;
	}

//	*sTransBuf = _sTmp_MidBuf;
	strcpy(_sTransBuf, _sTmp_MidBuf);

	
//	if(*sTransBuf != "")
	if(strlen(_sTransBuf))
		*bUpdated = TRUE;		// 情報更新

	*bInThread = FALSE;		// スレッド動作中



	
#endif
	
	return TRUE;
}

int CNetAccessThread::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNetAccessThread, CWinThread)
	//{{AFX_MSG_MAP(CNetAccessThread)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetAccessThread メッセージ ハンドラ

void CNetAccessThread::Receive_Mode_01()
{
#if 0
	int			i,j,_i,_j;
	int _nSkip=0;		// 先頭スキップ数のカウント
	int _nGetCt=0;		// 取得項目数のカウント
	CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
	CString		_sTmp_MidBuf;	// sTransBuf に渡す最終結果格納用
	CString		_sPhHeaderCur;	// 現在の区切り文字列
	CTime		_tm_trd;

	_sTmp_MidBuf = "";

	for(_i=0,_j=0; ; )
	{
		_j = sItems.Find("\n", _i);
		if(_j<0)
			_j = sItems.GetLength()+1;		// 次の行がない時（最後のNULLを指す)
		_sTmp3_trd = sItems.Mid(_i, _j-_i-1);	// １行分抽出

		if(_sTmp3_trd == "" || _sTmp3_trd == "\n")
		{
			_i = _j+1;							// 改行の次の文字
			if(_j >= sItems.GetLength()) break;	// 最終行を越えた
			continue;
		}

		// sURL の "%s" に _sTmp3_trd のコードを挿入する
		_sTmp4_trd.Format(sURL, _sTmp3_trd);

		// _sTmp4_trd のアドレスの HTML を受信し、 _sTmp3_trd に内容を格納
		receive_HTML(&_sTmp4_trd, &_sTmp3_trd);

		_sTmp_trd = _sTmp3_trd;		// 一時バッファにコピー
//		::MessageBox(NULL, _sTmp_trd, "debug - A", MB_OK);

		if(_sTmp_trd != "")
		{
			//**************************************
			// 検索抽出
			//**************************************
			_sPhHeaderCur = sPhHeader;	// 最初の区切り文字
			_nGetCt = 0;
			for(i=0,j=0 ; _nGetCt < nPhGetcount ; _nSkip++)
			{
				i = _sTmp_trd.Find(_sPhHeaderCur,j);	// 区切文字列を検索
				if(i<0) break;
				i += _sPhHeaderCur.GetLength();		// 区切文字列文だけインデックスを進める
				for(;;)
				{
					if(_sTmp_trd[i+1] == '<')
					{	// 直後に <...> が続いている場合
						// 次の ">" へ
						i = _sTmp_trd.Find(">", i+1) + 1;	// ">"の分の 1 加えておく
					}
					else if(_sTmp_trd.Find("<", i+1) > _sTmp_trd.Find(">", i+1))
					{	// 区切文字列が <...>の内部にあるとき
						// 次の ">" へ
						i = _sTmp_trd.Find(">", i+1) + 1;	// ">"の分の 1 加えておく
					}
					else break;	// 平文に差し掛かると、ループ抜ける
				}
				if(i<0) break;
				j = _sTmp_trd.Find("<", i+1);	// 次の <...>の始まりを見つける
				if(j<0) break;
				if(nPhSkip > _nSkip) continue;

				_sTmp2_trd = _sTmp_trd.Mid(i, j-i);		// １行抽出

				if(_sTmp2_trd == "" || _sTmp2_trd == "\n") continue;	// 空文字列スキップ
				_sTmp_MidBuf += _sTmp2_trd;		// バッファに追加
				if(_sTmp_MidBuf[_sTmp_MidBuf.GetLength()-1] == '\n')
				{
					_sTmp_MidBuf.SetAt(_sTmp_MidBuf.GetLength()-1, ' ');
					_sTmp_MidBuf += "  ";
				}
				else
					_sTmp_MidBuf += "   ";

				_nGetCt++;

				if(_nGetCt == 1)
					_sPhHeaderCur = sPhHeaderB;	// 最初の区切り文字
				else if(_nGetCt == 2)
					_sPhHeaderCur = sPhHeaderC;	// 最初の区切り文字
				else
					break;
			}

		}
		if(_sTmp_MidBuf.GetLength() >0)
		{
			if(_sTmp_MidBuf[_sTmp_MidBuf.GetLength()-1] != '\n')
				_sTmp_MidBuf += "\n";		// 最終文字が 改行 でない時は、改行をつける
		}


		_i = _j+1;							// 改行の次の文字
		if(_j >= sItems.GetLength()) break;	// 最終行を越えた

		::MessageBox(NULL, _sTmp_MidBuf, "debug", MB_OK);

	}

	//**************************************
	// タイトル付け
	//**************************************
	if(_sTmp_MidBuf != "" && sTitle != "")
	{
		_tm_trd = CTime::GetCurrentTime();
		_sTmp_trd.LoadString(IDS_STR_TITLEFMT);		// %s     %2d月%2d日%02d時%02d分 現在\n
		_sTmp2_trd.Format(_sTmp_trd, sTitle, _tm_trd.GetMonth(), _tm_trd.GetDay(), _tm_trd.GetHour(), _tm_trd.GetMinute());
		_sTmp_MidBuf = _sTmp2_trd + _sTmp_MidBuf;
	}

//	*sTransBuf = _sTmp_MidBuf;
	strcpy(_sTransBuf, _sTmp_MidBuf);

	return ;
#endif
}

BOOL CNetAccessThread::receive_HTML(CString *sRURL, CString *sHTML)
{
#if 0
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

	CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
	CString		_sTmp_MidBuf;	// sTransBuf に渡す最終結果格納用
	CString		_sPhHeaderCur;	// 現在の区切り文字列
	CTime		_tm_trd;


//	CSocket *_sock;
	WSADATA _sock_wsa;


	//***********************
	// 初期値の設定
	//***********************
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
	AfxSocketInit(&_sock_wsa);	// WinSock の初期化
//	WORD wVersionRequested = MAKEWORD( 2, 0 );
//	WSAStartup( wVersionRequested, &_sock_wsa );

//	_sock = new CSocket();

//	if(_sock->Create() == 0)
	if((sock=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
	{	// ソケット作成不能
		sHTML->LoadString(IDS_ERR_SOCK_CREAT);	// ネットワーク・エラー：Windowsソケットの作成失敗\n
//		delete _sock;
		return FALSE;
	}
	// ====================================================================
	// URLの先頭に "http://"がついていた場合は削除する
	// ====================================================================
	if(!strncmp((LPCSTR)*sRURL, "http://", 7))
	{
		_sTmp_trd = sRURL->Mid(7, sRURL->GetLength()-7);
		*sRURL = _sTmp_trd;
	}

	// ====================================================================
	// URLの分解
	// ====================================================================

	memset(szHostName,'\0',sizeof(szHostName));
	memset(szHtmlName,'\0',sizeof(szHtmlName));

	strcpy(szURL, *sRURL);
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
		lstrcat(szHtmlName,*sRURL);
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
		sHTML->LoadString(IDS_ERR_SOCK_GETHOST);		// ネットワーク・エラー：Windowsソケットでホスト名の解決ができません\n
		shutdown(sock,SD_BOTH);
		closesocket(sock);
		return FALSE;
	}

	// ====================================================================
	// 接続
	// ====================================================================

	sai.sin_port=htons((u_short)nPort);
	sai.sin_family=AF_INET;
	sai.sin_addr=*((LPIN_ADDR)*m_pHostEnt->h_addr_list);

//	if(_sock->Connect(szConnHostName, nPort) == 0)
	if(connect(sock,(LPSOCKADDR)&sai,sizeof(sai)) == SOCKET_ERROR)
	{	// 接続できませんでした
		int x = WSAGetLastError ();
		sHTML->LoadString(IDS_ERR_SOCK_CONN);	// ネットワーク・エラー：Windowsソケットの接続に失敗しました\n
		shutdown(sock,SD_BOTH);
		closesocket(sock);
//		_sock->ShutDown(2);
//		_sock->Close();
//		delete _sock;
		return FALSE;
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
//		i = _sock->Send(psz, lstrlen(psz));
		i = send(sock, psz, lstrlen(psz), 0);	// 送信
		if(i == SOCKET_ERROR)
		{
			sHTML->LoadString(IDS_ERR_SOCK_CONN);	// ネットワーク・エラー：Windowsソケットの接続に失敗しました\n
//			_sock->ShutDown(2);
//			_sock->Close();
			shutdown(sock,SD_BOTH);
			closesocket(sock);
//			delete _sock;
			return FALSE;
		}
		// 送信バイト数が i に格納される。
		psz += i;
	}

	// ====================================================================
	// 受信
	// ====================================================================

	for(;;)
	{
//		i = _sock->Receive(pszBuf, RECVSIZE);
		i = recv(sock, pszBuf, RECVSIZE, 0);		// 受信
		// 受信バイト数が i に格納される
		if(i<=0 || i==SOCKET_ERROR) break;		// 受信終了

		pszBuf[i] = (char)NULL;		// 文字列として完成させるため、末尾に NULL 追加

		_sTmp_trd += (LPCSTR)pszBuf;

	}
	dwTotalSize = _sTmp_trd.GetLength();

	if(i==SOCKET_ERROR)
		::MessageBox(NULL, "sock err", "debug -C", MB_OK);


	// ====================================================================
	// 通信終了
	// ====================================================================
//	_sock->ShutDown(2);
//	_sock->Close();
//	delete _sock;

	if(shutdown(sock,SD_BOTH))
	{
		sHTML->LoadString(IDS_ERR_SOCK_SHUT);		// ネットワーク・エラー：Windowsソケットの解放に失敗\n
		return FALSE;
	}
	if(sock!=INVALID_SOCKET)
	{
		if(closesocket(sock))
		{
			sHTML->LoadString(IDS_ERR_SOCK_CLOSE);	// ネットワーク・エラー：Windowsソケットの解放に失敗\n
			return FALSE;
		}
	}

//	::WSACleanup();

	//***********************
	// META SJIS  判定
	//***********************
	for(i=0,j=0;;)
	{
		i=_sTmp_trd.Find("<META", j);
		if(i<0)
		{
			break;
		}
		j=_sTmp_trd.Find(">", i+1);
		if(j<0) break;
		_sTmp2_trd = _sTmp_trd.Mid(i+1, j-i);
		if(_sTmp2_trd.Find("SHIFT",0)!=-1 || _sTmp2_trd.Find("shift",0)!=-1 || _sTmp2_trd.Find("Shift",0)!=-1 ||
					_sTmp2_trd.Find("sjis",0)!=-1 || _sTmp2_trd.Find("Sjis",0)!=-1 || _sTmp2_trd.Find("SJIS",0)!=-1)
		{
			isSJIS = TRUE;
			break;
		}
	}		
	for(i=0,j=0;;)
	{
		i=_sTmp_trd.Find("<meta", j);
		if(i<0)
		{
			break;
		}
		j=_sTmp_trd.Find(">", i+1);
		if(j<0) break;
		_sTmp2_trd = _sTmp_trd.Mid(i+1, j-i);
		if(_sTmp2_trd.Find("SHIFT",0)!=-1 || _sTmp2_trd.Find("shift",0)!=-1 || _sTmp2_trd.Find("Shift",0)!=-1 ||
					_sTmp2_trd.Find("sjis",0)!=-1 || _sTmp2_trd.Find("Sjis",0)!=-1 || _sTmp2_trd.Find("SJIS",0)!=-1)
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
	*sHTML = _sTmp_trd;
	::MessageBox(NULL, _sTmp_trd, "debug -B", MB_OK);
#endif

	return TRUE;
}
