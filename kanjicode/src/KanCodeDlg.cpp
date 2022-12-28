// KanCodeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "KanCode.h"
#include "KanCodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数を生成しオーバーライドします
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanCodeDlg ダイアログ

CKanCodeDlg::CKanCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKanCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKanCodeDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKanCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKanCodeDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKanCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CKanCodeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCNV, OnCnv)
	ON_BN_CLICKED(IDCCLP, OnCclp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanCodeDlg メッセージ ハンドラ

BOOL CKanCodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}
	
	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	VERIFY(EditInputBox.SubclassDlgItem(IDC_EDIT_INPUT, this));

	CHARFORMAT ch;
	EditInputBox.GetDefaultCharFormat(ch);
	ch.dwMask |= CFM_BOLD;
	ch.dwEffects |= CFE_BOLD;
	EditInputBox.SetDefaultCharFormat(ch);

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CKanCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CKanCodeDlg::OnPaint() 
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
HCURSOR CKanCodeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CKanCodeDlg::OnCnv() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	char str[10];
	unsigned int sjis,jis, kt, c;

	CHARFORMAT ch;
	CString test;
	EditInputBox.SetSel(0L,-1L);
	test = EditInputBox.GetSelText();
	EditInputBox.Cut();
	EditInputBox.GetSelectionCharFormat(ch);
	ch.dwMask |= CFM_BOLD;
	ch.dwEffects |= CFE_BOLD;
	EditInputBox.SetSelectionCharFormat(ch);

	GetDlgItemText(IDC_EDIT_INPUT, (LPSTR)str, 4);
	sjis = ((unsigned char)str[0])<<8 | (unsigned char)str[1];
	// SJISコードを表示
	sprintf((LPSTR)str, "%04X", sjis);
	SetDlgItemText(IDC_EDIT_SJIS, str);
	
	// Shift JIS から JIS への変換
	if(sjis >= 0xfa40)
	{	// 拡張領域(FA40H ～ FC4BH) の処理
		// SJIS の xx7F,xxFD ～ xx3F が存在しない事の処理
		(sjis-0xfa00)%0x100 > 0x7f ? c = sjis-1 : c = sjis;
		c = c - 0xfa40 - (sjis-0xfa00)/0x100*(1+3+0x40);
		// JIS の xx7F ～ xx20 のスキップ処理
		jis = 0x9321 + (c/(0x7e - 0x21 + 1))*0x100 + c%(0x7e - 0x21 + 1);
	}
	else jis = _mbcjmstojis(sjis);

	sprintf((LPSTR)str, "%04X", jis);
	SetDlgItemText(IDC_EDIT_JIS, str);
	
	// 区点コードの表示
	if(sjis >= 0xfa40)
	{	// 拡張領域(FA40H ～ FC4BH)の処理
		kt = 11501 + (c/94)*100 + c%94;
	}
	else
	{
		// JIS コードの「スペース文字 0x2121」からの相対位置を求める
		// JIS コードは 0x7F～0x20(0xa2個)が使われていない事を考慮する
		c = jis - 0x2121 - ((jis>>8) -0x21)*0xa2;
		// 区点コードは 95～00が使用されていない
		// 100までで、94個しか使われていない事を考慮する
		kt = 101 + (c/94)*100 + c%94;
	}
	if(jis == 0) kt = 0;

	sprintf((LPSTR)str, "%05u", kt);
	SetDlgItemText(IDC_EDIT_KU, str);

}

void CKanCodeDlg::OnCclp() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EditInputBox.Paste();
	OnCnv();
}
