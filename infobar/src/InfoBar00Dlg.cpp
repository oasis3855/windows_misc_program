// InfoBar00Dlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "InfoBar00.h"
#include "InfoBar00Dlg.h"
#include "DlgNetConf.h"
#include "DlgSysConf.h"
#include "DlgAbout.h"
#include <process.h>

#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ���z�r�b�g�}�b�v�̃T�C�Y
#define BITMAP_CX	3000
#define BITMAP_CY	50

// �^�C�}�[ ID
#define TIMER_ID	153

/////////////////////////////////////////////////////////////////////////////
// CInfoBar00Dlg �_�C�A���O

//*********************
// �_�C�A���O�̏�����
//
// �c�b�֘A�̏������A�����l�̐ݒ�A�^�C�}�[�̋N��
//*********************
CInfoBar00Dlg::CInfoBar00Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoBar00Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoBar00Dlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	IsInfoWndDC_active = FALSE;
	IsMemDC_active = FALSE;

}

void CInfoBar00Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoBar00Dlg)
	DDX_Control(pDX, IDC_INFO_AREA, m_ctrl_info_area);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInfoBar00Dlg, CDialog)
	//{{AFX_MSG_MAP(CInfoBar00Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MENU_CONFIG, OnMenuConfig)
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoBar00Dlg ���b�Z�[�W �n���h��

BOOL CInfoBar00Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	CString _sTmpStr1, _sTmpStr2;

	//**************************
	// �f�o�C�X�R���e�L�X�g�̐����Ə�����
	//**************************

	//**** �`��G���A�� DC
//	InfoWndDC = this->GetDC();
	InfoWndDC = m_ctrl_info_area.GetDC();
	if(InfoWndDC == NULL)
	{
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// �A�v���P�[�V�������iInfoBar�j
		_sTmpStr2.LoadString(IDS_ERR_GETDC);		// �f�o�C�X�E�R���e�L�X�g�𗘗p�ł��܂���
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		// �E�C���h�E�� WM_CLOSE ���b�Z�[�W�𑗂�A�I��������
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}
	IsInfoWndDC_active = TRUE;	// ���̃t���O�ŁA�I������ DC ���������

	//**** �`��G���A�� RECT �T�C�Y
	m_ctrl_info_area.GetWindowRect(&InfoWndRect);
	nInfoWndWidth = InfoWndRect.right - InfoWndRect.left;
	//**** �o�b�N�O���E���h�̃u���V�i�h��Ԃ��F�j
	BrushInfoBack.CreateSolidBrush(cBackColor);
	//**** ���[�N�G���A�p DC
	if(!MemDC.CreateCompatibleDC(InfoWndDC))
	{	// ERROR
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// �A�v���P�[�V�������iInfoBar�j
		_sTmpStr2.LoadString(IDS_ERR_WNDDC);		// ���z�f�o�C�X�E�R���e�L�X�g�̐����Ɏ��s���܂���
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		// �E�C���h�E�� WM_CLOSE ���b�Z�[�W�𑗂�A�I��������
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}
	if(!MemBmp.CreateCompatibleBitmap(InfoWndDC,BITMAP_CX,BITMAP_CY))
	{	// ERROR
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// �A�v���P�[�V�������iInfoBar�j
		_sTmpStr2.LoadString(IDS_ERR_COMPBMP);		// ���z�r�b�g�}�b�v�̐����Ɏ��s���܂���
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		MemDC.DeleteDC();
		// �E�C���h�E�� WM_CLOSE ���b�Z�[�W�𑗂�A�I��������
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}
	if(!MemDC.SelectObject(MemBmp))
	{
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// �A�v���P�[�V�������iInfoBar�j
		_sTmpStr2.LoadString(IDS_ERR_BMPTODC);		// ���z�r�b�g�}�b�v���f�o�C�X�E�R���e�L�X�g�Ƀ����N�ł��܂���
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		MemBmp.DeleteObject();
		MemDC.DeleteDC();
		// �E�C���h�E�� WM_CLOSE ���b�Z�[�W�𑗂�A�I��������
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}
	IsMemDC_active = TRUE;	// ���̃t���O�ŁA�I������ MemDC,MemBmp ���������


	//**************************
	// �����l�̐ݒ�
	//**************************

	ScrCurPos = 0;			// �X�N���[���ʒu
	_nTimCurArticle = 0;	// �\���A�C�e��

	bUpdated = TRUE;		// ���X�V
	bInThread = FALSE;		// �X���b�h���쒆
	sTransBuf.LoadString(IDS_MES_INITIAL);		// InfoBar  :   ��ʏ���E�N���b�N���Đݒ肵�Ă�������\n

	//**************************
	// �E�C���h�E��Ԃ̐ݒ�
	//**************************

	// �^�X�N�o�[�ɕ\�����邩�ǂ����̐ݒ�
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	ex_style &= ~WS_EX_APPWINDOW;
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style);

	// �E�C���h�E�ʒu�̐ݒ�
	if(bDispTopmost)
		SetWindowPos(&wndTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
	else
		SetWindowPos(&wndNoTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);

	// �����r�b�g�}�b�v�̕`��
	MemDC.SelectObject(BrushInfoBack);
	MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, cBackColor);
	MemDC.SetBkColor(cBackColor);
	MemDC.SetTextColor(cForeColor);
	MemDC.TextOut(nInfoWndWidth,5,"initialize ...");
	CalcMemStrLength();


	//**************************
	// �^�C�}�[�̐ݒ�
	//**************************

	nTimerID = SetTimer(TIMER_ID, nTimer, NULL);	// 1000msec ��� ON_TIMER �ɐ��䂪�ڂ�

	if(!nTimerID)
	{
		_sTmpStr1.LoadString(AFX_IDS_APP_TITLE);	// �A�v���P�[�V�������iInfoBar�j
		_sTmpStr2.LoadString(IDS_ERR_TIMER);		// �V�X�e���E�^�C�}�[�𗘗p�ł��܂���
		MessageBox(_sTmpStr2, _sTmpStr1, MB_OK|MB_ICONSTOP);
		// �E�C���h�E�� WM_CLOSE ���b�Z�[�W�𑗂�A�I��������
		PostMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}

	// ESC�ŏI���������Ȃ����߂̃t���O
	bExitSeq = FALSE;

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CInfoBar00Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CInfoBar00Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



//*********************
// �^�C�}�[���荞��
//
// ��ʂ����t���b�V������
//
//*********************
void CInfoBar00Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	CString _sCurStr;
	int i,j,k;
	time_t tNow;

	//****************
	// �����A�b�v�f�[�g
	//****************
	if(tPrev != 0)
	{	// tPrev=0 �́A�N������\��
		time(&tNow);
		if(tNow - tPrev > tInterval)
		{	// �C���^�[�o�����Ԃ��߂��Ă���΁A�����A�b�v�f�[�g
			bUpdated = FALSE;		// ���X�V
			bInThread = FALSE;		// �X���b�h���쒆
			KillTimer(nTimerID);
			if(_beginthread(thread_main,0,NULL)==-1)
			{
				sTransBuf.LoadString(IDS_ERR_THREAD);	// �V�X�e���E�G���[�F�X���b�h���N���ł��܂���
			}

			::Sleep(nTimer*3);
			nTimerID = SetTimer(nTimerID, nTimer, NULL);	// �w��b����AON_TIMER �ɐ��䂪�ڂ�

			time(&tPrev);
		}
	}

	//****************
	// �`��
	//****************
	if(sTransBuf == "" || sTransBuf == "\n") sTransBuf.LoadString(IDS_MES_NODATA);	// �f�[�^����\n
	if(bUpdated)
	{
		ScrCurPos = 0;
		_nTimCurArticle = 0;
		_sCurStr = sTransBuf.Mid(0, sTransBuf.Find("\n", 0));
		MemDC.SelectObject(BrushInfoBack);
		MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, cBackColor);
		MemDC.SetBkColor(cBackColor);
		MemDC.SetTextColor(cForeColor);
		MemDC.TextOut(nInfoWndWidth,5,_sCurStr);
		CalcMemStrLength();
		bUpdated = FALSE;
	}
	if(ScrCurPos > (int)MemStrLength)
	{	// �P�s�A�b�v�f�[�g
		ScrCurPos = 0;
		_nTimCurArticle++;
		for(i=0,k=0; k<_nTimCurArticle; k++)
		{
			i = sTransBuf.Find("\n", i) + 1;
			j = sTransBuf.Find("\n", i);
		}
		if(j<=0 || i<=0)
		{
			ScrCurPos = 0;
			_nTimCurArticle = 0;
			i=0;
		}
		_sCurStr = sTransBuf.Mid(i, sTransBuf.Find("\n", i)-i);
		MemDC.SelectObject(BrushInfoBack);
		MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, cBackColor);
		MemDC.SetBkColor(cBackColor);
		MemDC.SetTextColor(cForeColor);
		MemDC.TextOut(InfoWndRect.right,5,_sCurStr);
		CalcMemStrLength();

	}

	InfoWndDC->BitBlt(0,0,nInfoWndWidth,InfoWndRect.bottom-InfoWndRect.top,
						&MemDC,ScrCurPos,0,SRCCOPY);

	ScrCurPos += nMoveSpeed;		// �ړ�����h�b�g��
	
	CDialog::OnTimer(nIDEvent);
}



//*********************
// MemDC �ɏ������܂�Ă��镶���i�摜�j�̒����i�s�N�Z���j��Ԃ�
//
// +---------------------+-------------+
// |           ������ł�|             |
// +---------------------+-------------+
// 0                     ��            BITMAP_CX
//                  ���̍��W��Ԃ�
// 
//*********************
void CInfoBar00Dlg::CalcMemStrLength()
{
	int i,j;
	// �E�[���A���Ɍ������Č���
	// FillRect �� �E�Ɖ��̋��E����h��Ԃ��Ă��Ȃ����ߕ␳
	for(i=BITMAP_CX-2; i>=0; i--)
	{
		for(j=0; j<BITMAP_CY-2; j++)
		{
			if(MemDC.GetPixel(i,j) == cForeColor) break;
		}
		if(MemDC.GetPixel(i,j) == cForeColor) break;
	}

	MemStrLength = i;
}

//*********************
// �E�N���b�N�ɑ΂���R���e�L�X�g���j���[�\��
// 
//*********************
void CInfoBar00Dlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	CMenu menu;
	CPoint point_cur;
	::GetCursorPos(&point_cur);
	VERIFY(menu.LoadMenu(IDR_MENU_POPUP));
	CMenu *pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	SetForegroundWindow();
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point_cur.x, point_cur.y, this);
	SetForegroundWindow();
	PostMessage(WM_NULL, 0, 0);		// Win95/98 �o�O�΍�
	
	pPopup->DestroyMenu();
	
	
	CDialog::OnRButtonUp(nFlags, point);
}

//*********************
// ���N���b�N�ɑ΂��鏈��
//
// �_�C�A���O���ړ����郁�b�Z�[�W
// ���{�^�������m���A�E�C���h�E�ɑ΂��ړ����b�Z�[�W�𑗂�
//*********************
void CInfoBar00Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialog::OnLButtonDown(nFlags, point);
}

//*********************
// ���j���[�́u�ݒ�v��I�񂾂Ƃ��̏���
//
// ���j���[�̃N���X���A������ CDialog ����h�����Ă���
//*********************
void CInfoBar00Dlg::OnMenuConfig() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CPropertySheet DlgProp(IDS_CONFDLG_TITLE);	// ���\�[�X����^�C�g����������w�肷��

	CDlgNetConf dlg_netconf;
	CDlgSysConf dlg_sysconf;
	CDlgAbout dlg_about;

	dlg_netconf.m_sURL = sURL;
	dlg_netconf.m_nPort = nPort;
	dlg_netconf.m_sPhHeader = sPhHeader;
	dlg_netconf.m_nPhSkip = nPhSkip;
	dlg_netconf.m_nPhGetcount = nPhGetcount;
	dlg_netconf.m_sProxy = sProxy;
	dlg_netconf.m_sTitle = sTitle;

	dlg_sysconf.m_nTimer = nTimer;
	dlg_sysconf.m_nSpeed = nMoveSpeed;
	dlg_sysconf.m_cBackColor = cBackColor;
	dlg_sysconf.m_cForeColor = cForeColor;
	dlg_sysconf.m_tInterval = tInterval;

	dlg_sysconf.m_bTopmost = bDispTopmost;

	// �v���p�e�B�[�y�[�W�̘A��
	DlgProp.AddPage(&dlg_netconf);
	DlgProp.AddPage(&dlg_sysconf);
	DlgProp.AddPage(&dlg_about);
	DlgProp.SetActivePage(0); // �Q�y�[�W�ڂ�\�ɂ���

	if(DlgProp.DoModal() == IDOK)
	{
		sURL = dlg_netconf.m_sURL;
		nPort = dlg_netconf.m_nPort;
		sPhHeader = dlg_netconf.m_sPhHeader;
		nPhSkip = dlg_netconf.m_nPhSkip;
		nPhGetcount = dlg_netconf.m_nPhGetcount;
		sProxy = dlg_netconf.m_sProxy;
		sTitle = dlg_netconf.m_sTitle;

		nTimer = dlg_sysconf.m_nTimer;
		nMoveSpeed = dlg_sysconf.m_nSpeed;
		cBackColor = dlg_sysconf.m_cBackColor;
		cForeColor = dlg_sysconf.m_cForeColor;
		tInterval = dlg_sysconf.m_tInterval;

		bDispTopmost = dlg_sysconf.m_bTopmost;


		bUpdated = FALSE;		// ���X�V
		bInThread = FALSE;		// �X���b�h���쒆
		KillTimer(nTimerID);
		if(_beginthread(thread_main,0,NULL)==-1)
		{
			sTransBuf.LoadString(IDS_ERR_THREAD);	// �V�X�e���E�G���[�F�X���b�h���N���ł��܂���
		}

		::Sleep(nTimer*3);
		nTimerID = SetTimer(nTimerID, nTimer, NULL);	// �w��b����AON_TIMER �ɐ��䂪�ڂ�

		time(&tPrev);

		// �E�C���h�E�̈ʒu
		RECT winRect;
		GetWindowRect(&winRect);
		nPosX = winRect.left;
		nPosY = winRect.top;

		if(bDispTopmost)
			SetWindowPos(&wndTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
		else
			SetWindowPos(&wndNoTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);

	}

}

//*********************
// ��A�̏I�����̏���
//
//*********************
void CInfoBar00Dlg::EndDialogSeq()
{
	if(IsMemDC_active)
	{
		MemBmp.DeleteObject();
		MemDC.DeleteDC();
	}
	if(IsInfoWndDC_active)
		ReleaseDC(InfoWndDC);	// GetDC �̉��

	// WM_DESTROY �� �d��������h�~���邽�߂̑[�u
	IsInfoWndDC_active = FALSE;
	IsMemDC_active = FALSE;

	KillTimer(nTimerID);

	// �E�C���h�E�̈ʒu��ۑ�
	RECT winRect;
	GetWindowRect(&winRect);
	nPosX = winRect.left;
	nPosY = winRect.top;

}


//*********************
// ���j���[�́u�I���v��I�񂾂Ƃ��̏���
//
// ���j���[�̃N���X���A������ CDialog ����h�����Ă���
//*********************
void CInfoBar00Dlg::OnMenuExit() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	bExitSeq = TRUE;

	// �E�C���h�E�� WM_CLOSE ���b�Z�[�W�𑗂�A�I��������
	PostMessage(WM_CLOSE, 0, 0);
	
}

//*********************
// �E�C���h�E�I������
//
// ���z�X�N���[���ƕ`��DC�̉���B�^�C�}�[�̒�~
// WM_CLOSE -> WM_DESTROY �̏��ɌĂ΂��
//*********************
void CInfoBar00Dlg::OnClose() 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	EndDialogSeq();		// �I�����̈�A�̏���

	CDialog::OnClose();
}

//*********************
// �E�C���h�E�I������
//
// ���z�X�N���[���ƕ`��DC�̉���B�^�C�}�[�̒�~
//*********************
void CInfoBar00Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	EndDialogSeq();		// �I�����̈�A�̏���
}

#define DOT_PITCH 3

void CInfoBar00Dlg::MakeLCD()
{
	int i,j,k,m;
	BOOL flag;
	// �E�[���A���Ɍ������Č���
	// FillRect �� �E�Ɖ��̋��E����h��Ԃ��Ă��Ȃ����ߕ␳
	for(i=0; i<(int)MemStrLength+2; i+=DOT_PITCH)
	{
		for(j=0; j<BITMAP_CY-2; j+=DOT_PITCH)
		{
			flag = FALSE;
			for(k=0; k<DOT_PITCH; k++)
			{
				for(m=0; m<DOT_PITCH; m++)
				{
					if(MemDC.GetPixel(i+k,j+m) == cForeColor)
					{
						flag = TRUE;
						break;
					}
				}
			}
			if(flag)
			{
				for(k=0; k<DOT_PITCH; k++)
				{
					for(m=0; m<DOT_PITCH; m++)
					{
						if(k == DOT_PITCH-1 || m == DOT_PITCH-1)
							MemDC.SetPixel(i+k,j+m, cBackColor);
						else
							MemDC.SetPixel(i+k,j+m, cForeColor);
					}
				}
			}
		}
		if(MemDC.GetPixel(i,j) == cForeColor) break;
	}

	MemStrLength = i;

}

// **********************************
// �R�}���h�̃C���^�[�Z�v�g
//
// ESC�L�[��RETURN �L�[�̉����
// **********************************

BOOL CInfoBar00Dlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if(wParam == WM_DESTROY && !bExitSeq) return 1;	// "�����߷�"�͏������Ȃ��B	
	if(wParam == WM_CREATE) return 1;	// "���ݷ�"�͏������Ȃ��B	
	
	return CDialog::OnCommand(wParam, lParam);
}

