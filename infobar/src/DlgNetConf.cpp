// DlgNetConf.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "infobar00.h"
#include "DlgNetConf.h"

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
	DDX_Control(pDX, IDC_COMBO_QLIST, m_ctrl_cmb_qlist);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_sURL);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDV_MinMaxUInt(pDX, m_nPort, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_HEADER, m_sPhHeader);
	DDX_Text(pDX, IDC_EDIT_SKIP, m_nPhSkip);
	DDV_MinMaxUInt(pDX, m_nPhSkip, 0, 100);
	DDX_Text(pDX, IDC_EDIT_GETNUM, m_nPhGetcount);
	DDV_MinMaxUInt(pDX, m_nPhGetcount, 0, 100);
	DDX_Text(pDX, IDC_EDIT_PROXY, m_sProxy);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_sTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNetConf, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgNetConf)
	ON_CBN_SELCHANGE(IDC_COMBO_QLIST, OnSelchangeComboQlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNetConf メッセージ ハンドラ

void CDlgNetConf::OnSelchangeComboQlist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int _nSel = m_ctrl_cmb_qlist.GetCurSel();
	CString sTmpStr;

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
	case 10:	// ２ちゃんねる ニュース速報＋
		SetDlgItemText(IDC_EDIT_ADDR, "news2.2ch.net/newsplus/");
		SetDlgItemText(IDC_EDIT_HEADER, "<a href=\"#");
		break;
	case 11:	// ２ちゃんねる ニュース実況
		SetDlgItemText(IDC_EDIT_ADDR, "news2.2ch.net/liveplus/");
		SetDlgItemText(IDC_EDIT_HEADER, "<a href=\"#");
		break;
	case 12:	// ２ちゃんねる ＰＣニュース
		SetDlgItemText(IDC_EDIT_ADDR, "pc.2ch.net/pcnews/");
		SetDlgItemText(IDC_EDIT_HEADER, "<a href=\"#");
		break;
	case 13:	// 読売新聞 社会
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/04/index.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "◆<a href");
		break;
	case 14:	// 読売新聞 政治
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/01/");
		SetDlgItemText(IDC_EDIT_HEADER, "◆<a href");
		break;
	case 15:	// 読売新聞 経済
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/02/");
		SetDlgItemText(IDC_EDIT_HEADER, "◆<a href");
		break;
	case 16:	// 読売新聞 国際
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/05/");
		SetDlgItemText(IDC_EDIT_HEADER, "◆<a href");
		break;
	case 17:	// 朝日新聞 社会
		SetDlgItemText(IDC_EDIT_ADDR, "www.asahi.com/national/index.html");
		SetDlgItemText(IDC_EDIT_HEADER, "/national/update/");
		break;
	case 18:	// 朝日新聞 経済
		SetDlgItemText(IDC_EDIT_ADDR, "www.asahi.com/business/index.html");
		SetDlgItemText(IDC_EDIT_HEADER, "/business/update/");
		break;
	case 19:	// 朝日新聞 政治
		SetDlgItemText(IDC_EDIT_ADDR, "www.asahi.com/politics/index.html");
		SetDlgItemText(IDC_EDIT_HEADER, "/politics/update/");
		break;
	case 20:	// 朝日新聞 国際
		SetDlgItemText(IDC_EDIT_ADDR, "www.asahi.com/international/index.html");
		SetDlgItemText(IDC_EDIT_HEADER, "/international/update/");
		break;
	case 21:	// 産経新聞 社会
		SetDlgItemText(IDC_EDIT_ADDR, "www.sankei.co.jp/news/shakai.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "cut/dot_blue.gif");
		break;
	case 22:	// 産経新聞 経済
		SetDlgItemText(IDC_EDIT_ADDR, "www.sankei.co.jp/news/keizai.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "cut/dot_blue.gif");
		break;
	case 23:	// 産経新聞 政治
		SetDlgItemText(IDC_EDIT_ADDR, "www.sankei.co.jp/news/seiji.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "cut/dot_blue.gif");
		break;
	case 24:	// 産経新聞 国際
		SetDlgItemText(IDC_EDIT_ADDR, "www.sankei.co.jp/news/kokusai.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "cut/dot_blue.gif");
		break;
	case 25:	// 共同通信社 ニュースハイライト
		SetDlgItemText(IDC_EDIT_ADDR, "www.kyodo.co.jp/k2highlight.shtml");
		SetDlgItemText(IDC_EDIT_HEADER, "<strong>■");
		break;
	case 26:	// 時事通信社 政治
		SetDlgItemText(IDC_EDIT_ADDR, "www.jiji.com/cgi-bin/content.cgi?genre=pol");
		SetDlgItemText(IDC_EDIT_HEADER, "class=headline");
		break;
	case 27:	// 時事通信社 経済
		SetDlgItemText(IDC_EDIT_ADDR, "www.jiji.com/cgi-bin/content.cgi?genre=eco");
		SetDlgItemText(IDC_EDIT_HEADER, "class=headline");
		break;
	case 28:	// 時事通信社 国際
		SetDlgItemText(IDC_EDIT_ADDR, "www.jiji.com/cgi-bin/content.cgi?genre=int");
		SetDlgItemText(IDC_EDIT_HEADER, "class=headline");
		break;
	case 29:	// 時事通信社 社会
		SetDlgItemText(IDC_EDIT_ADDR, "www.jiji.com/cgi-bin/content.cgi?genre=soc");
		SetDlgItemText(IDC_EDIT_HEADER, "class=headline");
		break;
	case 30:	// Bloomberg 国内トップニュース
		SetDlgItemText(IDC_EDIT_ADDR, "quote.bloomberg.com/fgcgi2.cgi?T=japan_news.ht&nicat=TOPJ&adtag=topj1.html");
		SetDlgItemText(IDC_EDIT_HEADER, "T=japan_news_story");
		break;
	case 31:	// Bloomberg 海外トップニュース
		SetDlgItemText(IDC_EDIT_ADDR, "quote.bloomberg.com/fgcgi2.cgi?T=japan_news_topka.ht&nicat=TOPKAIGAI&adtag=topj2.html");
		SetDlgItemText(IDC_EDIT_HEADER, "T=japan_news_story");
		break;
	case 32:	// ＣＮＥＴ Japan ニュース
		SetDlgItemText(IDC_EDIT_ADDR, "japan.cnet.com/News/news.html");
		SetDlgItemText(IDC_EDIT_HEADER, "見出し*****-->");
		break;
	case 33:	// ＺＤｎｅｔ Japan ニュース
		SetDlgItemText(IDC_EDIT_ADDR, "www.zdnet.co.jp/news/");
		SetDlgItemText(IDC_EDIT_HEADER, "FONT SIZE=\"4\"");
		break;
	case 34:	// 首都圏 近距離ＪＲ運行状況
		SetDlgItemText(IDC_EDIT_ADDR, "ekimae3.toshiba.co.jp/jr-info/detail.html");
		SetDlgItemText(IDC_EDIT_HEADER, "     <p>");
		break;
	case 35:	// 首都圏 長距離ＪＲ運行状況
		SetDlgItemText(IDC_EDIT_ADDR, "ekimae3.toshiba.co.jp/jr-info/detail_cho.html");
		SetDlgItemText(IDC_EDIT_HEADER, "     <p>");
		break;
	case 36:	// 首都圏 新幹線運行状況
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
	
}
