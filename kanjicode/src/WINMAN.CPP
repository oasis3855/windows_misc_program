/*************************************************************************
 Windows �p MAN �u���E�U
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

	// �\���F�̎擾
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
	
	// TODO: ���̈ʒu�ɂ��̑��̏������p�R�[�h��ǉ����Ă��������B
	VERIFY(FilenameCB.SubclassDlgItem(IDC_CMB_ITEM, this));
	VERIFY(MainTextED.SubclassDlgItem(IDC_EDITMAIN, this));

	strcpy(mandir, ThisApp.GetProfileString((LPCSTR)"WinMan", (LPCSTR)"DataDir"));
	TextBufAllocated = 0;
	DisplayListBox();
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂��B
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
	static HBRUSH hBrush;	// ���[�U�[�J���[�ݒ�p

	CModalDialog::OnCtlColor(pDC, pWnd, nCtlColor);				// Call base class first

	hBrush = (HBRUSH)Ctl3dCtlColorEx(msg->message, msg->wParam, msg->lParam); // CTL3D ��÷�Ķװ���ύX����̂ł����ŌĂт����B
	if(pWnd->GetDlgCtrlID() == IDC_EDITMAIN)
	{
		pDC->SetTextColor(ThisApp.TextColor);
		pDC->SetBkMode(OPAQUE);
	}

	return hBrush;
}


void CMainDlgWindow::OnMenuSetup()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���̃R�[�h��ǉ����Ă��������B
	char buf[_MAX_PATH+_MAX_FNAME+_MAX_EXT+2];
	char pdrv[_MAX_DRIVE], pdir[_MAX_DIR]; 
	strcpy(buf, "");
	sprintf(buf, "%sSAMPLE.TXT", mandir);
	CFileDialog FileDialog(TRUE, NULL, (LPCSTR)buf,
		OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
		"�e�L�X�g�t�@�C��|*.TXT||", this);
	char bufTitle[] = "�\������÷��̧�ق̂����ިڸ�؂̑I��";
	FileDialog.m_ofn.lpstrTitle = (LPSTR)bufTitle;
	if(FileDialog.DoModal() == IDOK)
	{
		// �t�@�C�����̉��(�p�X�݂̂Ƃ���)
		CString fname;
		fname = FileDialog.GetPathName();
		strcpy(mandir, fname.GetBuffer(0));
		_splitpath(mandir, pdrv, pdir, NULL, NULL);
		sprintf(mandir, "%s%s", pdrv, pdir);	// �Ō�� \\ �܂ł��i�[�����
		ThisApp.WriteProfileString((LPCSTR)"WinMan", (LPCSTR)"DataDir", (LPCSTR)mandir);

		// �R���{�{�b�N�X�Ƀt�@�C������\������
		DisplayListBox();
	}
	SendMessage(WM_PAINT);
}

void CMainDlgWindow::DisplayListBox(void)
{
	char buf[_MAX_PATH+_MAX_FNAME+_MAX_EXT+2];
	// �R���{�{�b�N�X�Ƀt�@�C������\������
	int fmax,i;
	char *dummybuf, fbuf[_MAX_FNAME+_MAX_EXT+2];
			// �R���{�{�b�N�X���̃f�[�^�����ׂď��� 
	fmax = FilenameCB.GetCount();	// ���ڐ� n �̎��� n ���Ԃ�
	for(i=0; i<fmax; i++) FilenameCB.DeleteString(0);
		sprintf(buf, "%s*.TXT", mandir); // ���̃o�b�t�@�͕ύX����邩������Ȃ�		
	FilenameCB.Dir(DDL_ARCHIVE, (LPCSTR)buf);	// ��v����t�@�C�����݂̂��i�[�����
		// .TXT ����������
	fmax = FilenameCB.GetCount();	// ���ڐ� n �̎��� n ���Ԃ�
	for(i=0; i<fmax; i++)
	{
		FilenameCB.GetLBText(i, (LPSTR)buf);	// i�ڂ��ް������o��
		dummybuf = buf;
		strcpy(fbuf, strtok(dummybuf, "."));
		FilenameCB.DeleteString(i);
		FilenameCB.AddString((LPCSTR)fbuf);
	}
	if(FilenameCB.GetCount())
		FilenameCB.SelectString(0, (LPCSTR)"man");
	OnSelchangeCmbItem(); // ���ڂ̕\��

	return ;
}

void CMainDlgWindow::OnAbout()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���̃R�[�h��ǉ����Ă��������B
	CModalDialog aboutdlg(ABOUT_DLG_BOX, this);
	aboutdlg.DoModal();	
}

#include <malloc.h>

void CMainDlgWindow::OnDestroy()
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���̃R�[�h��ǉ����Ă��������B
	if(TextBufAllocated) _ffree(TextBuf);	
}

void CMainDlgWindow::OnSelchangeCmbItem()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���̃R�[�h��ǉ����Ă��������B
	if(!FilenameCB.GetCount()) return;
	// �t�@�C�����I�[�v�����A�\������B
	TRY{
		// �t�@�C���I�[�v�����ł��Ȃ������Ƃ��́A�G���[�� throw �����B����Ȍ�̊֐��͎��s����Ȃ��B
		char buf[_MAX_PATH+_MAX_FNAME+_MAX_EXT+2];
		char fbuf[_MAX_FNAME+_MAX_EXT+2];
		FilenameCB.GetLBText(FilenameCB.GetCurSel(), (LPSTR)fbuf);	// �ް������o��
		sprintf(buf, "%s%s.txt", mandir, fbuf);

		CStdioFile QRFFile((const char*)buf, CFile::modeRead|CFile::typeBinary); 
		UINT TextSize = (UINT)QRFFile.GetLength();
		// �G�f�B�b�g�{�b�N�X�̐���
		if(TextSize > (UINT)0xffff/(UINT)2-1)
		{
			::MessageBox(this->m_hWnd, (LPCSTR)"̧�ق̑傫����32KBytes�𒴂��Ă��܂��̂ŁA\n32KBytes�ȏ�̕����͉{���ł��܂���B",
					(LPCSTR)"����", MB_OK|MB_ICONEXCLAMATION); 
			TextSize = (UINT)0xffff/(UINT)2-1;
		}
		if(TextBufAllocated) _ffree(TextBuf);
		TextBuf = (char far *)_fmalloc(TextSize+(UINT)10);		// OnDestroy �ŊJ������
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
		// �t�@�C���I�[�v���Ɏ��s�����Ƃ��B
		CString HelpString;
		HelpString = "�f�[�^�t�@�C�� ���I�[�v���ł��܂���";
		SetDlgItemText(IDC_EDITMAIN, HelpString);
	}
	END_CATCH	
}

void CMainDlgWindow::OnClickedExitprog()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���̃R�[�h��ǉ����Ă��������B
	OnClose();	
}

void CMainDlgWindow::OnCopytext()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���̃R�[�h��ǉ����Ă��������B
	MainTextED.Copy();	
}

void CMainDlgWindow::OnSelectall()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���̃R�[�h��ǉ����Ă��������B
	MainTextED.SetSel(0, -1);	// ÷�Ă̂��ׂĂ�I��		
}

void CMainDlgWindow::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���̃R�[�h��ǉ����Ă��������B
	MainTextED.SetSel(-1, 0);	// ÷�đI���̉���		
}

void CMainDlgWindow::OnSettextcolor()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���̃R�[�h��ǉ����Ă��������B
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