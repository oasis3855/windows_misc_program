
#include "stdafx.h"

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "resource.h"

#include <mbstring.h>		// ************** JIS �R�[�h�ϊ��p

//********************
// �O���[�o���ϐ�
// �O���Ƃ̕ϐ��󂯓n���p
//********************
volatile BOOL bUpdated;		// �X���b�h�ōX�V�����������ʒm
volatile BOOL bInThread;	// �X���b�h���쒆�̃t���O
volatile int nThreadError;	// �X���b�h�ŋN�������G���[�̎�ނ�Ԃ�
CString sTransBuf;			// �X���b�h�ƃ_�C�A���O�̒ʐM�p
CString sURL;				// �X���b�h�ɓn�� URL
CString sProxy;				// �X���b�h�ɓn�� PROXY
volatile UINT nPort;		// �X���b�h�ɓn�� �|�[�g�ԍ�
CString sPhHeader;			// �؂蕪���p �w�b�_�[������
volatile int nPhSkip;		// �؂蕪���p �X�L�b�v��
volatile int nPhGetcount;	// �؂蕪���p �擾��
CString sTitle;				// �^�C�g��


//********************
// ���̃t�@�C���̒������Ŏg���ϐ�
// (�X���b�h���Ő錾����ƁA�o�b�t�@�I�[�o�[�t���[���邽�߁j
//********************

CString		_sTmp_trd, _sTmp2_trd;
CTime		_tm_trd;
//********************

#define		RECVSIZE	5000		// ��x�Ɏ�M�ł���ő�T�C�Y


//********************
// �X���b�h�֐�
// (�ʐM������S���j
//********************
void thread_main(void *pVoid)
{
	int			i,j;
	char		*psz,*pDest;
	char		pszBuf[RECVSIZE+1500];	// ��M�o�b�t�@ (RWIN �̕��������S�̂��߉����Ă����j
	char		szURL[1024];			// URL (�^����ꂽURL���R�s�[���Ă����Ɋi�[�j
	char		szHostName[1024];		// �^����ꂽ�t�q�k�𕪉����Đݒ�
	char		szConnHostName[1024];	// �ڑ�����z�X�g���i�v���L�Vor�_�C���N�g�j
	char		szHtmlName[1024];		// �^����ꂽ�t�q�k�𕪉����Đݒ�
	char		szCmd[1024];
	SOCKET		sock = INVALID_SOCKET;
	LPCSTR		pszUserAgent = "Mozilla/4.0";		// User-Agent
	DWORD		dwTotalSize;
	LPHOSTENT	m_pHostEnt;
	SOCKADDR_IN	sai;					// inet �A�h���X
	in_addr		iadr;					// inet �A�h���X

//	WSADATA _sock_wsa;

	// �X���b�h�̏d���N���͔�����
	if(bInThread == TRUE)
		_endthread();

	//***********************
	// �����l�̐ݒ�
	//***********************
	bInThread = TRUE;			// �X���b�h���쒆
	bUpdated = FALSE;			// �X�V���I����Ă��Ȃ����Ƃ�����
	_sTmp_trd = "";				// ��M���� HTML ���ꎞ�i�[
	dwTotalSize = 0L;			// �_�E�����[�h�����f�[�^�̃T�C�Y

	//***********************
	// EUC -> SJIS �ϊ��p
	//***********************
	BOOL		isSJIS = FALSE;			// Shift JIS �G���R�[�h���� TRUE
	BOOL		bInEuc = FALSE;
	unsigned int nEucCode;
	//***********************
	



	// ====================================================================
	// �\�P�b�g�쐬
	// ====================================================================
	if((sock=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
	{	// �\�P�b�g�쐬�s�\
		sTransBuf.LoadString(IDS_ERR_SOCK_CREAT);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̍쐬���s\n
		//****************
		// �X���b�h���f����
		bUpdated = TRUE;		// ���X�V
		bInThread = FALSE;
		_endthread();
		//****************
	}
	// ====================================================================
	// URL�̐擪�� "http://"�����Ă����ꍇ�͍폜����
	// ====================================================================
	if(!strncmp((LPCSTR)sURL, "http://", 7))
	{
		_sTmp_trd = sURL.Mid(7, sURL.GetLength()-7);
		sURL = _sTmp_trd;
	}

	// ====================================================================
	// URL�̕���
	// ====================================================================

	memset(szHostName,'\0',sizeof(szHostName));
	memset(szHtmlName,'\0',sizeof(szHtmlName));

	strcpy(szURL, sURL);
	psz=szURL;

	// �z�X�g���݂̂����o�� www.yahoo.co.jp
	pDest=szHostName;
	while(*psz)
	{
		if(*psz=='/')
			break;
		*pDest=*psz;
		pDest++;
		psz++;
	}
	if(sProxy != "")
	{	// �v���L�V����̂Ƃ�
		strcpy(szConnHostName, sProxy);
	}
	else
	{	// �v���L�V�����̂Ƃ�
		strcpy(szConnHostName, szHostName);
	}

	// �t�H���_���݂̂����o�� /index.html
	pDest=szHtmlName;
	if(*psz)
	{
		while(*psz)
		{
			*pDest=*psz;
			pDest++;
			psz++;
		}
	}
	if(szHtmlName[0]=='\0')
		lstrcpy(szHtmlName,"/");	// �擪�� �X���b�V��������

	if(sProxy != "")
	{	// �v���L�V����̂Ƃ��́A�擾����HTML�����擾�A�h���X�S�ĂƂ���
		lstrcpy(szHtmlName, "http://");
		lstrcat(szHtmlName,sURL);
	}

	// ====================================================================
	// �z�X�g����
	// ====================================================================

	iadr.s_addr=inet_addr(szConnHostName);

	if(iadr.s_addr==INADDR_NONE)
		m_pHostEnt=gethostbyname(szConnHostName);
	else
		m_pHostEnt=gethostbyaddr((const char *)&iadr,sizeof(iadr),AF_INET);

	if(!m_pHostEnt)
	{	// �z�X�g�����o�ł��܂���
		sTransBuf.LoadString(IDS_ERR_SOCK_GETHOST);		// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�Ńz�X�g���̉������ł��܂���\n
		//****************
		// �X���b�h���f����
		bUpdated = TRUE;		// ���X�V
		bInThread = FALSE;
		shutdown(sock,0x2);
		closesocket(sock);
		_endthread();
		//****************
	}

	// ====================================================================
	// �ڑ�
	// ====================================================================

	sai.sin_port=htons((u_short)nPort);
	sai.sin_family=AF_INET;
	sai.sin_addr=*((LPIN_ADDR)*m_pHostEnt->h_addr_list);

	if(connect(sock,(LPSOCKADDR)&sai,sizeof(sai)))
	{	// �ڑ��ł��܂���ł���
		sTransBuf.LoadString(IDS_ERR_SOCK_CONN);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̐ڑ��Ɏ��s���܂���\n
		//****************
		// �X���b�h���f����
		bUpdated = TRUE;		// ���X�V
		bInThread = FALSE;
		shutdown(sock,0x2);
		closesocket(sock);
		_endthread();
		//****************
	}

	// ====================================================================
	// ���N�G�X�g �i���M�j
	// ====================================================================

	if(sProxy=="")
	{	// �_�C���N�g�ڑ��̏ꍇ
		wsprintf(
			szCmd,
			"GET %s HTTP/1.0\r\n"
			"Host: %s:%d\r\n"
			"User-Agent: %s\r\n\r\n",
//			"Connection: Keep-Alive\r\n\r\n",
			szHtmlName,szHostName,nPort,pszUserAgent);
	}
	else
	{	// �v���L�V�o�R�ڑ��̏ꍇ
	wsprintf(
			szCmd,
			"GET %s HTTP/1.0\r\n"
			"Host: %s:%d\r\n"
			"User-Agent: %s\r\n\r\n",
			//"Proxy-Connection: Keep-Alive\r\n\r\n",
			szHtmlName,szHostName,80,pszUserAgent);
	}
	psz=szCmd;

	while(i < lstrlen(psz))
	{
		i = send(sock, psz, lstrlen(psz), 0);	// ���M
		if(i == SOCKET_ERROR)
		{
			sTransBuf.LoadString(IDS_ERR_SOCK_CONN);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̐ڑ��Ɏ��s���܂���\n
			//****************
			// �X���b�h���f����
			bInThread = FALSE;
			bUpdated = TRUE;		// ���X�V
			shutdown(sock,0x2);
			closesocket(sock);
			_endthread();
			//****************
		}
		// ���M�o�C�g���� i �Ɋi�[�����B
		psz += i;
	}

	// ====================================================================
	// ��M
	// ====================================================================

	for(;;)
	{
		i = recv(sock, pszBuf, RECVSIZE, 0);		// ��M
		// ��M�o�C�g���� i �Ɋi�[�����
		if(i<=0 || i==SOCKET_ERROR) break;		// ��M�I��

		pszBuf[i] = (char)NULL;		// ������Ƃ��Ċ��������邽�߁A������ NULL �ǉ�

		_sTmp_trd += (LPCSTR)pszBuf;

	}
	dwTotalSize = _sTmp_trd.GetLength();

	// ====================================================================
	// �ʐM�I��
	// ====================================================================
	if(shutdown(sock,0x2))
	{
		_sTmp2_trd.LoadString(IDS_ERR_SOCK_SHUT);		// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̉���Ɏ��s\n
		sTransBuf = _sTmp2_trd + sTransBuf;
	}
	if(sock!=INVALID_SOCKET)
	{
		if(closesocket(sock))
		{
			_sTmp2_trd.LoadString(IDS_ERR_SOCK_CLOSE);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̉���Ɏ��s\n
			sTransBuf = _sTmp2_trd + sTransBuf;
		}
	}

	//***********************
	// META SJIS  ����
	//***********************
	for(i=0,j=0;;)
	{
		i=_sTmp_trd.Find("<META", j);
		if(i<0)
		{
			i=_sTmp_trd.Find("<meta", j);
			if(i<0) break;
		}
		j=_sTmp_trd.Find(">", i+1);
		if(j<0) break;
		_sTmp2_trd = _sTmp_trd.Mid(i+1, j-i);
		if(_sTmp2_trd.Find("SHIFT",0) || _sTmp2_trd.Find("shift",0) || _sTmp2_trd.Find("Shift",0) ||
					_sTmp2_trd.Find("sjis",0) || _sTmp2_trd.Find("Sjis",0) || _sTmp2_trd.Find("SJIS",0))
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
			if((_sTmp_trd.GetAt(i) >= (char)0xa1) && (_sTmp_trd.GetAt(i) <= (char)0xfe))
			{
				if(i<(int)_sTmp_trd.GetLength()-1)
				{	// ���Ɉꕶ������ꍇ
					nEucCode = (0xff00 & (_sTmp_trd.GetAt(i) << 8)) | (0xff & _sTmp_trd.GetAt(i+1));
					nEucCode &= 0x7f7f;
					nEucCode = _mbcjistojms(nEucCode);
					if(nEucCode != 0)
					{
						_sTmp_trd.SetAt(i, (char)((nEucCode & 0xff00) >> 8));
						_sTmp_trd.SetAt(i+1, (char)(nEucCode & 0xff));
					}
					i++;

				}
				else
				{	// �Ō�̕����̏ꍇ
				}
			}
		}
	}


	int _nSkip=0;		// �擪�X�L�b�v���̃J�E���g
	int _nGetCt=0;		// �擾���ڐ��̃J�E���g
	sTransBuf = "";

	if(_sTmp_trd != "")
	{
		//**************************************
		// �������o
		//**************************************
		for(i=0,j=0 ; _nGetCt < nPhGetcount ; _nSkip++)
		{
			i = _sTmp_trd.Find(sPhHeader,j);	// ��ؕ����������
			if(i<0) break;
			i += sPhHeader.GetLength();		// ��ؕ����񕶂����C���f�b�N�X��i�߂�
			for(;;)
			{
				if(_sTmp_trd[i+1] == '<') i = _sTmp_trd.Find(">", i+1);		// ����� <...> �������Ă���ꍇ
				else if(_sTmp_trd.Find("<", i+1) > _sTmp_trd.Find(">", i+1))
				{	// ��ؕ����� <...>�̓����ɂ���Ƃ�
					i = _sTmp_trd.Find(">", i+1);
				}
				else break;	// �����ɍ����|����ƁA���[�v������
			}
			if(i<0) break;
			j = _sTmp_trd.Find("<", i+1);	// ���� <...>�̎n�܂��������
			if(j<0) break;
			if(nPhSkip > _nSkip) continue;
			sTransBuf += _sTmp_trd.Mid(i+1, j-i-1);
			sTransBuf += "\n";
			_nGetCt++;
		}

		bUpdated = TRUE;		// ���X�V
	}

	//**************************************
	// �^�C�g���t��
	//**************************************
	if(sTransBuf != "" && sTitle != "")
	{
		_tm_trd = CTime::GetCurrentTime();
		_sTmp_trd.LoadString(IDS_STR_TITLEFMT);		// %s     %2d��%2d��%02d��%02d�� ����\n
		_sTmp2_trd.Format(_sTmp_trd, sTitle, _tm_trd.GetMonth(), _tm_trd.GetDay(), _tm_trd.GetHour(), _tm_trd.GetMinute());
		sTransBuf = _sTmp2_trd + sTransBuf;
	}

	bInThread = FALSE;		// �X���b�h���쒆

	_endthread();
}
