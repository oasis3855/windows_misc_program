// GpsTimeSetDlg.cpp : ���C���_�C�A���O�����t�@�C��
//

// ***********************
// ���C���_�C�A���O�A�X���b�h�̏��� ver 1.0.0.1
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

#include "stdafx.h"
#include "GpsTimeSet.h"
#include "GpsTimeSetDlg.h"
#include ".\gpstimesetdlg.h"

#include "SerialCom.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGpsTimeSetDlg �_�C�A���O



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


// CGpsTimeSetDlg ���b�Z�[�W �n���h��


/********
OnInitDialog()
�_�C�A���O�\���O�̏����ݒ�

�����ϐ��̏����l�ݒ���s��
�|�[�g�ԍ��A�ʐM���x�h���b�v�_�E�����X�g�̏����ݒ���s��
********/

BOOL CGpsTimeSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B


	int i;
	char sTemp[7];

	// �R���{�{�b�N�X�FCOM�|�[�g�̏����l��ݒ�
	for(i=1; i<20; i++)
	{
		sprintf(sTemp, "COM%d", i);
		m_ctrlComboPortname.AddString(sTemp);
	}

	m_ctrlComboPortname.SetCurSel(3);

	// �R���{�{�b�N�X�FCOM�|�[�g���x�̏����l��ݒ�
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


	m_ThreadActivate = 0;					// �X���b�h��~��
	m_ThreadTerminateCommand = 0;			// �X���b�h��~���� ����

	m_ThreadSetTime = 0;					// �����ݒ�R�}���h ����

	m_pThread = NULL;		// �X���b�h�|�C���^������

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CGpsTimeSetDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CGpsTimeSetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/********
OnBnClickedBtnComopen()
��M�J�n�{�^�������������̏���

��M�X���b�h�iThreadMain�j���N������
********/

void CGpsTimeSetDlg::OnBnClickedBtnComopen()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B

	if(m_pThread)
	{	// �X���b�h�����ɑ��݂���ꍇ
		SetDlgItemText(IDC_EDIT_TIME_PC, (LPCTSTR)"Thread Already Running");
	}
	else
	{
		// COM�|�[�g�ԍ��ݒ���A�R���{�{�b�N�X���ǂݎ��
		GetDlgItemText(IDC_COMBO_PORTNAME, (LPTSTR)m_sPortName, 6);

		// Baud Rate�ݒ���A�R���{�{�b�N�X���ǂݎ��
		m_nBaudRate = GetDlgItemInt(IDC_COMBO_BAUD);

		// �X���b�h�����i��~��ԂŐ����j
		m_pThread = ::AfxBeginThread(ThreadMain, (LPVOID)this, 0, 0, CREATE_SUSPENDED, NULL);

		m_pThread->m_bAutoDelete = FALSE;	// �X���b�h�����j���t���O�N���A
		m_pThread->ResumeThread();			// �X���b�h����J�n
	}
}


/********
OnBnClickedBtnSettime()
�����ݒ�{�^�������������̏���

��M�X���b�h�iThreadMain�j�ɑ΂��āA�����ݒ�t���O���Z�b�g����
********/

void CGpsTimeSetDlg::OnBnClickedBtnSettime()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B

	m_ThreadSetTime = 1;		// �����ݒ�R�}���h ���荞��ON
}


/********
OnBnClickedBtnComclose()
��M���~�{�^�������������̏���

��M�X���b�h�iThreadMain�j���~����
�i�X���b�h��~�̂��߂̃t���O���Z�b�g���A�X���b�h��~��҂j
********/

void CGpsTimeSetDlg::OnBnClickedBtnComclose()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	// �X���b�h���N�����Ă��Ȃ��ꍇ
	if(m_ThreadActivate == 0)
	{
		SetDlgItemText(IDC_EDIT_TIME_PC, (LPCTSTR)"Thread Not Running");
		return;
	}

	//�X���b�h�ɏI���t���O�𑗐M����
	m_ThreadTerminateCommand = 1;

	// �X���b�h���I������̂�҂�
	for(int i=0; i<50; i++)
	{
		if(m_ThreadActivate == 0) break;
		::Sleep(100);
	}

	// �X���b�h���I�����Ă��邩�Ċm�F
	if(::WaitForSingleObject(m_pThread->m_hThread, 100) == WAIT_TIMEOUT)
	{	// �X���b�h�I�����m�F�ł����A�^�C���A�E�g����
		::TerminateThread(m_pThread->m_hThread, 0xffffffff);
	}

	// m_bAutoDelete = FALSE �̏ꍇ�A�X���b�h�̃I�u�W�F�N�g���蓮�ō폜
	delete m_pThread;
	m_pThread = NULL;

	SetDlgItemText(IDC_EDIT_TIME_PC, (LPCTSTR)"Thread Stop");

	m_ThreadActivate = 0;					// �X���b�h��~��
	m_ThreadTerminateCommand = 0;			// �X���b�h��~���� ����

}


/********
OnBnClickedOk()
����{�^�������������̏���

********/

void CGpsTimeSetDlg::OnBnClickedOk()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B

	// ��M���~�{�^���̏����i�X���b�h��~�j���s��
	if(m_ThreadActivate) OnBnClickedBtnComclose();

	OnOK();
}


/********
ThreadMain(LPVOID pParam)
�X���b�h�֐�

���́j
  ���C���_�C�A���O�N���X�ւ̃|�C���^�FpParam

�ˑ��j
�@���ۂ̎�M�����́AThreadDataReceive() �֐����Ăяo���Ă���

********/

UINT CGpsTimeSetDlg::ThreadMain(LPVOID pParam)
{
	// ���C���_�C�A���O�N���X�ւ̃|�C���^
	CGpsTimeSetDlg *pDlg = (CGpsTimeSetDlg*)pParam;

	return pDlg->ThreadDataReceive(pParam);
}


/********
ThreadDataReceive(LPVOID pParam)
�f�[�^��M���[�v

���́j
  ���C���_�C�A���O�N���X�ւ̃|�C���^�FpParam

�����g�p���Ă���N���X�ϐ��j
  �X���b�h�Ăяo�����ɁA�X���b�h�ғ�����m�点��ϐ��Fint m_ThreadActivate
  �X���b�h�Ăяo��������A��~���߂��󂯂�t���O�Fint m_ThreadTerminateCommand
  CSerialCom�Ŏ�M�����f�[�^���󂯎��FBYTE Com.m_LineBufferEx[]

�ˑ��j
�@�V���A���ʐM�̂��߁ACSerialCom�N���X�𐶐�����

********/

UINT CGpsTimeSetDlg::ThreadDataReceive(LPVOID pParam)
{
	CString sTemp = "";
	char sPortName[10];
	int nSize;

	// ���C���_�C�A���O�N���X�ւ̃|�C���^
	CGpsTimeSetDlg *pDlg = (CGpsTimeSetDlg*)pParam;

	pDlg->m_ThreadActivate = 1;			// �X���b�h�̓A�N�e�B�u
	pDlg->m_ThreadTerminateCommand = 0;	// ��~���߂��N���A

	CSerialCom Com;

	// �V���A���|�[�g���J��
	// Windows �̏ꍇ�� Com.Open("\\\\.\\COM4", 9600);
	// CE �̏ꍇ�� Com.Open(_T("COM4:"), 9600);
	sprintf(sPortName, "\\\\.\\%s", m_sPortName);
	if(!Com.Open(sPortName, m_nBaudRate))
	{
		pDlg->SetDlgItemText(IDC_EDIT_TIME_PC, "COM Open Error");
		return 0;
	}

	// �V���A����M ���[�v
	for(;;)
	{
		nSize = Com.ReadLineEx();

		// �X���b�h��~���߃t���O���󂯎�����ꍇ�A���[�v�𔲂���
		if(pDlg->m_ThreadTerminateCommand) break;

		if(nSize == 0) continue;		// �؂�o���f�[�^���[���̎��A���g���C
		if(nSize < 0) break;			// �G���[�̂Ƃ��A���[�v�𔲂���

		Com.m_LineBufferEx[nSize] = (BYTE)0;	// �����񖖒[��NULL��t��

		// NMEA�`�F�b�N�T�����i�̎��AGGA��ZDA�̉�͂ƕ\���������s��
		if(NmeaChecksum((LPCSTR)Com.m_LineBufferEx))
		{
			GGA_Process((LPCSTR)Com.m_LineBufferEx, pParam);
			ZDA_Process((LPCSTR)Com.m_LineBufferEx, pParam);
		}
	}

	// �V���A���|�[�g�����
	Com.Close();

	pDlg->m_ThreadActivate = 0;			// �X���b�h�͒�~
	pDlg->m_ThreadTerminateCommand = 0;	// ��~���߂��N���A

	return 0;

}


/********
NmeaChecksum(const char * sNmea)
NMEA�Z���e���X�̃`�F�b�N�T������������

���́j
�@��͂���NMEA�Z���e���X�Fconst char * sNmea

�߂�l�j
  �`�F�b�N�T����v�F1
  �`�F�b�N�T���s��v�F0
********/

BOOL CGpsTimeSetDlg::NmeaChecksum(const char * sNmea)
{
	char cksm = 0;
	char sCksm[5];

	// ������̐擪����A�ꕶ�����X�L�������ă`�F�b�N�T���v�Z
	for(unsigned int i=0; i<strlen(sNmea); i++)
	{
		// '$' �� '!' �͌v�Z���Ȃ��i�ǂݔ�΂��j
		if(sNmea[i] == '$' || sNmea[i] == '!') continue;

		// '*' �����o���ꂽ��I��
		if(sNmea[i] == '*') break;

		cksm = cksm ^ sNmea[i];
	}

	if(i >= strlen(sNmea)) return 0;	// �`�F�b�N�T�� �f���~�^ "*" ����������

	sprintf(sCksm, "%02X\r\n", cksm);
	if(!stricmp(sNmea+i+1, sCksm))
		return 1;	// �`�F�b�N�T������v�����ꍇ

	return 0;		// �`�F�b�N�T���s��v
}


/********
GGA_Process(const char * sNmea, LPVOID pParam)
GGA NMEA�Z���e���X����͂��A�_�C�A���O�ɕ\������B

���́j
�@��͂���NMEA�Z���e���X�Fconst char * sNmea
  �_�C�A���O�N���X�ւ̃|�C���^�FLPVOID pParam

�ˑ��j
�@NMEA�Z���e���X��','�Ő؂蕪���邽�ߗ��p�FSplitString()
  �������������͂��邽�ߗ��p�FTimeSeparate()
  PC�̃V�X�e�����Ԃ�\�����邽�ߗ��p�FPcTimeDisp()

�߂�l�j
  ��ɁF0

�Q�l�j
  NMEA�Z���e���X�̗� �iGGA - Global Positioning System Fix Data�j
  $GPGGA,031916.000,3512.3456,N,13512.3456,E,1,04,2.6,514.7,M,34.3,M,,0000*5C

  $GPGGA,[�����b],[�ܓx],[N/S],[�o�x],[W/E],[���[�h],[�⑫�q����],[HDOP],[���x],[�P�ʌn],[�W�I�C�h�␳],[�P�ʌn],[DGPS�G�C�W],[DGPS��n�R�[�h]*[�`�F�b�N�T��]

  �Q�ƁFhttp://gpsd.berlios.de/NMEA.txt
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

	// �����t�B�[���h
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

	// �ܓx�t�B�[���h
	sTemp = "";
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) sTemp = sToken;
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) sTemp = sTemp + " " + sToken;
	if(sTemp == "") sTemp = "----";
	pDlg->SetDlgItemText(IDC_EDIT_LATITUDE, sTemp);

	// �o�x�t�B�[���h
	sTemp = "";
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) sTemp = sToken;
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) sTemp = sTemp + " " + sToken;
	if(sTemp == "") sTemp = "----";
	pDlg->SetDlgItemText(IDC_EDIT_LONGITUDE, sTemp);

	// ��MFIX���[�h
	sToken = SplitString(NULL, sSeparator);

	// �q�����t�B�[���h
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_SAT, "----");
	else pDlg->SetDlgItemText(IDC_EDIT_SAT, sToken);

	// HDOP�t�B�[���h
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_HDOP, "----");
	else pDlg->SetDlgItemText(IDC_EDIT_HDOP, sToken);

	// ���x�t�B�[���h
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_ALTITUDE, "----");
	else pDlg->SetDlgItemText(IDC_EDIT_ALTITUDE, sToken);

	// �P�ʌn���[�h
	sToken = SplitString(NULL, sSeparator);

	// �W�I�C�h�␳�t�B�[���h
	sToken = SplitString(NULL, sSeparator);
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_GEOID, "----");
	else pDlg->SetDlgItemText(IDC_EDIT_GEOID, sToken);

	return 0;
}


/********
ZDA_Process(const char * sNmea, LPVOID pParam)
ZDA NMEA�Z���e���X����͂��A�_�C�A���O�ɕ\������B

���́j
�@��͂���NMEA�Z���e���X�Fconst char * sNmea
  �_�C�A���O�N���X�ւ̃|�C���^�FLPVOID pParam

�ˑ��j
�@NMEA�Z���e���X��','�Ő؂蕪���邽�ߗ��p�FSplitString()
  �������������͂��邽�ߗ��p�FTimeSeparate()
  PC�̃V�X�e�����Ԃ�\�����邽�ߗ��p�FPcTimeDisp()

�߂�l�j
  ��ɁF0

  �Q�l�j
  NMEA�Z���e���X�̗� �iZDA - Time & Date - UTC, day, month, year and local time zone�j
  $GPZDA,031921.000,19,04,2008,,*58

  $GPZDA,[�����b],[��],[��],[�N],[+-TZ hour],[TZ minutes]*[�`�F�b�N�T��]

  �Q�ƁFhttp://gpsd.berlios.de/NMEA.txt
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

	// ����
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL)
		TimeSeparate(sToken, &nHour, &nMinutes, &nSecond);

	// ��
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) nDay = ::atoi(sToken);
	// ��
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) nMonth = ::atoi(sToken);
	// �N
	sToken = SplitString(NULL, sSeparator);
	if(sToken != NULL) nYear = ::atoi(sToken);

	sTemp.Format("%04d/%02d/%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMinutes, nSecond);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_ZDA, sTemp);

	PcTimeDisp(nYear, nMonth, nDay, nHour, nMinutes, nSecond, pParam);

	return 0;
}


/********
TimeSeparate(char * sTimeStr, int *nHour, int *nMinutes, int *nSecond)
�����i�����b�j�������؂蕪���A�ϐ��ɑ������֐�

���́j
�@��͂��鎞���b�Z���e���X�Fchar * sTimeStr
  �؂蕪���������b���i�[����ϐ��Fint *nHour, int *nMinutes, int *nSecond

  �� sTimeStr�͔j�󂳂�Ȃ��istrtok���p�O�ɁA��Ɨp������ɃR�s�[�j

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

	::strcpy(sSplitBuffer, sTimeStr);				// �j�������邽�߁A��Ɨp������ɃR�s�[

	sToken = ::strtok(sSplitBuffer, sSeparator);	// '.' �Ő؂蕪��

	// �����b������́A6�����ł��邱�Ƃ��m�F
	if(::strlen(sToken) != 6) return;

	sTemp[2] = (char)0;		// ���E���E�b��2�������̂��߁A3�����ڂ�NULL���Z�b�g

	// ��
	sTemp[0] = sToken[0];
	sTemp[1] = sToken[1];
	*nHour = ::atoi(sTemp);

	// ��
	sTemp[0] = sToken[2];
	sTemp[1] = sToken[3];
	*nMinutes = ::atoi(sTemp);

	// �b
	sTemp[0] = sToken[4];
	sTemp[1] = sToken[5];
	*nSecond = ::atoi(sTemp);

	return;
}


/********
SplitString(char *sBuf, char *sSeparator)
��������A�Z�p���[�^�Ő؂蕪����

�� strtok�̓Z�p���[�^���A�������ꍇ�ǂݔ�΂����A���̊֐���NULL�������Ԃ��B
  ��F���͕����� "A,,,B" �� ','��p���Đ؂�o���ꍇ
      strtok -> "A","B"
	  SplitString -> "A","","","B"

���́j
�@��͂��镶����Fchar *sBuf
    �O��̌��ʂ���A�A�����Đ؂�o���ꍇNULL���Z�b�g�istrtok�d�l�Ɠ����j
  �Z�p���[�^�Fchar *sSeparator

  �� sBuf�͔j�󂳂��i�؂�o����NULL�������}�������j

********/

char * CGpsTimeSetDlg::SplitString(char *sBuf, char *sSeparator)
{
	static int nEndPt=0;
	static int nStartPt=0;
	static int nStrlen=0;
	static char *sBufCur=NULL;

	// ����
	if(sBuf != NULL)
	{
		nStrlen = (int)::strlen(sBuf);
		nEndPt = 0;
		sBufCur = sBuf;
	}
	// �A���؂�o��
	else
	{
		sBufCur += nEndPt + 1;
	}

	// �����񖖒[�̂Ƃ�
	if(sBufCur[0] == (char)NULL) return NULL;

	nEndPt = (int)::strcspn(sBufCur, sSeparator);

	// �Z�p���[�^��������Ȃ����́A�����񖖒[�܂ł̒������Ԃ����

	// �Z�p���[�^��NULL�ɒu��
	if(nEndPt >= 0)
	{
		sBufCur[nEndPt] = (char)NULL;
	}

	nEndPt = (int)::strlen(sBufCur);

	return sBufCur;

}


/********
PcTimeDisp(int nYear, int nMonth, int nDay, int nHour, int nMinutes, int nSecond, LPVOID pParam)
PC�̃V�X�e�������̕\���ƁA�V�X�e�������̕ύX

���́j
  GPS�œ����N�����Fint nYear, int nMonth, int nDay, 
  GPS�œ��������b�Fint nHour, int nMinutes, int nSecond
  ���C���_�C�A���O�N���X�ւ̃|�C���^�FpParam

�����g�p���Ă���N���X�ϐ��j
  �X���b�h�Ăяo��������A�����ݒ薽�߂��󂯂�t���O�Fint m_ThreadSetTime

�ˑ��j
�@Win32SDK SetLocalTime �� �V�X�e�������ύX�����������ꍇ�A�����������Ȃ�

********/
#define _TIME_FUNCTION_CE	// CE �݊��ɂ���ꍇ�͐錾����

void CGpsTimeSetDlg::PcTimeDisp(int nYear, int nMonth, int nDay, int nHour, int nMinutes, int nSecond, LPVOID pParam)
{
	SYSTEMTIME tmSystem;	// �V�X�e�������𓾂�A�ݒ肷�鎞�Ɏg�p
#ifdef _TIME_FUNCTION_CE	// (1) CTime���p��
	CTime tmGpsTime;			// �����v�Z�p�iGPS�����j
	CTime tmLocalTime;			// �����v�Z�p�i���[�J�������j
#else						// (2) C time�֐����p�� (WinCE�s�j
	struct tm tmGpsTime;		// �����v�Z�p�iGPS�����j
	struct tm *ptmLocalTime;	// �����v�Z�p�i���[�J�������j
	time_t nTimeTemp;			// �����v�Z�p
#endif //_TIME_FUNCTION_CE
	TIME_ZONE_INFORMATION tzInfo;
	DWORD nTzResult;
	CString sTemp;

	CGpsTimeSetDlg *pDlg = (CGpsTimeSetDlg*)pParam;

	::GetLocalTime(&tmSystem);		// �V�X�e���̃��[�J���^�C���𓾂�
	nTzResult = ::GetTimeZoneInformation(&tzInfo);		// �V�X�e���̃^�C���]�[���ݒ�𓾂�

	// �V�X�e���̃��[�J���^�C�����_�C�A���O�ɕ\������
	sTemp.Format("%04d/%02d/%02d %02d:%02d:%02d", tmSystem.wYear, tmSystem.wMonth, tmSystem.wDay, tmSystem.wHour, tmSystem.wMinute, tmSystem.wSecond);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_PC, sTemp);

	// �V�X�e���̃^�C���]�[�����_�C�A���O�ɕ\������
	sTemp.Format("%+02.1f", (double)tzInfo.Bias / 60.0);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_TZ, sTemp);

	// �����ݒ�
	if(pDlg->m_ThreadSetTime)
	{
		// 
		// �����̑g�ݓ���v�Z �iGPS���� �{ TZ �� ���[�J�������j
		//

#ifdef _TIME_FUNCTION_CE
		/*****(1) CTime���p�� */
		if(nYear != 0)
		{	// �N�������^�����Ă��鎞�iZDA��͂���̏��j
			tmGpsTime = CTime(nYear, nMonth, nDay, nHour, nMinutes, nSecond);

			tmLocalTime = tmGpsTime - (CTimeSpan)(tzInfo.Bias*60);

			tmSystem.wYear = tmLocalTime.GetYear();
			tmSystem.wMonth = tmLocalTime.GetMonth();
			tmSystem.wDay = tmLocalTime.GetDay();

			pDlg->m_ThreadSetTime = 0;		// �����ݒ薽�߃t���O���N���A
		}
		else
		{	// �N�������^�����Ă��Ȃ����iGGA��͂���̏��j
			tmGpsTime = CTime(tmSystem.wYear, tmSystem.wMonth, tmSystem.wDay, nHour, nMinutes, nSecond);

			tmLocalTime = tmGpsTime - (CTimeSpan)(tzInfo.Bias*60);
		}

		tmSystem.wHour = tmLocalTime.GetHour();
		tmSystem.wMinute = tmLocalTime.GetMinute();
		tmSystem.wSecond = tmLocalTime.GetSecond();
		
#else
		/**** (2) C time�֐����p�� (WinCE�s�j*/
		tmGpsTime.tm_hour = nHour;
		tmGpsTime.tm_min = nMinutes;
		tmGpsTime.tm_sec = nSecond;
		if(nYear != 0)
		{	// �N�������^�����Ă��鎞�iZDA��͂���̏��j
			tmGpsTime.tm_year = nYear - 1900;
			tmGpsTime.tm_mon = nMonth - 1;
			tmGpsTime.tm_mday = nDay;

			nTimeTemp = ::mktime(&tmGpsTime);	// �\���̂���long int�b�ɕϊ�
			nTimeTemp -= tzInfo.Bias * 60;		// �����i�b�j�̒���
			ptmLocalTime = ::localtime(&nTimeTemp);	// long int�b����\���̂ɕϊ�

			tmSystem.wYear = ptmLocalTime->tm_year + 1900;
			tmSystem.wMonth = ptmLocalTime->tm_mon + 1;
			tmSystem.wDay = ptmLocalTime->tm_mday;

			pDlg->m_ThreadSetTime = 0;		// �����ݒ薽�߃t���O���N���A
		}
		else
		{	// �N�������^�����Ă��Ȃ����iGGA��͂���̏��j
			tmGpsTime.tm_year = tmSystem.wYear - 1900;	// �V�X�e�������𗬗p
			tmGpsTime.tm_mon = tmSystem.wMonth - 1;	// �V�X�e�������𗬗p
			tmGpsTime.tm_mday = tmSystem.wDay;	// �V�X�e�������𗬗p

			nTimeTemp = ::mktime(&tmGpsTime);	// �\���̂���long int�b�ɕϊ�
			nTimeTemp -= tzInfo.Bias * 60;		// �����i�b�j�̒���
			ptmLocalTime = ::localtime(&nTimeTemp);	// long int�b����\���̂ɕϊ�
		}

		tmSystem.wHour = ptmLocalTime->tm_hour;
		tmSystem.wMinute = ptmLocalTime->tm_min;
		tmSystem.wSecond = ptmLocalTime->tm_sec;

#endif	// _TIME_FUNCTION_CE

		// �V�X�e���̃��[�J���^�C����ύX����i�v�F�����ύX�����j
		::SetLocalTime(&tmSystem);

		// ZDA�e�L�X�g�{�b�N�X�ɉ����\������Ă��Ȃ����́AGGA�Ŏ��Ԑݒ肵�ďI��
		pDlg->GetDlgItemText(IDC_EDIT_TIME_ZDA, sTemp);
		if(sTemp == "")
		{
			pDlg->m_ThreadSetTime = 0;		// �����ݒ薽�߃t���O���N���A
		}
	}

}

//EOF
