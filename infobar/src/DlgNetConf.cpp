// DlgNetConf.cpp : �C���v�������e�[�V���� �t�@�C��
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
// CDlgNetConf �v���p�e�B �y�[�W

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
	DDV_MinMaxUInt(pDX, m_nPort, 0, 622565);
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
// CDlgNetConf ���b�Z�[�W �n���h��

void CDlgNetConf::OnSelchangeComboQlist() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int _nSel = m_ctrl_cmb_qlist.GetCurSel();
	CString sTmpStr;

	switch(_nSel)
	{
	case 0:	// Yahoo ���{ �j���[�X ����
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=pol&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 1:	// Yahoo ���{ �j���[�X �Љ�
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=soci&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 2:	// Yahoo ���{ �j���[�X �C�O
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=int&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 3:	// Yahoo ���{ �j���[�X �o�ϑ���
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=bus_all&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 4:	// Yahoo ���{ �j���[�X �Y��
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=ind&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 5:	// Yahoo ���{ �j���[�X �R���s���[�^
		SetDlgItemText(IDC_EDIT_ADDR, "headlines.yahoo.co.jp/hl?c=sci&t=l");
		SetDlgItemText(IDC_EDIT_HEADER, "<li>");
		break;
	case 6:	// �Q�����˂� �j���[�X����{
		SetDlgItemText(IDC_EDIT_ADDR, "news2.2ch.net/newsplus/");
		SetDlgItemText(IDC_EDIT_HEADER, "<a href=\"#");
		break;
	case 7:	// �Q�����˂� �j���[�X����
		SetDlgItemText(IDC_EDIT_ADDR, "news2.2ch.net/liveplus/");
		SetDlgItemText(IDC_EDIT_HEADER, "<a href=\"#");
		break;
	case 8:	// �Q�����˂� �o�b�j���[�X
		SetDlgItemText(IDC_EDIT_ADDR, "pc.2ch.net/pcnews/");
		SetDlgItemText(IDC_EDIT_HEADER, "<a href=\"#");
		break;
	case 9:	// �ǔ��V�� �Љ�
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/04/index.htm");
		SetDlgItemText(IDC_EDIT_HEADER, "��<a href");
		break;
	case 10:	// �ǔ��V�� ����
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/01/");
		SetDlgItemText(IDC_EDIT_HEADER, "��<a href");
		break;
	case 11:	// �ǔ��V�� �o��
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/02/");
		SetDlgItemText(IDC_EDIT_HEADER, "��<a href");
		break;
	case 12:	// �ǔ��V�� ����
		SetDlgItemText(IDC_EDIT_ADDR, "www.yomiuri.co.jp/05/");
		SetDlgItemText(IDC_EDIT_HEADER, "��<a href");
		break;
	case 13:	// Yahoo USA top story
		SetDlgItemText(IDC_EDIT_ADDR, "news.yahoo.com/news?tmpl=index2&cid=716");
		SetDlgItemText(IDC_EDIT_HEADER, "class=topstory");
		break;
	case 14:	// Yahoo USA world selection
		SetDlgItemText(IDC_EDIT_ADDR, "news.yahoo.com/news?tmpl=index2&cid=721");
		SetDlgItemText(IDC_EDIT_HEADER, "class=topstory");
		break;
	case 15:	// Yahoo USA business selection
		SetDlgItemText(IDC_EDIT_ADDR, "news.yahoo.com/news?tmpl=index2&cid=749");
		SetDlgItemText(IDC_EDIT_HEADER, "class=topstory");
		break;
	case 16:	// Yahoo USA technology selection
		SetDlgItemText(IDC_EDIT_ADDR, "news.yahoo.com/news?tmpl=index2&cid=738");
		SetDlgItemText(IDC_EDIT_HEADER, "class=topstory");
		break;
	case 17:	// �����ʐM�� �j���[�X�n�C���C�g
		SetDlgItemText(IDC_EDIT_ADDR, "www.kyodo.co.jp/k2highlight.shtml");
		SetDlgItemText(IDC_EDIT_HEADER, "<strong>��");
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
