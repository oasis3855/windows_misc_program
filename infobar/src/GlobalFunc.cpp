
#include "stdafx.h"

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <time.h>
//#include "resource.h"

#include "GlobalFunc.h"

#include <mbstring.h>		// ************** JIS �R�[�h�ϊ��p


//********************
// ������o�b�t�@�̃T�C�Y����p
//********************
#define		RECVSIZE	5000		// ��x�Ɏ�M�ł���ő�T�C�Y
#define		URLSIZE		2048		// URL �̃T�C�Y
#define		TITLESIZE	1024		// �^�C�g��������̃T�C�Y
#define		ITEMSIZE	2048		// �،��f�[�^�̃R�[�h���X�g�̃T�C�Y
#define		BUFSIZE		100*1024	// �g�s�l�k��M�o�b�t�@�̃T�C�Y (100kbytes)
#define		TMPSIZE		2048		// ��������o���p�̈ꎞ�o�b�t�@�T�C�Y

#define		PADSIZE		128			// �o�b�t�@���I�[�o�[�t���[���Ȃ����߂̃p�f�B���O�T�C�Y

/*
 * WinSock 2 extension -- manifest constants for shutdown()
 */
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02


//********************
// �O���[�o���ϐ�
// �O���Ƃ̕ϐ��󂯓n���p
//********************
char _sTransBuf[BUFSIZE + PADSIZE];		// �X���b�h�ƃ_�C�A���O�̒ʐM�p
char _sURL[URLSIZE + PADSIZE];			// �X���b�h�ɓn�� URL
char _sProxy[URLSIZE + PADSIZE];		// �X���b�h�ɓn�� PROXY
char _sPhHeader[URLSIZE + PADSIZE];		// �؂蕪���p �w�b�_�[������
char _sPhHeaderB[URLSIZE + PADSIZE];	// �؂蕪���p �w�b�_�[������ B
char _sPhHeaderC[URLSIZE + PADSIZE];	// �؂蕪���p �w�b�_�[������ C
char _sTitle[TITLESIZE + PADSIZE];		// �^�C�g��
char _sItems[ITEMSIZE + PADSIZE];		// �،��f�[�^�̎w��ȂǂɎg���A�C�e��

BOOL bUpdated;		// �X���b�h�ōX�V�����������ʒm
BOOL bInThread;		// �X���b�h���쒆�̃t���O
int nThreadError;	// �X���b�h�ŋN�������G���[�̎�ނ�Ԃ�
int _nMode;			// ���샂�[�h�i�ʏ��M�E�،���M�c�j
UINT _nPort;		// �X���b�h�ɓn�� �|�[�g�ԍ�
int _nPhSkip;		// �؂蕪���p �X�L�b�v��
int _nPhGetcount;	// �؂蕪���p �擾��

BOOL bDebugMode;		// �f�o�b�O���[�h


//********************
// �����݂̂Ŏg���A�֐��Q�̐錾
//********************
int _inet_norm_process(char *sCompose, char *sHTML, char *szTemp, FILE **fo);
int _inet_stock_process(char *sCompose, char *sHTML, char *szTemp, FILE **fo);

BOOL receive_HTML(char *sRURL, char *sHTML, char *szTemp, FILE **fo);		// HTML�t�@�C���̎�M
int _str_find(char *szOrg, char *szKey, int seek);
void _conv_euc_to_sjis(char *buf);
void _add_title(char *sReturn, char *sHTML, char *sTitle, char *szTemp);


void _dbg_openfile(FILE **fo, char *szTemp);
void _dbg_write_init(FILE **fo, char *szTemp);
void _dbg_write_formated(FILE **fo, char *szBuf1, char *szBuf2);
void _dbg_write_formated_int(FILE **fo, char *szBuf1, int x);
void _dbg_bufferovrn_check(FILE **fo, char *szStr, int _sLen0, int _sPad, char *szTitle);

//********************
// �X���b�h�֐�
// (�ʐM������S���j
//********************
void thread_main(void *pVoid)
{
	char		sHTML[BUFSIZE + PADSIZE];			// HTML ��M�o�b�t�@
	char		sCompose[BUFSIZE + PADSIZE];		// ��͌�̕�����i�ꎞ�ۊǁj
	char		szTemp[TMPSIZE + PADSIZE];			// �ꎞ�o�b�t�@
	FILE		*fo;

	// �X���b�h�̏d���N���͔�����
	if(bInThread == TRUE)
		_endthread();


	//***********************
	// �����l�̐ݒ�
	//***********************
	bInThread = TRUE;			// �X���b�h���쒆
	bUpdated = FALSE;			// �X�V���I����Ă��Ȃ����Ƃ�����

	memset(sHTML, NULL, BUFSIZE+PADSIZE);
	memset(sCompose, NULL, BUFSIZE+PADSIZE);
	memset(szTemp, NULL, TMPSIZE+PADSIZE);

	strcpy(sHTML, "");
	strcpy(sCompose, "");


	if(strlen(_sPhHeader) == 0)
	{
		strcpy(_sTransBuf, "Error : Pharse Key is NULL\n");
		bUpdated = TRUE;
		bInThread = FALSE;		// �X���b�h���쒆
		_endthread();
	}

	if(bDebugMode)
	{
		fo = NULL;		// �t�@�C���|�C���^�� NULL �����蓖�ĂĂ���
		_dbg_openfile(&fo, szTemp);
		_dbg_write_init(&fo, szTemp);
	}

	if(strlen(_sURL) == 0)
	{
		strcpy(_sTransBuf, "Error : URL is NULL\n");
		bUpdated = TRUE;
		bInThread = FALSE;		// �X���b�h���쒆
		if(bDebugMode && fo != NULL) fclose(fo);
		_endthread();
	}

	//***********************
	// �^�X�N�ɂ���ĐU�蕪��
	//***********************
	if(_nMode == 1)
	{	// �،��f�[�^��M

		if(strlen(_sPhHeaderB) == 0 || strlen(_sPhHeaderC) == 0)
		{
			strcpy(_sTransBuf, "Error : Pharse Key (B,C) is NULL\n");
			bUpdated = TRUE;
			bInThread = FALSE;		// �X���b�h���쒆
			if(bDebugMode && fo != NULL) fclose(fo);
			_endthread();
		}

		_inet_stock_process(sCompose, sHTML, szTemp, &fo);
	}
	else
	{	// �ʏ��M
		_inet_norm_process(sCompose, sHTML, szTemp, &fo);
	}

	if(bDebugMode)
	{
		_dbg_write_formated(&fo, "sCompose", sCompose);
	}

	if(strlen(_sTransBuf) != 0)
		bUpdated = TRUE;
	else
		bUpdated = FALSE;

	bInThread = FALSE;		// �X���b�h���쒆

	//***********************
	// �X���b�h�̏I��
	//***********************
	if(bDebugMode)
	{
		_dbg_bufferovrn_check(&fo, sHTML, BUFSIZE, PADSIZE, "buffer over run check (sHTML)");
		_dbg_bufferovrn_check(&fo, sCompose, BUFSIZE, PADSIZE, "buffer over run check (sCompose)");
		_dbg_bufferovrn_check(&fo, szTemp, TMPSIZE, PADSIZE, "buffer over run check (szTemp)");
		_dbg_write_formated(&fo, "Thread Terminate", "debug file is closed");
		fclose(fo);
		bDebugMode = FALSE;
	}
	
	_endthread();
}


int _inet_norm_process(char *sCompose, char *sHTML, char *szTemp, FILE **fo)
{
	int			i ,j, k;			// �ꎞ�J�E���^
	int			_nSkip	= 0;		// �擪�X�L�b�v���̃J�E���g
	int			_nGetCt	= 0;		// �擾���ڐ��̃J�E���g
	BOOL		bSockResult;		// 

	//***********************
	// �C���^�[�l�b�g�����M
	//***********************
	// sURL �̃A�h���X�� HTML ����M���A sHTML �ɓ��e���i�[
	bSockResult = receive_HTML(_sURL, sHTML, szTemp, fo);

	//***********************
	// EUC �� ShiftJIS �ɕϊ�����
	//***********************
	_conv_euc_to_sjis(sHTML);

	if(bDebugMode)
	{
		_dbg_write_formated(fo, "sHTML", sHTML);
	}


	strcpy(sCompose, "");

	if(bSockResult)
	{	// ��M�����̏ꍇ
		if(strlen(sHTML) != 0)
		{
			//**************************************
			// �������o
			//**************************************
			for(i=0,j=0 ; _nGetCt < _nPhGetcount ; _nSkip++)
			{
				i = _str_find(sHTML, _sPhHeader, j);	// ��ؕ����������
				if(i<0) break;
				i += strlen(_sPhHeader);		// ��ؕ����񕶂����C���f�b�N�X��i�߂�
				for(;;)
				{
					if(sHTML[i] == '<')
					{	// ����� <...> �������Ă���ꍇ
						// ���� ">" ��
						i = _str_find(sHTML, ">", i) + 1;
					}
					else if(_str_find(sHTML, "<", i) > _str_find(sHTML, ">", i))
					{	// ��ؕ����� <...>�̓����ɂ���Ƃ�
						// ���� ">" ��
						i = _str_find(sHTML, ">", i) + 1;
					}
					else break;	// �����ɍ����|����ƁA���[�v������
				}
				if(i<0) break;
				j = _str_find(sHTML, "<", i+1);	// ���� <...>�̎n�܂��������
				if(j<0) break;
				if(_nPhSkip > _nSkip) continue;

				//**************************************
				// ���o
				//**************************************
				strncpy(szTemp, sHTML + i, j-i < TMPSIZE ? j-i : TMPSIZE);
				szTemp[j-i < TMPSIZE ? j-i : TMPSIZE] = (char)NULL;		// ������ NULL ��t��

				if(strlen(szTemp) == 0 || !strcmp(szTemp, "\n")) continue;	// �󕶎���X�L�b�v
				strcat(sCompose, szTemp);
				if(szTemp[strlen(szTemp)] != '\n')
					strcat(sCompose, "\n");		// �ŏI������ ���s �łȂ����́A���s������
				_nGetCt++;

			}

		}
		else
		{	// ��M�o�b�t�@����̏ꍇ
			strcat(sCompose, "Error : No HTML File\n");
		}
	}
	else
	{	// ��M���s�̏ꍇ (bSockResult == FALSE)
		strcat(sCompose, "Network Error : ");
		strcat(sCompose, sHTML);
		strcat(sCompose, "\n");
	}

	if(strlen(sCompose) == 0)
	{	// �g�s�l�k�͎�M���ꂽ���A�f�[�^�����o���Ȃ������ꍇ
		strcpy(sCompose, "Error : HTML received, but No target data (debug:head of HTML follows ...)\n");
		// �擪�T�s���v�����g
		for(i=0, j=0, k=0; i<5; i++)
		{
			k = _str_find(sHTML, "\n", j);
			if(k < 0) break;
			strncpy(szTemp, sHTML + j, k-j<TMPSIZE ? k-j : TMPSIZE-1);
			szTemp[k-j] = (char)NULL;
			strcat(sCompose, szTemp);
			strcat(sCompose, "\n");
			j = k+1;	// "\n" �̎�����n�߂�
			if(j >= (int)strlen(sHTML)) break;
		}
	}

	//**************************************
	// �^�C�g���t��
	//**************************************
	_add_title(_sTransBuf, sCompose, _sTitle, szTemp);

	return 0;
}

int _inet_stock_process(char *sCompose, char *sHTML, char *szTemp, FILE **fo)
{
	int			i, j, _i, _j;		// �ꎞ�J�E���^
	int			_nSkip	= 0;		// �擪�X�L�b�v���̃J�E���g
	int			_nGetCt	= 0;		// �擾���ڐ��̃J�E���g
	char		sComposeURL[URLSIZE + PADSIZE];		// �R�[�h��������URL�̍쐬�p
	BOOL		bSockResult;		// 

	strcpy(sCompose, "");

	for(_i=0,_j=0; ; )
	{
		_j = _str_find(_sItems, "\n", _i);
		if(_j<0)
			_j = strlen(_sItems) + 1;		// ���̍s���Ȃ����i�Ō��NULL���w��)
		strncpy(szTemp, _sItems + _i, _j-_i-1);
		szTemp[_j-_i-1] = (char)NULL;

		if(strlen(szTemp) == 0 || !strcmp(szTemp, "\n"))
		{
			_i = _j+1;							// ���s�̎��̕���
			if(_j >= (int)strlen(_sItems)) break;	// �ŏI�s���z����
			continue;
		}

		// sURL �� "%s" �� szTemp �̃R�[�h��}������
		sprintf(sComposeURL, _sURL, szTemp);

		//***********************
		// �C���^�[�l�b�g�����M
		//***********************
		// sURL �̃A�h���X�� HTML ����M���A sHTML �ɓ��e���i�[
		bSockResult = receive_HTML(sComposeURL, sHTML, szTemp, fo);
		//***********************
		// EUC �� ShiftJIS �ɕϊ�����
		//***********************
		_conv_euc_to_sjis(sHTML);

		if(bDebugMode)
		{
			_dbg_write_formated(fo, "sHTML", sHTML);
		}

		if(bSockResult)
		{	// ��M�����̏ꍇ
			if(strlen(sHTML) != 0)
			{
				//**************************************
				// �������o
				//**************************************
				strcpy(szTemp, _sPhHeader);	// �ŏ��̋�؂蕶��
				_nGetCt = 0;
				for(i=0,j=0 ; _nGetCt < _nPhGetcount ; _nSkip++)
				{
					i = _str_find(sHTML, szTemp, j);		// ��ؕ����������
					if(i<0) break;
					i += strlen(szTemp);		// ��ؕ����񕶂����C���f�b�N�X��i�߂�
					for(;;)
					{
						if(sHTML[i] == '<')
						{	// ����� <...> �������Ă���ꍇ
							// ���� ">" ��
							i = _str_find(sHTML, ">", i) + 1;
						}
						else if(_str_find(sHTML, "<", i) > _str_find(sHTML, ">", i))
						{	// ��ؕ����� <...>�̓����ɂ���Ƃ�
							// ���� ">" ��
							i = _str_find(sHTML, ">", i) + 1;
						}
						else break;	// �����ɍ����|����ƁA���[�v������
					}
					if(i<0) break;
					j = _str_find(sHTML, "<", i+1);	// ���� <...>�̎n�܂��������
					if(j<0) break;
					if(_nPhSkip > _nSkip) continue;

					//**************************************
					// ���o
					//**************************************
					strncpy(szTemp, sHTML + i, j-i < TMPSIZE ? j-i : TMPSIZE);
					szTemp[j-i < TMPSIZE ? j-i : TMPSIZE] = (char)NULL;		// ������ NULL ��t��

					if(strlen(szTemp) == 0 || !strcmp(szTemp, "\n")) continue;	// �󕶎���X�L�b�v
					if(szTemp[strlen(szTemp)-1] == (char)"\n")
					{	// �s���̉��s���폜����
						szTemp[strlen(szTemp)-1] = (char)NULL;
					}
					strcat(szTemp, "  ");	// �󔒂��ЂƂt����
					strcat(sCompose, szTemp);

					_nGetCt++;

					if(_nGetCt == 1)
						strcpy(szTemp, _sPhHeaderB);	// 2�ڂ̋�؂蕶��
					else if(_nGetCt == 2)
						strcpy(szTemp, _sPhHeaderC);	// 2�ڂ̋�؂蕶��
					else
						break;
				}

			}
			else
			{	// �،����i�P���ځj�̎�M�Ɏ��s
				strncpy(szTemp, _sItems + _i, _j-_i-1);
				szTemp[_j-_i-1] = (char)NULL;

				strcat(sCompose, "No HTML File : ");
				strcat(sCompose, szTemp);
			}

			if(_nGetCt <= 2)
			{	// �،��R�[�h���Ԉ���Ă����ꍇ�i�K��̉�͕���������Ȃ������ꍇ�j
				strncpy(szTemp, _sItems + _i, _j-_i-1);
				szTemp[_j-_i-1] = (char)NULL;

				strcat(sCompose, "Wrong Code : ");
				strcat(sCompose, szTemp);
			}
		}
		else
		{	// ��M���s�̏ꍇ (bSockResult == FALSE)
			strncpy(szTemp, _sItems + _i, _j-_i-1);
			szTemp[_j-_i-1] = (char)NULL;

			strcat(sCompose, "Network Error : ");
			strcat(sCompose, szTemp);
			strcat(sCompose, " : ");
			strcat(sCompose, sHTML);

		}

		// ���s������ �i�P�s�I���j
		if(strlen(sCompose) != 0)
		{
			if(sCompose[strlen(sCompose)-1] != (char)'\n')
				strcat(sCompose, "\n");		// �ŏI������ ���s �łȂ����́A���s������
		}

		_i = _j+1;							// ���s�̎��̕���
		if(_j >= (int)strlen(_sItems)) break;	// �ŏI�s���z����
	}

	//**************************************
	// �^�C�g���t��
	//**************************************
	_add_title(_sTransBuf, sCompose, _sTitle, szTemp);

	return 0;

}


//********************
// �w�肵���g�s�l�k����M����
// 
//********************
BOOL receive_HTML(char *sRURL, char *sHTML, char *szTemp, FILE **fo)
{
	int			i, j;
	char		*psz;
	char		szBuf[RECVSIZE+1500];				// ��M�o�b�t�@ (RWIN �̕��������S�̂��߉����Ă����j
	char		szHostName[URLSIZE + PADSIZE];		// �^����ꂽ�t�q�k�𕪉����Đݒ�
	char		szConnHostName[URLSIZE + PADSIZE];	// �ڑ�����z�X�g���i�v���L�Vor�_�C���N�g�j
	char		szHtmlName[URLSIZE + PADSIZE];		// �^����ꂽ�t�q�k�𕪉����Đݒ�
	SOCKET		sock = INVALID_SOCKET;
	LPCSTR		pszUserAgent = "Mozilla/4.0";		// User-Agent
	DWORD		dwTotalSize;
	LPHOSTENT	m_pHostEnt;
	SOCKADDR_IN	sai;					// inet �A�h���X
	in_addr		iadr;					// inet �A�h���X
	WSADATA _sock_wsa;

	//***********************
	// �����l�̐ݒ�
	//***********************
	dwTotalSize = 0L;			// �_�E�����[�h�����f�[�^�̃T�C�Y
	memset(szBuf, NULL, RECVSIZE+1500);
	memset(szHostName, NULL, URLSIZE + PADSIZE);
	memset(szConnHostName, NULL, URLSIZE + PADSIZE);
	memset(szHtmlName, NULL, URLSIZE + PADSIZE);

	//***********************
	// EUC -> SJIS �ϊ��p
	//***********************
	BOOL		isSJIS = FALSE;			// Shift JIS �G���R�[�h���� TRUE
	BOOL		bInEuc = FALSE;
	//***********************

	// ====================================================================
	// �\�P�b�g�쐬
	// ====================================================================
	// �o�[�W���� 1.1 ��v������ 
	// Ver 1.1 : Win95 , Ver 2.2 : Win98SE , 2000
	WORD wVersionRequested = MAKEWORD( 1, 1 );
	if(::WSAStartup( wVersionRequested, &_sock_wsa ))
	{
		sprintf(sHTML, "WSAStartup (error code : %d)", WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_CREAT);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̍쐬���s\n
		return FALSE;
	}
	if(bDebugMode)
	{
		sprintf(szTemp, "winsock required 0x%04X , max supported 0x%04X ,\r\ndesc :%s\r\nstat :%s", _sock_wsa.wVersion, _sock_wsa.wHighVersion, _sock_wsa.szDescription, _sock_wsa.szSystemStatus);
		_dbg_write_formated(fo, "WSAStartup", szTemp);
	}

	if(!::AfxSocketInit(NULL))	// WinSock �̏�����
	{
		strcpy(sHTML, "AfxSocketInit (error code : ---)");
//		sHTML->LoadString(IDS_ERR_SOCK_CREAT);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̍쐬���s\n
		return FALSE;
	}
	if(bDebugMode)
	{
		_dbg_write_formated(fo, "AfxSocketInit", "");
	}

	if((sock=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
	{	// �\�P�b�g�쐬�s�\
		sprintf(sHTML, "socket create (error code : %d)", WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_CREAT);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̍쐬���s\n
		::WSACleanup();
		return FALSE;
	}
	if(bDebugMode)
	{
		sprintf(szTemp, "socket # :0x%X", (UINT)sock);
		_dbg_write_formated(fo, "socket (create)", szTemp);
	}

	// ====================================================================
	// URL�̐擪�� "http://"�����Ă����ꍇ�͍폜����
	// ====================================================================
	if(!strncmp(sRURL, "http://", 7))
	{
		strncpy(szTemp, sRURL+7, strlen(sRURL)-7);
		szTemp[strlen(sRURL)-7] = (char)NULL;
		strcpy(sRURL, szTemp);
	}

	// ====================================================================
	// URL�̕���
	// ====================================================================
	// aaa.bbb.ccc/xxx/yyy.htm �̂Ƃ�
	// szConnHostName = aaa.bbb.ccc
	// szHtmlName = /xxx/yyy.htm
	//
	i = _str_find(sRURL, "/", 0);
	if(i >0)
	{	// aaa.bbb.ccc/xxx �̌`�̂Ƃ�
		strncpy(szConnHostName, sRURL, i);
		szConnHostName[i] = (char)NULL;
		strncpy(szHtmlName, sRURL + i, strlen(sRURL)-i+1);		// �X���b�V�����܂߂�
		szHtmlName[strlen(sRURL)-i+1] = (char)NULL;
	}
	else
	{	// aaa.bbb.ccc �̌`�̂Ƃ�
		strcpy(szConnHostName, sRURL);
		strcpy(szHtmlName, "/");			// �X���b�V���̂�
	}

	strcpy(szHostName, szConnHostName);		// HTTP - GET �Ŏg�p���邽��

	// �v���L�V�T�[�o��ʂ��Ƃ��̂��߂̏���
	if(strlen(_sProxy) != 0)
	{	// �v���L�V����
		strcpy(szConnHostName, _sProxy);
		sprintf(szHtmlName, "http://%s", sRURL);
	}
	else
	{	// �v���L�V����
	}


	// ====================================================================
	// �z�X�g����
	// ====================================================================

//	iadr.s_addr=inet_addr(szConnHostName);
	iadr.S_un.S_addr=inet_addr(szConnHostName);		// 999.999.999.999 �`���� IP �A�h���X�� long int �ɕϊ�

//	if(iadr.s_addr==INADDR_NONE)
	if(iadr.S_un.S_addr==INADDR_NONE)
	{	// szConnHostName �� 999.999.999.999 �`���ŗ^�����Ă��Ȃ������ꍇ
		// �u�z�X�g���v���\���̂ɑ������
		m_pHostEnt=gethostbyname(szConnHostName);
	}
	else
	{
		// 999.999.999.999 �`������A�u�z�X�g���v�𓾂�
		// winsock ver 1 (windows 95) �ł͎��s����
// ���̏����́A�z�X�g���𓾂邾���ŁA�s�K�v�Ȃ̂Ŗ���
//		m_pHostEnt=gethostbyaddr((const char *)&iadr.S_un.S_addr,sizeof(iadr.S_un.S_addr),AF_INET);
	}

	if(!m_pHostEnt && iadr.S_un.S_addr==INADDR_NONE)
	{	// �u�z�X�g���v�`���Őڑ��悪�^����ꂽ���A��͂ł��Ȃ������ꍇ
		sprintf(sHTML, "DNS lookup : %s (error code : %d)", szConnHostName, WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_GETHOST);		// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�Ńz�X�g���̉������ł��܂���\n
		shutdown(sock,SD_BOTH);
		closesocket(sock);
		::WSACleanup();
		return FALSE;
	}

	sai.sin_port=htons((u_short)_nPort);
	sai.sin_family=AF_INET;
	// �h�o�A�h���X�̑��
	if(iadr.S_un.S_addr == INADDR_NONE)
	{
		sai.sin_addr=*((LPIN_ADDR)*m_pHostEnt->h_addr_list);
	}
	else
	{
		sai.sin_addr.S_un.S_addr = iadr.S_un.S_addr;
		sai.sin_addr.S_un.S_un_b = iadr.S_un.S_un_b;
		sai.sin_addr.S_un.S_un_w.s_w1 = iadr.S_un.S_un_w.s_w1;
		sai.sin_addr.S_un.S_un_w.s_w2 = iadr.S_un.S_un_w.s_w2;
	}

	if(bDebugMode)
	{
		sprintf(szTemp, "address pharse : %s : %d.%d.%d.%d\r\n", szConnHostName, sai.sin_addr.S_un.S_un_b.s_b1, sai.sin_addr.S_un.S_un_b.s_b2, sai.sin_addr.S_un.S_un_b.s_b3, sai.sin_addr.S_un.S_un_b.s_b4);
		_dbg_write_formated(fo, "gethost", szTemp);
		sprintf(szTemp, "sin_family : %X\r\nsin_port : %X\r\nsin_addr.S_un.S_addr : %lX\r\nsin_addr.S_un.S_un_w.s_w1 : %X\r\nsin_addr.S_un.S_un_w.s_w2 : %X\r\nsin_addr.S_un.S_un_b.s_b1 : %X\r\nsin_addr.S_un.S_un_b.s_b2 : %X\r\nsin_addr.S_un.S_un_b.s_b3 : %X\r\nsin_addr.S_un.S_un_b.s_b4 : %X\r\n",
				sai.sin_family, sai.sin_port, sai.sin_addr.S_un.S_addr, sai.sin_addr.S_un.S_un_w.s_w1, sai.sin_addr.S_un.S_un_w.s_w2,sai.sin_addr.S_un.S_un_b.s_b1, sai.sin_addr.S_un.S_un_b.s_b2, sai.sin_addr.S_un.S_un_b.s_b3, sai.sin_addr.S_un.S_un_b.s_b4);
		_dbg_write_formated(fo, "SOCKADDR_IN info", szTemp);
	}



	// ====================================================================
	// �ڑ�
	// ====================================================================
	if(connect(sock,(LPSOCKADDR)&sai,sizeof(sai)) == SOCKET_ERROR)
	{	// �ڑ��ł��܂���ł���
		sprintf(sHTML, "socket connect : %s : %d.%d.%d.%d (port:%u) (error code : %d)", szConnHostName, sai.sin_addr.S_un.S_un_b.s_b1, sai.sin_addr.S_un.S_un_b.s_b2, sai.sin_addr.S_un.S_un_b.s_b3, sai.sin_addr.S_un.S_un_b.s_b4, _nPort, WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_CONN);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̐ڑ��Ɏ��s���܂���\n
		shutdown(sock,SD_BOTH);
		closesocket(sock);
		::WSACleanup();
		return FALSE;
	}

	if(bDebugMode)
	{
		sprintf(szTemp, "connect : %s : %d.%d.%d.%d", szConnHostName, sai.sin_addr.S_un.S_un_b.s_b1, sai.sin_addr.S_un.S_un_b.s_b2, sai.sin_addr.S_un.S_un_b.s_b3, sai.sin_addr.S_un.S_un_b.s_b4);
		_dbg_write_formated(fo, "socket (connect)", szTemp);
	}

	// ====================================================================
	// ���N�G�X�g �i���M�j
	// ====================================================================

	if(strlen(_sProxy) == 0)
	{	// �_�C���N�g�ڑ��̏ꍇ
		sprintf(
			szTemp,
			"GET %s HTTP/1.0\r\n"
			"Host: %s:%d\r\n"
			"User-Agent: %s\r\n\r\n",
			szHtmlName,szHostName,_nPort,pszUserAgent);
	}
	else
	{	// �v���L�V�o�R�ڑ��̏ꍇ
		sprintf(
			szTemp,
			"GET %s HTTP/1.0\r\n"
			"Host: %s:%d\r\n"
			"User-Agent: %s\r\n\r\n",
			szHtmlName,szHostName,80,pszUserAgent);
	}
	psz=szTemp;

	while(i < lstrlen(psz))
	{
		i = send(sock, psz, lstrlen(psz), 0);	// ���M
		if(i == SOCKET_ERROR)
		{
			sprintf(sHTML, "socket send (error code : %d)", WSAGetLastError ());
//			sHTML->LoadString(IDS_ERR_SOCK_CONN);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̐ڑ��Ɏ��s���܂���\n
			shutdown(sock,SD_BOTH);
			closesocket(sock);
			::WSACleanup();
			return FALSE;
		}
		// ���M�o�C�g���� i �Ɋi�[�����B
		psz += i;
	}
	if(bDebugMode)
	{
		_dbg_write_formated(fo, "socket (send)", szTemp);
	}

	// ====================================================================
	// ��M
	// ====================================================================

	strcpy(sHTML, "");
	for(j=0;;)
	{
		i = recv(sock, szBuf, RECVSIZE, 0);		// ��M
		// ��M�o�C�g���� i �Ɋi�[�����
		if(i<=0 || i==SOCKET_ERROR) break;		// ��M�I��
		j += i;

		szBuf[i] = (char)NULL;		// ������Ƃ��Ċ��������邽�߁A������ NULL �ǉ�

		if(strlen(sHTML) + i < BUFSIZE) strcat(sHTML, szBuf);

	}

	if(i == SOCKET_ERROR)
	{
		sprintf(sHTML, "socket recv (error code : %d)", WSAGetLastError ());
		shutdown(sock,SD_BOTH);
		closesocket(sock);
		::WSACleanup();
		return FALSE;
	}

	dwTotalSize = strlen(sHTML);

	if(bDebugMode)
	{
		sprintf(szTemp, "recv total size : %d, this program buffer %d\r\n", j, dwTotalSize);
		_dbg_write_formated(fo, "socket (recv) (size)", szTemp);
		_dbg_write_formated(fo, "socket (recv)", sHTML);
	}

	// ====================================================================
	// �ʐM�I��
	// ====================================================================

	if(shutdown(sock,SD_BOTH))
	{
		sprintf(sHTML, "socket shutdown (error code : %d)", WSAGetLastError ());
//		sHTML->LoadString(IDS_ERR_SOCK_SHUT);		// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̉���Ɏ��s\n
	}
	if(sock!=INVALID_SOCKET)
	{
		if(closesocket(sock))
		{
			sprintf(sHTML, "socket close (error code : %d)", WSAGetLastError ());
//			sHTML->LoadString(IDS_ERR_SOCK_CLOSE);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̉���Ɏ��s\n
		}
	}

	if(::WSACleanup())
	{
		sprintf(sHTML, "WSACleanup (error code : %d)", WSAGetLastError ());
//		sprintf(szTemp, "WSACleanup : error code : %d", WSAGetLastError ());
	}
	if(bDebugMode)
	{
		_dbg_write_formated(fo, "socket (close), WSACleanup", "");
		_dbg_bufferovrn_check(fo, szBuf, RECVSIZE, 1500, "buffer over run check (szBuf)");
		_dbg_bufferovrn_check(fo, szHostName, URLSIZE, PADSIZE, "buffer over run check (szHostName)");
		_dbg_bufferovrn_check(fo, szConnHostName, URLSIZE, PADSIZE, "buffer over run check (szConnHostName)");
		_dbg_bufferovrn_check(fo, szHtmlName, URLSIZE, PADSIZE, "buffer over run check (szHtmlName)");
	}

	return TRUE;
}

//********************
// ��������������A�擪����̕�������Ԃ�
// 
// szOrg ���� szKey ��T���B�i�X�^�[�g�n�_�́A�擪��� seek �����i�񂾈ʒu�j
// �߂�l�F0 �` �F��������������̐擪�A-1�F�����ł��Ȃ�����
//********************
int _str_find(char *szOrg, char *szKey, int seek)
{
	char *pszTmp;
	int nTemp;

	if((int)strlen(szOrg) <= seek) return -1;

	pszTmp = strstr(szOrg + seek, szKey);

	if(pszTmp == NULL) return -1;

	nTemp = pszTmp - szOrg;

	return nTemp;
}

//********************
// �d�t�b���r���������i�h�r�ɕϊ�����
// 
// sHTML �œn���ꂽ���̃o�b�t�@���̂�����������
// <meta> �L�[���� Shift-JIS ���錾����Ă���ꍇ�́A�ϊ������Ȃ�
//        ����ȊO�́A���ׂĕϊ����[�`����ʂ邪�AEUC�ȊO�̕����͉e�����󂯂Ȃ�
//********************
void _conv_euc_to_sjis(char *sHTML)
{
	int			i,j;
	BOOL		isSJIS = FALSE;			// Shift JIS �G���R�[�h���� TRUE
//	BOOL		bInEuc = FALSE;
	unsigned int nEucCode;
	char		szTemp[1024];			// ���M������̈ꎞ�o�b�t�@�A�Ɗe���Ɨp�o�b�t�@
	long		dwTotalSize;

	dwTotalSize = strlen(sHTML);

	//***********************
	// META SJIS  ����
	//***********************
	isSJIS = FALSE;

	for(i=0,j=0;;)
	{
		i=_str_find(sHTML, "<META", j);
		if(i<0)
		{
			break;
		}
		j=_str_find(sHTML, ">", i+1);
		if(j<0) break;
		strncpy(szTemp, sHTML + (i+1), j-i);
		if(_str_find(szTemp,"SHIFT",0)!=-1 || _str_find(szTemp,"shift",0)!=-1 || _str_find(szTemp,"Shift",0)!=-1 ||
					_str_find(szTemp,"sjis",0)!=-1 || _str_find(szTemp,"Sjis",0)!=-1 || _str_find(szTemp,"SJIS",0)!=-1)
		{
			isSJIS = TRUE;
			break;
		}
	}		
	for(i=0,j=0;;)
	{
		i=_str_find(sHTML, "<meta", j);
		if(i<0)
		{
			break;
		}
		j=_str_find(sHTML, ">", i+1);
		if(j<0) break;
		strncpy(szTemp, sHTML + (i+1), j-i);
		if(_str_find(szTemp,"SHIFT",0)!=-1 || _str_find(szTemp,"shift",0)!=-1 || _str_find(szTemp,"Shift",0)!=-1 ||
					_str_find(szTemp,"sjis",0)!=-1 || _str_find(szTemp,"Sjis",0)!=-1 || _str_find(szTemp,"SJIS",0)!=-1)
		{
			isSJIS = TRUE;
			break;
		}
	}		


	//***********************
	// EUC -> SJIS �ϊ��p
	//***********************
	if(!isSJIS)
	{
		for(i=0; i<(int)dwTotalSize; i++)
		{
			if((sHTML[i] >= (char)0xa1) && (sHTML[i] <= (char)0xfe))
			{
				if(i<(int)dwTotalSize-1)
				{	// ���Ɉꕶ������ꍇ
					nEucCode = (0xff00 & (sHTML[i] << 8)) | (0xff & sHTML[i+1]);
					nEucCode &= 0x7f7f;
					nEucCode = _mbcjistojms(nEucCode);
					if(nEucCode != 0)
					{
						sHTML[i] =  (char)((nEucCode & 0xff00) >> 8);
						sHTML[i+1] = (char)(nEucCode & 0xff);
					}
					i++;

				}
				else
				{	// �Ō�̕����̏ꍇ
				}
			}
		}
	}

}

//********************
// �^����ꂽ������̐擪�ɁA�^�C�g���s�i���ݎ�������j��t������B
// 
// __sTitle + ���� + "\n" + __sHTML �� __sReturn
//
// __sTitle ���m�t�k�k������̎��́A�������Ȃ�
//********************
void _add_title(char *__sReturn, char *__sHTML, char *__sTitle, char *szTemp)
{
	struct tm *_tm_loc;	// ���[�J������
	time_t	_tm_sys;	// �V�X�e�������ilong int)

	if(strlen(__sHTML) != 0 && strlen(__sTitle) != 0)
	{
		time(&_tm_sys);
		_tm_loc = localtime(&_tm_sys);
		strcpy(__sReturn, __sTitle);
		sprintf(szTemp, "     %4d/%4d/%4d  %2d:%02d\n", _tm_loc->tm_year+1900, _tm_loc->tm_mon+1, _tm_loc->tm_mday, _tm_loc->tm_hour, _tm_loc->tm_min);
		strcat(__sReturn, szTemp);
		strcat(__sReturn, __sHTML);
	}
	else
	{	// �^�C�g�������̂Ƃ�
		strcpy(__sReturn, __sHTML);
	}

	return;
}

//********************
// �f�o�b�O���[�h�ŁA�V�K�t�@�C���ۑ��̈�A�̍�Ƃ��s��
// 
// �u�t�@�C����ۑ�����v�_�C�A���O��\������
// �t�@�C�����J���A�t�@�C���|�C���^�ɐݒ肷��B
//********************
void _dbg_openfile(FILE **fo, char *szTemp)
{
	OPENFILENAME ofn;

	strcpy(szTemp, "");

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szTemp;
	ofn.nMaxFile = TMPSIZE;
	ofn.lpstrFilter = "Text file\0*.txt\0Data file\0*.dat\0All Files\n*.*\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = szTemp;
	ofn.nMaxFileTitle = TMPSIZE;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Save Debug Data";
	ofn.Flags = OFN_OVERWRITEPROMPT;

	// �u�t�@�C����ۑ�����v�_�C�A���O��\������
	if(GetSaveFileName(&ofn) != 0)
	{
		*fo = fopen(ofn.lpstrFile, "wb");
	}
	else
	{
		bDebugMode = FALSE;
	}
}

//********************
// �t�@�C���Ƀf�o�b�N������������
// 
// szBuf1 + "\n" + szBuf2 �� �t�@�C��
//********************
void _dbg_write_formated(FILE **fo, char *szBuf1, char *szBuf2)
{
	struct tm *_tm_loc;	// ���[�J������
	time_t	_tm_sys;	// �V�X�e�������ilong int)
	clock_t _tm_clk;	// CPU �N���b�N����

	if(*fo == NULL) return;

	time(&_tm_sys);
	_tm_loc = localtime(&_tm_sys);
	_tm_clk = clock();
	fprintf(*fo, "==> %04d/%04d/%04d %02d:%02d:%02d (%08lu) \r\n", _tm_loc->tm_year+1900, _tm_loc->tm_mon+1, _tm_loc->tm_mday, _tm_loc->tm_hour, _tm_loc->tm_min, _tm_loc->tm_sec, _tm_clk);
	fprintf(*fo, "%s : \r\n%s\r\n", szBuf1, szBuf2);
	fprintf(*fo, "===============================================\r\n");

}

//********************
// �t�@�C���Ƀf�o�b�N������������ (���l�𕶎���ɕϊ��j
// 
// szBuf1 + "\n" + x �� �t�@�C��
//********************
void _dbg_write_formated_int(FILE **fo, char *szBuf1, int x)
{
	struct tm *_tm_loc;	// ���[�J������
	time_t	_tm_sys;	// �V�X�e�������ilong int)
	clock_t _tm_clk;	// CPU �N���b�N����

	if(*fo == NULL) return;

	time(&_tm_sys);
	_tm_loc = localtime(&_tm_sys);
	_tm_clk = clock();
	fprintf(*fo, "==> %04d/%04d/%04d %02d:%02d:%02d (%08lu) \r\n", _tm_loc->tm_year+1900, _tm_loc->tm_mon+1, _tm_loc->tm_mday, _tm_loc->tm_hour, _tm_loc->tm_min, _tm_loc->tm_sec, _tm_clk);
	fprintf(*fo, "%s : \r\n%u\r\n", szBuf1, (UINT)x);
	fprintf(*fo, "===============================================\r\n");

}

//********************
// �t�@�C���Ƀf�o�b�N������������ �i�e�핶����̏����������܂Ƃ߂ď������ށj
// 
//********************
void _dbg_write_init(FILE **fo, char *szTemp)
{
	OSVERSIONINFO ver;
	LPHOSTENT _hostent;
	IN_ADDR in_addr;

	if(*fo == NULL) return;

	// OS�̃o�[�W������
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((LPOSVERSIONINFO)&ver);

	sprintf(szTemp, "dwMajorVersion : %Lu\r\ndwMinorVersion : %Lu\r\ndwBuildNumber : %u\r\nszCSDVersion : %s\r\n", ver.dwMajorVersion, ver.dwMinorVersion,
			ver.dwBuildNumber, ver.szCSDVersion);
	_dbg_write_formated(fo, "Windows Version", szTemp);

	// ���̃}�V���̃z�X�g��
	if(!gethostname(szTemp, TMPSIZE))
	{
		// �z�X�g���������o��
		_dbg_write_formated(fo, "gethostname (this unit)", szTemp);
		// �z�X�g�̂h�o�A�h���X�������o��
		_hostent = gethostbyname(szTemp);
		in_addr = *((LPIN_ADDR)*_hostent->h_addr_list);

		sprintf(szTemp, "%d.%d.%d.%d\r\n", in_addr.S_un.S_un_b.s_b1, in_addr.S_un.S_un_b.s_b2,
					in_addr.S_un.S_un_b.s_b3, in_addr.S_un.S_un_b.s_b4);
		_dbg_write_formated(fo, "gethostbyname (this unit)", szTemp);
	}
	else
	{
		sprintf(szTemp, "Error : gethostname (error code : %d)\r\n", WSAGetLastError ());
		_dbg_write_formated(fo, "gethostname (this unit)", szTemp);
	}

	// �e��ϐ��̏����l
	_dbg_write_formated(fo, "_sURL", _sURL);
	_dbg_write_formated(fo, "_sProxy", _sProxy);
	_dbg_write_formated_int(fo, "_nPort", _nPort);
	_dbg_write_formated(fo, "_sPhHeader", _sPhHeader);
	_dbg_write_formated(fo, "_sPhHeaderB", _sPhHeaderB);
	_dbg_write_formated(fo, "_sPhHeaderC", _sPhHeaderC);
	_dbg_write_formated(fo, "_sTitle", _sTitle);
	_dbg_write_formated(fo, "_sItems", _sItems);
	_dbg_write_formated_int(fo, "_nMode", _nMode);
	_dbg_write_formated_int(fo, "_nPhSkip", _nPhSkip);
	_dbg_write_formated_int(fo, "_nPhGetcount", _nPhGetcount);

}

//********************
// �t�@�C���Ƀf�o�b�N�����������ށi�o�b�t�@�I�[�o�[�����p�̃p�f�B���O�̈���������ށj
// 
// szStr �̃T�C�Y�� (_sLen0 + _sPad)�B�p�f�B���O�̈�� _sPad
//********************
void _dbg_bufferovrn_check(FILE **fo, char *szStr, int _sLen0, int _sPad, char *szTitle)
{
	int i;

	if(*fo == NULL) return;

	fprintf(*fo, "%s :\r\n", szTitle);

	for(i=0; i<_sPad; i++)
	{
		fprintf(*fo, "%02X ", szStr[_sLen0+i]);
	}
	fprintf(*fo, "\r\n===============================================\r\n");

}
