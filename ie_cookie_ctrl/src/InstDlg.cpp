// InstDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "cookiectrl.h"
#include "InstDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstDlg ダイアログ


CInstDlg::CInstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInstDlg)
	m_r_autoexec = -1;
	m_chk_uninst_cpl = FALSE;
	m_chk_uninst_start = FALSE;
	//}}AFX_DATA_INIT
}


void CInstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInstDlg)
	DDX_Control(pDX, IDC_R_AUTOEXEC, m_r_autoexec_ctrl1);	// 手動で追加
	DDX_Control(pDX, IDC_R_AUTOEXEC2, m_r_autoexec_ctrl2);	// 手動で追加
	DDX_Control(pDX, IDC_R_AUTOEXEC3, m_r_autoexec_ctrl3);	// 手動で追加
	DDX_Control(pDX, IDC_R_AUTOEXEC4, m_r_autoexec_ctrl4);	// 手動で追加
	DDX_Radio(pDX, IDC_R_AUTOEXEC, m_r_autoexec);
	DDX_Check(pDX, IDC_CHECK_UNINST_CPL, m_chk_uninst_cpl);
	DDX_Check(pDX, IDC_CHECK_UNINST_START, m_chk_uninst_start);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInstDlg, CDialog)
	//{{AFX_MSG_MAP(CInstDlg)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BTN_HELP, OnBtnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstDlg メッセージ ハンドラ

BOOL CInstDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->WinHelp(0);

	return 0;
}

BOOL CInstDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	// *******************
	// HKLM...run に書き込み権限の無いときは、アドミニストレータインストールを無効化
	// DDX_Control(pDX, IDC_R_AUTOEXEC, m_r_autoexec_ctrl); を  IDC_R_AUTOEXEC4 と書き換えた
	// *******************
	if(!m_IsAdminInst) m_r_autoexec_ctrl4.EnableWindow(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// **********************************
//  ヘルプの表示
// **********************************
void CInstDlg::OnBtnHelp() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CCookieCtrlApp *theApp;
	theApp = (CCookieCtrlApp *)AfxGetApp();

	theApp->WinHelp(0);
	
}
