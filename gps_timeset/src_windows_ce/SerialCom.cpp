// SerialCom.cpp : �V���A���ʐM�N���X
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

#include "StdAfx.h"
#include ".\serialcom.h"

/********
�f�o�b�O�p�t�@�C���o�͂��s���ꍇ�A�R�����g�A�E�g����
�i�t�@�C�����́A�R���X�g���N�^�Őݒ肷��j
********/
// #define __DEBUG_FILE 1


/********
CSerialCom()
�R���X�g���N�^

�����ϐ��̏����l�ݒ�
�i���̊֐��ł́A�܂��V���A���|�[�g�͊J����Ȃ��j
�i�f�o�b�O�p�t�@�C���o�͂��J���j
********/
CSerialCom::CSerialCom(void)
: fileDebugOut(NULL)
{
	// �N���X���ϐ��̏�����
	m_hComm = 0;		// �t�@�C���n���h�������蓖�Ă��Ă��Ȃ����A�[��
	nLineStart = 0;		// ���̐؂�o���f�[�^�̐擪�|�C���^��������
	nLineSize = 0;		// �ǂݍ��ݍς�Raw�f�[�^�̃T�C�Y��������
	nSizeSaved = 0;
	nSizePrevSaved = 0;	// 1�s���̃f�[�^�𕡐���œǂݎ��ꍇ�A�o�b�t�@�ɂ��łɊi�[����Ă���f�[�^�T�C�Y�̏�����

	#ifdef __DEBUG_FILE
	//�f�o�b�O�p�t�@�C�����J��
	fileDebugOut = NULL;
	fileDebugOut = ::fopen("\\SerialDebug.txt", "wb");
	#endif

}


/********
~CSerialCom()
�f�X�g���N�^

�i�f�o�b�O�p�t�@�C���o�͂����j
********/
CSerialCom::~CSerialCom(void)
{
	// �|�[�g���J�����ςȂ��̏ꍇ�A����
	if(m_hComm) Close();

	#ifdef __DEBUG_FILE
	//�f�o�b�O�p�t�@�C�������
	if(fileDebugOut) fclose(fileDebugOut);
	#endif

}


/********
Open()
�V���A���|�[�g���J��

���́j
  �|�[�g���iWindows�̎��� "\\\\.\\COM1"�ACE�̎��� "COM1:"�j�FlpsPortName
  ���x�F_BaudRate

�o�́j�t�@�C���n���h���Fm_hComm

�߂�l�j
  ���펞�F1
  �G���[���F0
********/
int CSerialCom::Open(LPCTSTR lpsPortName, DWORD _BaudRate, BYTE _ByteSize, BYTE _Parity, BYTE _StopBits, DWORD _RtsControl)
{
	DCB _Dcb;
	COMMTIMEOUTS _Timeouts;

	if(m_hComm) return 0;		// �t�@�C���n���h�������Ɋ��蓖�Ă��Ă��鎞�A�G���[

	// COM�|�[�g���J��
	m_hComm = ::CreateFile(lpsPortName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING,	FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_hComm == INVALID_HANDLE_VALUE)
	{	// COM�|�[�g���J���Ȃ���
		m_hComm = 0;

		#ifdef __DEBUG_FILE
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
		fputs("\r\n==Open>CreateFile: Error INVALID_HANDLE_VALUE ==\r\n", fileDebugOut);
		#endif

		return 0;
	}

	// COM�|�[�g�̃^�C���A�E�g���Ԃ̐ݒ�
	if(!::GetCommTimeouts(m_hComm, &_Timeouts))
	{
		Close();

		#ifdef __DEBUG_FILE
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
		fputs("\r\n==Open>GetCommTimeouts: Error ==\r\n", fileDebugOut);
		#endif

		return 0;
	}
	_Timeouts.ReadTotalTimeoutMultiplier = 0;
	_Timeouts.ReadTotalTimeoutConstant = 5000;		// 5�b
	if(!::SetCommTimeouts(m_hComm, &_Timeouts))
	{
		Close();

		#ifdef __DEBUG_FILE
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
		fputs("\r\n==Open>SetCommTimeouts: Error ==\r\n", fileDebugOut);
		#endif

		return 0;
	}


	// COM�|�[�g�̐ݒ�
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
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
		fputs("\r\n==Open>SetCommState: Error ==\r\n", fileDebugOut);
		#endif

		return 0;
	}

	// ���o�̓o�b�t�@��S�ăN���A
	::PurgeComm(m_hComm, PURGE_RXCLEAR);
	::PurgeComm(m_hComm, PURGE_TXCLEAR);
	::PurgeComm(m_hComm, PURGE_RXABORT);
	::PurgeComm(m_hComm, PURGE_TXABORT);


	return 1;
}


/********
Close()
�V���A���|�[�g�����

���́j�t�@�C���n���h���Fm_hComm
********/
void CSerialCom::Close(void)
{
	if(m_hComm == 0) return;		// �t�@�C���n���h�������蓖�Ă��Ă��Ȃ���

	// ���o�̓o�b�t�@��S�ăN���A
	::PurgeComm(m_hComm, PURGE_RXCLEAR);
	::PurgeComm(m_hComm, PURGE_TXCLEAR);
	::PurgeComm(m_hComm, PURGE_RXABORT);
	::PurgeComm(m_hComm, PURGE_TXABORT);

	// �t�@�C���n���h�������
	::CloseHandle(m_hComm);

	m_hComm = 0;
}


/********
ReadRaw()
�V���A���|�[�g����f�[�^��ǂݍ���

���́j
�@�V���A���|�[�g�̃n���h���Fm_hComm

�o�́j
�@�f�[�^�i�[��FBYTE m_DataBuffer[COM_BUFFERSIZE]

�߂�l�j
  �Ǎ��������F�f�[�^�T�C�Y�i0�`�f�[�^�̃o�C�g���j
  �G���[���F-1
********/
int CSerialCom::ReadRaw()
{
	DWORD _ErrorMask;
	COMSTAT _Stat;
	DWORD _nReadedSize = 0;

	if(m_hComm == 0) return -1;		// �V���A���|�[�g���J����Ă��Ȃ��ꍇ�A�G���[


	// ������M�C�x���g�A�G���[����������܂ő҂ݒ���s���Ƃ��A�R�����g�A�E�g����B
	// �������A�ʐM���؂�Ă��܂��Ă���ꍇ�AWaitCommEvent����߂����Ƃ��o���Ȃ��ꍇ������B

/****
	// �V���A���|�[�g�ŊĎ�����C�x���g��ݒ�iEV_RXCHAR�F1������M�j
	if(!::SetCommMask(m_hComm, EV_RXCHAR|EV_BREAK|EV_ERR))
	{
		#ifdef __DEBUG_FILE
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
		fputs("\r\n==ReadRaw>SetCommMask: Error ==\r\n", fileDebugOut);
		#endif
		return -1;
	}

	// �V���A���|�[�g�ŃC�x���g����������̂�҂�
	if(!::WaitCommEvent(m_hComm, &_ErrorMask, NULL))
	{
		#ifdef __DEBUG_FILE
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
		fputs("\r\n==WaitCommEvent: Error ==\r\n", fileDebugOut);
		#endif
		return -1;
	}
	if((_ErrorMask & EV_BREAK) || (_ErrorMask & EV_ERR))
	{	// ���������C�x���g���A������M�ȊO��������
		#ifdef __DEBUG_FILE
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
		if(_ErrorMask & EV_BREAK) fputs("\r\n==WaitCommEvent: Event EV_BREAK ==\r\n", fileDebugOut);
		if(_ErrorMask & EV_ERR) fputs("\r\n==WaitCommEvent: Event EV_ERR ==\r\n", fileDebugOut);
		#endif
		return -1;
	}

***/

	// �V���A���|�[�g�̃f�[�^�o�b�t�@�ɗ��܂��Ă���f�[�^�̃T�C�Y�i_Stat.cbInQue�j�𓾂�
	if(!::ClearCommError(m_hComm, &_ErrorMask, &_Stat))
	{
		#ifdef __DEBUG_FILE
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
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
	{	// �o�b�t�@��̃f�[�^���[���̎�
		#ifdef __DEBUG_FILE
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
		fputs("\r\n==ReadRaw>ClearCommError: _Stat.cbInQue=0 ==\r\n", fileDebugOut);
		#endif

		return 0;
	}


	// �V���A���|�[�g����f�[�^��ǂݍ���
	if(!::ReadFile(m_hComm, &m_DataBuffer, _Stat.cbInQue > COM_BUFFERSIZE ? COM_BUFFERSIZE : _Stat.cbInQue, &_nReadedSize, NULL))
	{
		// �ǂݍ��݃G���[�̎�
		if(::GetLastError() == ERROR_IO_PENDING)
		{
			#ifdef __DEBUG_FILE
			//�f�o�b�O�p�t�@�C���փf�[�^���o��
			fputs("\r\n==ReadRaw>ReadFile ERROR_IO_PENDING ==\r\n", fileDebugOut);
			#endif
		}
		else
		{
			#ifdef __DEBUG_FILE
			//�f�o�b�O�p�t�@�C���փf�[�^���o��
			fputs("\r\n==ReadRaw>ReadFile : Error ==\r\n", fileDebugOut);
			#endif
		}

		return -1;
	}

	#ifdef __DEBUG_FILE
	//�f�o�b�O�p�t�@�C���փf�[�^���o��
	fwrite(m_DataBuffer, 1, _nReadedSize, fileDebugOut);
	#endif


	return _nReadedSize;
}

/********
ReadLine(BYTE Delimiter)
�V���A���|�[�g����f�[�^��ǂݍ��݁A�f���~�^�ŋ�؂�ꂽ�f�[�^�ɕ����A1�s���̃f�[�^�����o��
�i�o�b�t�@�ɑO�̃f�[�^���c���Ă���ꍇ�́A��������؂�o���B�o�b�t�@�Ƀf�[�^�������ꍇ��
�V���A���|�[�g����ǂݏo���j
�f���~�^�������؂�o����̃f�[�^�Ƃ��ďo�͂����
�؂�o���f�[�^�͕����񂾂��ł͂Ȃ��̂ŁA�f�[�^�̖�����NULL�͕t�����Ȃ��`�ł̏o��

�� �V���A���|�[�g����ǂݏo�����f�[�^���A1�s�������i�r���Ő؂�Ă���j�ꍇ�́A
�@ �����܂ł̏o�͂ƂȂ�B1�s�A���S�ȃf�[�^�ɂȂ�܂œǂݍ��ނ̂� ReadLineEx�֐����g���B

���́j
  �f���~�^�i�f�[�^����؂�1�o�C�g�j�FBYTE Delimiter

�����g�p���Ă���N���X�ϐ��j
  Raw�f�[�^�o�b�t�@�iReadRaw�֐�����󂯎��j�FBYTE m_DataBuffer[COM_BUFFERSIZE]
  Raw�o�b�t�@�Ɋi�[����Ă���f�[�^�T�C�Y�Fint nLineSize
  ���ɐ؂�o���f�[�^�̐擪�|�C���^�Fint nLineStart

�ˑ��j
�@ReadRaw() �֐����Ăяo���āA�V���A���|�[�g����܂Ƃ߂ăf�[�^��ǂݍ���

�o�́j
�@�؂�o�����f�[�^�FBYTE m_LineBuffer[COM_BUFFERSIZE+1]

�߂�l�j
  �Ǎ��������F�f�[�^�T�C�Y�i0�`�f�[�^�̃o�C�g���j
  �G���[���F-1
********/

int CSerialCom::ReadLine(BYTE Delimiter)
{
	int _nWaitCounter = 0;	// ��M���s�����g���C�����
	int nSize;				// �؂�o����̃f�[�^�T�C�Y
	int i;
	DWORD nDummy = 0;

	if(nLineSize == 0)
	{	// �o�b�t�@�Ɏc��̃f�[�^�������ꍇ�A�V���A���|�[�g����f�[�^��ǂݍ���
		nLineStart = 0;						// ����̓f�[�^�̐擪���A�o�b�t�@�擪��
		for(;;)
		{
			nLineSize = ReadRaw();			// �V���A���|�[�g����f�[�^�ǂݍ���
			if(nLineSize > 0) break;		// �f�[�^���ǂݍ��߂��ꍇ�A���[�v������
			if(nLineSize < 0) return -1;	// �G���[�̂Ƃ��A���^�[������i�G���[�j
			// �f�[�^���ǂݍ��߂Ȃ������ꍇ�i�f�[�^�T�C�Y���[���̎��j
			::Sleep(10);					// 10msec �҂�

			// �f�[�^����M�ł��Ă��Ȃ��ꍇ�ł��A��莞�Ԃ����ɏ�ʊ֐��ɐ������U�Ԃ�
			// ���Ƃɂ���āA���[�U�ɂ�鏈�����荞�݂��\�ɂ��鏈���̂���
			if(_nWaitCounter++ > 50) return 0;	// 0.5�b�҂��ēǂݍ��߂Ȃ��Ƃ��́A���^�[���i�f�[�^�T�C�Y�[���j
		}
	}

	// �f���~�^�i1�o�C�g�j�̈ʒu��T��
	for(i=nLineStart; i<nLineSize; i++)
	{
		if(m_DataBuffer[i] == Delimiter) break;
	}
	if(i>=nLineSize) i--;		// �o�b�t�@�̍Ō�܂œ��B�������A�f�[�^�����܂Ŗ߂�
	
	nSize = i-nLineStart+1;		// �f���~�^�ŋ�؂�ꂽ�f�[�^1�s�̃o�C�g���v�Z
	::memcpy(&m_LineBuffer, m_DataBuffer+nLineStart, nSize);	// 1�s���̃f�[�^���R�s�[

	if(i >= nLineSize-1) nLineSize = 0;		// �o�b�t�@�ǂݏI���̏ꍇ�A�o�b�t�@�T�C�Y���[���ɂ��āA���̃V���A���|�[�g�ǂݍ��݂ɔ�����
	nLineStart = i+1;			// ����͂̃f�[�^�̐擪���A����؂�o�����f�[�^�̈���ɂ��炷

	return nSize;				// �f�[�^�̃o�C�g����Ԃ�
}


/********
ReadLineEx(BYTE Delimiter)
�V���A���|�[�g����f�[�^��ǂݍ��݁A�f���~�^�ŋ�؂�ꂽ�f�[�^�ɕ����A1�s���̃f�[�^�����o��
�i�o�b�t�@�ɑO�̃f�[�^���c���Ă���ꍇ�́A��������؂�o���B�o�b�t�@�Ƀf�[�^�������ꍇ��
�V���A���|�[�g����ǂݏo���j
�f���~�^�������؂�o����̃f�[�^�Ƃ��ďo�͂����
�؂�o���f�[�^�͕����񂾂��ł͂Ȃ��̂ŁA�f�[�^�̖�����NULL�͕t�����Ȃ��`�ł̏o��

�� �f�[�^��1�s���ɂȂ�܂ŁA������ɂ킽��V���A���|�[�g����f�[�^����M����B

���́j
  �f���~�^�i�f�[�^����؂�1�o�C�g�j�FBYTE Delimiter

�����g�p���Ă���N���X�ϐ��j
  �؂�o���O�̃f�[�^�iReadLine�֐�����󂯎��j�FBYTE m_LineBuffer[COM_BUFFERSIZE+1]
  �؂�o����o�b�t�@m_LineBufferEx�Ɉꎞ�i�[����Ă���f�[�^�T�C�Y�Fint nSizePrevSaved

�ˑ��j
�@ReadLine() �֐����Ăяo���āA1�s���f�[�^�i�V���A���ʐM�P�ʍl�������j��ǂݍ���ł���

�o�́j
�@�؂�o�����f�[�^�FBYTE m_LineBufferEx[COM_BUFFERSIZE+1]

�߂�l�j
  �Ǎ��������F�f�[�^�T�C�Y�i0�`�f�[�^�̃o�C�g���j
  �G���[���F-1
********/

int CSerialCom::ReadLineEx(BYTE Delimiter)
{
	int nSize;

	nSizeSaved = 0;		// �ۑ��ς݃f�[�^�N���A�i�f�[�^��m_LineBufferEx�j


	if(nSizePrevSaved >= 0)
	{	// �O��A�T�C�Y���߂Ō����t�������������񂩂珈���J�n

		// �ۑ��ς݃f�[�^�ɁA�O�񌋍��t����������R�s�[ 
		::memcpy(m_LineBufferEx, m_LineBuffer, nSizePrevSaved);
		nSizeSaved = nSizePrevSaved;
		nSizePrevSaved = 0;
	}


	for(;;)
	{
		nSize = this->ReadLine();	// m_LineBuffer�ɓǂݍ���
		if(nSize <= 0) return nSize;	// �G���[�A�܂��͓ǂݍ��݃T�C�Y�[���͂�������A��

		if(nSize + nSizeSaved < COM_BUFFERSIZE)
		{	// �ۑ��ς݃f�[�^ �{ ����f�[�^���o�b�t�@�T�C�Y�ȉ��̂Ƃ��A����

			::memcpy(m_LineBufferEx + nSizeSaved, m_LineBuffer, nSize);
			if(m_LineBufferEx[nSizeSaved + nSize - 1] == Delimiter)
			{	// �f�[�^�I���f���~�^�����o���ꂽ�Ƃ�
				return nSizeSaved + nSize;	// �f�[�^1�s���ǂݍ��݊���
			}

			nSizeSaved += nSize;		// �ۑ��ς݃f�[�^�̃T�C�Y�X�V

		}
		else
		{
			nSizePrevSaved = nSize;		// ����A�����J�n����f�[�^�T�C�Y�i�f�[�^��m_LineBuffer�j
			return nSizeSaved;			// ����́A���łɂ��܂��Ă���f�[�^��Ԃ��i�f�[�^��m_LineBufferEx�j
		}

	}


}

/********
WriteRaw(int nSize)
�V���A���|�[�g�փf�[�^����������

�� �f�[�^��1�s���i�V���A���|�[�g�̃o�b�t�@�ő�l�j�܂ł����������߂Ȃ�

���́j
  �������݃T�C�Y�Fint nSize
  �������݃f�[�^�FBYTE m_WriteBuffer[COM_BUFFERSIZE+1]

�߂�l�j
�@�����F1
  ���s�F-1

********/

int CSerialCom::WriteRaw(int nSize)
{
	DWORD nWrittenSize;

	if(m_hComm == 0) return -1;		// �V���A���|�[�g���J����Ă��Ȃ��ꍇ�A�G���[


	// �V���A���|�[�g�փf�[�^����������
	if(!::WriteFile(m_hComm, this->m_WriteBuffer, nSize, &nWrittenSize, NULL))
	{
		// �������݃G���[�̎�
		#ifdef __DEBUG_FILE
		//�f�o�b�O�p�t�@�C���փf�[�^���o��
		fputs("\r\n==WriteRaw>WriteFile ERROR==\r\n", fileDebugOut);
		#endif

		return -1;
	}

	#ifdef __DEBUG_FILE
	//�f�o�b�O�p�t�@�C���փf�[�^���o��
	fwrite(buffer, 1, nSize, fileDebugOut);
	#endif

	return 1;
}

// EOF
