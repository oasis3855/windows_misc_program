// DlgNetStock.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "infobar00.h"
#include "DlgNetStock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNetStock �_�C�A���O


CDlgNetStock::CDlgNetStock(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNetStock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNetStock)
	m_sURL = _T("");
	m_sItems = _T("");
	m_sPhHeader = _T("");
	m_sPhHeaderB = _T("");
	m_sPhHeaderC = _T("");
	//}}AFX_DATA_INIT
}


void CDlgNetStock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNetStock)
	DDX_Control(pDX, IDC_CMB_PRESET, m_cmb_preset_ctrl);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_sURL);
	DDV_MaxChars(pDX, m_sURL, 1024);
	DDX_Text(pDX, IDC_EDIT_ITEMS, m_sItems);
	DDV_MaxChars(pDX, m_sItems, 2048);
	DDX_Text(pDX, IDC_EDIT_PH, m_sPhHeader);
	DDV_MaxChars(pDX, m_sPhHeader, 1024);
	DDX_Text(pDX, IDC_EDIT_PHB, m_sPhHeaderB);
	DDV_MaxChars(pDX, m_sPhHeaderB, 1024);
	DDX_Text(pDX, IDC_EDIT_PHC, m_sPhHeaderC);
	DDV_MaxChars(pDX, m_sPhHeaderC, 1024);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNetStock, CDialog)
	//{{AFX_MSG_MAP(CDlgNetStock)
	ON_CBN_SELCHANGE(IDC_CMB_PRESET, OnSelchangeCmbPreset)
	ON_BN_CLICKED(IDC_BTN_SMPL, OnBtnSmpl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNetStock ���b�Z�[�W �n���h��

void CDlgNetStock::OnSelchangeCmbPreset() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int _nSel = m_cmb_preset_ctrl.GetCurSel();
	CString _sTmp;

	switch(_nSel)
	{
	case 0:	// Yahoo Japan Finance �O���ב�
		SetDlgItemText(IDC_EDIT_ADDR, "quote.yahoo.co.jp/q?d=t&s=%s=X");
		SetDlgItemText(IDC_EDIT_PH, "</b> (---:");
		SetDlgItemText(IDC_EDIT_PHB, "����l");
		SetDlgItemText(IDC_EDIT_PHC, "<b>");
		GetDlgItemText(IDC_EDIT_ITEMS, _sTmp);
		SetDlgItemText(IDC_EDIT_ITEMS,"USDJPY\r\nJPYUSD\r\nEURJPY\r\nJPYEUR\r\nUSDEUR\r\nEURUSD\r\nGBPJPY\r\nJPYGBP\r\nHKDJPY\r\nJPYHKD");
		break;
	case 1:	// Yahoo Japan Finance ����
		SetDlgItemText(IDC_EDIT_ADDR, "quote.yahoo.co.jp/q?s=%s");
		SetDlgItemText(IDC_EDIT_PH, "<td nowrap align=left><small>");
		SetDlgItemText(IDC_EDIT_PHB, "<td nowrap><b>");
		SetDlgItemText(IDC_EDIT_PHC, "<td nowrap>");
		GetDlgItemText(IDC_EDIT_ITEMS, _sTmp);
		// 998407.O ���o���ρA998405.T TPOIX�A8306.T �O�H�����A8604.T �쑺�A7203.T �g���^�A7201.T ���Y�A7751.T �L���m���A6758.T �\�j�[�A6752.T �����A6501.T �����A4502.T ���c��i�A4901.T �x�m�ʐ^�A6753.T �V���[�v�A8183.T �Z�u���C���u���A8264.T �C�g�[���[�J���A9501.T ���d�A9432.T NTT�A9437.T DoCoMo�A9020.T JR��
		SetDlgItemText(IDC_EDIT_ITEMS,"998407.O\r\n998405.T\r\n23337.Q\r\n8306.T\r\n8604.T\r\n7203.T\r\n7201.T\r\n7751.T\r\n6758.T\r\n6752.T\r\n6501.T\r\n4502.T\r\n4901.T\r\n6753.T\r\n8183.T\r\n8264.T\r\n9501.T\r\n9432.T\r\n9437.T\r\n9020.T");
		break;
	case 2:	// Yahoo USA Finance �O���ב�
		SetDlgItemText(IDC_EDIT_ADDR, "finance.yahoo.com/q?s=%s=X");
		SetDlgItemText(IDC_EDIT_PH, "<font face=arial size=-1><a href=\"/q?s=");
		SetDlgItemText(IDC_EDIT_PHB, "<font face=arial size=-1><b>");
		SetDlgItemText(IDC_EDIT_PHC, "<td nowrap>");
		GetDlgItemText(IDC_EDIT_ITEMS, _sTmp);
		SetDlgItemText(IDC_EDIT_ITEMS,"USDJPY\r\nJPYUSD\r\nEURJPY\r\nJPYEUR\r\nUSDEUR\r\nEURUSD\r\nGBPJPY\r\nJPYGBP\r\nHKDJPY\r\nJPYHKD");
		break;
	case 3:	// Yahoo USA Finance ����
		SetDlgItemText(IDC_EDIT_ADDR, "finance.yahoo.com/q?d=t&s=%s");
		SetDlgItemText(IDC_EDIT_PH, "<td colspan=7><font face=arial size=-1><b>");
		SetDlgItemText(IDC_EDIT_PHB, "<b>");
		SetDlgItemText(IDC_EDIT_PHC, "Change<br>");
		GetDlgItemText(IDC_EDIT_ITEMS, _sTmp);
		SetDlgItemText(IDC_EDIT_ITEMS,"^DJI\r\n^IXIC\r\nGE\r\nBA\r\nC\r\nPFE\r\nEMC\r\nAOL\r\nF\r\nT\r\nTXN\r\nXOM\r\nMCD\r\nWMT\r\nMSFT");
		break;
	default :
		SetDlgItemText(IDC_EDIT_ADDR, "");
		SetDlgItemText(IDC_EDIT_PH, "");
		SetDlgItemText(IDC_EDIT_PHB, "");
		SetDlgItemText(IDC_EDIT_PHC, "");
		break;
	}

}

void CDlgNetStock::OnBtnSmpl() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetDlgItemText(IDC_EDIT_ITEMS,"");

}
