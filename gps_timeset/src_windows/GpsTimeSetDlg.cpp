// GpsTimeSetDlg.cpp : メインダイアログ実装ファイル
//

// ***********************
// メインダイアログ、スレッドの処理 ver 1.0.0.1
// 
// Copyright (C) 2008 INOUE. Hirokazu
//
// 
// http://www.opensource.jp/gpl/gpl.ja.html
// このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフトウェ
// ア財団によって発行された GNU 一般公衆利用許諾契約書(バージョン2か、希
// 望によってはそれ以降のバージョンのうちどれか)の定める条件の下で再頒布
// または改変することができます。
// 
// このプログラムは有用であることを願って頒布されますが、*全くの無保証* 
// です。商業可能性の保証や特定の目的への適合性は、言外に示されたものも含
// め全く存在しません。詳しくはGNU 一般公衆利用許諾契約書をご覧ください。
// 
// あなたはこのプログラムと共に、GNU 一般公衆利用許諾契約書の複製物を一部
// 受け取ったはずです。もし受け取っていなければ、フリーソフトウェア財団ま
// で請求してください(宛先は the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA 02111-1307 USA)。
//

#include "stdafx.h"
#include "GpsTimeSet.h"
#include "GpsTimeSetDlg.h"
#include ".\gpstimesetdlg.h"

#include "SerialCom.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGpsTimeSetDlg ダイアログ



CGpsTimeSetDlg::CGpsTimeSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGpsTimeSetDlg::IDD, pParent)
	, m_ThreadSetTime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGpsTimeSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORTNAME, m_ctrlComboPortname);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_ctrlComboBaud);
}

BEGIN_MESSAGE_MAP(CGpsTimeSetDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_COMOPEN, OnBnClickedBtnComopen)
	ON_BN_CLICKED(IDC_BTN_COMCLOSE, OnBnClickedBtnComclose)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SETTIME, OnBnClickedBtnSettime)
END_MESSAGE_MAP()


// CGpsTimeSetDlg メッセージ ハンドラ


/********
OnInitDialog()
ダイアログ表示前の初期設定

内部変数の初期値設定を行う
ポート番号、通信速度ドロップダウンリストの初期設定を行う
********/

BOOL CGpsTimeSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。


	int i;
	char sTemp[7];

	// コンボボックス：COMポートの初期値を設定
	for(i=1; i<20; i++)
	{
		sprintf(sTemp, "COM%d", i);
		m_ctrlComboPortname.AddString(sTemp);
	}

	m_ctrlComboPortname.SetCurSel(3);

	// コンボボックス：COMポート速度の初期値を設定
	m_ctrlComboBaud.AddString(_T("1200"));
	m_ctrlComboBaud.AddString(_T("2400"));
	m_ctrlComboBaud.AddString(_T("4800"));
	m_ctrlComboBaud.AddString(_T("9600"));
	m_ctrlComboBaud.AddString(_T("14400"));
	m_ctrlComboBaud.AddString(_T("19200"));
	m_ctrlComboBaud.AddString(_T("38400"));
	m_ctrlComboBaud.AddString(_T("57600"));
	m_ctrlComboBaud.AddString(_T("115200"));

	m_ctrlComboBaud.SetCurSel(6);


	m_ThreadActivate = 0;					// スレッド停止中
	m_ThreadTerminateCommand = 0;			// スレッド停止命令 無効

	m_ThreadSetTime = 0;					// 時刻設定コマンド 無効

	m_pThread = NULL;		// スレッドポインタ初期化

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CGpsTimeSetDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CGpsTimeSetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/********
OnBnClickedBtnComopen()
受信開始ボタンを押した時の処理

受信スレッド（ThreadMain）を起動する
********/

void CGpsTimeSetDlg::OnBnClickedBtnComopen()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。

	if(m_pThread)
	{	// スレッドが既に存在する場合
		SetDlgItemText(IDC_EDIT_TIME_PC, (LPCTSTR)"Thread Already Running");
	}
	else
	{
		// COMポート番号設定を、コンボボックスより読み取る
		GetDlgItemText(IDC_COMBO_PORTNAME, (LPTSTR)m_sPortName, 6);

		// Baud Rate設定を、コンボボックスより読み取る
		m_nBaudRate = GetDlgItemInt(IDC_COMBO_BAUD);

		// スレッド生成（停止状態で生成）
		m_pThread = ::AfxBeginThread(ThreadMain, (LPVOID)this, 0, 0, CREATE_SUSPENDED, NULL);

		m_pThread->m_bAutoDelete = FALSE;	// スレッド自動破棄フラグクリア
		m_pThread->ResumeThread();			// スレッド動作開始
	}
}


/********
OnBnClickedBtnSettime()
時刻設定ボタンを押した時の処理

受信スレッド（ThreadMain）に対して、時刻設定フラグをセットする
********/

void CGpsTimeSetDlg::OnBnClickedBtnSettime()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。

	m_ThreadSetTime = 1;		// 時刻設定コマンド 割り込みON
}


/********
OnBnClickedBtnComclose()
受信中止ボタンを押した時の処理

受信スレッド（ThreadMain）を停止する
（スレッド停止のためのフラグをセットし、スレッド停止を待つ）
********/

void CGpsTimeSetDlg::OnBnClickedBtnComclose()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	// スレッドが起動していない場合
	if(m_ThreadActivate == 0)
	{
		SetDlgItemText(IDC_EDIT_TIME_PC, (LPCTSTR)"Thread Not Running");
		return;
	}

	//スレッドに終了フラグを送信する
	m_ThreadTerminateCommand = 1;

	// スレッドが終了するのを待つ
	for(int i=0; i<50; i++)
	{
		if(m_ThreadActivate == 0) break;
		::Sleep(100);
	}

	// スレッドが終了しているか再確認
	if(::WaitForSingleObject(m_pThread->m_hThread, 100) == WAIT_TIMEOUT)
	{	// スレッド終了が確認できず、タイムアウトした
		::TerminateThread(m_pThread->m_hThread, 0xffffffff);
	}

	// m_bAutoDelete = FALSE の場合、スレッドのオブジェクトを手動で削除
	delete m_pThread;
	m_pThread = NULL;

	SetDlgItemText(IDC_EDIT_TIME_PC, (LPCTSTR)"Thread Stop");

	m_ThreadActivate = 0;					// スレッド停止中
	m_ThreadTerminateCommand = 0;			// スレッド停止命令 無効

}


/********
OnBnClickedOk()
閉じるボタンを押した時の処理

********/

void CGpsTimeSetDlg::OnBnClickedOk()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。

	// 受信中止ボタンの処理（スレッド停止）を行う
	if(m_ThreadActivate) OnBnClickedBtnComclose();

	OnOK();
}


/********
ThreadMain(LPVOID pParam)
スレッド関数

入力）
  メインダイアログクラスへのポインタ：pParam

依存）
　実際の受信処理は、ThreadDataReceive() 関数を呼び出している

********/

UINT CGpsTimeSetDlg::ThreadMain(LPVOID pParam)
{
	// メインダイアログクラスへのポインタ
	CGpsTimeSetDlg *pDlg = (CGpsTimeSetDlg*)pParam;

	return pDlg->ThreadDataReceive(pParam);
}


/********
ThreadDataReceive(LPVOID pParam)
データ受信ループ

入力）
  メインダイアログクラスへのポインタ：pParam

内部使用しているクラス変数）
  スレッド呼び出し側に、スレッド稼動中を知らせる変数：int m_ThreadActivate
  スレッド呼び出し側から、停止命令を受けるフラグ：int m_ThreadTerminateCommand
  CSerialComで受信したデータを受け取る：BYTE Com.m_LineBufferEx[]

依存）
　シリアル通信のため、CSerialComクラスを生成する

********/

UINT CGpsTimeSetDlg::ThreadDataReceive(LPVOID pParam)
{
	CString sTemp = "";
	char sPortName[10];
	int nSize;

	// メインダイアログクラスへのポインタ
	CGpsTimeSetDlg *pDlg = (CGpsTimeSetDlg*)pParam;

	pDlg->m_ThreadActivate = 1;			// スレッドはアクティブ
	pDlg->m_ThreadTerminateCommand = 0;	// 停止命令をクリア

	CSerialCom Com;

	// シリアルポートを開く
	// Windows の場合は Com.Open("\\\\.\\COM4", 9600);
	// CE の場合は Com.Open(_T("COM4:"), 9600);
	sprintf(sPortName, "\\\\.\\%s", m_sPortName);
	if(!Com.Open(sPortName, m_nBaudRate))
	{
		pDlg->SetDlgItemText(IDC_EDIT_TIME_PC, "COM Open Error");
		return 0;
	}

	// シリアル受信 ループ
	for(;;)
	{
		nSize = Com.ReadLineEx();

		// スレッド停止命令フラグを受け取った場合、ループを抜ける
		if(pDlg->m_ThreadTerminateCommand) break;

		if(nSize == 0) continue;		// 切り出しデータがゼロの時、リトライ
		if(nSize < 0) break;			// エラーのとき、ループを抜ける

		Com.m_LineBufferEx[nSize] = (BYTE)0;	// 文字列末端のNULLを付加

		// NMEAチェックサム合格の時、GGAとZDAの解析と表示処理を行う
		if(NmeaChecksum((LPCSTR)Com.m_LineBufferEx))
		{
			GGA_Process((LPCSTR)Com.m_LineBufferEx, pParam);
			ZDA_Process((LPCSTR)Com.m_LineBufferEx, pParam);
		}
	}

	// シリアルポートを閉じる
	Com.Close();

	pDlg->m_ThreadActivate = 0;			// スレッドは停止
	pDlg->m_ThreadTerminateCommand = 0;	// 停止命令をクリア

	return 0;

}


/********
NmeaChecksum(const char * sNmea)
NMEAセンテンスのチェックサムを検査する

入力）
　解析するNMEAセンテンス：const char * sNmea

戻り値）
  チェックサム一致：1
  チェックサム不一致：0
********/

BOOL CGpsTimeSetDlg::NmeaChecksum(const char * sNmea)
{
	char cksm = 0;
	char sCksm[5];

	// 文字列の先頭から、一文字ずつスキャンしてチェックサム計算
	for(unsigned int i=0; i<strlen(sNmea); i++)
	{
		// '$' と '!' は計算しない（読み飛ばす）
		if(sNmea[i] == '$' || sNmea[i] == '!') continue;

		// '*' が検出されたら終了
		if(sNmea[i] == '*') break;

		cksm = cksm ^ sNmea[i];
	}

	if(i >= strlen(sNmea)) return 0;	// チェックサム デリミタ "*" が無かった

	sprintf(sCksm, "%02X\r\n", cksm);
	if(!stricmp(sNmea+i+1, sCksm))
		return 1;	// チェックサムが一致した場合

	return 0;		// チェックサム不一致
}


/********
GGA_Process(const char * sNmea, LPVOID pParam)
GGA NMEAセンテンスを解析し、ダイアログに表示する。

入力）
　解析するNMEAセンテンス：const char * sNmea
  ダイアログクラスへのポインタ：LPVOID pParam

依存）
　NMEAセンテンスの','で切り分けるため利用：SplitString()
  時刻文字列を解析するため利用：TimeSeparate()
  PCのシステム時間を表示するため利用：PcTimeDisp()

戻り値）
  常に：0

参考）
  NMEAセンテンスの例 （GGA - Global Positioning System Fix Data）
  $GPGGA,031916.000,3512.3456,N,13512.3456,E,1,04,2.6,514.7,M,34.3,M,,0000*5C

  $GPGGA,[時分秒],[緯度],[N/S],[経度],[W/E],[モード],[補足衛星数],[HDOP],[高度],[単位系],[ジオイド補正],[単位系],[DGPSエイジ],[DGPS基地コード]*[チェックサム]

  参照：http://gpsd.berlios.de/NMEA.txt
********/

int CGpsTimeSetDlg::GGA_Process(const char * sNmea, LPVOID pParam)
{
	char *sSeparator = ",*";
	char *sToken;
	CString sTemp;
	int nYear=0, nMonth=0, nDay=0, nHour=0, nMinutes=0, nSecond=0;
	char sSplitBuffer[COM_BUFFERSIZE+1];

	CGpsTimeSetDlg *pDlg = (CGpsTimeSetDlg*)pParam;

	::strcpy(sSplitBuffer, sNmea);
	sToken = SplitString(sSplitBuffer, sSeparator);

	if(::stricmp(sSplitBuffer, "$GPGGA")) return -1;

	// 時刻フィールド
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL)
	{
		pDlg->SetDlgItemText(IDC_EDIT_TIME, "----");
	}
	else
	{
		TimeSeparate(sToken, &nHour, &nMinutes, &nSecond);
		sTemp.Format("%02d:%02d:%02d", nHour, nMinutes, nSecond);
		pDlg->SetDlgItemText(IDC_EDIT_TIME, sTemp);

		PcTimeDisp(nYear, nMonth, nDay, nHour, nMinutes, nSecond, pParam);
	}

	// 緯度フィールド
	sTemp = "";
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) sTemp = sToken;
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) sTemp = sTemp + " " + sToken;
	if(sTemp == "") sTemp = "----";
	pDlg->SetDlgItemText(IDC_EDIT_LATITUDE, sTemp);

	// 経度フィールド
	sTemp = "";
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) sTemp = sToken;
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) sTemp = sTemp + " " + sToken;
	if(sTemp == "") sTemp = "----";
	pDlg->SetDlgItemText(IDC_EDIT_LONGITUDE, sTemp);

	// 受信FIXモード
	sToken = SplitString(NULL, sSeparator);

	// 衛星数フィールド
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_SAT, "----");
	else pDlg->SetDlgItemText(IDC_EDIT_SAT, sToken);

	// HDOPフィールド
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_HDOP, "----");
	else pDlg->SetDlgItemText(IDC_EDIT_HDOP, sToken);

	// 高度フィールド
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_ALTITUDE, "----");
	else pDlg->SetDlgItemText(IDC_EDIT_ALTITUDE, sToken);

	// 単位系モード
	sToken = SplitString(NULL, sSeparator);

	// ジオイド補正フィールド
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_GEOID, "----");
	else pDlg->SetDlgItemText(IDC_EDIT_GEOID, sToken);

	return 0;
}


/********
ZDA_Process(const char * sNmea, LPVOID pParam)
ZDA NMEAセンテンスを解析し、ダイアログに表示する。

入力）
　解析するNMEAセンテンス：const char * sNmea
  ダイアログクラスへのポインタ：LPVOID pParam

依存）
　NMEAセンテンスの','で切り分けるため利用：SplitString()
  時刻文字列を解析するため利用：TimeSeparate()
  PCのシステム時間を表示するため利用：PcTimeDisp()

戻り値）
  常に：0

  参考）
  NMEAセンテンスの例 （ZDA - Time & Date - UTC, day, month, year and local time zone）
  $GPZDA,031921.000,19,04,2008,,*58

  $GPZDA,[時分秒],[日],[月],[年],[+-TZ hour],[TZ minutes]*[チェックサム]

  参照：http://gpsd.berlios.de/NMEA.txt
********/

int CGpsTimeSetDlg::ZDA_Process(const char * sNmea, LPVOID pParam)
{
	char *sSeparator = ",*";
	char *sToken;
	CString sTemp;
	int nYear=0, nMonth=0, nDay=0, nHour=0, nMinutes=0, nSecond=0;
	char sSplitBuffer[COM_BUFFERSIZE+1];


	CGpsTimeSetDlg *pDlg = (CGpsTimeSetDlg*)pParam;

	::strcpy(sSplitBuffer, sNmea);
	sToken = SplitString(sSplitBuffer, sSeparator);

	if(::stricmp(sToken, "$GPZDA")) return -1;

	sTemp = "";

	// 時刻
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL)
		TimeSeparate(sToken, &nHour, &nMinutes, &nSecond);

	// 日
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) nDay = ::atoi(sToken);
	// 月
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) nMonth = ::atoi(sToken);
	// 年
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) nYear = ::atoi(sToken);

	sTemp.Format("%04d/%02d/%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMinutes, nSecond);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_ZDA, sTemp);

	PcTimeDisp(nYear, nMonth, nDay, nHour, nMinutes, nSecond, pParam);

	return 0;
}


/********
TimeSeparate(char * sTimeStr, int *nHour, int *nMinutes, int *nSecond)
時刻（時分秒）文字列を切り分け、変数に代入する関数

入力）
　解析する時分秒センテンス：char * sTimeStr
  切り分けた時分秒を格納する変数：int *nHour, int *nMinutes, int *nSecond

  ※ sTimeStrは破壊されない（strtok利用前に、作業用文字列にコピー）

********/

void CGpsTimeSetDlg::TimeSeparate(char * sTimeStr, int *nHour, int *nMinutes, int *nSecond)
{
	char *sSeparator = ".";
	char *sToken;
	char sTemp[3];
	char sSplitBuffer[COM_BUFFERSIZE+1];

	*nHour = 0;
	*nMinutes = 0;
	*nSecond = 0;

	::strcpy(sSplitBuffer, sTimeStr);				// 破壊を避けるため、作業用文字列にコピー

	sToken = ::strtok(sSplitBuffer, sSeparator);	// '.' で切り分け

	// 時分秒文字列は、6文字であることを確認
	if(::strlen(sToken) != 6) return;

	sTemp[2] = (char)0;		// 時・分・秒は2文字ずつのため、3文字目にNULLをセット

	// 時
	sTemp[0] = sToken[0];
	sTemp[1] = sToken[1];
	*nHour = ::atoi(sTemp);

	// 分
	sTemp[0] = sToken[2];
	sTemp[1] = sToken[3];
	*nMinutes = ::atoi(sTemp);

	// 秒
	sTemp[0] = sToken[4];
	sTemp[1] = sToken[5];
	*nSecond = ::atoi(sTemp);

	return;
}


/********
SplitString(char *sBuf, char *sSeparator)
文字列を、セパレータで切り分ける

※ strtokはセパレータが連続した場合読み飛ばすが、この関数はNULL文字列を返す。
  例：入力文字列が "A,,,B" で ','を用いて切り出す場合
      strtok -> "A","B"
	  SplitString -> "A","","","B"

入力）
　解析する文字列：char *sBuf
    前回の結果から、連続して切り出す場合NULLをセット（strtok仕様と同じ）
  セパレータ：char *sSeparator

  ※ sBufは破壊される（切り出しでNULLが順次挿入される）

********/

char * CGpsTimeSetDlg::SplitString(char *sBuf, char *sSeparator)
{
	static int nEndPt=0;
	static int nStartPt=0;
	static int nStrlen=0;
	static char *sBufCur=NULL;

	// 初回
	if(sBuf != NULL)
	{
		nStrlen = (int)::strlen(sBuf);
		nEndPt = 0;
		sBufCur = sBuf;
	}
	// 連続切り出し
	else
	{
		sBufCur += nEndPt + 1;
	}

	// 文字列末端のとき
	if(sBufCur[0] == (char)NULL) return NULL;

	nEndPt = (int)::strcspn(sBufCur, sSeparator);

	// セパレータが見つからない時は、文字列末端までの長さが返される

	// セパレータをNULLに置換
	if(nEndPt >= 0)
	{
		sBufCur[nEndPt] = (char)NULL;
	}

	nEndPt = (int)::strlen(sBufCur);

	return sBufCur;

}


/********
PcTimeDisp(int nYear, int nMonth, int nDay, int nHour, int nMinutes, int nSecond, LPVOID pParam)
PCのシステム時刻の表示と、システム時刻の変更

入力）
  GPSで得た年月日：int nYear, int nMonth, int nDay, 
  GPSで得た時分秒：int nHour, int nMinutes, int nSecond
  メインダイアログクラスへのポインタ：pParam

内部使用しているクラス変数）
  スレッド呼び出し側から、時刻設定命令を受けるフラグ：int m_ThreadSetTime

依存）
　Win32SDK SetLocalTime は システム時刻変更権限が無い場合、何も処理しない

********/
#define _TIME_FUNCTION_CE	// CE 互換にする場合は宣言する

void CGpsTimeSetDlg::PcTimeDisp(int nYear, int nMonth, int nDay, int nHour, int nMinutes, int nSecond, LPVOID pParam)
{
	SYSTEMTIME tmSystem;	// システム時刻を得る、設定する時に使用
#ifdef _TIME_FUNCTION_CE	// (1) CTime利用時
	CTime tmGpsTime;			// 時差計算用（GPS時刻）
	CTime tmLocalTime;			// 時差計算用（ローカル時刻）
#else						// (2) C time関数利用時 (WinCE不可）
	struct tm tmGpsTime;		// 時差計算用（GPS時刻）
	struct tm *ptmLocalTime;	// 時差計算用（ローカル時刻）
	time_t nTimeTemp;			// 時差計算用
#endif //_TIME_FUNCTION_CE
	TIME_ZONE_INFORMATION tzInfo;
	DWORD nTzResult;
	CString sTemp;

	CGpsTimeSetDlg *pDlg = (CGpsTimeSetDlg*)pParam;

	::GetLocalTime(&tmSystem);		// システムのローカルタイムを得る
	nTzResult = ::GetTimeZoneInformation(&tzInfo);		// システムのタイムゾーン設定を得る

	// システムのローカルタイムをダイアログに表示する
	sTemp.Format("%04d/%02d/%02d %02d:%02d:%02d", tmSystem.wYear, tmSystem.wMonth, tmSystem.wDay, tmSystem.wHour, tmSystem.wMinute, tmSystem.wSecond);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_PC, sTemp);

	// システムのタイムゾーンをダイアログに表示する
	sTemp.Format("%+02.1f", (double)tzInfo.Bias / 60.0);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_TZ, sTemp);

	// 時刻設定
	if(pDlg->m_ThreadSetTime)
	{
		// 
		// 時差の組み入れ計算 （GPS時刻 ＋ TZ ＝ ローカル時刻）
		//

#ifdef _TIME_FUNCTION_CE
		/*****(1) CTime利用時 */
		if(nYear != 0)
		{	// 年月日が与えられている時（ZDA解析からの情報）
			tmGpsTime = CTime(nYear, nMonth, nDay, nHour, nMinutes, nSecond);

			tmLocalTime = tmGpsTime - (CTimeSpan)(tzInfo.Bias*60);

			tmSystem.wYear = tmLocalTime.GetYear();
			tmSystem.wMonth = tmLocalTime.GetMonth();
			tmSystem.wDay = tmLocalTime.GetDay();

			pDlg->m_ThreadSetTime = 0;		// 時刻設定命令フラグをクリア
		}
		else
		{	// 年月日が与えられていない時（GGA解析からの情報）
			tmGpsTime = CTime(tmSystem.wYear, tmSystem.wMonth, tmSystem.wDay, nHour, nMinutes, nSecond);

			tmLocalTime = tmGpsTime - (CTimeSpan)(tzInfo.Bias*60);
		}

		tmSystem.wHour = tmLocalTime.GetHour();
		tmSystem.wMinute = tmLocalTime.GetMinute();
		tmSystem.wSecond = tmLocalTime.GetSecond();
		
#else
		/**** (2) C time関数利用時 (WinCE不可）*/
		tmGpsTime.tm_hour = nHour;
		tmGpsTime.tm_min = nMinutes;
		tmGpsTime.tm_sec = nSecond;
		if(nYear != 0)
		{	// 年月日が与えられている時（ZDA解析からの情報）
			tmGpsTime.tm_year = nYear - 1900;
			tmGpsTime.tm_mon = nMonth - 1;
			tmGpsTime.tm_mday = nDay;

			nTimeTemp = ::mktime(&tmGpsTime);	// 構造体からlong int秒に変換
			nTimeTemp -= tzInfo.Bias * 60;		// 時差（秒）の調整
			ptmLocalTime = ::localtime(&nTimeTemp);	// long int秒から構造体に変換

			tmSystem.wYear = ptmLocalTime->tm_year + 1900;
			tmSystem.wMonth = ptmLocalTime->tm_mon + 1;
			tmSystem.wDay = ptmLocalTime->tm_mday;

			pDlg->m_ThreadSetTime = 0;		// 時刻設定命令フラグをクリア
		}
		else
		{	// 年月日が与えられていない時（GGA解析からの情報）
			tmGpsTime.tm_year = tmSystem.wYear - 1900;	// システム時刻を流用
			tmGpsTime.tm_mon = tmSystem.wMonth - 1;	// システム時刻を流用
			tmGpsTime.tm_mday = tmSystem.wDay;	// システム時刻を流用

			nTimeTemp = ::mktime(&tmGpsTime);	// 構造体からlong int秒に変換
			nTimeTemp -= tzInfo.Bias * 60;		// 時差（秒）の調整
			ptmLocalTime = ::localtime(&nTimeTemp);	// long int秒から構造体に変換
		}

		tmSystem.wHour = ptmLocalTime->tm_hour;
		tmSystem.wMinute = ptmLocalTime->tm_min;
		tmSystem.wSecond = ptmLocalTime->tm_sec;

#endif	// _TIME_FUNCTION_CE

		// システムのローカルタイムを変更する（要：時刻変更権限）
		::SetLocalTime(&tmSystem);

		// ZDAテキストボックスに何も表示されていない時は、GGAで時間設定して終了
		pDlg->GetDlgItemText(IDC_EDIT_TIME_ZDA, sTemp);
		if(sTemp == "")
		{
			pDlg->m_ThreadSetTime = 0;		// 時刻設定命令フラグをクリア
		}
	}

}

//EOF
