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
	ON_BN_CLICKED(IDC_BTN_FONT, OnBtnFont)
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


void CDlgSysConf::OnBtnFont() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CDC *pDC;
	CFont fnt;		// 暫定フォント作成用
	CFont *pfnt;	// DCに割り当てられている現在のフォントを得るときに使う
	LOGFONT lf, lf2;
	CString sTmp;

	if(nFontPoint <= 0)
	{	// フォント名がまだ指定されていないとき
		//*******************
		// とりあえず適当なフォントを現在のデバイスコンテキストの割り付け
		// そのとき返ってくる、現在のフォント情報を得る
		//*******************
		pDC = this->GetDC();				// デバイスコンテキストを得る
		this->GetFont()->GetLogFont(&lf);	// このダイアログの現在のフォント
		fnt.CreateFontIndirect(&lf);		// 暫定フォント作成
		pfnt = pDC->SelectObject(&fnt);		// 暫定フォントの割付と、現在フォントの取得
		pfnt->GetLogFont(&lf2);				// 現在フォントを LOGFONT 構造体へ
											// １回の使用で、pfnt は破壊される

		pDC->SelectStockObject(DEVICE_DEFAULT_FONT);	// システムフォントを割り付け
		fnt.DeleteObject();					// 暫定フォントの解放
		ReleaseDC(pDC);						// DCの解放
	}
	else
	{
		fnt.CreatePointFont(nFontPoint, sFontName);
		fnt.GetLogFont(&lf2);
		fnt.DeleteObject();
	}

	
	CFontDialog dlg(&lf2);

//	dlg.m_cf.iPointSize = 120;		// 12pt
	dlg.m_cf.rgbColors  = m_cForeColor;


	if(dlg.DoModal() == IDOK)
	{
		nFontPoint = dlg.m_cf.iPointSize;
		sFontName = dlg.m_lf.lfFaceName;

	sTmp.Format("%s %.1f point", sFontName, nFontPoint/10.0);

		SetDlgItemText(IDC_TXT_FONT,sTmp);
	}
}

BOOL CDlgSysConf::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CString sTmp;

	sTmp.Format("%s %.1f point", sFontName, nFontPoint/10.0);

	SetDlgItemText(IDC_TXT_FONT,sTmp);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
