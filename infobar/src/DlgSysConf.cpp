// DlgSysConf.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "infobar00.h"
#include "DlgSysConf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysConf プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgSysConf, CPropertyPage)

CDlgSysConf::CDlgSysConf() : CPropertyPage(CDlgSysConf::IDD)
{
	//{{AFX_DATA_INIT(CDlgSysConf)
	m_nSpeed = 0;
	m_nTimer = 0;
	m_tInterval = 0;
	m_bTopmost = FALSE;
	//}}AFX_DATA_INIT
}

CDlgSysConf::~CDlgSysConf()
{
}

void CDlgSysConf::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysConf)
	DDX_Control(pDX, IDC_AREA_FORE_COLOR, m_ctrl_area_fore_color);
	DDX_Control(pDX, IDC_AREA_BACK_COLOR, m_ctrl_area_back_color);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_nSpeed);
	DDV_MinMaxUInt(pDX, m_nSpeed, 1, 20);
	DDX_Text(pDX, IDC_EDIT_TIMER, m_nTimer);
	DDV_MinMaxUInt(pDX, m_nTimer, 10, 1000);
	DDX_Text(pDX, IDC_EDIT_REFRESH, m_tInterval);
	DDV_MinMaxUInt(pDX, m_tInterval, 0, 65536);
	DDX_Check(pDX, IDC_CHK_TOPMOST, m_bTopmost);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysConf, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgSysConf)
	ON_BN_CLICKED(IDC_BTN_BACK_COLOR, OnBtnBackColor)
	ON_BN_CLICKED(IDC_BTN_FORE_COLOR, OnBtnForeColor)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysConf メッセージ ハンドラ

void CDlgSysConf::OnBtnBackColor() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CColorDialog dlg;
	CDC *pDC;
	CRect rect;

	dlg.m_cc.Flags |= CC_RGBINIT;
	dlg.m_cc.rgbResult = m_cBackColor;

	if(dlg.DoModal() == IDOK)
	{
		pDC = m_ctrl_area_back_color.GetDC();
		m_ctrl_area_back_color.GetWindowRect(&rect);
		pDC->FillSolidRect(1,1,rect.right-rect.left-2, rect.bottom-rect.top-2,dlg.GetColor());

		m_cBackColor = dlg.GetColor();

		ReleaseDC(pDC);
	}
}

void CDlgSysConf::OnBtnForeColor() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CColorDialog dlg;
	CDC *pDC;
	CRect rect;

	dlg.m_cc.Flags |= CC_RGBINIT;
	dlg.m_cc.rgbResult = m_cForeColor;

	if(dlg.DoModal() == IDOK)
	{
		pDC = m_ctrl_area_fore_color.GetDC();
		m_ctrl_area_fore_color.GetWindowRect(&rect);
		pDC->FillSolidRect(1,1,rect.right-rect.left-2, rect.bottom-rect.top-2,dlg.GetColor());

		m_cForeColor = dlg.GetColor();

		ReleaseDC(pDC);
	}
	
}


void CDlgSysConf::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	CDC *pDC;
	CRect rect;

	pDC = m_ctrl_area_back_color.GetDC();
	m_ctrl_area_back_color.GetWindowRect(&rect);
	pDC->FillSolidRect(1,1,rect.right-rect.left-2, rect.bottom-rect.top-2,m_cBackColor);
	ReleaseDC(pDC);

	pDC = m_ctrl_area_fore_color.GetDC();
	m_ctrl_area_fore_color.GetWindowRect(&rect);
	pDC->FillSolidRect(1,1,rect.right-rect.left-2, rect.bottom-rect.top-2,m_cForeColor);
	ReleaseDC(pDC);
	
	// 描画用メッセージとして CPropertyPage::OnPaint() を呼び出してはいけません
}
