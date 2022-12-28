/*************************************************************************
 Windows 用 MAN ブラウザ
*************************************************************************/

#include <afxwin.h>
#include <afxdlgs.h>
#include <afxext.h>
#include "resource.h"
#include "resrc1.h"
#include <ctl3d.h>
#include "Winman.h"

/////////////////////////////////////////////////////////////////////////////
// ThisApp: Just creating this application object runs the whole application.
/////////////////////////////////////////////////////////////////////////////
CWinmanApp ThisApp;

/////////////////////////////////////////////////////////////////////////////
// InitInstance:
// When any App object is created, this member function is automatically
// called.  Any data may be set up at this point.
//
// Also, the main window of the application should be created and shown here.
// Return TRUE if the initialization is successful.
/////////////////////////////////////////////////////////////////////////////
BOOL CWinmanApp::InitInstance()
{
	unsigned long CH, CL;
	m_pMainWnd = new CMainDlgWindow();
	
	Ctl3dRegister(AfxGetInstanceHandle());
	Ctl3dAutoSubclass(AfxGetInstanceHandle());
	Ctl3dSubclassDlg(m_pMainWnd->m_hWnd, CTL3D_ALL);
	m_pMainWnd->Invalidate(TRUE);
	m_pMainWnd->ShowWindow(SW_SHOW);

//	SetClassWord(m_pMainWnd->m_hWnd,GCW_HICON, (WORD)LoadIcon(AFX_IDI_STD_FRAME));

	// 表示色の取得
	CH = ThisApp.GetProfileInt((LPCSTR)"WinMan", (LPCSTR)"ColorH", 0);	
	CL = ThisApp.GetProfileInt((LPCSTR)"WinMan", (LPCSTR)"ColorL", 0);	
	TextColor = ((CH<<16)&0xffff0000L)| CL;
	
	return TRUE;
}

//-----------------------------------------------------------------------
// CMainDlgWindow constructor:
// Create the dialog box using the definition in the dialog template
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMainDlgWindow, CDialog)
	//{{AFX_MSG_MAP(CMainDlgWindow)
	ON_WM_CLOSE()
	ON_COMMAND(ID_EXIT, OnExit)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_MOPTION, OnMenuSetup)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CMB_ITEM, OnSelchangeCmbItem)
	ON_BN_CLICKED(ID_EXITPROG, OnClickedExitprog)
	ON_COMMAND(ID_COPYTEXT, OnCopytext)
	ON_COMMAND(ID_SELECTALL, OnSelectall)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_SETTEXTCOLOR, OnSettextcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMainDlgWindow::CMainDlgWindow()
{
	Create(MAIN_DLG_WINDOW);		// Create the Main Dialog Window
}

BOOL CMainDlgWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: この位置にその他の初期化用コードを追加してください。
	VERIFY(FilenameCB.SubclassDlgItem(IDC_CMB_ITEM, this));
	VERIFY(MainTextED.SubclassDlgItem(IDC_EDITMAIN, this));

	strcpy(mandir, ThisApp.GetProfileString((LPCSTR)"WinMan", (LPCSTR)"DataDir"));
	TextBufAllocated = 0;
	DisplayListBox();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります。
}

void CMainDlgWindow::OnClose()
{
	DestroyWindow();				// Destroy the dialog box window
	delete this;					// Delete 'this' dialog box object
	Ctl3dUnregister(AfxGetInstanceHandle());
	PostQuitMessage(0);				// End the application
}

void CMainDlgWindow::OnExit()
{
	OnClose();						// Do the OnClose member function
}



HBRUSH CMainDlgWindow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	const MSG	*msg=GetCurrentMessage();							// Get the current message
	static HBRUSH hBrush;	// ユーザーカラー設定用

	CModalDialog::OnCtlColor(pDC, pWnd, nCtlColor);				// Call base class first

	hBrush = (HBRUSH)Ctl3dCtlColorEx(msg->message, msg->wParam, msg->lParam); // CTL3D はﾃｷｽﾄｶﾗｰも変更するのでここで呼びだす。
	if(pWnd->GetDlgCtrlID() == IDC_EDITMAIN)
	{
		pDC->SetTextColor(ThisApp.TextColor);
		pDC->SetBkMode(OPAQUE);
	}

	return hBrush;
}


void CMainDlgWindow::OnMenuSetup()
{
	// TODO: この位置にコマンド ハンドラのコードを追加してください。
	char buf[_MAX_PATH+_MAX_FNAME+_MAX_EXT+2];
	char pdrv[_MAX_DRIVE], pdir[_MAX_DIR]; 
	strcpy(buf, "");
	sprintf(buf, "%sSAMPLE.TXT", mandir);
	CFileDialog FileDialog(TRUE, NULL, (LPCSTR)buf,
		OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
		"テキストファイル|*.TXT||", this);
	char bufTitle[] = "表示するﾃｷｽﾄﾌｧｲﾙのあるﾃﾞｨﾚｸﾄﾘの選択";
	FileDialog.m_ofn.lpstrTitle = (LPSTR)bufTitle;
	if(FileDialog.DoModal() == IDOK)
	{
		// ファイル名の解析(パスのみとする)
		CString fname;
		fname = FileDialog.GetPathName();
		strcpy(mandir, fname.GetBuffer(0));
		_splitpath(mandir, pdrv, pdir, NULL, NULL);
		sprintf(mandir, "%s%s", pdrv, pdir);	// 最後の \\ までが格納される
		ThisApp.WriteProfileString((LPCSTR)"WinMan", (LPCSTR)"DataDir", (LPCSTR)mandir);

		// コンボボックスにファイル名を表示する
		DisplayListBox();
	}
	SendMessage(WM_PAINT);
}

void CMainDlgWindow::DisplayListBox(void)
{
	char buf[_MAX_PATH+_MAX_FNAME+_MAX_EXT+2];
	// コンボボックスにファイル名を表示する
	int fmax,i;
	char *dummybuf, fbuf[_MAX_FNAME+_MAX_EXT+2];
			// コンボボックス内のデータをすべて消去 
	fmax = FilenameCB.GetCount();	// 項目数 n 個の時は n が返る
	for(i=0; i<fmax; i++) FilenameCB.DeleteString(0);
		sprintf(buf, "%s*.TXT", mandir); // このバッファは変更されるかもしれない		
	FilenameCB.Dir(DDL_ARCHIVE, (LPCSTR)buf);	// 一致するファイル名のみが格納される
		// .TXT 部分を消去
	fmax = FilenameCB.GetCount();	// 項目数 n 個の時は n が返る
	for(i=0; i<fmax; i++)
	{
		FilenameCB.GetLBText(i, (LPSTR)buf);	// i個目のﾃﾞｰﾀを取り出す
		dummybuf = buf;
		strcpy(fbuf, strtok(dummybuf, "."));
		FilenameCB.DeleteString(i);
		FilenameCB.AddString((LPCSTR)fbuf);
	}
	if(FilenameCB.GetCount())
		FilenameCB.SelectString(0, (LPCSTR)"man");
	OnSelchangeCmbItem(); // 項目の表示

	return ;
}

void CMainDlgWindow::OnAbout()
{
	// TODO: この位置にコマンド ハンドラのコードを追加してください。
	CModalDialog aboutdlg(ABOUT_DLG_BOX, this);
	aboutdlg.DoModal();	
}

#include <malloc.h>

void CMainDlgWindow::OnDestroy()
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラのコードを追加してください。
	if(TextBufAllocated) _ffree(TextBuf);	
}

void CMainDlgWindow::OnSelchangeCmbItem()
{
	// TODO: この位置にコントロール通知ハンドラのコードを追加してください。
	if(!FilenameCB.GetCount()) return;
	// ファイルをオープンし、表示する。
	TRY{
		// ファイルオープンができなかったときは、エラーが throw される。それ以後の関数は実行されない。
		char buf[_MAX_PATH+_MAX_FNAME+_MAX_EXT+2];
		char fbuf[_MAX_FNAME+_MAX_EXT+2];
		FilenameCB.GetLBText(FilenameCB.GetCurSel(), (LPSTR)fbuf);	// ﾃﾞｰﾀを取り出す
		sprintf(buf, "%s%s.txt", mandir, fbuf);

		CStdioFile QRFFile((const char*)buf, CFile::modeRead|CFile::typeBinary); 
		UINT TextSize = (UINT)QRFFile.GetLength();
		// エディットボックスの制限
		if(TextSize > (UINT)0xffff/(UINT)2-1)
		{
			::MessageBox(this->m_hWnd, (LPCSTR)"ﾌｧｲﾙの大きさが32KBytesを超えていますので、\n32KBytes以上の部分は閲覧できません。",
					(LPCSTR)"注意", MB_OK|MB_ICONEXCLAMATION); 
			TextSize = (UINT)0xffff/(UINT)2-1;
		}
		if(TextBufAllocated) _ffree(TextBuf);
		TextBuf = (char far *)_fmalloc(TextSize+(UINT)10);		// OnDestroy で開放する
		TextBufAllocated = 1;
		if(TextBuf != (char far *)NULL)
		{
			QRFFile.Read((void far *)TextBuf, TextSize);
			*(TextBuf + TextSize) = (char)NULL; 
			SetDlgItemText(IDC_EDITMAIN, TextBuf);
			QRFFile.Close();
		}
	}
	CATCH(CFileException, e){
		// ファイルオープンに失敗したとき。
		CString HelpString;
		HelpString = "データファイル をオープンできません";
		SetDlgItemText(IDC_EDITMAIN, HelpString);
	}
	END_CATCH	
}

void CMainDlgWindow::OnClickedExitprog()
{
	// TODO: この位置にコントロール通知ハンドラのコードを追加してください。
	OnClose();	
}

void CMainDlgWindow::OnCopytext()
{
	// TODO: この位置にコマンド ハンドラのコードを追加してください。
	MainTextED.Copy();	
}

void CMainDlgWindow::OnSelectall()
{
	// TODO: この位置にコマンド ハンドラのコードを追加してください。
	MainTextED.SetSel(0, -1);	// ﾃｷｽﾄのすべてを選択		
}

void CMainDlgWindow::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラのコードを追加してください。
	MainTextED.SetSel(-1, 0);	// ﾃｷｽﾄ選択の解除		
}

void CMainDlgWindow::OnSettextcolor()
{
	// TODO: この位置にコマンド ハンドラのコードを追加してください。
	unsigned CH, CL;
	CColorDialog ColorDialog(ThisApp.TextColor);		
	if(ColorDialog.DoModal() == IDOK)
	{
		ThisApp.TextColor = ColorDialog.GetColor();
		CH = (UINT)((ThisApp.TextColor>>16)&0xffff);
		CL = (UINT)(ThisApp.TextColor & 0xffff);
		ThisApp.WriteProfileInt((LPCSTR)"WinMan", (LPCSTR)"ColorH", CH);	
		ThisApp.WriteProfileInt((LPCSTR)"WinMan", (LPCSTR)"ColorL", CL);	

	}
	Invalidate();
}
