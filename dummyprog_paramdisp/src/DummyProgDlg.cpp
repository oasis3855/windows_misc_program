// DummyProgDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DummyProg.h"
#include "DummyProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDummyProgDlg ダイアログ

CDummyProgDlg::CDummyProgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDummyProgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDummyProgDlg)
	m_edit_param = _T("");
	m_edit_thisprog = _T("");
	m_edit_curdir = _T("");
	m_edit_winver = _T("");
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDummyProgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDummyProgDlg)
	DDX_Text(pDX, IDC_EDIT_PARAM, m_edit_param);
	DDX_Text(pDX, IDC_EDIT_THISPROG, m_edit_thisprog);
	DDX_Text(pDX, IDC_EDIT_CURDIR, m_edit_curdir);
	DDX_Text(pDX, IDC_EDIT_WINVER, m_edit_winver);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDummyProgDlg, CDialog)
	//{{AFX_MSG_MAP(CDummyProgDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyProgDlg メッセージ ハンドラ

BOOL CDummyProgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CDummyProgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CDummyProgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
