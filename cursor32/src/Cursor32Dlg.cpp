// Cursor32Dlg.cpp : インプリメンテーション ファイル
// ダイアログベースアプリケーション用のメインダイアログ

#include "stdafx.h"
#include "Cursor32.h"
//#include "Cursor32Dlg.h"
#include "PropertySheet.h"
#include<math.h>	// 移動距離の積算に浮動小数点を使っている 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCursor32Dlg ダイアログ  アプリケーション．ウインドウ

CCursor32Dlg::CCursor32Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCursor32Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCursor32Dlg)
 		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
 	//}}AFX_DATA_INIT
 	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
}

// DDXは多分使用されない(これはﾒｲﾝｳｲﾝﾄﾞｳだから)
void CCursor32Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCursor32Dlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCursor32Dlg, CDialog)
	//{{AFX_MSG_MAP(CCursor32Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_APP_ABOUT, OnAppAbout)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCursor32Dlg メッセージ ハンドラ

// 初期化
BOOL CCursor32Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: 特別な初期化を行う時はこの場所に追加してください。

	// タイマーの設定
	TimerOK = 1;
	if(!SetTimer(USER_TIMER_ID, timertick, NULL))
	{
		TimerOK = 0;
		AfxMessageBox("タイマーの使用ができないため、このソフトは使用できません。",
						MB_OK|MB_ICONSTOP|MB_APPLMODAL);
		CloseDialog();		// ダイアログの終了	
	}

	// 移動距離のリセット
	Counter=0L;

	// 表示位置、タイトル、その他の見かけの設定
	UpdateAppearance();

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// システムコマンド(ボタン)の処理
void CCursor32Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID == SC_CLOSE) CloseDialog();
	else CDialog::OnSysCommand(nID, lParam);
}

// ダイアログを描画する
void CCursor32Dlg::OnPaint() 
{
	// 表示メインルーチン
	CDialog::OnPaint();
	POINT ppt;
	// マウスカーソルの現在位置を取得
	::GetCursorPos((POINT far *)&ppt);
	// ダイアログに座標を表示
	SetDlgItemInt(IDC_X, ppt.x);
	SetDlgItemInt(IDC_Y, ppt.y);
}

// 終了時の現在座標を保存する
void CCursor32Dlg::CloseDialog() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(TimerOK) KillTimer(USER_TIMER_ID);
	// 最終のウインドウ位置の取得(CWinAppでレジストリに書き込むため)
	RECT winRect;
	GetWindowRect(&winRect);
	x_init = winRect.left;
	y_init = winRect.top;
	EndDialog(IDOK);
}

void CCursor32Dlg::OnClose() 
{
	// このｱﾌﾟﾘｹｰｼｮﾝではOnClose() が呼ばれることは通常無いが、安全のため
	// 終了コマンドを呼んでおく。
	CloseDialog();
}

// タイマーから呼ばれる
// 座標のセットなど
void CCursor32Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	// X,Y 座標のセット
	POINT ppt;
	static int lastX=-1, lastY=-1;
	::GetCursorPos((POINT far *)&ppt);	// マウスの現在位置
	if(ppt.x != lastX || ppt.y != lastY)
	{	// 位置が変わっていた場合のみ、表示更新
		::GetCursorPos((POINT far *)&ppt);
		SetDlgItemInt(IDC_X, ppt.x);
		SetDlgItemInt(IDC_Y, ppt.y);
		if(lastX>=0)
		{	// 最初の1回は走行距離を計算しない
			// 走行距離の積算
			double tmpx=ppt.x-lastX, tmpy=ppt.y-lastY;
			Counter += (unsigned long)(sqrt(tmpx*tmpx + tmpy*tmpy)*10);
		}
		// 一つ前座標のバッファに代入
		lastX = ppt.x;
		lastY = ppt.y;
	}

	// カーソルアシスタンス
	static int newkey=1,lockY=0, lockX=0;
	if(IsAssist)
	{
		if((::GetAsyncKeyState(hotkey1) < 0) &&
				(::GetAsyncKeyState(hotkey2) < 0))
		{	// 正方形モード
			if(newkey)
			{
				lockY=ppt.y; lockX=ppt.x;
			}
			newkey=0;
			if(ppt.x>=lockX)
			{
				if(ppt.y>lockY) ::SetCursorPos(ppt.x,abs(lockY+ppt.x-lockX));
				else ::SetCursorPos(ppt.x,abs(lockY-ppt.x+lockX));
			}
			else
			{
				if(ppt.y<lockY) ::SetCursorPos(ppt.x,abs(lockY+ppt.x-lockX));
				else ::SetCursorPos(ppt.x,abs(lockY-ppt.x+lockX));
			}
		}
		else if(::GetAsyncKeyState(hotkey1) < 0)
		{	// 水平モード
			if(newkey) lockY=ppt.y;
			newkey=0;
			::SetCursorPos(ppt.x,lockY);
		}
		else if(::GetAsyncKeyState(hotkey2) < 0)
		{	// 垂直モード
			if(newkey) lockX=ppt.x;
			newkey=0;
			::SetCursorPos(lockX,ppt.y);
		}
		else newkey=1;
	}
	CDialog::OnTimer(nIDEvent);
}

// 機能設定 ダイアログの呼び出し。
void CCursor32Dlg::OnAppAbout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UINT keycode[] = {VK_SHIFT, VK_CONTROL, VK_MENU, VK_INSERT, VK_DELETE,
			VK_BACK, VK_TAB, VK_F9, VK_F10, VK_F11, VK_F12};
	HICON h_infoicon, h_conficon;

	// プロパティーダイアログ本体
	CPropertySheet DlgProp(IDS_PROPTITLE);	// リソースからタイトル文字列を指定する
	// プロパティシートの各ページをリソースから構築する
	CPropAbout AboutPage;
	CPropConf ConfPage;
	// プロパティシートの各値を設定
	ConfPage.m_ddx_assist = IsAssist;
	ConfPage.m_ddx_title = title;
	ConfPage.m_ddx_timertick = timertick;
	ConfPage.m_ddx_changecolor = nChangeColor;
	ConfPage.m_TextColor = TextColor;
	ConfPage.m_ddx_nChangeDlgColor = nChangeDlgColor;
	ConfPage.m_DlgBackColor = DlgBackColor;
	ConfPage.m_DlgColor = DlgColor;
	ConfPage.m_ddx_isNotTaskbar = isNotTaskbar;
	ConfPage.m_ddx_isTopmost = isTopmost;
	AboutPage.m_ddx_odometer.Format("%ld",Counter/10);
	// ホットキーを設定
	// この設定値を利用して、ON_INITDIALOG で SetCurSelを使用する
	int i;
	for(i=0; i<11; i++)
	{
		if(keycode[i] == hotkey1) ConfPage.m_ddx_hotkey1 = i;
		if(keycode[i] == hotkey2) ConfPage.m_ddx_hotkey2 = i;
	}
	// 現在座標の取得と設定
	RECT winRect;
	GetWindowRect(&winRect);
	x_init = winRect.left;
	y_init = winRect.top;
	ConfPage.m_ddx_x = x_init;
	ConfPage.m_ddx_y = y_init;

	// プロパティシート・タイトルのアイコン設定
	h_infoicon = AfxGetApp()->LoadIcon(IDI_INFOICON);
	h_conficon = AfxGetApp()->LoadIcon(IDI_CONFICON);
	AboutPage.m_psp.hIcon = h_infoicon;
	ConfPage.m_psp.hIcon = h_conficon;
	AboutPage.m_psp.dwFlags |= PSP_USEHICON;
	ConfPage.m_psp.dwFlags |= PSP_USEHICON;
	// プロパティーページの連結
	DlgProp.AddPage(&AboutPage);
	DlgProp.AddPage(&ConfPage);
	// プロパティーシートの属性変更
	DlgProp.m_psh.dwFlags=DlgProp.m_psh.dwFlags|PSH_NOAPPLYNOW;
	DlgProp.SetActivePage(1); // ２ページ目を表にする
	// 機能設定ダイアログを表示する
	if (DlgProp.DoModal() == IDOK && !AboutPage.m_uninstall)
	{	// OK ボタンが押された時
		timertick = ConfPage.m_ddx_timertick;
		title = ConfPage.m_ddx_title;
		IsAssist = ConfPage.m_ddx_assist;
		hotkey1 = keycode[ConfPage.m_ddx_hotkey1];
		hotkey2 = keycode[ConfPage.m_ddx_hotkey2];
		if(hotkey1 == hotkey2)
		{
			AfxMessageBox("２つのホットキーは異なるキーでなければいけません。\n標準設定にもどしました。",
						MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
			hotkey1 = VK_CONTROL;
			hotkey2 = VK_SHIFT;
		}
		nChangeColor = ConfPage.m_ddx_changecolor;
		TextColor = ConfPage.m_TextColor;
		nChangeDlgColor = ConfPage.m_ddx_nChangeDlgColor;
		DlgBackColor = ConfPage.m_DlgBackColor;
		DlgColor = ConfPage.m_DlgColor;
		isNotTaskbar = ConfPage.m_ddx_isNotTaskbar;
		isTopmost = ConfPage.m_ddx_isTopmost;

		UpdateAppearance();
	}
	if(::DestroyIcon(h_infoicon)!=TRUE) MessageBox("csr32 内部エラー ico");
	if(::DestroyIcon(h_conficon)!=TRUE) MessageBox("csr32 内部エラー ico");
	// アンインストール処理がされた後の処理
	if(AboutPage.m_uninstall)
	{
		if(TimerOK) KillTimer(USER_TIMER_ID);
		AfxMessageBox("Cursor32（マウス座標表示）のレジストリ情報は削除されました。",
						MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
		EndDialog(IDOK);
	}
}

// コマンドのインターセプト
BOOL CCursor32Dlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if(wParam == WM_DESTROY) return 1;	// "ｴｽｹｰﾌﾟｷｰ"は処理しない。	
	if(wParam == WM_CREATE) return 1;	// "ﾘﾀｰﾝｷｰ"は処理しない。	
	return CDialog::OnCommand(wParam, lParam);
}

// XY座標のカラー変更
HBRUSH CCursor32Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: この位置で DC のアトリビュートを変更してください
	if(nChangeColor)
	{
		if(nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_MSGBOX)
		{
			// X,Y表示のテキストの時のみ表示色変更
			if(pWnd->GetDlgCtrlID()== IDC_X) pDC->SetTextColor(TextColor);
			if(pWnd->GetDlgCtrlID()== IDC_Y) pDC->SetTextColor(TextColor);
		}
	}
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;	// バックグラウンドカラーは標準のものを使用
}

// ダイアログの見かけを変更する
void CCursor32Dlg::UpdateAppearance()
{
	// 変更をダイアログに反映させる
	SetWindowText(title);
	InvalidateRect(NULL);

	// ウインドウ位置の設定
	RECT wRect;
	// 画面外のときは原点に表示する処理
	GetClientRect(&wRect);
	if(GetSystemMetrics(SM_CXFULLSCREEN) < x_init + wRect.right-wRect.left - 20
			|| x_init<0) x_init=0;
	if(GetSystemMetrics(SM_CYFULLSCREEN) < y_init  + wRect.bottom-wRect.top
			|| y_init<0) y_init=0;
	// ウインドウ位置の設定　＆　常に手前に表示かどうかの設定
	if(isTopmost)
		SetWindowPos(&wndTopMost, x_init,y_init,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);	
	else
	{
		SetWindowPos(&wndBottom, x_init,y_init,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
		SetWindowPos(&wndTop, x_init,y_init,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
	}

	// タスクバーに表示するかどうかの設定
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if(isNotTaskbar) ex_style &= ~WS_EX_APPWINDOW;
	else ex_style |= WS_EX_APPWINDOW;
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style);
	ShowWindow(SW_HIDE);	// 非表示、表示を切り替える
	ShowWindow(SW_SHOW);
}
