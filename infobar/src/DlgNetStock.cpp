// DlgNetStock.cpp : インプリメンテーション ファイル
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
// CDlgNetStock ダイアログ


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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNetStock メッセージ ハンドラ

void CDlgNetStock::OnSelchangeCmbPreset() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int _nSel = m_cmb_preset_ctrl.GetCurSel();
	CString _sTmp;

	switch(_nSel)
	{
	case 0:	// Yahoo Japan Finance 外国為替
		SetDlgItemText(IDC_EDIT_ADDR, "quote.yahoo.co.jp/q?d=t&s=%s=X");
		SetDlgItemText(IDC_EDIT_PH, "</b> (---:");
		SetDlgItemText(IDC_EDIT_PHB, "取引値");
		SetDlgItemText(IDC_EDIT_PHC, "<b>");
		GetDlgItemText(IDC_EDIT_ITEMS, _sTmp);
		if(_sTmp == "")
		{
			SetDlgItemText(IDC_EDIT_ITEMS,"USDJPY\r\nJPYUSD\r\nEURJPY\r\nJPYEUR\r\nUSDEUR\r\nEURUSD\r\nGBPJPY\r\nJPYGBP\r\nHKDJPY\r\nJPYHKD");
		}
		break;
	case 1:	// Yahoo Japan Finance 株式
		SetDlgItemText(IDC_EDIT_ADDR, "quote.yahoo.co.jp/q?s=%s");
		SetDlgItemText(IDC_EDIT_PH, "<td nowrap align=left><small>");
		SetDlgItemText(IDC_EDIT_PHB, "<td nowrap><b>");
		SetDlgItemText(IDC_EDIT_PHC, "<td nowrap>");
		GetDlgItemText(IDC_EDIT_ITEMS, _sTmp);
		if(_sTmp == "")
		{
			SetDlgItemText(IDC_EDIT_ITEMS,"998407.O\r\n998405.T\r\n23337.Q\r\n8318.T\r\n8604.T\r\n5401.T\r\n7203.T\r\n6501.T\r\n6758.T\r\n9020.T\r\n8233.T\r\n1812.T\r\n9432.T");
		}
		break;
	case 2:	// Yahoo USA Finance 外国為替
		SetDlgItemText(IDC_EDIT_ADDR, "finance.yahoo.com/q?s=%s=X");
		SetDlgItemText(IDC_EDIT_PH, "<font face=arial size=-1><a href=\"/q?s=");
		SetDlgItemText(IDC_EDIT_PHB, "<font face=arial size=-1><b>");
		SetDlgItemText(IDC_EDIT_PHC, "<td nowrap>");
		GetDlgItemText(IDC_EDIT_ITEMS, _sTmp);
		if(_sTmp == "")
		{
			SetDlgItemText(IDC_EDIT_ITEMS,"USDJPY\r\nJPYUSD\r\nEURJPY\r\nJPYEUR\r\nUSDEUR\r\nEURUSD\r\nGBPJPY\r\nJPYGBP\r\nHKDJPY\r\nJPYHKD");
		}
		break;
	case 3:	// Yahoo USA Finance 株式
		SetDlgItemText(IDC_EDIT_ADDR, "finance.yahoo.com/q?d=t&s=%s");
		SetDlgItemText(IDC_EDIT_PH, "<td colspan=7><font face=arial size=-1><b>");
		SetDlgItemText(IDC_EDIT_PHB, "<b>");
		SetDlgItemText(IDC_EDIT_PHC, "Change<br>");
		GetDlgItemText(IDC_EDIT_ITEMS, _sTmp);
		if(_sTmp == "")
		{
			SetDlgItemText(IDC_EDIT_ITEMS,"^DJI\r\n^IXIC\r\nGE\r\nBA\r\nC\r\nPFE\r\nEMC\r\nAOL\r\nF\r\nT\r\nTXN\r\nXOM\r\nMCD\r\nWMT\r\nMSFT");
		}
		break;
	default :
		SetDlgItemText(IDC_EDIT_ADDR, "");
		SetDlgItemText(IDC_EDIT_PH, "");
		SetDlgItemText(IDC_EDIT_PHB, "");
		SetDlgItemText(IDC_EDIT_PHC, "");
		break;
	}

}
