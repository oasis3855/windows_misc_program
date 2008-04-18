// GpsTimeSetCEDlg.cpp : implementation file
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
#include "GpsTimeSetCE.h"
#include "GpsTimeSetCEDlg.h"

#include "SerialCom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGpsTimeSetCEDlg dialog

CGpsTimeSetCEDlg::CGpsTimeSetCEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGpsTimeSetCEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGpsTimeSetCEDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGpsTimeSetCEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGpsTimeSetCEDlg)
	DDX_Control(pDX, IDC_COMBO_BAUD, m_ctrlComboBaud);
	DDX_Control(pDX, IDC_COMBO_PORTNAME, m_ctrlComboPortname);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGpsTimeSetCEDlg, CDialog)
	//{{AFX_MSG_MAP(CGpsTimeSetCEDlg)
	ON_BN_CLICKED(IDC_BTN_COMOPEN, OnBtnComopen)
	ON_BN_CLICKED(IDC_BTN_SETTIME, OnBtnSettime)
	ON_BN_CLICKED(IDC_BTN_COMCLOSE, OnBtnComclose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGpsTimeSetCEDlg message handlers


/********
OnInitDialog()
ダイアログ表示前の初期設定

内部変数の初期値設定を行う
ポート番号、通信速度ドロップダウンリストの初期設定を行う
********/

BOOL CGpsTimeSetCEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here
	int i;
	char sTemp[7];
	TCHAR wsTemp[7];						// Windows CE Wide-Char 対応用

	// コンボボックス：COMポートの初期値を設定
	for(i=1; i<20; i++)
	{
		sprintf(sTemp, "COM%d", i);
		mbstowcs(wsTemp, sTemp, 6);			// Windows CE 対応, MBCSからWide-Charへ変換
		m_ctrlComboPortname.AddString(wsTemp);
	}

	m_ctrlComboPortname.SetCurSel(5);

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

	m_ThreadWriteInterrupt = 0;				// 書き込み命令無し

	m_ThreadSetTime = 0;					// 時刻設定コマンド 無効

	m_pThread = NULL;		// スレッドポインタ初期化

	return TRUE;  // return TRUE  unless you set the focus to a control
}


/********
OnBnClickedBtnComopen()
受信開始ボタンを押した時の処理

受信スレッド（ThreadMain）を起動する
********/

void CGpsTimeSetCEDlg::OnBtnComopen() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	TCHAR wsTemp[7];		// Windows CE Wide-Char 対応用
	CString sTemp;

	if(m_pThread != NULL)
	{	// スレッドが既に存在する場合
		SetDlgItemText(IDC_EDIT_TIME_PC, _T("Thread is already running"));
	}
	else
	{

		// COMポート番号設定を、コンボボックスより読み取る
		GetDlgItemText(IDC_COMBO_PORTNAME, wsTemp, 6);
		wcstombs((LPSTR)m_sPortName, wsTemp, 6);	// Windows CE 対応, Wide-CharからMBCSへ変換

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

void CGpsTimeSetCEDlg::OnBtnSettime() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_ThreadSetTime = 1;		// 時刻設定コマンド 割り込みON

}


/********
OnBnClickedBtnComclose()
受信中止ボタンを押した時の処理

受信スレッド（ThreadMain）を停止する
（スレッド停止のためのフラグをセットし、スレッド停止を待つ）
********/

void CGpsTimeSetCEDlg::OnBtnComclose() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	// スレッドが起動していない場合
	if(m_ThreadActivate == 0)
	{
		SetDlgItemText(IDC_EDIT_TIME_PC, _T("Thread Not Running"));
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

	SetDlgItemText(IDC_EDIT_TIME_PC, _T("Thread Stop"));

	m_ThreadActivate = 0;					// スレッド停止中
	m_ThreadTerminateCommand = 0;			// スレッド停止命令 無効

}


/********
OnOk()
閉じるボタンを押した時の処理

********/

void CGpsTimeSetCEDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	// 受信中止ボタンの処理（スレッド停止）を行う
	if(m_ThreadActivate) OnBtnComclose();

	CDialog::OnOK();
}


/********
ThreadMain(LPVOID pParam)
スレッド関数

入力）
  メインダイアログクラスへのポインタ：pParam

依存）
　実際の受信処理は、ThreadDataReceive() 関数を呼び出している

********/

UINT CGpsTimeSetCEDlg::ThreadMain(LPVOID pParam)
{
	// メインダイアログクラスへのポインタ
	CGpsTimeSetCEDlg *pDlg = (CGpsTimeSetCEDlg*)pParam;

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

UINT CGpsTimeSetCEDlg::ThreadDataReceive(LPVOID pParam)
{
	CString sTemp = "";
	char sPortName[10];
	TCHAR wsTemp[7];					// Windows CE Wide-Char 対応用
	int nSize;

	// メインダイアログクラスへのポインタ
	CGpsTimeSetCEDlg *pDlg = (CGpsTimeSetCEDlg*)pParam;

	pDlg->m_ThreadActivate = 1;			// スレッドはアクティブ
	pDlg->m_ThreadTerminateCommand = 0;	// 停止命令をクリア

	CSerialCom Com;

	// シリアルポートを開く
	// Windows の場合は Com.Open("\\\\.\\COM4", 9600);
	// CE の場合は Com.Open(_T("COM4:"), 9600);
	sprintf(sPortName, "%s:", pDlg->m_sPortName);
	mbstowcs(wsTemp, (LPCSTR)sPortName, 6);			// Windows CE 対応, MBCSからWide-Charへ変換
	if(!Com.Open(wsTemp, pDlg->m_nBaudRate))
	{
		pDlg->SetDlgItemText(IDC_EDIT_TIME_PC, _T("COM Open Error"));
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

BOOL CGpsTimeSetCEDlg::NmeaChecksum(const char * sNmea)
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
	if(!_stricmp(sNmea+i+1, sCksm))
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

int CGpsTimeSetCEDlg::GGA_Process(const char * sNmea, LPVOID pParam)
{
	char *sSeparator = ",*";
	char *sToken;
	CString sTemp;
	int nYear=0, nMonth=0, nDay=0, nHour=0, nMinutes=0, nSecond=0;
	char sSplitBuffer[COM_BUFFERSIZE+1];

	CGpsTimeSetCEDlg *pDlg = (CGpsTimeSetCEDlg*)pParam;

	::strcpy(sSplitBuffer, sNmea);
	sToken = SplitString(sSplitBuffer, sSeparator);

	if(::_stricmp(sSplitBuffer, "$GPGGA")) return -1;

	// 時刻フィールド
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL)
	{
		pDlg->SetDlgItemText(IDC_EDIT_TIME, _T("----"));
	}
	else
	{
		TimeSeparate(sToken, &nHour, &nMinutes, &nSecond);
		sTemp.Format(_T("%02d:%02d:%02d"), nHour, nMinutes, nSecond);
		pDlg->SetDlgItemText(IDC_EDIT_TIME, sTemp);

		PcTimeDisp(nHour, nMinutes, nSecond, pParam);
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
	sTemp = sToken;		// Windows CE Wide-Char対応のため、一旦 CStringに入れる
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_SAT, _T("----"));
	else pDlg->SetDlgItemText(IDC_EDIT_SAT, sTemp);

	// HDOPフィールド
	sToken = SplitString(NULL, sSeparator);
	sTemp = sToken;		// Windows CE Wide-Char対応のため、一旦 CStringに入れる
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_HDOP, _T("----"));
	else pDlg->SetDlgItemText(IDC_EDIT_HDOP, sTemp);

	// 高度フィールド
	sToken = SplitString(NULL, sSeparator);
	sTemp = sToken;		// Windows CE Wide-Char対応のため、一旦 CStringに入れる
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_ALTITUDE, _T("----"));
	else pDlg->SetDlgItemText(IDC_EDIT_ALTITUDE, sTemp);


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

int CGpsTimeSetCEDlg::ZDA_Process(const char * sNmea, LPVOID pParam)
{
	char *sSeparator = ",*";
	char *sToken;
	CString sTemp;
	int nYear=0, nMonth=0, nDay=0, nHour=0, nMinutes=0, nSecond=0;
	char sSplitBuffer[COM_BUFFERSIZE+1];


	CGpsTimeSetCEDlg *pDlg = (CGpsTimeSetCEDlg*)pParam;

	::strcpy(sSplitBuffer, sNmea);
	sToken = SplitString(sSplitBuffer, sSeparator);

	if(::_stricmp(sToken, "$GPZDA")) return -1;

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

	sTemp.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMinutes, nSecond);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_ZDA, sTemp);

	PcTimeDisp(nHour, nMinutes, nSecond, pParam);

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

void CGpsTimeSetCEDlg::TimeSeparate(char * sTimeStr, int *nHour, int *nMinutes, int *nSecond)
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

char * CGpsTimeSetCEDlg::SplitString(char *sBuf, char *sSeparator)
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
PcTimeDisp(int nHour, int nMinutes, int nSecond, LPVOID pParam)
PCのシステム時刻の表示と、システム時刻の変更

入力）
  GPSで得た時分秒：int nHour, int nMinutes, int nSecond
  メインダイアログクラスへのポインタ：pParam

内部使用しているクラス変数）
  スレッド呼び出し側から、時刻設定命令を受けるフラグ：int m_ThreadSetTime

依存）
　Win32SDK SetLocalTime は システム時刻変更権限が無い場合、何も処理しない

********/

void CGpsTimeSetCEDlg::PcTimeDisp(int nHour, int nMinutes, int nSecond, LPVOID pParam)
{
	SYSTEMTIME tmSystem;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD nTzResult;
	double nTz;
	long int nTimeNow;
	CString sTemp;

	CGpsTimeSetCEDlg *pDlg = (CGpsTimeSetCEDlg*)pParam;

	::GetLocalTime(&tmSystem);		// システムのローカルタイムを得る
	nTzResult = ::GetTimeZoneInformation(&tzInfo);		// システムのタイムゾーン設定を得る

	// システムのローカルタイムをダイアログに表示する
	sTemp.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), tmSystem.wYear, tmSystem.wMonth, tmSystem.wDay, tmSystem.wHour, tmSystem.wMinute, tmSystem.wSecond);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_PC, sTemp);

	// システムのタイムゾーンをダイアログに表示する
	nTz = (double)tzInfo.Bias / 60.0;
	sTemp.Format(_T("%+02.1f"), nTz);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_TZ, sTemp);

	// 時刻設定
	if(pDlg->m_ThreadSetTime)
	{
		// GPS時刻にタイムゾーンを加算して、ローカルタイムにする
		nTimeNow = nHour*60*60 + nMinutes*60 + nSecond - tzInfo.Bias*60;

		// 0 ～ 24 時間内に収める処理（ムチャクチャ適当）
		if(nTimeNow < 0) nTimeNow += 60*60*12;		// マイナスになったら、12時間進める
		else if(nTimeNow > 24*60*60) nTimeNow -= 60*60*12;		// 24時間以上は、12時間戻す

		// GPS時刻から得たローカルタイムを、年月日・時分秒変数に代入
		tmSystem.wHour = (WORD)(nTimeNow / 60 / 60);
		tmSystem.wMinute = (WORD)((nTimeNow - tmSystem.wHour*60*60) / 60);
		tmSystem.wSecond = (WORD)(nTimeNow - tmSystem.wHour*60*60 - tmSystem.wMinute*60);

		// システムのローカルタイムを変更する（要：時刻変更権限）
		::SetLocalTime(&tmSystem);

		pDlg->m_ThreadSetTime = 0;
	}

}

//EOF
