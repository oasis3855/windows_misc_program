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
#define BITMAP_CX	3000
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
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	IsInfoWndDC_active = FALSE;
	IsMemDC_active = FALSE;

}

void CInfoBar00Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoBar00Dlg)
	DDX_Control(pDX, IDC_INFO_AREA, m_ctrl_info_area);
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

	//**************************
	// デバイスコンテキストの生成と初期化
	//**************************

	//**** 描画エリアの DC
//	InfoWndDC = this->GetDC();
	InfoWndDC = m_ctrl_info_area.GetDC();
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
	m_ctrl_info_area.GetWindowRect(&InfoWndRect);
	nInfoWndWidth = InfoWndRect.right - InfoWndRect.left;
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
	sTransBuf.LoadString(IDS_MES_INITIAL);		// InfoBar  :   画面上を右クリックして設定してください\n

	//**************************
	// ウインドウ状態の設定
	//**************************

	// タスクバーに表示するかどうかの設定
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	ex_style &= ~WS_EX_APPWINDOW;
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style);

	// ウインドウ位置の設定
	if(bDispTopmost)
		SetWindowPos(&wndTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
	else
		SetWindowPos(&wndNoTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);

	// 初期ビットマップの描画
	MemDC.SelectObject(BrushInfoBack);
	MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, cBackColor);
	MemDC.SetBkColor(cBackColor);
	MemDC.SetTextColor(cForeColor);
	MemDC.TextOut(nInfoWndWidth,5,"initialize ...");
	CalcMemStrLength();


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

	// ESCで終了処理しないためのフラグ
	bExitSeq = FALSE;

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

	//****************
	// 自動アップデート
	//****************
	if(tPrev != 0)
	{	// tPrev=0 は、起動時を表す
		time(&tNow);
		if(tNow - tPrev > tInterval)
		{	// インターバル時間を過ぎていれば、自動アップデート
			bUpdated = FALSE;		// 情報更新
			bInThread = FALSE;		// スレッド動作中
			KillTimer(nTimerID);
			if(_beginthread(thread_main,0,NULL)==-1)
			{
				sTransBuf.LoadString(IDS_ERR_THREAD);	// システム・エラー：スレッドが起動できません
			}

			::Sleep(nTimer*3);
			nTimerID = SetTimer(nTimerID, nTimer, NULL);	// 指定秒数後、ON_TIMER に制御が移る

			time(&tPrev);
		}
	}

	//****************
	// 描画
	//****************
	if(sTransBuf == "" || sTransBuf == "\n") sTransBuf.LoadString(IDS_MES_NODATA);	// データ無し\n
	if(bUpdated)
	{
		ScrCurPos = 0;
		_nTimCurArticle = 0;
		_sCurStr = sTransBuf.Mid(0, sTransBuf.Find("\n", 0));
		MemDC.SelectObject(BrushInfoBack);
		MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, cBackColor);
		MemDC.SetBkColor(cBackColor);
		MemDC.SetTextColor(cForeColor);
		MemDC.TextOut(nInfoWndWidth,5,_sCurStr);
		CalcMemStrLength();
		bUpdated = FALSE;
	}
	if(ScrCurPos > (int)MemStrLength)
	{	// １行アップデート
		ScrCurPos = 0;
		_nTimCurArticle++;
		for(i=0,k=0; k<_nTimCurArticle; k++)
		{
			i = sTransBuf.Find("\n", i) + 1;
			j = sTransBuf.Find("\n", i);
		}
		if(j<=0 || i<=0)
		{
			ScrCurPos = 0;
			_nTimCurArticle = 0;
			i=0;
		}
		_sCurStr = sTransBuf.Mid(i, sTransBuf.Find("\n", i)-i);
		MemDC.SelectObject(BrushInfoBack);
		MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, cBackColor);
		MemDC.SetBkColor(cBackColor);
		MemDC.SetTextColor(cForeColor);
		MemDC.TextOut(InfoWndRect.right,5,_sCurStr);
		CalcMemStrLength();

	}

	InfoWndDC->BitBlt(0,0,nInfoWndWidth,InfoWndRect.bottom-InfoWndRect.top,
						&MemDC,ScrCurPos,0,SRCCOPY);

	ScrCurPos += nMoveSpeed;		// 移動するドット数
	
	CDialog::OnTimer(nIDEvent);
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
void CInfoBar00Dlg::CalcMemStrLength()
{
	int i,j;
	// 右端より、左に向かって検査
	// FillRect が 右と下の境界線を塗りつぶしていないため補正
	for(i=BITMAP_CX-2; i>=0; i--)
	{
		for(j=0; j<BITMAP_CY-2; j++)
		{
			if(MemDC.GetPixel(i,j) == cForeColor) break;
		}
		if(MemDC.GetPixel(i,j) == cForeColor) break;
	}

	MemStrLength = i;
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

	dlg_netconf.m_sURL = sURL;
	dlg_netconf.m_nPort = nPort;
	dlg_netconf.m_sPhHeader = sPhHeader;
	dlg_netconf.m_nPhSkip = nPhSkip;
	dlg_netconf.m_nPhGetcount = nPhGetcount;
	dlg_netconf.m_sProxy = sProxy;
	dlg_netconf.m_sTitle = sTitle;

	dlg_sysconf.m_nTimer = nTimer;
	dlg_sysconf.m_nSpeed = nMoveSpeed;
	dlg_sysconf.m_cBackColor = cBackColor;
	dlg_sysconf.m_cForeColor = cForeColor;
	dlg_sysconf.m_tInterval = tInterval;

	dlg_sysconf.m_bTopmost = bDispTopmost;

	// プロパティーページの連結
	DlgProp.AddPage(&dlg_netconf);
	DlgProp.AddPage(&dlg_sysconf);
	DlgProp.AddPage(&dlg_about);
	DlgProp.SetActivePage(0); // ２ページ目を表にする

	if(DlgProp.DoModal() == IDOK)
	{
		sURL = dlg_netconf.m_sURL;
		nPort = dlg_netconf.m_nPort;
		sPhHeader = dlg_netconf.m_sPhHeader;
		nPhSkip = dlg_netconf.m_nPhSkip;
		nPhGetcount = dlg_netconf.m_nPhGetcount;
		sProxy = dlg_netconf.m_sProxy;
		sTitle = dlg_netconf.m_sTitle;

		nTimer = dlg_sysconf.m_nTimer;
		nMoveSpeed = dlg_sysconf.m_nSpeed;
		cBackColor = dlg_sysconf.m_cBackColor;
		cForeColor = dlg_sysconf.m_cForeColor;
		tInterval = dlg_sysconf.m_tInterval;

		bDispTopmost = dlg_sysconf.m_bTopmost;


		bUpdated = FALSE;		// 情報更新
		bInThread = FALSE;		// スレッド動作中
		KillTimer(nTimerID);
		if(_beginthread(thread_main,0,NULL)==-1)
		{
			sTransBuf.LoadString(IDS_ERR_THREAD);	// システム・エラー：スレッドが起動できません
		}

		::Sleep(nTimer*3);
		nTimerID = SetTimer(nTimerID, nTimer, NULL);	// 指定秒数後、ON_TIMER に制御が移る

		time(&tPrev);

		// ウインドウの位置
		RECT winRect;
		GetWindowRect(&winRect);
		nPosX = winRect.left;
		nPosY = winRect.top;

		if(bDispTopmost)
			SetWindowPos(&wndTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
		else
			SetWindowPos(&wndNoTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);

	}

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

	KillTimer(nTimerID);

	// ウインドウの位置を保存
	RECT winRect;
	GetWindowRect(&winRect);
	nPosX = winRect.left;
	nPosY = winRect.top;

}


//*********************
// メニューの「終了」を選んだときの処理
//
// メニューのクラスを、既存の CDialog から派生している
//*********************
void CInfoBar00Dlg::OnMenuExit() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	bExitSeq = TRUE;

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
// コマンドのインターセプト
//
// ESCキーとRETURN キーの横取り
// **********************************

BOOL CInfoBar00Dlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if(wParam == WM_DESTROY && !bExitSeq) return 1;	// "ｴｽｹｰﾌﾟｷｰ"は処理しない。	
	if(wParam == WM_CREATE) return 1;	// "ﾘﾀｰﾝｷｰ"は処理しない。	
	
	return CDialog::OnCommand(wParam, lParam);
}

