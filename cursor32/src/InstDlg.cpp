// InstDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "cursor32.h"
#include "InstDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstDlg �_�C�A���O


CInstDlg::CInstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInstDlg)
	m_r_autoexec = -1;
	m_chk_uninst_start = FALSE;
	m_chk_uninst_cpl = FALSE;
	//}}AFX_DATA_INIT
}


void CInstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInstDlg)
	DDX_Radio(pDX, IDC_R_AUTOEXEC, m_r_autoexec);
	DDX_Check(pDX, IDC_CHECK_UNINST_START, m_chk_uninst_start);
	DDX_Check(pDX, IDC_CHECK_UNINST_CPL, m_chk_uninst_cpl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInstDlg, CDialog)
	//{{AFX_MSG_MAP(CInstDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstDlg ���b�Z�[�W �n���h��
