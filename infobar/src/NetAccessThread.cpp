// NetAccessThread.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "infobar00.h"
#include "NetAccessThread.h"
//#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 * WinSock 2 extension -- manifest constants for shutdown()
 */
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02

#define		RECVSIZE	5000		// ��x�Ɏ�M�ł���ő�T�C�Y

//extern CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
//extern CString		_sTmp_MidBuf;	// sTransBuf �ɓn���ŏI���ʊi�[�p
//extern CString		_sPhHeaderCur;	// ���݂̋�؂蕶����
//extern CTime		_tm_trd;


/////////////////////////////////////////////////////////////////////////////
// CNetAccessThread

IMPLEMENT_DYNCREATE(CNetAccessThread, CWinThread)

CNetAccessThread::CNetAccessThread()
{
}

CNetAccessThread::~CNetAccessThread()
{
}

BOOL CNetAccessThread::InitInstance()
{
	// TODO: ���̈ʒu�ɃX���b�h�P�ʂ̏������R�[�h��ǉ����Ă��������B
#if 0
	CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
	CString		_sTmp_MidBuf;	// sTransBuf �ɓn���ŏI���ʊi�[�p
	CString		_sPhHeaderCur;	// ���݂̋�؂蕶����
	CTime		_tm_trd;
	
	int			i,j;
	BOOL		bSockResult;
	int _nSkip=0;		// �擪�X�L�b�v���̃J�E���g
	int _nGetCt=0;		// �擾���ڐ��̃J�E���g

	// �X���b�h�̏d���N���͔�����
	if(*bInThread == TRUE)
		return TRUE;

	sURL = _sURL;
	sProxy = _sProxy;
	sPhHeader = _sPhHeader;
	sPhHeaderB = _sPhHeaderB;
	sPhHeaderC = _sPhHeaderC;
	sTitle = _sTitle;
	sItems = _sItems;


	//***********************
	// �����l�̐ݒ�
	//***********************
	*bInThread = TRUE;			// �X���b�h���쒆
	*bUpdated = FALSE;			// �X�V���I����Ă��Ȃ����Ƃ�����
	_sTmp_trd = "";				// ��M���� HTML ���ꎞ�i�[
	_sTmp_MidBuf = "";

	if(nMode == 1)
	{	// Yahoo �̏،��f�[�^����M����

		Receive_Mode_01();		// ��M�֐��̃��C��

//		if(*sTransBuf != "")
		if(strlen(_sTransBuf))
			*bUpdated = TRUE;		// ���X�V

		*bInThread = FALSE;		// �X���b�h���쒆

		return TRUE;
	}



	// sURL �̃A�h���X�� HTML ����M���A _sTmp3_trd �ɓ��e���i�[
	bSockResult = receive_HTML(&sURL, &_sTmp3_trd);

	_sTmp_trd = _sTmp3_trd;		// �ꎞ�o�b�t�@�ɃR�s�[

	if(_sTmp_trd != "" && bSockResult)
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
				if(_sTmp_trd[i+1] == '<')
				{	// ����� <...> �������Ă���ꍇ
					// ���� ">" ��
					i = _sTmp_trd.Find(">", i+1) + 1;	// ">"�̕��� 1 �����Ă���
				}
				else if(_sTmp_trd.Find("<", i+1) > _sTmp_trd.Find(">", i+1))
				{	// ��ؕ����� <...>�̓����ɂ���Ƃ�
					// ���� ">" ��
					i = _sTmp_trd.Find(">", i+1) + 1;	// ">"�̕��� 1 �����Ă���
				}
				else break;	// �����ɍ����|����ƁA���[�v������
			}
			if(i<0) break;
			j = _sTmp_trd.Find("<", i+1);	// ���� <...>�̎n�܂��������
			if(j<0) break;
			if(nPhSkip > _nSkip) continue;

			_sTmp2_trd = _sTmp_trd.Mid(i, j-i);		// �P�s���o

			if(_sTmp2_trd == "" || _sTmp2_trd == "\n") continue;	// �󕶎���X�L�b�v
			_sTmp_MidBuf += _sTmp2_trd;		// �o�b�t�@�ɒǉ�
			if(_sTmp_MidBuf[_sTmp_MidBuf.GetLength()-1] != '\n')
				_sTmp_MidBuf += "\n";		// �ŏI������ ���s �łȂ����́A���s������
			_nGetCt++;

		}

	}



	//**************************************
	// �^�C�g���t��
	//**************************************
	if(_sTmp_MidBuf != "" && sTitle != "" && bSockResult)
	{
		_tm_trd = CTime::GetCurrentTime();
		_sTmp_trd.LoadString(IDS_STR_TITLEFMT);		// %s     %2d��%2d��%02d��%02d�� ����\n
		_sTmp2_trd.Format(_sTmp_trd, sTitle, _tm_trd.GetMonth(), _tm_trd.GetDay(), _tm_trd.GetHour(), _tm_trd.GetMinute());
		_sTmp_MidBuf = _sTmp2_trd + _sTmp_MidBuf;
	}

	if(!bSockResult)
	{
		_sTmp_MidBuf = _sTmp_trd;
	}

//	*sTransBuf = _sTmp_MidBuf;
	strcpy(_sTransBuf, _sTmp_MidBuf);

	
//	if(*sTransBuf != "")
	if(strlen(_sTransBuf))
		*bUpdated = TRUE;		// ���X�V

	*bInThread = FALSE;		// �X���b�h���쒆



	
#endif
	
	return TRUE;
}

int CNetAccessThread::ExitInstance()
{
	// TODO: ���̈ʒu�ɃX���b�h�P�ʂ̏������R�[�h��ǉ����Ă��������B
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CNetAccessThread, CWinThread)
	//{{AFX_MSG_MAP(CNetAccessThread)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ����܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetAccessThread ���b�Z�[�W �n���h��

void CNetAccessThread::Receive_Mode_01()
{
#if 0
	int			i,j,_i,_j;
	int _nSkip=0;		// �擪�X�L�b�v���̃J�E���g
	int _nGetCt=0;		// �擾���ڐ��̃J�E���g
	CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
	CString		_sTmp_MidBuf;	// sTransBuf �ɓn���ŏI���ʊi�[�p
	CString		_sPhHeaderCur;	// ���݂̋�؂蕶����
	CTime		_tm_trd;

	_sTmp_MidBuf = "";

	for(_i=0,_j=0; ; )
	{
		_j = sItems.Find("\n", _i);
		if(_j<0)
			_j = sItems.GetLength()+1;		// ���̍s���Ȃ����i�Ō��NULL���w��)
		_sTmp3_trd = sItems.Mid(_i, _j-_i-1);	// �P�s�����o

		if(_sTmp3_trd == "" || _sTmp3_trd == "\n")
		{
			_i = _j+1;							// ���s�̎��̕���
			if(_j >= sItems.GetLength()) break;	// �ŏI�s���z����
			continue;
		}

		// sURL �� "%s" �� _sTmp3_trd �̃R�[�h��}������
		_sTmp4_trd.Format(sURL, _sTmp3_trd);

		// _sTmp4_trd �̃A�h���X�� HTML ����M���A _sTmp3_trd �ɓ��e���i�[
		receive_HTML(&_sTmp4_trd, &_sTmp3_trd);

		_sTmp_trd = _sTmp3_trd;		// �ꎞ�o�b�t�@�ɃR�s�[
//		::MessageBox(NULL, _sTmp_trd, "debug - A", MB_OK);

		if(_sTmp_trd != "")
		{
			//**************************************
			// �������o
			//**************************************
			_sPhHeaderCur = sPhHeader;	// �ŏ��̋�؂蕶��
			_nGetCt = 0;
			for(i=0,j=0 ; _nGetCt < nPhGetcount ; _nSkip++)
			{
				i = _sTmp_trd.Find(_sPhHeaderCur,j);	// ��ؕ����������
				if(i<0) break;
				i += _sPhHeaderCur.GetLength();		// ��ؕ����񕶂����C���f�b�N�X��i�߂�
				for(;;)
				{
					if(_sTmp_trd[i+1] == '<')
					{	// ����� <...> �������Ă���ꍇ
						// ���� ">" ��
						i = _sTmp_trd.Find(">", i+1) + 1;	// ">"�̕��� 1 �����Ă���
					}
					else if(_sTmp_trd.Find("<", i+1) > _sTmp_trd.Find(">", i+1))
					{	// ��ؕ����� <...>�̓����ɂ���Ƃ�
						// ���� ">" ��
						i = _sTmp_trd.Find(">", i+1) + 1;	// ">"�̕��� 1 �����Ă���
					}
					else break;	// �����ɍ����|����ƁA���[�v������
				}
				if(i<0) break;
				j = _sTmp_trd.Find("<", i+1);	// ���� <...>�̎n�܂��������
				if(j<0) break;
				if(nPhSkip > _nSkip) continue;

				_sTmp2_trd = _sTmp_trd.Mid(i, j-i);		// �P�s���o

				if(_sTmp2_trd == "" || _sTmp2_trd == "\n") continue;	// �󕶎���X�L�b�v
				_sTmp_MidBuf += _sTmp2_trd;		// �o�b�t�@�ɒǉ�
				if(_sTmp_MidBuf[_sTmp_MidBuf.GetLength()-1] == '\n')
				{
					_sTmp_MidBuf.SetAt(_sTmp_MidBuf.GetLength()-1, ' ');
					_sTmp_MidBuf += "  ";
				}
				else
					_sTmp_MidBuf += "   ";

				_nGetCt++;

				if(_nGetCt == 1)
					_sPhHeaderCur = sPhHeaderB;	// �ŏ��̋�؂蕶��
				else if(_nGetCt == 2)
					_sPhHeaderCur = sPhHeaderC;	// �ŏ��̋�؂蕶��
				else
					break;
			}

		}
		if(_sTmp_MidBuf.GetLength() >0)
		{
			if(_sTmp_MidBuf[_sTmp_MidBuf.GetLength()-1] != '\n')
				_sTmp_MidBuf += "\n";		// �ŏI������ ���s �łȂ����́A���s������
		}


		_i = _j+1;							// ���s�̎��̕���
		if(_j >= sItems.GetLength()) break;	// �ŏI�s���z����

		::MessageBox(NULL, _sTmp_MidBuf, "debug", MB_OK);

	}

	//**************************************
	// �^�C�g���t��
	//**************************************
	if(_sTmp_MidBuf != "" && sTitle != "")
	{
		_tm_trd = CTime::GetCurrentTime();
		_sTmp_trd.LoadString(IDS_STR_TITLEFMT);		// %s     %2d��%2d��%02d��%02d�� ����\n
		_sTmp2_trd.Format(_sTmp_trd, sTitle, _tm_trd.GetMonth(), _tm_trd.GetDay(), _tm_trd.GetHour(), _tm_trd.GetMinute());
		_sTmp_MidBuf = _sTmp2_trd + _sTmp_MidBuf;
	}

//	*sTransBuf = _sTmp_MidBuf;
	strcpy(_sTransBuf, _sTmp_MidBuf);

	return ;
#endif
}

BOOL CNetAccessThread::receive_HTML(CString *sRURL, CString *sHTML)
{
#if 0
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

	CString		_sTmp_trd, _sTmp2_trd, _sTmp3_trd, _sTmp4_trd;
	CString		_sTmp_MidBuf;	// sTransBuf �ɓn���ŏI���ʊi�[�p
	CString		_sPhHeaderCur;	// ���݂̋�؂蕶����
	CTime		_tm_trd;


//	CSocket *_sock;
	WSADATA _sock_wsa;


	//***********************
	// �����l�̐ݒ�
	//***********************
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
	AfxSocketInit(&_sock_wsa);	// WinSock �̏�����
//	WORD wVersionRequested = MAKEWORD( 2, 0 );
//	WSAStartup( wVersionRequested, &_sock_wsa );

//	_sock = new CSocket();

//	if(_sock->Create() == 0)
	if((sock=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
	{	// �\�P�b�g�쐬�s�\
		sHTML->LoadString(IDS_ERR_SOCK_CREAT);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̍쐬���s\n
//		delete _sock;
		return FALSE;
	}
	// ====================================================================
	// URL�̐擪�� "http://"�����Ă����ꍇ�͍폜����
	// ====================================================================
	if(!strncmp((LPCSTR)*sRURL, "http://", 7))
	{
		_sTmp_trd = sRURL->Mid(7, sRURL->GetLength()-7);
		*sRURL = _sTmp_trd;
	}

	// ====================================================================
	// URL�̕���
	// ====================================================================

	memset(szHostName,'\0',sizeof(szHostName));
	memset(szHtmlName,'\0',sizeof(szHtmlName));

	strcpy(szURL, *sRURL);
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
		lstrcat(szHtmlName,*sRURL);
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
		sHTML->LoadString(IDS_ERR_SOCK_GETHOST);		// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�Ńz�X�g���̉������ł��܂���\n
		shutdown(sock,SD_BOTH);
		closesocket(sock);
		return FALSE;
	}

	// ====================================================================
	// �ڑ�
	// ====================================================================

	sai.sin_port=htons((u_short)nPort);
	sai.sin_family=AF_INET;
	sai.sin_addr=*((LPIN_ADDR)*m_pHostEnt->h_addr_list);

//	if(_sock->Connect(szConnHostName, nPort) == 0)
	if(connect(sock,(LPSOCKADDR)&sai,sizeof(sai)) == SOCKET_ERROR)
	{	// �ڑ��ł��܂���ł���
		int x = WSAGetLastError ();
		sHTML->LoadString(IDS_ERR_SOCK_CONN);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̐ڑ��Ɏ��s���܂���\n
		shutdown(sock,SD_BOTH);
		closesocket(sock);
//		_sock->ShutDown(2);
//		_sock->Close();
//		delete _sock;
		return FALSE;
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
//		i = _sock->Send(psz, lstrlen(psz));
		i = send(sock, psz, lstrlen(psz), 0);	// ���M
		if(i == SOCKET_ERROR)
		{
			sHTML->LoadString(IDS_ERR_SOCK_CONN);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̐ڑ��Ɏ��s���܂���\n
//			_sock->ShutDown(2);
//			_sock->Close();
			shutdown(sock,SD_BOTH);
			closesocket(sock);
//			delete _sock;
			return FALSE;
		}
		// ���M�o�C�g���� i �Ɋi�[�����B
		psz += i;
	}

	// ====================================================================
	// ��M
	// ====================================================================

	for(;;)
	{
//		i = _sock->Receive(pszBuf, RECVSIZE);
		i = recv(sock, pszBuf, RECVSIZE, 0);		// ��M
		// ��M�o�C�g���� i �Ɋi�[�����
		if(i<=0 || i==SOCKET_ERROR) break;		// ��M�I��

		pszBuf[i] = (char)NULL;		// ������Ƃ��Ċ��������邽�߁A������ NULL �ǉ�

		_sTmp_trd += (LPCSTR)pszBuf;

	}
	dwTotalSize = _sTmp_trd.GetLength();

	if(i==SOCKET_ERROR)
		::MessageBox(NULL, "sock err", "debug -C", MB_OK);


	// ====================================================================
	// �ʐM�I��
	// ====================================================================
//	_sock->ShutDown(2);
//	_sock->Close();
//	delete _sock;

	if(shutdown(sock,SD_BOTH))
	{
		sHTML->LoadString(IDS_ERR_SOCK_SHUT);		// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̉���Ɏ��s\n
		return FALSE;
	}
	if(sock!=INVALID_SOCKET)
	{
		if(closesocket(sock))
		{
			sHTML->LoadString(IDS_ERR_SOCK_CLOSE);	// �l�b�g���[�N�E�G���[�FWindows�\�P�b�g�̉���Ɏ��s\n
			return FALSE;
		}
	}

//	::WSACleanup();

	//***********************
	// META SJIS  ����
	//***********************
	for(i=0,j=0;;)
	{
		i=_sTmp_trd.Find("<META", j);
		if(i<0)
		{
			break;
		}
		j=_sTmp_trd.Find(">", i+1);
		if(j<0) break;
		_sTmp2_trd = _sTmp_trd.Mid(i+1, j-i);
		if(_sTmp2_trd.Find("SHIFT",0)!=-1 || _sTmp2_trd.Find("shift",0)!=-1 || _sTmp2_trd.Find("Shift",0)!=-1 ||
					_sTmp2_trd.Find("sjis",0)!=-1 || _sTmp2_trd.Find("Sjis",0)!=-1 || _sTmp2_trd.Find("SJIS",0)!=-1)
		{
			isSJIS = TRUE;
			break;
		}
	}		
	for(i=0,j=0;;)
	{
		i=_sTmp_trd.Find("<meta", j);
		if(i<0)
		{
			break;
		}
		j=_sTmp_trd.Find(">", i+1);
		if(j<0) break;
		_sTmp2_trd = _sTmp_trd.Mid(i+1, j-i);
		if(_sTmp2_trd.Find("SHIFT",0)!=-1 || _sTmp2_trd.Find("shift",0)!=-1 || _sTmp2_trd.Find("Shift",0)!=-1 ||
					_sTmp2_trd.Find("sjis",0)!=-1 || _sTmp2_trd.Find("Sjis",0)!=-1 || _sTmp2_trd.Find("SJIS",0)!=-1)
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
	*sHTML = _sTmp_trd;
	::MessageBox(NULL, _sTmp_trd, "debug -B", MB_OK);
#endif

	return TRUE;
}
