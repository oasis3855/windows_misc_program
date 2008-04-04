// BacklightControlDlg.cpp : �_�C�A���O �N���X �����t�@�C��
//

// ***********************
// BacklightControl for CE  (Windows CE �̃o�b�N���C�g����j
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
#include "BacklightControl.h"
#include "BacklightControlDlg.h"

#include <pm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBacklightControlDlg dialog

CBacklightControlDlg::CBacklightControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBacklightControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBacklightControlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBacklightControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBacklightControlDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBacklightControlDlg, CDialog)
	//{{AFX_MSG_MAP(CBacklightControlDlg)
	ON_BN_CLICKED(IDC_BUTTON_D4, OnButtonD4)
	ON_BN_CLICKED(IDC_BUTTON_D0, OnButtonD0)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, OnButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBacklightControlDlg message handlers

BOOL CBacklightControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here

	// ��ԕ\��
	OnButtonGet();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// ***********************
// �o�b�N���C�g OFF
// ***********************
void CBacklightControlDlg::OnButtonD4() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF

	// ��ԕ\��
	OnButtonGet();
	
}

// ***********************
// �o�b�N���C�g ON
// ***********************
void CBacklightControlDlg::OnButtonD0() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF (��UOFF�ɂ��Ȃ��Ɣ������Ȃ��ꍇ����)
	::Sleep(200);
	::SetDevicePower(_T("BKL1:"), POWER_NAME, D0);	// ON
	
	// ��ԕ\��
	OnButtonGet();

}

// ***********************
// �o�b�N���C�g����������i�p���[�}�l�W�����g�ɏ]���j
// ***********************
void CBacklightControlDlg::OnButtonAuto() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF (��UOFF�ɂ��Ȃ��Ɣ������Ȃ��ꍇ����)
	::Sleep(200);
	::SetDevicePower(_T("BKL1:"), POWER_NAME, PwrDeviceUnspecified);	// ��������
	
	// ��ԕ\��
	OnButtonGet();

}

// ***********************
// �e�L�X�g�{�b�N�X�Ɍ��݂̏�Ԃ�\��
// ***********************
void CBacklightControlDlg::OnButtonGet() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CEDEVICE_POWER_STATE devState;

	if(::GetDevicePower(_T("BKL1:"), POWER_NAME, &devState) != ERROR_SUCCESS)
	{
		SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Read Error"));
	}

	if(devState == PwrDeviceUnspecified) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Unspecified"));
	if(devState == D0) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Full ON (D0)"));
	if(devState == D1) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Low ON (D1)"));
	if(devState == D2) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Standby (D2)"));
	if(devState == D3) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Sleep (D3)"));
	if(devState == D4) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("OFF (D4)"));
	if(devState == PwrDeviceMaximum) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Max"));
	
}
