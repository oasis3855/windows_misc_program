// DlgInst.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "cursor32.h"
#include "DlgInst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInst �_�C�A���O


CDlgInst::CDlgInst(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInst::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInst)
	m_logon = FALSE;
	m_manual = FALSE;
	m_uninst = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgInst::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInst)
	DDX_Check(pDX, IDC_LOGON, m_logon);
	DDX_Check(pDX, IDC_MANUAL, m_manual);
	DDX_Check(pDX, IDC_UNINST, m_uninst);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInst, CDialog)
	//{{AFX_MSG_MAP(CDlgInst)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInst ���b�Z�[�W �n���h��
