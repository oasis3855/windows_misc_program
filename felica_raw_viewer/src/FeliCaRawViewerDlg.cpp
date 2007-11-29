/***************************************************

FeliCaRawViewerDlg.cpp

FeliCa Raw Viewer �\�t�g�E�G�A
  Copyright (C) 2007, INOUE. Hirokazu
  All rights reserved.

���̃\�t�g�E�G�A�ł́Afelicalib ���C�u�����𗘗p���Ă��܂�
  Copyright (C) 2007, Takuya Murakami
  All rights reserved.

The BSD License (http://opensource.org/licenses/bsd-license.php)
---------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer. 
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution. 
* Neither the name of the this project nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

�ȉ��̓��{���͎Q�l�̂��߂ɓY�t�������̂ł��B�p��ł������@���I�ɗL���ł���
���Ƃɒ��ӂ��Ă��������B

BSD���C�Z���X���� �i���{���j
    (http://sourceforge.jp/projects/opensource/wiki/licenses%2Fnew_BSD_license)
-----------------------------
�\�[�X�R�[�h�`�����o�C�i���`�����A�ύX���邩���Ȃ������킸�A�ȉ��̏�����
�������ꍇ�Ɍ���A�ĔЕz����юg�p��������܂��B

* �\�[�X�R�[�h���ĔЕz����ꍇ�A��L�̒��쌠�\���A�{�����ꗗ�A����щ��L
  �Ɛӏ������܂߂邱�ƁB 
* �o�C�i���`���ōĔЕz����ꍇ�A�Еz���ɕt���̃h�L�������g���̎����ɁA��L��
  ���쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB 
* ���ʂɂ����ʂ̋��Ȃ��ɁA�{�\�t�g�E�F�A����h���������i�̐�`�܂��͔̔�
  ���i�ɁA���̃v���W�F�N�g�̖��O�܂��̓R���g���r���[�^�[�̖��O���g�p���Ă�
  �Ȃ�Ȃ��B

�{�\�t�g�E�F�A�́A���쌠�҂���уR���g���r���[�^�[�ɂ���āu����̂܂܁v��
����Ă���A�����َ����킸�A���ƓI�Ȏg�p�\���A����ѓ���̖ړI�ɑ΂���
�K�����Ɋւ���Öق̕ۏ؂��܂߁A�܂�����Ɍ��肳��Ȃ��A�����Ȃ�ۏ؂�
����܂���B���쌠�҂��R���g���r���[�^�[���A���R�̂�������킸�A���Q������
������������킸�A���ӔC�̍������_��ł��邩���i�ӔC�ł��邩�i�ߎ�
���̑��́j�s�@�s�ׂł��邩���킸�A���ɂ��̂悤�ȑ��Q����������\����
�m�炳��Ă����Ƃ��Ă��A�{�\�t�g�E�F�A�̎g�p�ɂ���Ĕ��������i��֕i�܂���
��p�T�[�r�X�̒��B�A�g�p�̑r���A�f�[�^�̑r���A���v�̑r���A�Ɩ��̒��f���܂߁A
�܂�����Ɍ��肳��Ȃ��j���ڑ��Q�A�Ԑڑ��Q�A�����I�ȑ��Q�A���ʑ��Q�A�����I
���Q�A�܂��͌��ʑ��Q�ɂ��āA��ؐӔC�𕉂�Ȃ����̂Ƃ��܂��B


***************************************************/

#include "stdafx.h"
#include "FeliCaRawViewer.h"
#include "FeliCaRawViewerDlg.h"

#include <HtmlHelp.h>

#include "_FelicaLib\felicalib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeliCaRawViewerDlg �_�C�A���O

CFeliCaRawViewerDlg::CFeliCaRawViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFeliCaRawViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeliCaRawViewerDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFeliCaRawViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeliCaRawViewerDlg)
	DDX_Control(pDX, IDC_EDIT_DATA, m_editData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeliCaRawViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CFeliCaRawViewerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_READ, OnBtnRead)
	ON_BN_CLICKED(IDC_BTN_COPY, OnBtnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeliCaRawViewerDlg ���b�Z�[�W �n���h��

BOOL CFeliCaRawViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			pSysMenu->AppendMenu(MF_STRING, IDM_HELP, "�w���v�̕\�� ...");
		}
	}

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B

	CheckDlgButton(IDC_CHECK_PROTECT, TRUE);
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

void CFeliCaRawViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == IDM_HELP)
	{
		SendMessage(WM_HELP);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CFeliCaRawViewerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CFeliCaRawViewerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/**************
FeliCa �f�[�^�ǂݍ��݃{�^�����������Ƃ��̏���
***************/

void CFeliCaRawViewerDlg::OnBtnRead() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	pasori *p;
	felica *f, *f2;
	char sTemp[128];
	CString sDumpMsg;
    int i, j, k;

	sDumpMsg = "";		// �_���v ���b�Z�[�W�i������j�̏�����

	SetDlgItemText(IDC_EDIT_IDM, "");
	SetDlgItemText(IDC_EDIT_PMM, "");
	SetDlgItemText(IDC_EDIT_DATA, "");

	if(!GetFelicaDllPath())
	{
		SetDlgItemText(IDC_EDIT_DATA, "felica.dll��������܂���");
		return;
	}

	p = pasori_open(NULL);

	if(!p)
	{
		SetDlgItemText(IDC_EDIT_DATA, "�J�[�h���[�_�ɐڑ��ł��܂���");
		return;
	}

	pasori_init(p);

	f = felica_polling(p, POLLING_ANY, 0, 0);
	if(!f)
	{
		SetDlgItemText(IDC_EDIT_DATA, "�|�[�����O���s");
		pasori_close(p);
		return;
	}

	// IDm �� PMm ���_�C�A���O�{�b�N�X�ɕ\������
	HexDumpToStr(f->IDm, sTemp, 8);
	SetDlgItemText(IDC_EDIT_IDM, sTemp);

	HexDumpToStr(f->PMm, sTemp, 8);
	SetDlgItemText(IDC_EDIT_PMM, sTemp);

	felica_free(f);

	// �ȉ��A�_���v �f�[�^�p�Ƀe�L�X�g�isDumpMsg�j���쐬����

	f = felica_enum_systemcode(p);
	if(!f)
	{
		SetDlgItemText(IDC_EDIT_DATA, "�V�X�e���R�[�h�ǂݍ��ݎ��s");
		pasori_close(p);
		return;
	}

	HexDumpToStr(f->IDm, sTemp, 8);
	sDumpMsg += "IDm : ";
	sDumpMsg += sTemp;
	sDumpMsg += "\r\n";

	HexDumpToStr(f->PMm, sTemp, 8);
	sDumpMsg += "PMm : ";
	sDumpMsg += sTemp;
	sDumpMsg += "\r\n\r\n";

	sprintf(sTemp, "* Number of service = %d\r\n\r\n", f->num_system_code);
	sDumpMsg += sTemp;

	for (i = 0; i < f->num_system_code; i++)		// �V�X�e���̐��������[�v����
	{
		sprintf(sTemp, "# System code: %04X\r\n", N2HS(f->system_code[i]));
		sDumpMsg += sTemp;

		f2 = felica_enum_service(p, N2HS(f->system_code[i]));
		if (!f2)
		{
			sDumpMsg += " Enum service failed.\r\n";
		}
		else
		{

			sprintf(sTemp, " # Number of area = %d\r\n", f2->num_area_code);
			sDumpMsg += sTemp;

			for (j = 0; j < f2->num_area_code; j++)		// �G���A�̐��������[�v����
			{
				sprintf(sTemp, "  # Area: %04X - %04X\r\n", f2->area_code[j], f2->end_service_code[j]);
				sDumpMsg += sTemp;
			}	    

			sprintf(sTemp, " # Number of service code = %d\r\n", f2->num_service_code);
			sDumpMsg += sTemp;

			for (j = 0; j < f2->num_service_code; j++)	// �T�[�r�X�̐��������[�v����
			{
				uint16 service = f2->service_code[j];
				strcpy(sTemp, printserviceinfo(service));
				sDumpMsg += "  ";
				sDumpMsg += sTemp;

				if((service & 0x1) || !IsDlgButtonChecked(IDC_CHECK_PROTECT))
				{
					for (k = 0; k < 255; k++)
					{
						uint8 data[16];

						if (felica_read_without_encryption02(f2, service, 0, (uint8)k, data))
						{
							sprintf(sTemp, "   %04X:%04X data is not available\r\n", service, k);
							sDumpMsg += sTemp;
							break;
						}

						sprintf(sTemp, "   %04X:%04X ", service, k);
						sDumpMsg += sTemp;

						HexDumpToStr(data, sTemp, 16);
						sDumpMsg += sTemp;
						sDumpMsg += "\r\n";
					}
				}
			}
			sDumpMsg += "\r\n---------\r\n";

			felica_free(f2);
		}

	}

	felica_free(f);
	pasori_close(p);

	// �_���v �f�[�^���_�C�A���O�{�b�N�X�ɕ\������
	SetDlgItemText(IDC_EDIT_DATA, sDumpMsg);

	
}

/**************
�N���b�v�{�[�h�փR�s�[�{�^�����������Ƃ��̏���
***************/

void CFeliCaRawViewerDlg::OnBtnCopy() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_editData.SetSel(0, -1, TRUE);
	m_editData.Copy();
}

/**************
// �w���v�\���֐����I�[�o�[���C�h���āAHTML�w���v��\������悤�ɂ���
***************/

void CFeliCaRawViewerDlg::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
//	CDialog::WinHelp(dwData, nCmd);

	// HELP_CONTEXT �ȊO�͉������Ȃ�
	if(nCmd != HELP_CONTEXT) return;

	// HTML�w���v��hWnd�n���h�� �i���s����NULL�j
	HWND hWnd_Help;
	// �w���v�t�@�C���ւ̐�΃p�X����邽�߂́A�p�X����p�ꎞ������
	char sChmPath[MAX_PATH];
	char *sDotPtr;

	CString sTmp, sAfxMsg;

	// �A�v���P�[�V�������g�̃p�X���擾���A�g���q�� chm �ɏ���������
	// (HtmlHelp�֐��̓J�����g�t�H���_�̃w���v�t�@�C�����擾���悤�Ƃ��邽��)
	if(!::GetModuleFileName(NULL, sChmPath, MAX_PATH)) return;

	// �Ō��'.'�������������A����ȍ~���폜���� '.chm' �𑫂�
	sDotPtr = strrchr(sChmPath, '.');
	if(sDotPtr == NULL)
		return;					// '.' ���������݂��Ȃ����Ƃ͂��肦�Ȃ�
	strcpy(sDotPtr, ".chm");	// �w���v�t�@�C���̃t���p�X������

	// �w���v�̕\��
	hWnd_Help = ::HtmlHelp(this->m_hWnd, sChmPath, HH_HELP_CONTEXT, LOWORD(dwData));

	if(hWnd_Help == NULL)
	{	// �w���v�t�@�C���̋N���Ɏ��s�����ꍇ
		MessageBox("�w���v�t�@�C����\���ł��܂���", "FeliCaRawViewer", MB_ICONWARNING);
	}

}
