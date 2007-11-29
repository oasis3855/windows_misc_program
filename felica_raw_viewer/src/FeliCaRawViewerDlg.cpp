/***************************************************

FeliCaRawViewerDlg.cpp

FeliCa Raw Viewer ソフトウエア
  Copyright (C) 2007, INOUE. Hirokazu
  All rights reserved.

このソフトウエアでは、felicalib ライブラリを利用しています
  Copyright (C) 2007, Takuya Murakami
  All rights reserved.

The BSD License (http://opensource.org/licenses/bsd-license.php)
---------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer. 
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution. 
* Neither the name of the this project nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

以下の日本語訳は参考のために添付したものです。英語版だけが法律的に有効である
ことに注意してください。

BSDライセンス条項 （日本語訳）
    (http://sourceforge.jp/projects/opensource/wiki/licenses%2Fnew_BSD_license)
-----------------------------
ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を
満たす場合に限り、再頒布および使用が許可されます。

* ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記
  免責条項を含めること。 
* バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の
  著作権表示、本条件一覧、および下記免責条項を含めること。 
* 書面による特別の許可なしに、本ソフトウェアから派生した製品の宣伝または販売
  促進に、このプロジェクトの名前またはコントリビューターの名前を使用しては
  ならない。

本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供
されており、明示黙示を問わず、商業的な使用可能性、および特定の目的に対する
適合性に関する暗黙の保証も含め、またそれに限定されない、いかなる保証も
ありません。著作権者もコントリビューターも、事由のいかんを問わず、損害発生の
原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任であるか（過失
その他の）不法行為であるかを問わず、仮にそのような損害が発生する可能性を
知らされていたとしても、本ソフトウェアの使用によって発生した（代替品または
代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、
またそれに限定されない）直接損害、間接損害、偶発的な損害、特別損害、懲罰的
損害、または結果損害について、一切責任を負わないものとします。


***************************************************/

#include "stdafx.h"
#include "FeliCaRawViewer.h"
#include "FeliCaRawViewerDlg.h"

#include <HtmlHelp.h>

#include "_FelicaLib\felicalib.h"

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

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
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
// CFeliCaRawViewerDlg ダイアログ

CFeliCaRawViewerDlg::CFeliCaRawViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFeliCaRawViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeliCaRawViewerDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFeliCaRawViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeliCaRawViewerDlg)
	DDX_Control(pDX, IDC_EDIT_DATA, m_editData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeliCaRawViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CFeliCaRawViewerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_READ, OnBtnRead)
	ON_BN_CLICKED(IDC_BTN_COPY, OnBtnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeliCaRawViewerDlg メッセージ ハンドラ

BOOL CFeliCaRawViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			pSysMenu->AppendMenu(MF_STRING, IDM_HELP, "ヘルプの表示 ...");
		}
	}

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。

	CheckDlgButton(IDC_CHECK_PROTECT, TRUE);
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CFeliCaRawViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == IDM_HELP)
	{
		SendMessage(WM_HELP);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CFeliCaRawViewerDlg::OnPaint() 
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
HCURSOR CFeliCaRawViewerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/**************
FeliCa データ読み込みボタンを押したときの処理
***************/

void CFeliCaRawViewerDlg::OnBtnRead() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	pasori *p;
	felica *f, *f2;
	char sTemp[128];
	CString sDumpMsg;
    int i, j, k;

	sDumpMsg = "";		// ダンプ メッセージ（文字列）の初期化

	SetDlgItemText(IDC_EDIT_IDM, "");
	SetDlgItemText(IDC_EDIT_PMM, "");
	SetDlgItemText(IDC_EDIT_DATA, "");

	if(!GetFelicaDllPath())
	{
		SetDlgItemText(IDC_EDIT_DATA, "felica.dllが見つかりません");
		return;
	}

	p = pasori_open(NULL);

	if(!p)
	{
		SetDlgItemText(IDC_EDIT_DATA, "カードリーダに接続できません");
		return;
	}

	pasori_init(p);

	f = felica_polling(p, POLLING_ANY, 0, 0);
	if(!f)
	{
		SetDlgItemText(IDC_EDIT_DATA, "ポーリング失敗");
		pasori_close(p);
		return;
	}

	// IDm と PMm をダイアログボックスに表示する
	HexDumpToStr(f->IDm, sTemp, 8);
	SetDlgItemText(IDC_EDIT_IDM, sTemp);

	HexDumpToStr(f->PMm, sTemp, 8);
	SetDlgItemText(IDC_EDIT_PMM, sTemp);

	felica_free(f);

	// 以下、ダンプ データ用にテキスト（sDumpMsg）を作成する

	f = felica_enum_systemcode(p);
	if(!f)
	{
		SetDlgItemText(IDC_EDIT_DATA, "システムコード読み込み失敗");
		pasori_close(p);
		return;
	}

	HexDumpToStr(f->IDm, sTemp, 8);
	sDumpMsg += "IDm : ";
	sDumpMsg += sTemp;
	sDumpMsg += "\r\n";

	HexDumpToStr(f->PMm, sTemp, 8);
	sDumpMsg += "PMm : ";
	sDumpMsg += sTemp;
	sDumpMsg += "\r\n\r\n";

	sprintf(sTemp, "* Number of service = %d\r\n\r\n", f->num_system_code);
	sDumpMsg += sTemp;

	for (i = 0; i < f->num_system_code; i++)		// システムの数だけループする
	{
		sprintf(sTemp, "# System code: %04X\r\n", N2HS(f->system_code[i]));
		sDumpMsg += sTemp;

		f2 = felica_enum_service(p, N2HS(f->system_code[i]));
		if (!f2)
		{
			sDumpMsg += " Enum service failed.\r\n";
		}
		else
		{

			sprintf(sTemp, " # Number of area = %d\r\n", f2->num_area_code);
			sDumpMsg += sTemp;

			for (j = 0; j < f2->num_area_code; j++)		// エリアの数だけループする
			{
				sprintf(sTemp, "  # Area: %04X - %04X\r\n", f2->area_code[j], f2->end_service_code[j]);
				sDumpMsg += sTemp;
			}	    

			sprintf(sTemp, " # Number of service code = %d\r\n", f2->num_service_code);
			sDumpMsg += sTemp;

			for (j = 0; j < f2->num_service_code; j++)	// サービスの数だけループする
			{
				uint16 service = f2->service_code[j];
				strcpy(sTemp, printserviceinfo(service));
				sDumpMsg += "  ";
				sDumpMsg += sTemp;

				if((service & 0x1) || !IsDlgButtonChecked(IDC_CHECK_PROTECT))
				{
					for (k = 0; k < 255; k++)
					{
						uint8 data[16];

						if (felica_read_without_encryption02(f2, service, 0, (uint8)k, data))
						{
							sprintf(sTemp, "   %04X:%04X data is not available\r\n", service, k);
							sDumpMsg += sTemp;
							break;
						}

						sprintf(sTemp, "   %04X:%04X ", service, k);
						sDumpMsg += sTemp;

						HexDumpToStr(data, sTemp, 16);
						sDumpMsg += sTemp;
						sDumpMsg += "\r\n";
					}
				}
			}
			sDumpMsg += "\r\n---------\r\n";

			felica_free(f2);
		}

	}

	felica_free(f);
	pasori_close(p);

	// ダンプ データをダイアログボックスに表示する
	SetDlgItemText(IDC_EDIT_DATA, sDumpMsg);

	
}

/**************
クリップボードへコピーボタンを押したときの処理
***************/

void CFeliCaRawViewerDlg::OnBtnCopy() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_editData.SetSel(0, -1, TRUE);
	m_editData.Copy();
}

/**************
// ヘルプ表示関数をオーバーライドして、HTMLヘルプを表示するようにする
***************/

void CFeliCaRawViewerDlg::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
//	CDialog::WinHelp(dwData, nCmd);

	// HELP_CONTEXT 以外は何もしない
	if(nCmd != HELP_CONTEXT) return;

	// HTMLヘルプのhWndハンドラ （失敗時はNULL）
	HWND hWnd_Help;
	// ヘルプファイルへの絶対パスを作るための、パス分解用一時文字列
	char sChmPath[MAX_PATH];
	char *sDotPtr;

	CString sTmp, sAfxMsg;

	// アプリケーション自身のパスを取得し、拡張子を chm に書き換える
	// (HtmlHelp関数はカレントフォルダのヘルプファイルを取得しようとするため)
	if(!::GetModuleFileName(NULL, sChmPath, MAX_PATH)) return;

	// 最後の'.'文字を検索し、それ以降を削除して '.chm' を足す
	sDotPtr = strrchr(sChmPath, '.');
	if(sDotPtr == NULL)
		return;					// '.' 文字が存在しないことはありえない
	strcpy(sDotPtr, ".chm");	// ヘルプファイルのフルパス名完成

	// ヘルプの表示
	hWnd_Help = ::HtmlHelp(this->m_hWnd, sChmPath, HH_HELP_CONTEXT, LOWORD(dwData));

	if(hWnd_Help == NULL)
	{	// ヘルプファイルの起動に失敗した場合
		MessageBox("ヘルプファイルを表示できません", "FeliCaRawViewer", MB_ICONWARNING);
	}

}
