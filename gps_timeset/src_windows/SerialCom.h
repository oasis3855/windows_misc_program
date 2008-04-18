// SerialCom.h : �V���A���ʐM�N���X
//

// ***********************
// �V���A���ʐM�N���X�i�T���v���Łj ver 0.1
// 
// Copyright (C) 2008 INOUE. Hirokazu
//
// 
// http://www.opensource.jp/gpl/gpl.ja.html
// ���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t�g�E�F
// �A���c�ɂ���Ĕ��s���ꂽ GNU ��ʌ��O���p�����_��(�o�[�W����2���A��
// �]�ɂ���Ă͂���ȍ~�̃o�[�W�����̂����ǂꂩ)�̒�߂�����̉��ōĔЕz
// �܂��͉��ς��邱�Ƃ��ł��܂��B
// 
// ���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖��ۏ�* 
// �ł��B���Ɖ\���̕ۏ؂����̖ړI�ւ̓K�����́A���O�Ɏ����ꂽ���̂���
// �ߑS�����݂��܂���B�ڂ�����GNU ��ʌ��O���p�����_�񏑂��������������B
// 
// ���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU ��ʌ��O���p�����_�񏑂̕��������ꕔ
// �󂯎�����͂��ł��B�����󂯎���Ă��Ȃ���΁A�t���[�\�t�g�E�F�A���c��
// �Ő������Ă�������(����� the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA 02111-1307 USA)�B
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
