// *******************************************************************
// プログラムに渡される引数を表示するプログラム
//
// *******************************************************************
#include "stdafx.h"
#include "DummyProg.h"
#include "DummyProgDlg.h"
#include "PharseCmdline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDummyProgApp

BEGIN_MESSAGE_MAP(CDummyProgApp, CWinApp)
	//{{AFX_MSG_MAP(CDummyProgApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyProgApp クラスの構築

CDummyProgApp::CDummyProgApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CDummyProgApp オブジェクト

CDummyProgApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDummyProgApp クラスの初期化

#include<direct.h>

BOOL CDummyProgApp::InitInstance()
{
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	char szTmp[MAX_PATH];
	OSVERSIONINFO ver;


	CDummyProgDlg dlg;
	m_pMainWnd = &dlg;

	// プログラム本体のフルパス
	::GetModuleFileName(NULL, szTmp, MAX_PATH);
	dlg.m_edit_thisprog = szTmp;

	// 引数
//	dlg.m_edit_param = m_lpCmdLine;
	CPharseCmdline PharseCmdline;
	PharseCmdline.SetOrgStr(m_lpCmdLine);
	int i = PharseCmdline.CountParam();
	dlg.m_edit_param.Format("渡された引数は %d 個\r\n", i);
	CString sTmp1;
	do{
		sTmp1 = PharseCmdline.GetNextParam();
		if(sTmp1 == "") break;
		dlg.m_edit_param = dlg.m_edit_param + sTmp1 + "\r\n";
	}while(1);

	// カレント･ディレクトリ
	::_getcwd(szTmp, sizeof(szTmp));
	dlg.m_edit_curdir = szTmp;

	// OSバージョン
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((LPOSVERSIONINFO)&ver);

	dlg.m_edit_winver = "Windows ";
	switch(ver.dwPlatformId)
	{
		case VER_PLATFORM_WIN32s :
				dlg.m_edit_winver += "3.1 Win32s";
				break;
		case VER_PLATFORM_WIN32_WINDOWS :
			if(ver.dwMinorVersion == 0)
			{
				if((ver.dwBuildNumber&0xffff) < 1000)
				{
					dlg.m_edit_winver += "95";
				}
				else dlg.m_edit_winver += "95 OSR2";
				break;
			}
			else
			{
				if(ver.dwBuildNumber&0xffff < 2000)
					dlg.m_edit_winver += "98";
				else dlg.m_edit_winver += "98SE";
				break;
			}
		case VER_PLATFORM_WIN32_NT :
			dlg.m_edit_winver += "NT, 2000";
			break;
		default : dlg.m_edit_winver += "Unknown"; break;
	}
	sprintf(szTmp, "  Version %Lu.%Lu.%u %s", ver.dwMajorVersion, ver.dwMinorVersion,
			ver.dwBuildNumber, ver.szCSDVersion);
	dlg.m_edit_winver += szTmp;


	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//       記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <ｷｬﾝｾﾙ> で消された時のコードを
		//       記述してください。
	}

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}
