// GpsTimeSetCEDlg.cpp : implementation file
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
�_�C�A���O�\���O�̏����ݒ�

�����ϐ��̏����l�ݒ���s��
�|�[�g�ԍ��A�ʐM���x�h���b�v�_�E�����X�g�̏����ݒ���s��
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
	TCHAR wsTemp[7];						// Windows CE Wide-Char �Ή��p

	// �R���{�{�b�N�X�FCOM�|�[�g�̏����l��ݒ�
	for(i=1; i<20; i++)
	{
		sprintf(sTemp, "COM%d", i);
		mbstowcs(wsTemp, sTemp, 6);			// Windows CE �Ή�, MBCS����Wide-Char�֕ϊ�
		m_ctrlComboPortname.AddString(wsTemp);
	}

	m_ctrlComboPortname.SetCurSel(5);

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

	m_ThreadWriteInterrupt = 0;				// �������ݖ��ߖ���

	m_ThreadSetTime = 0;					// �����ݒ�R�}���h ����

	m_pThread = NULL;		// �X���b�h�|�C���^������

	return TRUE;  // return TRUE  unless you set the focus to a control
}


/********
OnBnClickedBtnComopen()
��M�J�n�{�^�������������̏���

��M�X���b�h�iThreadMain�j���N������
********/

void CGpsTimeSetCEDlg::OnBtnComopen() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	TCHAR wsTemp[7];		// Windows CE Wide-Char �Ή��p
	CString sTemp;

	if(m_pThread != NULL)
	{	// �X���b�h�����ɑ��݂���ꍇ
		SetDlgItemText(IDC_EDIT_TIME_PC, _T("Thread is already running"));
	}
	else
	{

		// COM�|�[�g�ԍ��ݒ���A�R���{�{�b�N�X���ǂݎ��
		GetDlgItemText(IDC_COMBO_PORTNAME, wsTemp, 6);
		wcstombs((LPSTR)m_sPortName, wsTemp, 6);	// Windows CE �Ή�, Wide-Char����MBCS�֕ϊ�

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

void CGpsTimeSetCEDlg::OnBtnSettime() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_ThreadSetTime = 1;		// �����ݒ�R�}���h ���荞��ON

}


/********
OnBnClickedBtnComclose()
��M���~�{�^�������������̏���

��M�X���b�h�iThreadMain�j���~����
�i�X���b�h��~�̂��߂̃t���O���Z�b�g���A�X���b�h��~��҂j
********/

void CGpsTimeSetCEDlg::OnBtnComclose() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	// �X���b�h���N�����Ă��Ȃ��ꍇ
	if(m_ThreadActivate == 0)
	{
		SetDlgItemText(IDC_EDIT_TIME_PC, _T("Thread Not Running"));
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

	SetDlgItemText(IDC_EDIT_TIME_PC, _T("Thread Stop"));

	m_ThreadActivate = 0;					// �X���b�h��~��
	m_ThreadTerminateCommand = 0;			// �X���b�h��~���� ����

}


/********
OnOk()
����{�^�������������̏���

********/

void CGpsTimeSetCEDlg::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	// ��M���~�{�^���̏����i�X���b�h��~�j���s��
	if(m_ThreadActivate) OnBtnComclose();

	CDialog::OnOK();
}


/********
ThreadMain(LPVOID pParam)
�X���b�h�֐�

���́j
  ���C���_�C�A���O�N���X�ւ̃|�C���^�FpParam

�ˑ��j
�@���ۂ̎�M�����́AThreadDataReceive() �֐����Ăяo���Ă���

********/

UINT CGpsTimeSetCEDlg::ThreadMain(LPVOID pParam)
{
	// ���C���_�C�A���O�N���X�ւ̃|�C���^
	CGpsTimeSetCEDlg *pDlg = (CGpsTimeSetCEDlg*)pParam;

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

UINT CGpsTimeSetCEDlg::ThreadDataReceive(LPVOID pParam)
{
	CString sTemp = "";
	char sPortName[10];
	TCHAR wsTemp[7];					// Windows CE Wide-Char �Ή��p
	int nSize;

	// ���C���_�C�A���O�N���X�ւ̃|�C���^
	CGpsTimeSetCEDlg *pDlg = (CGpsTimeSetCEDlg*)pParam;

	pDlg->m_ThreadActivate = 1;			// �X���b�h�̓A�N�e�B�u
	pDlg->m_ThreadTerminateCommand = 0;	// ��~���߂��N���A

	CSerialCom Com;

	// �V���A���|�[�g���J��
	// Windows �̏ꍇ�� Com.Open("\\\\.\\COM4", 9600);
	// CE �̏ꍇ�� Com.Open(_T("COM4:"), 9600);
	sprintf(sPortName, "%s:", pDlg->m_sPortName);
	mbstowcs(wsTemp, (LPCSTR)sPortName, 6);			// Windows CE �Ή�, MBCS����Wide-Char�֕ϊ�
	if(!Com.Open(wsTemp, pDlg->m_nBaudRate))
	{
		pDlg->SetDlgItemText(IDC_EDIT_TIME_PC, _T("COM Open Error"));
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

BOOL CGpsTimeSetCEDlg::NmeaChecksum(const char * sNmea)
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
	if(!_stricmp(sNmea+i+1, sCksm))
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

	// �����t�B�[���h
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
	sTemp = sToken;		// Windows CE Wide-Char�Ή��̂��߁A��U CString�ɓ����
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_SAT, _T("----"));
	else pDlg->SetDlgItemText(IDC_EDIT_SAT, sTemp);

	// HDOP�t�B�[���h
	sToken = SplitString(NULL, sSeparator);
	sTemp = sToken;		// Windows CE Wide-Char�Ή��̂��߁A��U CString�ɓ����
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_HDOP, _T("----"));
	else pDlg->SetDlgItemText(IDC_EDIT_HDOP, sTemp);

	// ���x�t�B�[���h
	sToken = SplitString(NULL, sSeparator);
	sTemp = sToken;		// Windows CE Wide-Char�Ή��̂��߁A��U CString�ɓ����
	if(sToken == NULL) pDlg->SetDlgItemText(IDC_EDIT_ALTITUDE, _T("----"));
	else pDlg->SetDlgItemText(IDC_EDIT_ALTITUDE, sTemp);


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

	sTemp.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMinutes, nSecond);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_ZDA, sTemp);

	PcTimeDisp(nHour, nMinutes, nSecond, pParam);

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

void CGpsTimeSetCEDlg::TimeSeparate(char * sTimeStr, int *nHour, int *nMinutes, int *nSecond)
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

char * CGpsTimeSetCEDlg::SplitString(char *sBuf, char *sSeparator)
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
PcTimeDisp(int nHour, int nMinutes, int nSecond, LPVOID pParam)
PC�̃V�X�e�������̕\���ƁA�V�X�e�������̕ύX

���́j
  GPS�œ��������b�Fint nHour, int nMinutes, int nSecond
  ���C���_�C�A���O�N���X�ւ̃|�C���^�FpParam

�����g�p���Ă���N���X�ϐ��j
  �X���b�h�Ăяo��������A�����ݒ薽�߂��󂯂�t���O�Fint m_ThreadSetTime

�ˑ��j
�@Win32SDK SetLocalTime �� �V�X�e�������ύX�����������ꍇ�A�����������Ȃ�

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

	::GetLocalTime(&tmSystem);		// �V�X�e���̃��[�J���^�C���𓾂�
	nTzResult = ::GetTimeZoneInformation(&tzInfo);		// �V�X�e���̃^�C���]�[���ݒ�𓾂�

	// �V�X�e���̃��[�J���^�C�����_�C�A���O�ɕ\������
	sTemp.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), tmSystem.wYear, tmSystem.wMonth, tmSystem.wDay, tmSystem.wHour, tmSystem.wMinute, tmSystem.wSecond);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_PC, sTemp);

	// �V�X�e���̃^�C���]�[�����_�C�A���O�ɕ\������
	nTz = (double)tzInfo.Bias / 60.0;
	sTemp.Format(_T("%+02.1f"), nTz);
	pDlg->SetDlgItemText(IDC_EDIT_TIME_TZ, sTemp);

	// �����ݒ�
	if(pDlg->m_ThreadSetTime)
	{
		// GPS�����Ƀ^�C���]�[�������Z���āA���[�J���^�C���ɂ���
		nTimeNow = nHour*60*60 + nMinutes*60 + nSecond - tzInfo.Bias*60;

		// 0 �` 24 ���ԓ��Ɏ��߂鏈���i���`���N�`���K���j
		if(nTimeNow < 0) nTimeNow += 60*60*12;		// �}�C�i�X�ɂȂ�����A12���Ԑi�߂�
		else if(nTimeNow > 24*60*60) nTimeNow -= 60*60*12;		// 24���Ԉȏ�́A12���Ԗ߂�

		// GPS�������瓾�����[�J���^�C�����A�N�����E�����b�ϐ��ɑ��
		tmSystem.wHour = (WORD)(nTimeNow / 60 / 60);
		tmSystem.wMinute = (WORD)((nTimeNow - tmSystem.wHour*60*60) / 60);
		tmSystem.wSecond = (WORD)(nTimeNow - tmSystem.wHour*60*60 - tmSystem.wMinute*60);

		// �V�X�e���̃��[�J���^�C����ύX����i�v�F�����ύX�����j
		::SetLocalTime(&tmSystem);

		pDlg->m_ThreadSetTime = 0;
	}

}

//EOF
