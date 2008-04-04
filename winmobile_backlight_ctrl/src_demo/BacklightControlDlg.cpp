// BacklightControlDlg.cpp : ダイアログ クラス 実装ファイル
//

// ***********************
// BacklightControl for CE  (Windows CE のバックライト制御）
// 
// Copyright (C) 2008 INOUE. Hirokazu
//
// 
// http://www.opensource.jp/gpl/gpl.ja.html
// このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフトウェ
// ア財団によって発行された GNU 一般公衆利用許諾契約書(バージョン2か、希
// 望によってはそれ以降のバージョンのうちどれか)の定める条件の下で再頒布
// または改変することができます。
// 
// このプログラムは有用であることを願って頒布されますが、*全くの無保証* 
// です。商業可能性の保証や特定の目的への適合性は、言外に示されたものも含
// め全く存在しません。詳しくはGNU 一般公衆利用許諾契約書をご覧ください。
// 
// あなたはこのプログラムと共に、GNU 一般公衆利用許諾契約書の複製物を一部
// 受け取ったはずです。もし受け取っていなければ、フリーソフトウェア財団ま
// で請求してください(宛先は the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA 02111-1307 USA)。
//


#include "stdafx.h"
#include "BacklightControl.h"
#include "BacklightControlDlg.h"

#include <pm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBacklightControlDlg dialog

CBacklightControlDlg::CBacklightControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBacklightControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBacklightControlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBacklightControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBacklightControlDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBacklightControlDlg, CDialog)
	//{{AFX_MSG_MAP(CBacklightControlDlg)
	ON_BN_CLICKED(IDC_BUTTON_D4, OnButtonD4)
	ON_BN_CLICKED(IDC_BUTTON_D0, OnButtonD0)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, OnButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBacklightControlDlg message handlers

BOOL CBacklightControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here

	// 状態表示
	OnButtonGet();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// ***********************
// バックライト OFF
// ***********************
void CBacklightControlDlg::OnButtonD4() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF

	// 状態表示
	OnButtonGet();
	
}

// ***********************
// バックライト ON
// ***********************
void CBacklightControlDlg::OnButtonD0() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF (一旦OFFにしないと反応しない場合あり)
	::Sleep(200);
	::SetDevicePower(_T("BKL1:"), POWER_NAME, D0);	// ON
	
	// 状態表示
	OnButtonGet();

}

// ***********************
// バックライト制御を自動（パワーマネジメントに従う）
// ***********************
void CBacklightControlDlg::OnButtonAuto() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF (一旦OFFにしないと反応しない場合あり)
	::Sleep(200);
	::SetDevicePower(_T("BKL1:"), POWER_NAME, PwrDeviceUnspecified);	// 自動制御
	
	// 状態表示
	OnButtonGet();

}

// ***********************
// テキストボックスに現在の状態を表示
// ***********************
void CBacklightControlDlg::OnButtonGet() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CEDEVICE_POWER_STATE devState;

	if(::GetDevicePower(_T("BKL1:"), POWER_NAME, &devState) != ERROR_SUCCESS)
	{
		SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Read Error"));
	}

	if(devState == PwrDeviceUnspecified) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Unspecified"));
	if(devState == D0) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Full ON (D0)"));
	if(devState == D1) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Low ON (D1)"));
	if(devState == D2) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Standby (D2)"));
	if(devState == D3) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Sleep (D3)"));
	if(devState == D4) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("OFF (D4)"));
	if(devState == PwrDeviceMaximum) SetDlgItemText(IDC_EDIT_POWERSTATE, _T("Max"));
	
}
