// DlgAbout.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "infobar00.h"
#include "DlgAbout.h"
#include "InstFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CDlgAbout, CPropertyPage)

CDlgAbout::CDlgAbout() : CPropertyPage(CDlgAbout::IDD)
{
	//{{AFX_DATA_INIT(CDlgAbout)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�̏�����������ǉ����܂��B
	//}}AFX_DATA_INIT
}

CDlgAbout::~CDlgAbout()
{
}

void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAbout)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAbout, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgAbout)
	ON_BN_CLICKED(IDC_BTN_MAKEUNIN, OnBtnMakeunin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout ���b�Z�[�W �n���h��

// **********************************
//  �A���C���X�g�[���E�A�C�R���̍쐬
//
// **********************************
void CDlgAbout::OnBtnMakeunin() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	::MkUninstMnu();

	MessageBox("Uninstall shortcut is made on start menu .", "InfoBar", MB_OK|MB_ICONINFORMATION);
	
}