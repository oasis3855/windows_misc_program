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
#define BITMAP_CX	5000
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
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	IsInfoWndDC_active = FALSE;
	IsMemDC_active = FALSE;

	// �E�C���h�E�T�C�Y���ύX���ꂽ�ꍇ�� TRUE
	bWndSizeChanged = FALSE;

}

void CInfoBar00Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoBar00Dlg)
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
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_HELP, OnMenuHelp)
	ON_WM_LBUTTONDBLCLK()
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

	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	//**************************
	// �t�H���g�̐ݒ�
	//**************************
	LOGFONT lf;
	this->GetFont()->GetLogFont(&lf);
	m_Font.CreateFontIndirect(&lf);

	m_Font.DeleteObject();
	//**************************
	// �E�C���h�E��Ԃ̐ݒ�
	//**************************

	// �^�X�N�o�[�ɕ\�����邩�ǂ����̐ݒ�
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	ex_style &= ~WS_EX_APPWINDOW;
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style);

	// �E�C���h�E�ʒu�̐ݒ�
	if(nInfoWndWidth != 0 && nInfoWndHeight != 0)
	{	// �E�C���h�E�T�C�Y�w�肪�L���Ȏ�
		if(bDispTopmost)
			SetWindowPos(&wndTopMost, nPosX, nPosY,nInfoWndWidth,nInfoWndHeight, SWP_SHOWWINDOW);
		else
			SetWindowPos(&wndNoTopMost, nPosX, nPosY,nInfoWndWidth,nInfoWndHeight, SWP_SHOWWINDOW);
	}
	else
	{
		if(bDispTopmost)
			SetWindowPos(&wndTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
		else
			SetWindowPos(&wndNoTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
	}

	//**************************
	// �f�o�C�X�R���e�L�X�g�̐����Ə�����
	//**************************

	//**** �`��G���A�� DC
	InfoWndDC = this->GetDC();		// �E�C���h�E���g�̂c�b�i�S�ʕ`��j
//	InfoWndDC = m_ctrl_info_area.GetDC();
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
//	m_ctrl_info_area.GetWindowRect(&InfoWndRect);
	this->GetWindowRect(&InfoWndRect);
	nInfoWndWidth = InfoWndRect.right - InfoWndRect.left;
	nInfoWndHeight = InfoWndRect.bottom - InfoWndRect.top;
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
	bDebugMode = FALSE;
	theApp->sTransBuf.LoadString(IDS_MES_INITIAL);		// InfoBar  :   ��ʏ���E�N���b�N���Đݒ肵�Ă�������\n
	strcpy(_sTransBuf, theApp->sTransBuf);


	//**************************
	// �����r�b�g�}�b�v�̕`��
	//**************************
	_sTmpStr1 = "initialize ...";
	DrawStringOnBmp(&_sTmpStr1);		// ���z�r�b�g�}�b�v�ɕ�����`��


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
	BOOL bStrNotSpace;

//	CNetAccessThread* pThread;
	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	if(bWndSizeChanged)
	{	// �E�C���h�E�T�C�Y���ύX���ꂽ�Ƃ�
		this->GetWindowRect(&InfoWndRect);
		nInfoWndWidth = InfoWndRect.right - InfoWndRect.left;
		nInfoWndHeight = InfoWndRect.bottom - InfoWndRect.top;
		nPosX = InfoWndRect.left;
		nPosY = InfoWndRect.top;
		if(InfoWndRect.bottom - InfoWndRect.top > BITMAP_CY)
		{
			// �E�C���h�E�ʒu�̐ݒ�
			if(bDispTopmost)
				SetWindowPos(&wndTopMost, nPosX, nPosY,nInfoWndWidth,BITMAP_CY, SWP_SHOWWINDOW);
			else
				SetWindowPos(&wndNoTopMost, nPosX, nPosY,nInfoWndWidth,BITMAP_CY, SWP_SHOWWINDOW);
		}
		bWndSizeChanged = FALSE;
	}

	//****************
	// �����A�b�v�f�[�g
	//****************
	if(tPrev != 0)
	{	// tPrev=0 �́A�N������\��
		time(&tNow);
		if(tNow - tPrev > tInterval)
		{	// �C���^�[�o�����Ԃ��߂��Ă���΁A�����A�b�v�f�[�g
			if(!bInThread)
			{
				// �X���b�h���Ŏg�p����O���[�o���ϐ���ݒ�
				CopyGlobalParam();
				
				bUpdated = FALSE;		// ���X�V
				bInThread = FALSE;		// �X���b�h���쒆
				if(nTimerID != 0) KillTimer(nTimerID);		// �^�C�}�[�̒�~
				if(_beginthread(thread_main,0,NULL)==-1)
				{
					theApp->sTransBuf.LoadString(IDS_ERR_THREAD);	// �V�X�e���E�G���[�F�X���b�h���N���ł��܂���
				}

				::Sleep(nTimer*3);								// �X���b�h�N���̂��߁A�E�G�C�g
				nTimerID = SetTimer(nTimerID, nTimer, NULL);	// �w��b����AON_TIMER �ɐ��䂪�ڂ�
			}
			else
			{
				// �X���b�h�̏d���N���͏o���Ȃ��̂ŁA�A�b�v�f�[�g�͂��Ȃ��B
				time(&tPrev);		// ���ݎ����̃A�b�v�f�[�g
			}

			time(&tPrev);
		}

		//****************
		// ���W�X�g�������A�b�v�f�[�g
		//****************
		// �\�����W���ς���Ă�����A���W�X�g���ɕۑ�
		RECT winRect;
		GetWindowRect(&winRect);
		if((int)nPosX != winRect.left || (int)nPosY != winRect.top)
		{
			nPosX = winRect.left;
			nPosY = winRect.top;
			// ���W�X�g���ɕۑ�
			CInfoBar00App *theApp;
			theApp = (CInfoBar00App *)AfxGetApp();
			theApp->RegConfigWrite(this);
		}

	}

	//****************
	// �`��
	//****************
	if(theApp->sTransBuf == "" || theApp->sTransBuf == "\n") theApp->sTransBuf.LoadString(IDS_MES_NODATA);	// �f�[�^����\n
	if(bUpdated)
	{
		theApp->sTransBuf = _sTransBuf;
		ScrCurPos = 0;
		_nTimCurArticle = 0;
		_sCurStr = theApp->sTransBuf.Mid(0, theApp->sTransBuf.Find("\n", 0));

		DrawStringOnBmp(&_sCurStr);		// ���z�r�b�g�}�b�v�ɕ�����`��

		bUpdated = FALSE;
	}
	if(ScrCurPos > (int)MemStrLength)
	{	// �P�s�A�b�v�f�[�g
		_nTimCurArticle++;
		for(i=0,k=0; k<_nTimCurArticle; k++)
		{
			i = theApp->sTransBuf.Find("\n", i) + 1;
			j = theApp->sTransBuf.Find("\n", i);
		}
		if(j<=0 || i<=0)
		{
			ScrCurPos = 0;
			_nTimCurArticle = 0;
			i=0;
		}
		_sCurStr = theApp->sTransBuf.Mid(i, theApp->sTransBuf.Find("\n", i)-i);

		// ���o���������񂪁A�󔒂�����A���s�����̏ꍇ�̏���
		bStrNotSpace = FALSE;
		for(i=0;i<_sCurStr.GetLength();i++)
		{
			if(_sCurStr.GetAt(i) != (char)' ' && _sCurStr.GetAt(i) != (char)NULL)
			{
				bStrNotSpace = TRUE;
				break;
			}
		}

		// �󔒂Ŗ����ꍇ�̂݁A�V�K�̕������`�悷��
		if(bStrNotSpace)
		{
			DrawStringOnBmp(&_sCurStr);		// ���z�r�b�g�}�b�v�ɕ�����`��
			ScrCurPos = 0;		// ���W���Z�b�g
		}
	}

	InfoWndDC->BitBlt(0, 0, nInfoWndWidth, nInfoWndHeight-0, &MemDC, ScrCurPos, 0, SRCCOPY);

	if(bInThread)
	{	// �X���b�h�����쒆
		InfoWndDC->FillSolidRect(5,1,10,3, 0x0000a0);
	}
//	else
//	{	// �X���b�h�͓��삵�Ă��Ȃ�
//		InfoWndDC->FillSolidRect(1,1,20,3, cBackColor);
//	}

	ScrCurPos += nMoveSpeed;		// �ړ�����h�b�g��
	
	CDialog::OnTimer(nIDEvent);
}

// **********************************
// ���z�r�b�g�}�b�v�ɁA�������`�悷��
// �܂��A�X�N���[���͈͌���̂��߂́A������̉������̃T�C�Y���Z�o
//
// **********************************
void CInfoBar00Dlg::DrawStringOnBmp(CString *sBuf)
{
	CFont fnt;		// �b��t�H���g�쐬�p


	if(nFontPoint == 0)
	{	// �t�H���g����`����Ă��Ȃ��Ƃ��́A�V�X�e���t�H���g���g�p
		MemDC.SelectStockObject(DEVICE_DEFAULT_FONT);	// �V�X�e���t�H���g������t��
	}
	else
	{
		fnt.CreatePointFont(nFontPoint, sFontName);
		MemDC.SelectObject(&fnt);
	}

	CalcMemStrLength(sBuf);		// ������̃r�b�g�}�b�v���̌v�Z

	MemDC.SelectObject(BrushInfoBack);
	MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, cBackColor);
	MemDC.SetBkColor(cBackColor);
	MemDC.SetTextColor(cForeColor);
	MemDC.TextOut(nInfoWndWidth,5,*sBuf);

	if(nFontPoint != 0)
	{
		MemDC.SelectStockObject(DEVICE_DEFAULT_FONT);	// �V�X�e���t�H���g������t��
		fnt.DeleteObject();
	}

	return ;
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
void CInfoBar00Dlg::CalcMemStrLength(CString *sBuf)
{
	int i,j,k, n_max, n_min;

	// �����p�ɕ������`��i�o�b�N�O���E���h�͍��B�����`��̈�͔��j
	MemDC.FillSolidRect(0,0,BITMAP_CX-1,BITMAP_CY-1, 0x000000);
	MemDC.SetBkColor(0xffffff);
	MemDC.SetTextColor(0xffffff);
	MemDC.TextOut(nInfoWndWidth,5,*sBuf);

	n_max = BITMAP_CX-2;		// �ő�
	n_min = nInfoWndWidth;		// �ŏ�
	for(i = (n_max+n_min)/2, j=n_min, k=n_max; ; )
	{
		if(MemDC.GetPixel(i,5) == 0)
		{	// �����łȂ� �i���փX�L�����j
			k = i;			// k �� i �̈ʒu�܂ňړ�
			i = (i+j)/2;	// i �� j �̒��Ԃ�
		}
		else
		{	// �����ł��� �i�E�փX�L�����j
			j = i;			// j �� i �̈ʒu�܂ňړ�
			i = (i+k)/2;	// i �� k �̒��Ԃ�
		}
		if(i <= j || i >= k) break;
	}

	MemStrLength = i;

/**	
	// �E�[���A���Ɍ������Č���
	// FillRect �� �E�Ɖ��̋��E����h��Ԃ��Ă��Ȃ����ߕ␳
	for(i=BITMAP_CX-2; i>=0; i--)
	{
		for(j=0; j<BITMAP_CY-2; j++)
		{
			if(MemDC.GetPixel(i,j) == 0xffffff) break;
		}
		if(MemDC.GetPixel(i,j) == 0xffffff) break;
	}

	MemStrLength = i;
	**/
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

	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	dlg_netconf.m_sURL = theApp->sURL;
	dlg_netconf.m_nPort = theApp->nPort;
	dlg_netconf.m_sPhHeader = theApp->sPhHeader;
	dlg_netconf.m_nPhSkip = theApp->nPhSkip;
	dlg_netconf.m_nPhGetcount = theApp->nPhGetcount;
	dlg_netconf.m_sProxy = theApp->sProxy;
	dlg_netconf.m_sTitle = theApp->sTitle;
	dlg_netconf.m_nMode = theApp->nMode;

	dlg_sysconf.m_nTimer = nTimer;
	dlg_sysconf.m_nSpeed = nMoveSpeed;
	dlg_sysconf.m_cBackColor = cBackColor;
	dlg_sysconf.m_cForeColor = cForeColor;
	dlg_sysconf.m_tInterval = tInterval;

	dlg_sysconf.m_bTopmost = bDispTopmost;

	dlg_sysconf.nFontPoint = nFontPoint;
	dlg_sysconf.sFontName = sFontName;

	// �v���p�e�B�[�y�[�W�̘A��
	DlgProp.AddPage(&dlg_netconf);
	DlgProp.AddPage(&dlg_sysconf);
	DlgProp.AddPage(&dlg_about);
	DlgProp.SetActivePage(0); // �Q�y�[�W�ڂ�\�ɂ���

	if(DlgProp.DoModal() == IDOK)
	{	// �ݒ�_�C�A���O�� OK �{�^���������ꂽ�Ƃ�
		theApp->sURL = dlg_netconf.m_sURL;
		theApp->nPort = dlg_netconf.m_nPort;
		theApp->sPhHeader = dlg_netconf.m_sPhHeader;
		theApp->nPhSkip = dlg_netconf.m_nPhSkip;
		theApp->nPhGetcount = dlg_netconf.m_nPhGetcount;
		theApp->sProxy = dlg_netconf.m_sProxy;
		theApp->sTitle = dlg_netconf.m_sTitle;
		theApp->nMode = dlg_netconf.m_nMode;

		nTimer = dlg_sysconf.m_nTimer;
		nMoveSpeed = dlg_sysconf.m_nSpeed;
		cBackColor = dlg_sysconf.m_cBackColor;
		cForeColor = dlg_sysconf.m_cForeColor;
		tInterval = dlg_sysconf.m_tInterval;

		bDispTopmost = dlg_sysconf.m_bTopmost;

		nFontPoint = dlg_sysconf.nFontPoint;
		sFontName = dlg_sysconf.sFontName;

		if(dlg_sysconf.m_chk_debug == TRUE)
		{
			// ���̂P�s�����Ȃ��ƁA���s���삵�Ă���X���b�h���ŋ}�Ƀf�o�b�O���[�h�ƂȂ�A�G���[�ƂȂ�
			if(bInThread)
				bDebugMode = FALSE;
			else
				bDebugMode = TRUE;
		}
		else
			bDebugMode = FALSE;

		if(!bInThread)
		{
			// �X���b�h���Ŏg�p����O���[�o���ϐ���ݒ�
			CopyGlobalParam();
			bUpdated = FALSE;		// ���X�V
			bInThread = FALSE;		// �X���b�h���쒆
			if(nTimerID != 0) KillTimer(nTimerID);		// �^�C�}�[�̈ꎞ��~
			if(_beginthread(thread_main,0,NULL)==-1)
			{
				theApp->sTransBuf.LoadString(IDS_ERR_THREAD);	// �V�X�e���E�G���[�F�X���b�h���N���ł��܂���
			}
			::Sleep(nTimer*3);							// �X���b�h�N���̂��߁A�E�G�C�g
			nTimerID = SetTimer(nTimerID, nTimer, NULL);	// �w��b����AON_TIMER �ɐ��䂪�ڂ�

			time(&tPrev);								// �X�V���Ԃ��A�b�v�f�[�g
		}
		else
		{
			// �X���b�h�̏d���N���͏o���Ȃ��̂ŁA���b�Z�[�W��\������
			::MessageBox(NULL, "Winsock Thread still working", "warning", MB_OK);
		}

		// �E�C���h�E�̈ʒu
		RECT winRect;
		GetWindowRect(&winRect);
		nPosX = winRect.left;
		nPosY = winRect.top;
		nInfoWndWidth = winRect.right - winRect.left;
		nInfoWndHeight = winRect.bottom - winRect.top;

		if(bDispTopmost)
			SetWindowPos(&wndTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
		else
			SetWindowPos(&wndNoTopMost, nPosX, nPosY,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);

		//****************
		// ���W�X�g�������A�b�v�f�[�g
		//****************
		// ���W�X�g���ɕۑ�
		CInfoBar00App *theApp;
		theApp = (CInfoBar00App *)AfxGetApp();
		theApp->RegConfigWrite(this);
	}

}

// **********************************
// ���j���[�́u�w���v�v��I�񂾂Ƃ��̏���
//
// **********************************
void CInfoBar00Dlg::OnMenuHelp() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	OnHelp();
}


//*********************
// ���j���[�́u�I���v��I�񂾂Ƃ��̏���
//
// ���j���[�̃N���X���A������ CDialog ����h�����Ă���
//*********************
void CInfoBar00Dlg::OnMenuExit() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

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

	// �^�C�}�[�̍폜
	if(nTimerID != 0) KillTimer(nTimerID);
	nTimerID = 0;

	// �E�C���h�E�̈ʒu��ۑ�
	RECT winRect;
	GetWindowRect(&winRect);
	nPosX = winRect.left;
	nPosY = winRect.top;
	nInfoWndWidth = winRect.right - winRect.left;
	nInfoWndHeight = winRect.bottom - winRect.top;

}


// **********************************
// �R�}���h�̃C���^�[�Z�v�g
//
// ESC�L�[��RETURN �L�[�̉����
//
// OnCommand(WPARAM wParam, LPARAM lParam) �𗘗p����ƁA�����I�����ɂ��܂������Ȃ�
//
// **********************************
BOOL CInfoBar00Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if( pMsg->message == WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{
		case VK_RETURN:  //  Enter�L�[����
			//  �Ǝ��̏���

			//  �f�t�H���g�̓�������������Ȃ�����TRUE��Ԃ�
			return TRUE;

		case VK_ESCAPE:  //  Esc�L�[����
			//  �Ǝ��̏���

			//  �f�t�H���g�̓�������������Ȃ�����TRUE��Ԃ�
			return TRUE;
		default:
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

// **********************************
// �_�C�A���O�̃T�C�Y���ύX���ꂽ�Ƃ�
//
// **********************************
void CInfoBar00Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	if(!bWndSizeChanged) bWndSizeChanged = TRUE;		// ���̕`��Ŕ��f�����邽��
}

// **********************************
// �_�C�A���O���_�u���N���b�N�����Ƃ�
//
// ���ݕ\�����Ă���j���[�X�̂t�q�k���u���E�U�ŊJ���܂��B
// **********************************
void CInfoBar00Dlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	CString sTmpURL;
	CInfoBar00App *theApp;
	theApp = (CInfoBar00App *)AfxGetApp();

	if(theApp->nMode == 0)
	{	// �ʏ탂�[�h�ȊO�ł́A�u���E�U�\���͍s��Ȃ�

		// �擪�� http:// ��t���鏈��
		if(theApp->sURL.Find("http://", 0) != 0)
			sTmpURL = "http://" + theApp->sURL;
		else
			sTmpURL = theApp->sURL;

		HINSTANCE hInst = ::ShellExecute(NULL, "open", sTmpURL, NULL, NULL, SW_SHOWNORMAL);
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}


// **********************************
// ���z�r�b�g�}�b�v��̉摜���A�k�b�c���ɂ���i�e�X�g�֐��j
//
// ���ۂɂ͎g�p���Ă��Ȃ��B
// **********************************
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
// �l�e�b�Ŏg���Ă��鏉���l�ނ��X���b�h�p�̃O���[�o���ϐ��ɃR�s�[����
//
// **********************************
void CInfoBar00Dlg::CopyGlobalParam()
{
	CInfoBar00App *theApp;

	theApp = (CInfoBar00App *)AfxGetApp();

	strcpy(_sTransBuf, theApp->sTransBuf);		// �X���b�h�ƃ_�C�A���O�̒ʐM�p
	strcpy(_sURL, theApp->sURL);				// �X���b�h�ɓn�� URL
	strcpy(_sProxy, theApp->sProxy);			// �X���b�h�ɓn�� PROXY
	strcpy(_sPhHeader, theApp->sPhHeader);		// �؂蕪���p �w�b�_�[������
	strcpy(_sPhHeaderB, theApp->sPhHeaderB);	// �؂蕪���p �w�b�_�[������ B
	strcpy(_sPhHeaderC, theApp->sPhHeaderC);	// �؂蕪���p �w�b�_�[������ C
	strcpy(_sTitle, theApp->sTitle);			// �^�C�g��
	strcpy(_sItems, theApp->sItems);			// �،��f�[�^�̎w��ȂǂɎg���A�C�e��

	_nMode = theApp->nMode;						// ���샂�[�h�i�ʏ��M�E�،���M�c�j
	_nPort = theApp->nPort;						// �X���b�h�ɓn�� �|�[�g�ԍ�
	_nPhSkip = theApp->nPhSkip;					// �؂蕪���p �X�L�b�v��
	_nPhGetcount = theApp->nPhGetcount;			// �؂蕪���p �擾��

}
