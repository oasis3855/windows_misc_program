// SerialCom.h : シリアル通信クラス
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

#pragma once

#define COM_BUFFERSIZE	2048

class CSerialCom
{
public:

	CSerialCom(void);
	~CSerialCom(void);

	HANDLE m_hComm;

	int Open(LPCTSTR lpsPortName, DWORD _BaudRate=9600, BYTE _ByteSize=8, BYTE _Parity=NOPARITY, BYTE _StopBits=ONESTOPBIT, DWORD _RtsControl=RTS_CONTROL_DISABLE);
	void Close(void);

	int ReadLine(BYTE Delimiter=0x0a);
	int ReadLineEx(BYTE Delimiter=0x0a);

	int WriteRaw(int nSize);

	BYTE m_LineBuffer[COM_BUFFERSIZE+1];
	BYTE m_LineBufferEx[COM_BUFFERSIZE+1];

	BYTE m_WriteBuffer[COM_BUFFERSIZE+1];

protected:

	int ReadRaw(void);

	BYTE m_DataBuffer[COM_BUFFERSIZE];
	
	int nLineStart;
	int nLineSize;
	int nSizePrevSaved;
	int nSizeSaved;

	FILE *fileDebugOut;

};
