// DlgNetConf.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "infobar00.h"
#include "DlgNetConf.h"
#include "DlgNetStock.h"

#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNetConf プロパティ ページ

IMPLEMENT_DYNCREATE(CDlgNetConf, CPropertyPage)

CDlgNetConf::CDlgNetConf() : CPropertyPage(CDlgNetConf::IDD)
{
	//{{AFX_DATA_INIT(CDlgNetConf)
	m_sURL = _T("");
	m_nPort = 0;
	m_sPhHeader = _T("");
	m_nPhSkip = 0;
	m_nPhGetcount = 0;
	m_sProxy = _T("");
	m_sTitle = _T("");
	//}}AFX_DATA_INIT
}

CDlgNetConf::~CDlgNetConf()
{
}

void CDlgNetConf::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNetConf)
	DDX_Control(pDX, IDC_EDIT_ADDR, m_sURL_ctrl);
	DDX_Control(pDX, IDC_BTN_DETAIL, m_btn_detail_ctrl);
	DDX_Control(pDX, IDC_EDIT_HEADER, m_sPhHeader_ctrl);
	DDX_Control(pDX, IDC_EDIT_GETNUM, m_nPhGetcount_ctrl);
	DDX_Control(pDX, IDC_EDIT_SKIP, m_nPhSkip_ctrl);
	DDX_Control(pDX, IDC_COMBO_QLIST, m_ctrl_cmb_qlist);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_sURL);
	DDV_MaxChars(pDX, m_sURL, 1024);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDV_MinMaxUInt(pDX, m_nPort, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_HEADER, m_sPhHeader);
	DDV_MaxChars(pDX, m_sPhHeader, 1024);
	DDX_Text(pDX, IDC_EDIT_SKIP, m_nPhSkip);
	DDV_MinMaxUInt(pDX, m_nPhSkip, 0, 100);
	DDX_Text(pDX, IDC_EDIT_GETNUM, m_nPhGetcount);
	DDV_MinMaxUInt(pDX, m_nPhGetcount, 0, 100);
	DDX_Text(pDX, IDC_EDIT_PROXY, m_sProxy);
	DDV_MaxChars(pDX, m_sProxy, 1024);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_sTitle);
	DDV_MaxChars(pDX, m_sTitle, 1024);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNetConf, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgNetConf)
	ON_CBN_SELCHANGE(IDC_COMBO_QLIST, OnSelchangeComboQlist)
	ON_BN_CLICKED(IDC_BTN_DETAIL, OnBtnDetail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNetConf メッセージ ハンドラ

void CDlgNetConf::OnSelchangeComboQlist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int _nSel = m_ctrl_cmb_qlist.GetCurSel();
	CString sTmpStr;
	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	m_nMode = 0;		// モード切り替え

	switch(_nSel)
	{
	case 0:	// Yahoo 日本 ニュース 政治
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=pol&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 1:	// Yahoo 日本 ニュース 社会
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=soci&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 2:	// Yahoo 日本 ニュース 海外
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=int&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 3:	// Yahoo 日本 ニュース 経済総合
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=bus_all&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 4:	// Yahoo 日本 ニュース 産業
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=ind&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 5:	// Yahoo 日本 ニュース コンピュータ
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=sci&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 6:	// Yahoo USA top story
		SetDlgItemText(IDC_EDIT_ADDR, "news.yahoo.com/news?tmpl=index2&cid=716");
		SetDlgItemText(IDC_EDIT_HEADER, "class=topstory");
		break;
	case 7:	// Yahoo USA world selection
		SetDlgItemText(IDC_EDIT_ADDR, "news.yahoo.com/news?tmpl=index2&cid=721");
		SetDlgItemText(IDC_EDIT_HEADER, "class=topstory");
		break;
	case 8:	// Yahoo USA business selection
		SetDlgItemText(IDC_EDIT_ADDR, "news.yahoo.com/news?tmpl=index2&cid=749");
		SetDlgItemText(IDC_EDIT_HEADER, "class=topstory");
		break;
	case 9:	// Yahoo USA technology selection
		SetDlgItemText(IDC_EDIT_ADDR, "news.yahoo.com/news?tmpl=index2&cid=738");
		SetDlgItemText(IDC_EDIT_HEADER, "class=topstory");
		break;
	case 10:	// ■証券モード■ Yahoo FINANE 証券市場・為替市場
		SetDlgItemText(IDC_EDIT_ADDR, "quote.yahoo.co.jp/q?s=%s");
		m_nMode = 1;		// モード切り替え
		break;
	case 11:	// ２ちゃんねる ニュース速報＋
		SetDlgItemText(IDC_EDIT_ADDR, "news2.2ch.net/newsplus/");
		SetDlgItemText(IDC_EDIT_HEADER, "<a href=\"#");
		break;
	case 12:	// ２ちゃんねる ニュース実況
		SetDlgItemText(IDC_EDIT_ADDR, "news2.2ch.net/liveplus/");
		SetDlgItemText(IDC_EDIT_HEADER, "<a href=\"#");
		break;
	case 13:	// ２ちゃんねる ＰＣニュース
		SetDlgItemText(IDC_EDIT_ADDR, "pc.2ch.net/pcnews/");
		SetDlgItemText(IDC_EDIT_HEADER, "<a href=\"#");
		break;
	case 14:	// 読売新聞 社会
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/04/index.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "◆<a href");
		break;
	case 15:	// 読売新聞 政治
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/01/");
		SetDlgItemText(IDC_EDIT_HEADER, "◆<a href");
		break;
	case 16:	// 読売新聞 経済
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/02/");
		SetDlgItemText(IDC_EDIT_HEADER, "◆<a href");
		break;
	case 17:	// 読売新聞 国際
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/05/");
		SetDlgItemText(IDC_EDIT_HEADER, "◆<a href");
		break;
	case 18:	// 朝日新聞 社会
		SetDlgItemText(IDC_EDIT_ADDR, "www.asahi.com/national/index.html");
		SetDlgItemText(IDC_EDIT_HEADER, "/national/update/");
		break;
	case 19:	// 朝日新聞 経済
		SetDlgItemText(IDC_EDIT_ADDR, "www.asahi.com/business/index.html");
		SetDlgItemText(IDC_EDIT_HEADER, "/business/update/");
		break;
	case 20:	// 朝日新聞 政治
		SetDlgItemText(IDC_EDIT_ADDR, "www.asahi.com/politics/index.html");
		SetDlgItemText(IDC_EDIT_HEADER, "/politics/update/");
		break;
	case 21:	// 朝日新聞 国際
		SetDlgItemText(IDC_EDIT_ADDR, "www.asahi.com/international/index.html");
		SetDlgItemText(IDC_EDIT_HEADER, "/international/update/");
		break;
	case 22:	// 産経新聞 社会
		SetDlgItemText(IDC_EDIT_ADDR, "www.sankei.co.jp/news/shakai.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "cut/dot_blue.gif");
		break;
	case 23:	// 産経新聞 経済
		SetDlgItemText(IDC_EDIT_ADDR, "www.sankei.co.jp/news/keizai.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "cut/dot_blue.gif");
		break;
	case 24:	// 産経新聞 政治
		SetDlgItemText(IDC_EDIT_ADDR, "www.sankei.co.jp/news/seiji.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "cut/dot_blue.gif");
		break;
	case 25:	// 産経新聞 国際
		SetDlgItemText(IDC_EDIT_ADDR, "www.sankei.co.jp/news/kokusai.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "cut/dot_blue.gif");
		break;
	case 26:	// 共同通信社 ニュースハイライト
		SetDlgItemText(IDC_EDIT_ADDR, "www.kyodo.co.jp/k2highlight.shtml");
		SetDlgItemText(IDC_EDIT_HEADER, "<strong>■");
		break;
	case 27:	// 時事通信社 政治
		SetDlgItemText(IDC_EDIT_ADDR, "www.jiji.com/cgi-bin/content.cgi?genre=pol");
		SetDlgItemText(IDC_EDIT_HEADER, "class=headline");
		break;
	case 28:	// 時事通信社 経済
		SetDlgItemText(IDC_EDIT_ADDR, "www.jiji.com/cgi-bin/content.cgi?genre=eco");
		SetDlgItemText(IDC_EDIT_HEADER, "class=headline");
		break;
	case 29:	// 時事通信社 国際
		SetDlgItemText(IDC_EDIT_ADDR, "www.jiji.com/cgi-bin/content.cgi?genre=int");
		SetDlgItemText(IDC_EDIT_HEADER, "class=headline");
		break;
	case 30:	// 時事通信社 社会
		SetDlgItemText(IDC_EDIT_ADDR, "www.jiji.com/cgi-bin/content.cgi?genre=soc");
		SetDlgItemText(IDC_EDIT_HEADER, "class=headline");
		break;
	case 31:	// Bloomberg 国内トップニュース
		SetDlgItemText(IDC_EDIT_ADDR, "quote.bloomberg.com/fgcgi2.cgi?T=japan_news.ht&nicat=TOPJ&adtag=topj1.html");
		SetDlgItemText(IDC_EDIT_HEADER, "T=japan_news_story");
		break;
	case 32:	// Bloomberg 海外トップニュース
		SetDlgItemText(IDC_EDIT_ADDR, "quote.bloomberg.com/fgcgi2.cgi?T=japan_news_topka.ht&nicat=TOPKAIGAI&adtag=topj2.html");
		SetDlgItemText(IDC_EDIT_HEADER, "T=japan_news_story");
		break;
	case 33:	// ＣＮＥＴ Japan ニュース
		SetDlgItemText(IDC_EDIT_ADDR, "japan.cnet.com/News/news.html");
		SetDlgItemText(IDC_EDIT_HEADER, "見出し*****-->");
		break;
	case 34:	// ＺＤｎｅｔ Japan ニュース
		SetDlgItemText(IDC_EDIT_ADDR, "www.zdnet.co.jp/news/");
		SetDlgItemText(IDC_EDIT_HEADER, "FONT SIZE=\"4\"");
		break;
	case 35:	// 首都圏 近距離ＪＲ運行状況
		SetDlgItemText(IDC_EDIT_ADDR, "ekimae3.toshiba.co.jp/jr-info/detail.html");
		SetDlgItemText(IDC_EDIT_HEADER, "     <p>");
		break;
	case 36:	// 首都圏 長距離ＪＲ運行状況
		SetDlgItemText(IDC_EDIT_ADDR, "ekimae3.toshiba.co.jp/jr-info/detail_cho.html");
		SetDlgItemText(IDC_EDIT_HEADER, "     <p>");
		break;
	case 37:	// 首都圏 新幹線運行状況
		SetDlgItemText(IDC_EDIT_ADDR, "ekimae3.toshiba.co.jp/jr-info/detail_sin.html");
		SetDlgItemText(IDC_EDIT_HEADER, "     <p>");
		break;
	default:	//
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	}

	m_ctrl_cmb_qlist.GetLBText(_nSel, sTmpStr);
	SetDlgItemText(IDC_EDIT_TITLE,sTmpStr);

	GetDlgItemText(IDC_EDIT_PROXY, sTmpStr);
	if(sTmpStr != "")
		SetDlgItemInt(IDC_EDIT_PORT, 8080);
	else
		SetDlgItemInt(IDC_EDIT_PORT, 80);
	SetDlgItemInt(IDC_EDIT_SKIP, 0);
	SetDlgItemInt(IDC_EDIT_GETNUM, 10);

	
	if(m_nMode != 0)
	{
		m_sURL_ctrl.EnableWindow(FALSE);
		m_sPhHeader_ctrl.EnableWindow(FALSE);
		m_nPhSkip_ctrl.EnableWindow(FALSE);
		m_nPhGetcount_ctrl.EnableWindow(FALSE);
		m_btn_detail_ctrl.EnableWindow(TRUE);
	}
	else
	{
		m_sURL_ctrl.EnableWindow(TRUE);
		m_sPhHeader_ctrl.EnableWindow(TRUE);
		m_nPhSkip_ctrl.EnableWindow(TRUE);
		m_nPhGetcount_ctrl.EnableWindow(TRUE);
		m_btn_detail_ctrl.EnableWindow(FALSE);
	}

	if(m_nMode == 1)
	{
		CDlgNetStock dlg;

		GetDlgItemText(IDC_EDIT_ADDR, sTmpStr);
		dlg.m_sURL = sTmpStr;
		GetDlgItemText(IDC_EDIT_HEADER, sTmpStr);
		dlg.m_sPhHeader = sTmpStr;
		dlg.m_sPhHeaderB = theApp->sPhHeaderB;
		dlg.m_sPhHeaderC = theApp->sPhHeaderC;
		dlg.m_sItems = theApp->sItems;

		if(dlg.DoModal() == IDOK)
		{
			SetDlgItemText(IDC_EDIT_ADDR, dlg.m_sURL);
			SetDlgItemText(IDC_EDIT_HEADER, dlg.m_sPhHeader);
			theApp->sPhHeaderB = dlg.m_sPhHeaderB;
			theApp->sPhHeaderC = dlg.m_sPhHeaderC;
			theApp->sItems = dlg.m_sItems;

		}
	}

}

BOOL CDlgNetConf::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	if(m_nMode != 0)
	{
		m_sURL_ctrl.EnableWindow(FALSE);
		m_sPhHeader_ctrl.EnableWindow(FALSE);
		m_nPhSkip_ctrl.EnableWindow(FALSE);
		m_nPhGetcount_ctrl.EnableWindow(FALSE);
		m_btn_detail_ctrl.EnableWindow(TRUE);
	}
	else
	{
		m_btn_detail_ctrl.EnableWindow(FALSE);
	}

	// 証券モードのとき、該当タイトルに位置付ける
	if(m_nMode == 1)
		m_ctrl_cmb_qlist.SetCurSel(10);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDlgNetConf::OnBtnDetail() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CDlgNetStock dlg;
	CString sTmpStr;
	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	GetDlgItemText(IDC_EDIT_ADDR, sTmpStr);
	dlg.m_sURL = sTmpStr;
	GetDlgItemText(IDC_EDIT_HEADER, sTmpStr);
	dlg.m_sPhHeader = sTmpStr;
	dlg.m_sPhHeaderB = theApp->sPhHeaderB;
	dlg.m_sPhHeaderC = theApp->sPhHeaderC;
	dlg.m_sItems = theApp->sItems;

	if(dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT_ADDR, dlg.m_sURL);
		SetDlgItemText(IDC_EDIT_HEADER, dlg.m_sPhHeader);
		theApp->sPhHeaderB = dlg.m_sPhHeaderB;
		theApp->sPhHeaderC = dlg.m_sPhHeaderC;
		theApp->sItems = dlg.m_sItems;
	}
	
}
