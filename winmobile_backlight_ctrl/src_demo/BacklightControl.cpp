// BacklightControl.cpp : Defines the class behaviors for the application.
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBacklightControlApp

BEGIN_MESSAGE_MAP(CBacklightControlApp, CWinApp)
	//{{AFX_MSG_MAP(CBacklightControlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBacklightControlApp construction

CBacklightControlApp::CBacklightControlApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBacklightControlApp object

CBacklightControlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBacklightControlApp initialization

BOOL CBacklightControlApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// メインダイアログの表示
	CBacklightControlDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

