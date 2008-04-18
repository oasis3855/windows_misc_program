// SerialCom.cpp : シリアル通信クラス
//

// ***********************
// シリアル通信クラス（サンプル版） ver 0.1
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

#include "StdAfx.h"
#include ".\serialcom.h"

/********
デバッグ用ファイル出力を行う場合、コメントアウトする
（ファイル名は、コンストラクタで設定する）
********/
// #define __DEBUG_FILE 1


/********
CSerialCom()
コンストラクタ

内部変数の初期値設定
（この関数では、まだシリアルポートは開かれない）
（デバッグ用ファイル出力を開く）
********/
CSerialCom::CSerialCom(void)
: fileDebugOut(NULL)
{
	// クラス内変数の初期化
	m_hComm = 0;		// ファイルハンドルが割り当てられていない時、ゼロ
	nLineStart = 0;		// 次の切り出しデータの先頭ポインタを初期化
	nLineSize = 0;		// 読み込み済みRawデータのサイズを初期化
	nSizeSaved = 0;
	nSizePrevSaved = 0;	// 1行分のデータを複数回で読み取る場合、バッファにすでに格納されているデータサイズの初期化

	#ifdef __DEBUG_FILE
	//デバッグ用ファイルを開く
	fileDebugOut = NULL;
	fileDebugOut = ::fopen("\\SerialDebug.txt", "wb");
	#endif

}


/********
~CSerialCom()
デストラクタ

（デバッグ用ファイル出力を閉じる）
********/
CSerialCom::~CSerialCom(void)
{
	// ポートが開きっぱなしの場合、閉じる
	if(m_hComm) Close();

	#ifdef __DEBUG_FILE
	//デバッグ用ファイルを閉じる
	if(fileDebugOut) fclose(fileDebugOut);
	#endif

}


/********
Open()
シリアルポートを開く

入力）
  ポート名（Windowsの時は "\\\\.\\COM1"、CEの時は "COM1:"）：lpsPortName
  速度：_BaudRate

出力）ファイルハンドル：m_hComm

戻り値）
  正常時：1
  エラー時：0
********/
int CSerialCom::Open(LPCTSTR lpsPortName, DWORD _BaudRate, BYTE _ByteSize, BYTE _Parity, BYTE _StopBits, DWORD _RtsControl)
{
	DCB _Dcb;
	COMMTIMEOUTS _Timeouts;

	if(m_hComm) return 0;		// ファイルハンドルが既に割り当てられている時、エラー

	// COMポートを開く
	m_hComm = ::CreateFile(lpsPortName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING,	FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_hComm == INVALID_HANDLE_VALUE)
	{	// COMポートが開けない時
		m_hComm = 0;

		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		fputs("\r\n==Open>CreateFile: Error INVALID_HANDLE_VALUE ==\r\n", fileDebugOut);
		#endif

		return 0;
	}

	// COMポートのタイムアウト時間の設定
	if(!::GetCommTimeouts(m_hComm, &_Timeouts))
	{
		Close();

		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		fputs("\r\n==Open>GetCommTimeouts: Error ==\r\n", fileDebugOut);
		#endif

		return 0;
	}
	_Timeouts.ReadTotalTimeoutMultiplier = 0;
	_Timeouts.ReadTotalTimeoutConstant = 5000;		// 5秒
	if(!::SetCommTimeouts(m_hComm, &_Timeouts))
	{
		Close();

		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		fputs("\r\n==Open>SetCommTimeouts: Error ==\r\n", fileDebugOut);
		#endif

		return 0;
	}


	// COMポートの設定
	::ZeroMemory(&_Dcb, sizeof(_Dcb));
	_Dcb.BaudRate = _BaudRate;
	_Dcb.ByteSize = _ByteSize;
	_Dcb.Parity = _Parity;
	_Dcb.StopBits = _StopBits;
	_Dcb.fRtsControl = _RtsControl;
	if(!::SetCommState(m_hComm, &_Dcb))
	{
		Close();

		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		fputs("\r\n==Open>SetCommState: Error ==\r\n", fileDebugOut);
		#endif

		return 0;
	}

	// 入出力バッファを全てクリア
	::PurgeComm(m_hComm, PURGE_RXCLEAR);
	::PurgeComm(m_hComm, PURGE_TXCLEAR);
	::PurgeComm(m_hComm, PURGE_RXABORT);
	::PurgeComm(m_hComm, PURGE_TXABORT);


	return 1;
}


/********
Close()
シリアルポートを閉じる

入力）ファイルハンドル：m_hComm
********/
void CSerialCom::Close(void)
{
	if(m_hComm == 0) return;		// ファイルハンドルが割り当てられていない時

	// 入出力バッファを全てクリア
	::PurgeComm(m_hComm, PURGE_RXCLEAR);
	::PurgeComm(m_hComm, PURGE_TXCLEAR);
	::PurgeComm(m_hComm, PURGE_RXABORT);
	::PurgeComm(m_hComm, PURGE_TXABORT);

	// ファイルハンドルを閉じる
	::CloseHandle(m_hComm);

	m_hComm = 0;
}


/********
ReadRaw()
シリアルポートからデータを読み込む

入力）
　シリアルポートのハンドル：m_hComm

出力）
　データ格納先：BYTE m_DataBuffer[COM_BUFFERSIZE]

戻り値）
  読込完了時：データサイズ（0～データのバイト数）
  エラー時：-1
********/
int CSerialCom::ReadRaw()
{
	DWORD _ErrorMask;
	COMSTAT _Stat;
	DWORD _nReadedSize = 0;

	if(m_hComm == 0) return -1;		// シリアルポートが開かれていない場合、エラー


	// 文字受信イベント、エラーが発生するまで待つ設定を行うとき、コメントアウトする。
	// ただし、通信が切れてしまっている場合、WaitCommEventから戻すことが出来ない場合もある。

/****
	// シリアルポートで監視するイベントを設定（EV_RXCHAR：1文字受信）
	if(!::SetCommMask(m_hComm, EV_RXCHAR|EV_BREAK|EV_ERR))
	{
		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		fputs("\r\n==ReadRaw>SetCommMask: Error ==\r\n", fileDebugOut);
		#endif
		return -1;
	}

	// シリアルポートでイベントが発生するのを待つ
	if(!::WaitCommEvent(m_hComm, &_ErrorMask, NULL))
	{
		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		fputs("\r\n==WaitCommEvent: Error ==\r\n", fileDebugOut);
		#endif
		return -1;
	}
	if((_ErrorMask & EV_BREAK) || (_ErrorMask & EV_ERR))
	{	// 発生したイベントが、文字受信以外だった時
		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		if(_ErrorMask & EV_BREAK) fputs("\r\n==WaitCommEvent: Event EV_BREAK ==\r\n", fileDebugOut);
		if(_ErrorMask & EV_ERR) fputs("\r\n==WaitCommEvent: Event EV_ERR ==\r\n", fileDebugOut);
		#endif
		return -1;
	}

***/

	// シリアルポートのデータバッファに溜まっているデータのサイズ（_Stat.cbInQue）を得る
	if(!::ClearCommError(m_hComm, &_ErrorMask, &_Stat))
	{
		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		fputs("\r\n==ReadRaw>ClearCommError:Errors ", fileDebugOut);
		if(_ErrorMask & CE_BREAK) fputs("CE_BREAK ", fileDebugOut);
		if(_ErrorMask & CE_FRAME) fputs("CE_FRAME ", fileDebugOut);
		if(_ErrorMask & CE_IOE) fputs("CE_IOE ", fileDebugOut);
		if(_ErrorMask & CE_MODE) fputs("CE_MODE ", fileDebugOut);
		if(_ErrorMask & CE_OVERRUN) fputs("CE_OVERRUN ", fileDebugOut);
		if(_ErrorMask & CE_RXOVER) fputs("CE_RXOVER ", fileDebugOut);
		if(_ErrorMask & CE_RXPARITY) fputs("CE_RXPARITY ", fileDebugOut);
		fputs(" ==\r\n", fileDebugOut);
		#endif

		return -1;
	}
	if(_Stat.cbInQue <= 0)
	{	// バッファ上のデータがゼロの時
		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		fputs("\r\n==ReadRaw>ClearCommError: _Stat.cbInQue=0 ==\r\n", fileDebugOut);
		#endif

		return 0;
	}


	// シリアルポートからデータを読み込む
	if(!::ReadFile(m_hComm, &m_DataBuffer, _Stat.cbInQue > COM_BUFFERSIZE ? COM_BUFFERSIZE : _Stat.cbInQue, &_nReadedSize, NULL))
	{
		// 読み込みエラーの時
		if(::GetLastError() == ERROR_IO_PENDING)
		{
			#ifdef __DEBUG_FILE
			//デバッグ用ファイルへデータを出力
			fputs("\r\n==ReadRaw>ReadFile ERROR_IO_PENDING ==\r\n", fileDebugOut);
			#endif
		}
		else
		{
			#ifdef __DEBUG_FILE
			//デバッグ用ファイルへデータを出力
			fputs("\r\n==ReadRaw>ReadFile : Error ==\r\n", fileDebugOut);
			#endif
		}

		return -1;
	}

	#ifdef __DEBUG_FILE
	//デバッグ用ファイルへデータを出力
	fwrite(m_DataBuffer, 1, _nReadedSize, fileDebugOut);
	#endif


	return _nReadedSize;
}

/********
ReadLine(BYTE Delimiter)
シリアルポートからデータを読み込み、デリミタで区切られたデータに分割、1行分のデータを取り出す
（バッファに前のデータが残っている場合は、そこから切り出す。バッファにデータが無い場合は
シリアルポートから読み出す）
デリミタ文字も切り出し後のデータとして出力される
切り出しデータは文字列だけではないので、データの末尾にNULLは付けられない形での出力

※ シリアルポートから読み出したデータが、1行分無い（途中で切れている）場合は、
　 そこまでの出力となる。1行、完全なデータになるまで読み込むのは ReadLineEx関数を使う。

入力）
  デリミタ（データを区切る1バイト）：BYTE Delimiter

内部使用しているクラス変数）
  Rawデータバッファ（ReadRaw関数から受け取る）：BYTE m_DataBuffer[COM_BUFFERSIZE]
  Rawバッファに格納されているデータサイズ：int nLineSize
  次に切り出すデータの先頭ポインタ：int nLineStart

依存）
　ReadRaw() 関数を呼び出して、シリアルポートからまとめてデータを読み込む

出力）
　切り出したデータ：BYTE m_LineBuffer[COM_BUFFERSIZE+1]

戻り値）
  読込完了時：データサイズ（0～データのバイト数）
  エラー時：-1
********/

int CSerialCom::ReadLine(BYTE Delimiter)
{
	int _nWaitCounter = 0;	// 受信失敗をリトライする回数
	int nSize;				// 切り出し後のデータサイズ
	int i;
	DWORD nDummy = 0;

	if(nLineSize == 0)
	{	// バッファに残りのデータが無い場合、シリアルポートからデータを読み込む
		nLineStart = 0;						// 次解析データの先頭を、バッファ先頭へ
		for(;;)
		{
			nLineSize = ReadRaw();			// シリアルポートからデータ読み込み
			if(nLineSize > 0) break;		// データが読み込めた場合、ループ抜ける
			if(nLineSize < 0) return -1;	// エラーのとき、リターンする（エラー）
			// データが読み込めなかった場合（データサイズがゼロの時）
			::Sleep(10);					// 10msec 待つ

			// データが受信できていない場合でも、一定時間おきに上位関数に制御を一旦返す
			// ことによって、ユーザによる処理割り込みを可能にする処理のため
			if(_nWaitCounter++ > 50) return 0;	// 0.5秒待って読み込めないときは、リターン（データサイズゼロ）
		}
	}

	// デリミタ（1バイト）の位置を探す
	for(i=nLineStart; i<nLineSize; i++)
	{
		if(m_DataBuffer[i] == Delimiter) break;
	}
	if(i>=nLineSize) i--;		// バッファの最後まで到達した時、データ長さまで戻す
	
	nSize = i-nLineStart+1;		// デリミタで区切られたデータ1行のバイト数計算
	::memcpy(&m_LineBuffer, m_DataBuffer+nLineStart, nSize);	// 1行分のデータをコピー

	if(i >= nLineSize-1) nLineSize = 0;		// バッファ読み終わりの場合、バッファサイズをゼロにして、次のシリアルポート読み込みに備える
	nLineStart = i+1;			// 次解析のデータの先頭を、今回切り出したデータの一つ後ろにずらす

	return nSize;				// データのバイト数を返す
}


/********
ReadLineEx(BYTE Delimiter)
シリアルポートからデータを読み込み、デリミタで区切られたデータに分割、1行分のデータを取り出す
（バッファに前のデータが残っている場合は、そこから切り出す。バッファにデータが無い場合は
シリアルポートから読み出す）
デリミタ文字も切り出し後のデータとして出力される
切り出しデータは文字列だけではないので、データの末尾にNULLは付けられない形での出力

※ データが1行分になるまで、複数回にわたりシリアルポートからデータを受信する。

入力）
  デリミタ（データを区切る1バイト）：BYTE Delimiter

内部使用しているクラス変数）
  切り出し前のデータ（ReadLine関数から受け取る）：BYTE m_LineBuffer[COM_BUFFERSIZE+1]
  切り出し先バッファm_LineBufferExに一時格納されているデータサイズ：int nSizePrevSaved

依存）
　ReadLine() 関数を呼び出して、1行分データ（シリアル通信単位考慮せず）を読み込んでいる

出力）
　切り出したデータ：BYTE m_LineBufferEx[COM_BUFFERSIZE+1]

戻り値）
  読込完了時：データサイズ（0～データのバイト数）
  エラー時：-1
********/

int CSerialCom::ReadLineEx(BYTE Delimiter)
{
	int nSize;

	nSizeSaved = 0;		// 保存済みデータクリア（データはm_LineBufferEx）


	if(nSizePrevSaved >= 0)
	{	// 前回、サイズ超過で結合付加だった文字列から処理開始

		// 保存済みデータに、前回結合付加文字列をコピー 
		::memcpy(m_LineBufferEx, m_LineBuffer, nSizePrevSaved);
		nSizeSaved = nSizePrevSaved;
		nSizePrevSaved = 0;
	}


	for(;;)
	{
		nSize = this->ReadLine();	// m_LineBufferに読み込む
		if(nSize <= 0) return nSize;	// エラー、または読み込みサイズゼロはいったん帰る

		if(nSize + nSizeSaved < COM_BUFFERSIZE)
		{	// 保存済みデータ ＋ 今回データがバッファサイズ以下のとき、結合

			::memcpy(m_LineBufferEx + nSizeSaved, m_LineBuffer, nSize);
			if(m_LineBufferEx[nSizeSaved + nSize - 1] == Delimiter)
			{	// データ終了デリミタが検出されたとき
				return nSizeSaved + nSize;	// データ1行分読み込み完了
			}

			nSizeSaved += nSize;		// 保存済みデータのサイズ更新

		}
		else
		{
			nSizePrevSaved = nSize;		// 次回、処理開始するデータサイズ（データはm_LineBuffer）
			return nSizeSaved;			// 今回は、すでにたまっているデータを返す（データはm_LineBufferEx）
		}

	}


}

/********
WriteRaw(int nSize)
シリアルポートへデータを書き込む

※ データが1行分（シリアルポートのバッファ最大値）までしか書き込めない

入力）
  書き込みサイズ：int nSize
  書き込みデータ：BYTE m_WriteBuffer[COM_BUFFERSIZE+1]

戻り値）
　成功：1
  失敗：-1

********/

int CSerialCom::WriteRaw(int nSize)
{
	DWORD nWrittenSize;

	if(m_hComm == 0) return -1;		// シリアルポートが開かれていない場合、エラー


	// シリアルポートへデータを書き込む
	if(!::WriteFile(m_hComm, this->m_WriteBuffer, nSize, &nWrittenSize, NULL))
	{
		// 書き込みエラーの時
		#ifdef __DEBUG_FILE
		//デバッグ用ファイルへデータを出力
		fputs("\r\n==WriteRaw>WriteFile ERROR==\r\n", fileDebugOut);
		#endif

		return -1;
	}

	#ifdef __DEBUG_FILE
	//デバッグ用ファイルへデータを出力
	fwrite(buffer, 1, nSize, fileDebugOut);
	#endif

	return 1;
}

// EOF
