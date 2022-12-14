// InfoBar00Dlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "InfoBar00.h"
#include "InfoBar00Dlg.h"
#include "DlgNetConf.h"
#include "DlgSysConf.h"
#include "DlgAbout.h"
#include <process.h>

#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 仮想ビットマップのサイズ
//#define BITMAP_CX	5000
#define BITMAP_CX	8000		// 2003/06/01
#define BITMAP_CY	50

// タイマー ID
#define TIMER_ID	153

/////////////////////////////////////////////////////////////////////////////
// CInfoBar00Dlg ダイアログ

//*********************
// ダイアログの初期化
//
// ＤＣ関連の初期化、初期値の設定、タイマーの起動
//*********************
CInfoBar00Dlg::CInfoBar00Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoBar00Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoBar00Dlg)
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	IsInfoWndDC_active = FALSE;
	IsMemDC_active = FALSE;

	// ウインドウサイズが変更された場合に TRUE
	bWndSizeChanged = FALSE;

}

void CInfoBar00Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoBar00Dlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInfoBar00Dlg, CDialog)
	//{{AFX_MSG_MAP(CInfoBar00Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MENU_CONFIG, OnMenuConfig)
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_HELP, OnMenuHelp)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoBar00Dlg メッセージ ハンドラ

BOOL CInfoBar00Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	CString _sTmpStr1, _sTmpStr2;

	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	//**************************
	// フォントの設定
	//**************************
	LOGFONT lf;
	this->GetFont()->GetLogFont(&lf);
	m_Font.CreateFontIndirect(&lf);

	m_Font.DeleteObject();
	//**************************
	// ウインドウ状態の設定
	//**************************

	// タスクバーに表示するかどうかの設定
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	ex_style &= ~WS_EX_APPWINDOW;
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style);

	// ウインドウ位置の設定
	if(nInfoWndWidth != 0 && nInfoWndHeight != 0)
	{	// ウインドウサイズ指定が有効な時
		if(bDispTopmost)
			SetWindowPos(&wndTopMost, nPosX, nPosY,nInfoWndWidth,nInfoWndHeight, SWP_SHOWWINDOW);
		else
			SetWindowPos(&wndNoTopMost, nPosX, nPosY,nInfoWndWidth,nInfoWndHeight, SWP_SHOWWINDOW);
	}
	else
	{
		if(bDispTopmost)
			SetWindowPos(&wndTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
		else
			SetWindowPos(&wndNoTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
	}

	//**************************
	// デバイスコンテキストの生成と初期化
	//**************************

	//**** 描画エリアの DC
	InfoWndDC = this->GetDC();		// ウインドウ自身のＤＣ（全面描画）
//	InfoWndDC = m_ctrl_info_area.GetDC();
	if(InfoWndDC == NULL)
	{
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// アプリケーション名（InfoBar）
		_sTmpStr2.LoadString(IDS_ERR_GETDC);		// デバイス・コンテキストを利用できません
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		// ウインドウに WM_CLOSE メッセージを送り、終了させる
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}
	IsInfoWndDC_active = TRUE;	// このフラグで、終了時に DC を解放する

	//**** 描画エリアの RECT サイズ
//	m_ctrl_info_area.GetWindowRect(&InfoWndRect);
	this->GetWindowRect(&InfoWndRect);
	nInfoWndWidth = InfoWndRect.right - InfoWndRect.left;
	nInfoWndHeight = InfoWndRect.bottom - InfoWndRect.top;
	//**** バックグラウンドのブラシ（塗りつぶし色）
	BrushInfoBack.CreateSolidBrush(cBackColor);
	//**** ワークエリア用 DC
	if(!MemDC.CreateCompatibleDC(InfoWndDC))
	{	// ERROR
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// アプリケーション名（InfoBar）
		_sTmpStr2.LoadString(IDS_ERR_WNDDC);		// 仮想デバイス・コンテキストの生成に失敗しました
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		// ウインドウに WM_CLOSE メッセージを送り、終了させる
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}
	if(!MemBmp.CreateCompatibleBitmap(InfoWndDC,BITMAP_CX,BITMAP_CY))
	{	// ERROR
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// アプリケーション名（InfoBar）
		_sTmpStr2.LoadString(IDS_ERR_COMPBMP);		// 仮想ビットマップの生成に失敗しました
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		MemDC.DeleteDC();
		// ウインドウに WM_CLOSE メッセージを送り、終了させる
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}
	if(!MemDC.SelectObject(MemBmp))
	{
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// アプリケーション名（InfoBar）
		_sTmpStr2.LoadString(IDS_ERR_BMPTODC);		// 仮想ビットマップをデバイス・コンテキストにリンクできません
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		MemBmp.DeleteObject();
		MemDC.DeleteDC();
		// ウインドウに WM_CLOSE メッセージを送り、終了させる
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}
	IsMemDC_active = TRUE;	// このフラグで、終了時に MemDC,MemBmp を解放する


	//**************************
	// 初期値の設定
	//**************************

	ScrCurPos = 0;			// スクロール位置
	_nTimCurArticle = 0;	// 表示アイテム

	bUpdated = TRUE;		// 情報更新
	bInThread = FALSE;		// スレッド動作中
	bDebugMode = FALSE;
	theApp->sTransBuf.LoadString(IDS_MES_INITIAL);		// InfoBar  :   画面上を右クリックして設定してください\n
	strcpy(_sTransBuf, theApp->sTransBuf);


	//**************************
	// 初期ビットマップの描画
	//**************************
	_sTmpStr1 = "initialize ...";
	DrawStringOnBmp(&_sTmpStr1);		// 仮想ビットマップに文字列描画


	//**************************
	// タイマーの設定
	//**************************

	nTimerID = SetTimer(TIMER_ID, nTimer, NULL);	// 1000msec 後に ON_TIMER に制御が移る

	if(!nTimerID)
	{
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// アプリケーション名（InfoBar）
		_sTmpStr2.LoadString(IDS_ERR_TIMER);		// システム・タイマーを利用できません
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		// ウインドウに WM_CLOSE メッセージを送り、終了させる
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CInfoBar00Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CInfoBar00Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



//*********************
// タイマー割り込み
//
// 画面をリフレッシュする
//
//*********************
void CInfoBar00Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CString _sCurStr;
	int i,j,k;
	time_t tNow;
	BOOL bStrNotSpace;

//	CNetAccessThread* pThread;
	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	if(bWndSizeChanged)
	{	// ウインドウサイズが変更されたとき
		this->GetWindowRect(&InfoWndRect);
		nInfoWndWidth = InfoWndRect.right - InfoWndRect.left;
		nInfoWndHeight = InfoWndRect.bottom - InfoWndRect.top;
		nPosX = InfoWndRect.left;
		nPosY = InfoWndRect.top;
		if(InfoWndRect.bottom - InfoWndRect.top > BITMAP_CY)
		{
			// ウインドウ位置の設定
			if(bDispTopmost)
				SetWindowPos(&wndTopMost, nPosX, nPosY,nInfoWndWidth,BITMAP_CY, SWP_SHOWWINDOW);
			else
				SetWindowPos(&wndNoTopMost, nPosX, nPosY,nInfoWndWidth,BITMAP_CY, SWP_SHOWWINDOW);
		}
		bWndSizeChanged = FALSE;
	}

	//****************
	// 自動アップデート
	//****************
	if(tPrev != 0)
	{	// tPrev=0 は、起動時を表す
		time(&tNow);
		if(tNow - tPrev > tInterval)
		{	// インターバル時間を過ぎていれば、自動アップデート
			if(!bInThread)
			{
				// スレッド内で使用するグローバル変数を設定
				CopyGlobalParam();
				
				bUpdated = FALSE;		// 情報更新
				bInThread = FALSE;		// スレッド動作中
				if(nTimerID != 0) KillTimer(nTimerID);		// タイマーの停止
				if(_beginthread(thread_main,0,NULL)==-1)
				{
					theApp->sTransBuf.LoadString(IDS_ERR_THREAD);	// システム・エラー：スレッドが起動できません
				}

				::Sleep(nTimer*3);								// スレッド起動のため、ウエイト
				nTimerID = SetTimer(nTimerID, nTimer, NULL);	// 指定秒数後、ON_TIMER に制御が移る
			}
			else
			{
				// スレッドの重複起動は出来ないので、アップデートはしない。
				time(&tPrev);		// 現在時刻のアップデート
			}

			time(&tPrev);
		}

		//****************
		// レジストリ自動アップデート
		//****************
		// 表示座標が変わっていたら、レジストリに保存
		RECT winRect;
		GetWindowRect(&winRect);
		if((int)nPosX != winRect.left || (int)nPosY != winRect.top)
		{
			nPosX = winRect.left;
			nPosY = winRect.top;
			// レジストリに保存
			CInfoBar00App *theApp;
			theApp = (CInfoBar00App *)AfxGetApp();
			theApp->RegConfigWrite(this);
		}

	}

	//****************
	// 描画
	//****************
	if(theApp->sTransBuf == "" || theApp->sTransBuf == "\n") theApp->sTransBuf.LoadString(IDS_MES_NODATA);	// データ無し\n
	if(bUpdated)
	{
		theApp->sTransBuf = _sTransBuf;
		ScrCurPos = 0;
		_nTimCurArticle = 0;
		_sCurStr = theApp->sTransBuf.Mid(0, theApp->sTransBuf.Find("\n", 0));

		DrawStringOnBmp(&_sCurStr);		// 仮想ビットマップに文字列描画

		bUpdated = FALSE;
	}
	if(ScrCurPos > (int)MemStrLength)
	{	// １行アップデート
		_nTimCurArticle++;
		for(i=0,k=0; k<_nTimCurArticle; k++)
		{
			i = theApp->sTransBuf.Find("\n", i) + 1;
			j = theApp->sTransBuf.Find("\n", i);
		}
		if(j<=0 || i<=0)
		{
			ScrCurPos = 0;
			_nTimCurArticle = 0;
			i=0;
		}
		_sCurStr = theApp->sTransBuf.Mid(i, theApp->sTransBuf.Find("\n", i)-i);

		// 取り出した文字列が、空白だけや、改行だけの場合の処理
		bStrNotSpace = FALSE;
		for(i=0;i<_sCurStr.GetLength();i++)
		{
			if(_sCurStr.GetAt(i) != (char)' ' && _sCurStr.GetAt(i) != (char)NULL)
			{
				bStrNotSpace = TRUE;
				break;
			}
		}

		// 空白で無い場合のみ、新規の文字列を描画する
		if(bStrNotSpace)
		{
			DrawStringOnBmp(&_sCurStr);		// 仮想ビットマップに文字列描画
			ScrCurPos = 0;		// 座標リセット
		}
	}

	InfoWndDC->BitBlt(0, 0, nInfoWndWidth, nInfoWndHeight-0, &MemDC, ScrCurPos, 0, SRCCOPY);

	if(bInThread)
	{	// スレッドが動作中
		InfoWndDC->FillSolidRect(5,1,10,3, 0x0000a0);
	}
//	else
//	{	// スレッドは動作していない
//		InfoWndDC->FillSolidRect(1,1,20,3, cBackColor);
//	}

	ScrCurPos += nMoveSpeed;		// 移動するドット数
	
	CDialog::OnTimer(nIDEvent);
}

// **********************************
// 仮想ビットマップに、文字列を描画する
// また、スクロール範囲決定のための、文字列の横方向のサイズを算出
//
// **********************************
void CInfoBar00Dlg::DrawStringOnBmp(CString *sBuf)
{
	CFont fnt;		// 暫定フォント作成用


	if(nFontPoint == 0)
	{	// フォントが定義されていないときは、システムフォントを使用
		MemDC.SelectStockObject(DEVICE_DEFAULT_FONT);	// システムフォントを割り付け
	}
	else
	{
		fnt.CreatePointFont(nFontPoint, sFontName);
		MemDC.SelectObject(&fnt);
	}

	CalcMemStrLength(sBuf);		// 文字列のビットマップ長の計算

	// 文字列が仮想ビットマップの右端まで描画されているとき
	// ウインドウ幅だけ文字終端位置を手前にずらす
	if(BITMAP_CX - (int)MemStrLength < (int)nInfoWndWidth)
		MemStrLength = BITMAP_CX - nInfoWndWidth;

	MemDC.SelectObject(BrushInfoBack);
	MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, cBackColor);
	MemDC.SetBkColor(cBackColor);
	MemDC.SetTextColor(cForeColor);
	MemDC.TextOut(nInfoWndWidth,5,*sBuf);

	// 仮想ビットマップ右端ボーダーから100ドット目までに赤のストライプを入れる 2003/06/06
	// ビットマップ不足の警告用
	for(int i=0; i<10; i++)
	{
		MemDC.FillSolidRect(BITMAP_CX-100+i*10,0,1,BITMAP_CY-1, 0x0000ff);
	}

	if(nFontPoint != 0)
	{
		MemDC.SelectStockObject(DEVICE_DEFAULT_FONT);	// システムフォントを割り付け
		fnt.DeleteObject();
	}

	return ;
}

//*********************
// MemDC に書き込まれている文字（画像）の長さ（ピクセル）を返す
//
// +---------------------+-------------+
// |           文字列です|             |
// +---------------------+-------------+
// 0                     ↑            BITMAP_CX
//                  この座標を返す
// 
//*********************
void CInfoBar00Dlg::CalcMemStrLength(CString *sBuf)
{
	int i,j,k, n_max, n_min;

	// 検査用に文字列を描画（バックグラウンドは黒。文字描画領域は白）
	MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, 0x000000);
	MemDC.SetBkColor(0xffffff);
	MemDC.SetTextColor(0xffffff);
	MemDC.TextOut(nInfoWndWidth,5,*sBuf);

	n_max = BITMAP_CX-2;		// 最大
	n_min = nInfoWndWidth;		// 最小
	for(i = (n_max+n_min)/2, j=n_min, k=n_max; ; )
	{
		if(MemDC.GetPixel(i,5) == 0)
		{	// 文字でない （左へスキャン）
			k = i;			// k は i の位置まで移動
			i = (i+j)/2;	// i と j の中間へ
		}
		else
		{	// 文字である （右へスキャン）
			j = i;			// j は i の位置まで移動
			i = (i+k)/2;	// i と k の中間へ
		}
		if(i <= j || i >= k) break;
	}

	MemStrLength = i;

/**	
	// 右端より、左に向かって検査
	// FillRect が 右と下の境界線を塗りつぶしていないため補正
	for(i=BITMAP_CX-2; i>=0; i--)
	{
		for(j=0; j<BITMAP_CY-2; j++)
		{
			if(MemDC.GetPixel(i,j) == 0xffffff) break;
		}
		if(MemDC.GetPixel(i,j) == 0xffffff) break;
	}

	MemStrLength = i;
	**/
}

//*********************
// 右クリックに対するコンテキストメニュー表示
// 
//*********************
void CInfoBar00Dlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	CMenu menu;
	CPoint point_cur;
	::GetCursorPos(&point_cur);
	VERIFY(menu.LoadMenu(IDR_MENU_POPUP));
	CMenu *pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	SetForegroundWindow();
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point_cur.x, point_cur.y, this);
	SetForegroundWindow();
	PostMessage(WM_NULL, 0, 0);		// Win95/98 バグ対策
	
	pPopup->DestroyMenu();
	
	
	CDialog::OnRButtonUp(nFlags, point);
}

//*********************
// 左クリックに対する処理
//
// ダイアログを移動するメッセージ
// 左ボタンを検知し、ウインドウに対し移動メッセージを送る
//*********************
void CInfoBar00Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialog::OnLButtonDown(nFlags, point);
}

//*********************
// メニューの「設定」を選んだときの処理
//
// メニューのクラスを、既存の CDialog から派生している
//*********************
void CInfoBar00Dlg::OnMenuConfig() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CPropertySheet DlgProp(IDS_CONFDLG_TITLE);	// リソースからタイトル文字列を指定する

	CDlgNetConf dlg_netconf;
	CDlgSysConf dlg_sysconf;
	CDlgAbout dlg_about;

	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	dlg_netconf.m_sURL = theApp->sURL;
	dlg_netconf.m_nPort = theApp->nPort;
	dlg_netconf.m_sPhHeader = theApp->sPhHeader;
	dlg_netconf.m_nPhSkip = theApp->nPhSkip;
	dlg_netconf.m_nPhGetcount = theApp->nPhGetcount;
	dlg_netconf.m_sProxy = theApp->sProxy;
	dlg_netconf.m_sTitle = theApp->sTitle;
	dlg_netconf.m_nMode = theApp->nMode;
	dlg_netconf.m_chkDelSpace = theApp->nDelSpace;		// 2003/06/01
	dlg_netconf.m_chkCr2Spc = theApp->nCr2Spc;			// 2003/06/02

	dlg_sysconf.m_nTimer = nTimer;
	dlg_sysconf.m_nSpeed = nMoveSpeed;
	dlg_sysconf.m_cBackColor = cBackColor;
	dlg_sysconf.m_cForeColor = cForeColor;
	dlg_sysconf.m_tInterval = tInterval;

	dlg_sysconf.m_bTopmost = bDispTopmost;

	dlg_sysconf.nFontPoint = nFontPoint;
	dlg_sysconf.sFontName = sFontName;

	// プロパティーページの連結
	DlgProp.AddPage(&dlg_netconf);
	DlgProp.AddPage(&dlg_sysconf);
	DlgProp.AddPage(&dlg_about);
	DlgProp.SetActivePage(0); // ２ページ目を表にする

	if(DlgProp.DoModal() == IDOK)
	{	// 設定ダイアログで OK ボタンが押されたとき
		theApp->sURL = dlg_netconf.m_sURL;
		theApp->nPort = dlg_netconf.m_nPort;
		theApp->sPhHeader = dlg_netconf.m_sPhHeader;
		theApp->nPhSkip = dlg_netconf.m_nPhSkip;
		theApp->nPhGetcount = dlg_netconf.m_nPhGetcount;
		theApp->sProxy = dlg_netconf.m_sProxy;
		theApp->sTitle = dlg_netconf.m_sTitle;
		theApp->nMode = dlg_netconf.m_nMode;
		theApp->nDelSpace = dlg_netconf.m_chkDelSpace;
		theApp->nCr2Spc = dlg_netconf.m_chkCr2Spc;

		nTimer = dlg_sysconf.m_nTimer;
		nMoveSpeed = dlg_sysconf.m_nSpeed;
		cBackColor = dlg_sysconf.m_cBackColor;
		cForeColor = dlg_sysconf.m_cForeColor;
		tInterval = dlg_sysconf.m_tInterval;

		bDispTopmost = dlg_sysconf.m_bTopmost;

		nFontPoint = dlg_sysconf.nFontPoint;
		sFontName = dlg_sysconf.sFontName;

		if(dlg_sysconf.m_chk_debug == TRUE)
		{
			// この１行を入れないと、並行動作しているスレッド内で急にデバッグモードとなり、エラーとなる
			if(bInThread)
				bDebugMode = FALSE;
			else
				bDebugMode = TRUE;
		}
		else
			bDebugMode = FALSE;

		if(!bInThread)
		{
			// スレッド内で使用するグローバル変数を設定
			CopyGlobalParam();
			bUpdated = FALSE;		// 情報更新
			bInThread = FALSE;		// スレッド動作中
			if(nTimerID != 0) KillTimer(nTimerID);		// タイマーの一時停止
			if(_beginthread(thread_main,0,NULL)==-1)
			{
				theApp->sTransBuf.LoadString(IDS_ERR_THREAD);	// システム・エラー：スレッドが起動できません
			}
			::Sleep(nTimer*3);							// スレッド起動のため、ウエイト
			nTimerID = SetTimer(nTimerID, nTimer, NULL);	// 指定秒数後、ON_TIMER に制御が移る

			time(&tPrev);								// 更新時間をアップデート
		}
		else
		{
			// スレッドの重複起動は出来ないので、メッセージを表示する
			::MessageBox(NULL, "Winsock Thread still working", "warning", MB_OK);
		}

		// ウインドウの位置
		RECT winRect;
		GetWindowRect(&winRect);
		nPosX = winRect.left;
		nPosY = winRect.top;
		nInfoWndWidth = winRect.right - winRect.left;
		nInfoWndHeight = winRect.bottom - winRect.top;

		if(bDispTopmost)
			SetWindowPos(&wndTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
		else
			SetWindowPos(&wndNoTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);

		//****************
		// レジストリ自動アップデート
		//****************
		// レジストリに保存
		CInfoBar00App *theApp;
		theApp = (CInfoBar00App *)AfxGetApp();
		theApp->RegConfigWrite(this);
	}

}

// **********************************
// メニューの「ヘルプ」を選んだときの処理
//
// **********************************
void CInfoBar00Dlg::OnMenuHelp() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnHelp();
}


//*********************
// メニューの「終了」を選んだときの処理
//
// メニューのクラスを、既存の CDialog から派生している
//*********************
void CInfoBar00Dlg::OnMenuExit() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	// ウインドウに WM_CLOSE メッセージを送り、終了させる
	PostMessage(WM_CLOSE, 0, 0);
	
}

//*********************
// ウインドウ終了処理
//
// 仮想スクリーンと描画DCの解放。タイマーの停止
// WM_CLOSE -> WM_DESTROY の順に呼ばれる
//*********************
void CInfoBar00Dlg::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	EndDialogSeq();		// 終了時の一連の処理

	CDialog::OnClose();
}

//*********************
// ウインドウ終了処理
//
// 仮想スクリーンと描画DCの解放。タイマーの停止
//*********************
void CInfoBar00Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	EndDialogSeq();		// 終了時の一連の処理
}

//*********************
// 一連の終了時の処理
//
//*********************
void CInfoBar00Dlg::EndDialogSeq()
{
	if(IsMemDC_active)
	{
		MemBmp.DeleteObject();
		MemDC.DeleteDC();
	}
	if(IsInfoWndDC_active)
		ReleaseDC(InfoWndDC);	// GetDC の解放

	// WM_DESTROY で 重複消去を防止するための措置
	IsInfoWndDC_active = FALSE;
	IsMemDC_active = FALSE;

	// タイマーの削除
	if(nTimerID != 0) KillTimer(nTimerID);
	nTimerID = 0;

	// ウインドウの位置を保存
	RECT winRect;
	GetWindowRect(&winRect);
	nPosX = winRect.left;
	nPosY = winRect.top;
	nInfoWndWidth = winRect.right - winRect.left;
	nInfoWndHeight = winRect.bottom - winRect.top;

}


// **********************************
// コマンドのインターセプト
//
// ESCキーとRETURN キーの横取り
//
// OnCommand(WPARAM wParam, LPARAM lParam) を利用すると、強制終了時にうまく働かない
//
// **********************************
BOOL CInfoBar00Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{
		case VK_RETURN:  //  Enterキー押下
			//  独自の処理

			//  デフォルトの動作をさせたくない時はTRUEを返す
			return TRUE;

		case VK_ESCAPE:  //  Escキー押下
			//  独自の処理

			//  デフォルトの動作をさせたくない時はTRUEを返す
			return TRUE;
		default:
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

// **********************************
// ダイアログのサイズが変更されたとき
//
// **********************************
void CInfoBar00Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if(!bWndSizeChanged) bWndSizeChanged = TRUE;		// 次の描画で反映させるため
}

// **********************************
// ダイアログをダブルクリックしたとき
//
// 現在表示しているニュースのＵＲＬをブラウザで開きます。
// **********************************
void CInfoBar00Dlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CString sTmpURL;
	CInfoBar00App *theApp;
	theApp = (CInfoBar00App *)AfxGetApp();

	if(theApp->nMode == 0)
	{	// 通常モード以外では、ブラウザ表示は行わない

		// 先頭に http:// を付ける処理
		if(theApp->sURL.Find("http://", 0) != 0)
			sTmpURL = "http://" + theApp->sURL;
		else
			sTmpURL = theApp->sURL;

		HINSTANCE hInst = ::ShellExecute(NULL, "open", sTmpURL, NULL, NULL, SW_SHOWNORMAL);
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}


// **********************************
// 仮想ビットマップ上の画像を、ＬＣＤ風にする（テスト関数）
//
// 実際には使用していない。
// **********************************
#define DOT_PITCH 3
void CInfoBar00Dlg::MakeLCD()
{
	int i,j,k,m;
	BOOL flag;
	// 右端より、左に向かって検査
	// FillRect が 右と下の境界線を塗りつぶしていないため補正
	for(i=0; i<(int)MemStrLength+2; i+=DOT_PITCH)
	{
		for(j=0; j<BITMAP_CY-2; j+=DOT_PITCH)
		{
			flag = FALSE;
			for(k=0; k<DOT_PITCH; k++)
			{
				for(m=0; m<DOT_PITCH; m++)
				{
					if(MemDC.GetPixel(i+k,j+m) == cForeColor)
					{
						flag = TRUE;
						break;
					}
				}
			}
			if(flag)
			{
				for(k=0; k<DOT_PITCH; k++)
				{
					for(m=0; m<DOT_PITCH; m++)
					{
						if(k == DOT_PITCH-1 || m == DOT_PITCH-1)
							MemDC.SetPixel(i+k,j+m, cBackColor);
						else
							MemDC.SetPixel(i+k,j+m, cForeColor);
					}
				}
			}
		}
		if(MemDC.GetPixel(i,j) == cForeColor) break;
	}

	MemStrLength = i;

}

// **********************************
// ＭＦＣで使われている初期値類をスレッド用のグローバル変数にコピーする
//
// **********************************
#define		RECVSIZE	5000		// 一度に受信できる最大サイズ
#define		URLSIZE		2048		// URL のサイズ
#define		TITLESIZE	1024		// タイトル文字列のサイズ
#define		ITEMSIZE	2048		// 証券データのコードリストのサイズ
//#define		BUFSIZE		100*1024	// ＨＴＭＬ受信バッファのサイズ (100kbytes)
#define		BUFSIZE		150*1024	// ＨＴＭＬ受信バッファのサイズ (150kbytes)	2003/06/01
//#define		TMPSIZE		2048		// 文字列取り出し用の一時バッファサイズ
#define		TMPSIZE		4096		// 文字列取り出し用の一時バッファサイズ		2003/06/01

#define		PADSIZE		128			// バッファがオーバーフローしないためのパディングサイズ


void CInfoBar00Dlg::CopyGlobalParam()
{
	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	//  str「n」cpy (Buffer Overflow対策 2003/06/07)

	strncpy(_sTransBuf, theApp->sTransBuf, BUFSIZE-1);	// スレッドとダイアログの通信用
	strncpy(_sURL, theApp->sURL, URLSIZE-1);			// スレッドに渡す URL
	strncpy(_sProxy, theApp->sProxy, URLSIZE-1);		// スレッドに渡す PROXY
	strncpy(_sPhHeader, theApp->sPhHeader, URLSIZE);	// 切り分け用 ヘッダー文字列
	strncpy(_sPhHeaderB, theApp->sPhHeaderB, URLSIZE);	// 切り分け用 ヘッダー文字列 B
	strncpy(_sPhHeaderC, theApp->sPhHeaderC, URLSIZE);	// 切り分け用 ヘッダー文字列 C
	strncpy(_sTitle, theApp->sTitle, TITLESIZE-1);		// タイトル
	strncpy(_sItems, theApp->sItems, ITEMSIZE-1);		// 証券データの指定などに使うアイテム

	_nMode = theApp->nMode;						// 動作モード（通常受信・証券受信…）
	_nPort = theApp->nPort;						// スレッドに渡す ポート番号
	_nPhSkip = theApp->nPhSkip;					// 切り分け用 スキップ個数
	_nPhGetcount = theApp->nPhGetcount;			// 切り分け用 取得数
	_nDelSpace = theApp->nDelSpace;				// 重複空白文字の削除モード		2003/06/01
	_nCr2Spc = theApp->nCr2Spc;					// 全データ１行化（改行→空白）モード		2003/06/02

}

