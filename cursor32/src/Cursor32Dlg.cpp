// Cursor32Dlg.cpp : �C���v�������e�[�V���� �t�@�C��
// �_�C�A���O�x�[�X�A�v���P�[�V�����p�̃��C���_�C�A���O

#include "stdafx.h"
#include "Cursor32.h"
//#include "Cursor32Dlg.h"
#include "PropertySheet.h"
#include<math.h>	// �ړ������̐ώZ�ɕ��������_���g���Ă��� 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCursor32Dlg �_�C�A���O  �A�v���P�[�V�����D�E�C���h�E

CCursor32Dlg::CCursor32Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCursor32Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCursor32Dlg)
 		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
 	//}}AFX_DATA_INIT
 	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
}

// DDX�͑����g�p����Ȃ�(�����Ҳݳ���޳������)
void CCursor32Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCursor32Dlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCursor32Dlg, CDialog)
	//{{AFX_MSG_MAP(CCursor32Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_APP_ABOUT, OnAppAbout)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCursor32Dlg ���b�Z�[�W �n���h��

// ������
BOOL CCursor32Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B

	// �^�C�}�[�̐ݒ�
	TimerOK = 1;
	if(!SetTimer(USER_TIMER_ID, timertick, NULL))
	{
		TimerOK = 0;
		AfxMessageBox("�^�C�}�[�̎g�p���ł��Ȃ����߁A���̃\�t�g�͎g�p�ł��܂���B",
						MB_OK|MB_ICONSTOP|MB_APPLMODAL);
		CloseDialog();		// �_�C�A���O�̏I��	
	}

	// �ړ������̃��Z�b�g
	Counter=0L;

	// �\���ʒu�A�^�C�g���A���̑��̌������̐ݒ�
	UpdateAppearance();

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �V�X�e���R�}���h(�{�^��)�̏���
void CCursor32Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID == SC_CLOSE) CloseDialog();
	else CDialog::OnSysCommand(nID, lParam);
}

// �_�C�A���O��`�悷��
void CCursor32Dlg::OnPaint() 
{
	// �\�����C�����[�`��
	CDialog::OnPaint();
	POINT ppt;
	// �}�E�X�J�[�\���̌��݈ʒu���擾
	::GetCursorPos((POINT far *)&ppt);
	// �_�C�A���O�ɍ��W��\��
	SetDlgItemInt(IDC_X, ppt.x);
	SetDlgItemInt(IDC_Y, ppt.y);
}

// �I�����̌��ݍ��W��ۑ�����
void CCursor32Dlg::CloseDialog() 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	if(TimerOK) KillTimer(USER_TIMER_ID);
	// �ŏI�̃E�C���h�E�ʒu�̎擾(CWinApp�Ń��W�X�g���ɏ������ނ���)
	RECT winRect;
	GetWindowRect(&winRect);
	x_init = winRect.left;
	y_init = winRect.top;
	EndDialog(IDOK);
}

void CCursor32Dlg::OnClose() 
{
	// ���̱��ع���݂ł�OnClose() ���Ă΂�邱�Ƃ͒ʏ햳�����A���S�̂���
	// �I���R�}���h���Ă�ł����B
	CloseDialog();
}

// �^�C�}�[����Ă΂��
// ���W�̃Z�b�g�Ȃ�
void CCursor32Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	// X,Y ���W�̃Z�b�g
	POINT ppt;
	static int lastX=-1, lastY=-1;
	::GetCursorPos((POINT far *)&ppt);	// �}�E�X�̌��݈ʒu
	if(ppt.x != lastX || ppt.y != lastY)
	{	// �ʒu���ς���Ă����ꍇ�̂݁A�\���X�V
		::GetCursorPos((POINT far *)&ppt);
		SetDlgItemInt(IDC_X, ppt.x);
		SetDlgItemInt(IDC_Y, ppt.y);
		if(lastX>=0)
		{	// �ŏ���1��͑��s�������v�Z���Ȃ�
			// ���s�����̐ώZ
			double tmpx=ppt.x-lastX, tmpy=ppt.y-lastY;
			Counter += (unsigned long)(sqrt(tmpx*tmpx + tmpy*tmpy)*10);
		}
		// ��O���W�̃o�b�t�@�ɑ��
		lastX = ppt.x;
		lastY = ppt.y;
	}

	// �J�[�\���A�V�X�^���X
	static int newkey=1,lockY=0, lockX=0;
	if(IsAssist)
	{
		if((::GetAsyncKeyState(hotkey1) < 0) &&
				(::GetAsyncKeyState(hotkey2) < 0))
		{	// �����`���[�h
			if(newkey)
			{
				lockY=ppt.y; lockX=ppt.x;
			}
			newkey=0;
			if(ppt.x>=lockX)
			{
				if(ppt.y>lockY) ::SetCursorPos(ppt.x,abs(lockY+ppt.x-lockX));
				else ::SetCursorPos(ppt.x,abs(lockY-ppt.x+lockX));
			}
			else
			{
				if(ppt.y<lockY) ::SetCursorPos(ppt.x,abs(lockY+ppt.x-lockX));
				else ::SetCursorPos(ppt.x,abs(lockY-ppt.x+lockX));
			}
		}
		else if(::GetAsyncKeyState(hotkey1) < 0)
		{	// �������[�h
			if(newkey) lockY=ppt.y;
			newkey=0;
			::SetCursorPos(ppt.x,lockY);
		}
		else if(::GetAsyncKeyState(hotkey2) < 0)
		{	// �������[�h
			if(newkey) lockX=ppt.x;
			newkey=0;
			::SetCursorPos(lockX,ppt.y);
		}
		else newkey=1;
	}
	CDialog::OnTimer(nIDEvent);
}

// �@�\�ݒ� �_�C�A���O�̌Ăяo���B
void CCursor32Dlg::OnAppAbout() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UINT keycode[] = {VK_SHIFT, VK_CONTROL, VK_MENU, VK_INSERT, VK_DELETE,
			VK_BACK, VK_TAB, VK_F9, VK_F10, VK_F11, VK_F12};
	HICON h_infoicon, h_conficon;

	// �v���p�e�B�[�_�C�A���O�{��
	CPropertySheet DlgProp(IDS_PROPTITLE);	// ���\�[�X����^�C�g����������w�肷��
	// �v���p�e�B�V�[�g�̊e�y�[�W�����\�[�X����\�z����
	CPropAbout AboutPage;
	CPropConf ConfPage;
	// �v���p�e�B�V�[�g�̊e�l��ݒ�
	ConfPage.m_ddx_assist = IsAssist;
	ConfPage.m_ddx_title = title;
	ConfPage.m_ddx_timertick = timertick;
	ConfPage.m_ddx_changecolor = nChangeColor;
	ConfPage.m_TextColor = TextColor;
	ConfPage.m_ddx_nChangeDlgColor = nChangeDlgColor;
	ConfPage.m_DlgBackColor = DlgBackColor;
	ConfPage.m_DlgColor = DlgColor;
	ConfPage.m_ddx_isNotTaskbar = isNotTaskbar;
	ConfPage.m_ddx_isTopmost = isTopmost;
	AboutPage.m_ddx_odometer.Format("%ld",Counter/10);
	// �z�b�g�L�[��ݒ�
	// ���̐ݒ�l�𗘗p���āAON_INITDIALOG �� SetCurSel���g�p����
	int i;
	for(i=0; i<11; i++)
	{
		if(keycode[i] == hotkey1) ConfPage.m_ddx_hotkey1 = i;
		if(keycode[i] == hotkey2) ConfPage.m_ddx_hotkey2 = i;
	}
	// ���ݍ��W�̎擾�Ɛݒ�
	RECT winRect;
	GetWindowRect(&winRect);
	x_init = winRect.left;
	y_init = winRect.top;
	ConfPage.m_ddx_x = x_init;
	ConfPage.m_ddx_y = y_init;

	// �v���p�e�B�V�[�g�E�^�C�g���̃A�C�R���ݒ�
	h_infoicon = AfxGetApp()->LoadIcon(IDI_INFOICON);
	h_conficon = AfxGetApp()->LoadIcon(IDI_CONFICON);
	AboutPage.m_psp.hIcon = h_infoicon;
	ConfPage.m_psp.hIcon = h_conficon;
	AboutPage.m_psp.dwFlags |= PSP_USEHICON;
	ConfPage.m_psp.dwFlags |= PSP_USEHICON;
	// �v���p�e�B�[�y�[�W�̘A��
	DlgProp.AddPage(&AboutPage);
	DlgProp.AddPage(&ConfPage);
	// �v���p�e�B�[�V�[�g�̑����ύX
	DlgProp.m_psh.dwFlags=DlgProp.m_psh.dwFlags|PSH_NOAPPLYNOW;
	DlgProp.SetActivePage(1); // �Q�y�[�W�ڂ�\�ɂ���
	// �@�\�ݒ�_�C�A���O��\������
	if (DlgProp.DoModal() == IDOK && !AboutPage.m_uninstall)
	{	// OK �{�^���������ꂽ��
		timertick = ConfPage.m_ddx_timertick;
		title = ConfPage.m_ddx_title;
		IsAssist = ConfPage.m_ddx_assist;
		hotkey1 = keycode[ConfPage.m_ddx_hotkey1];
		hotkey2 = keycode[ConfPage.m_ddx_hotkey2];
		if(hotkey1 == hotkey2)
		{
			AfxMessageBox("�Q�̃z�b�g�L�[�͈قȂ�L�[�łȂ���΂����܂���B\n�W���ݒ�ɂ��ǂ��܂����B",
						MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
			hotkey1 = VK_CONTROL;
			hotkey2 = VK_SHIFT;
		}
		nChangeColor = ConfPage.m_ddx_changecolor;
		TextColor = ConfPage.m_TextColor;
		nChangeDlgColor = ConfPage.m_ddx_nChangeDlgColor;
		DlgBackColor = ConfPage.m_DlgBackColor;
		DlgColor = ConfPage.m_DlgColor;
		isNotTaskbar = ConfPage.m_ddx_isNotTaskbar;
		isTopmost = ConfPage.m_ddx_isTopmost;

		UpdateAppearance();
	}
	if(::DestroyIcon(h_infoicon)!=TRUE) MessageBox("csr32 �����G���[ ico");
	if(::DestroyIcon(h_conficon)!=TRUE) MessageBox("csr32 �����G���[ ico");
	// �A���C���X�g�[�����������ꂽ��̏���
	if(AboutPage.m_uninstall)
	{
		if(TimerOK) KillTimer(USER_TIMER_ID);
		AfxMessageBox("Cursor32�i�}�E�X���W�\���j�̃��W�X�g�����͍폜����܂����B",
						MB_OK|MB_ICONINFORMATION|MB_APPLMODAL);
		EndDialog(IDOK);
	}
}

// �R�}���h�̃C���^�[�Z�v�g
BOOL CCursor32Dlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if(wParam == WM_DESTROY) return 1;	// "�����߷�"�͏������Ȃ��B	
	if(wParam == WM_CREATE) return 1;	// "���ݷ�"�͏������Ȃ��B	
	return CDialog::OnCommand(wParam, lParam);
}

// XY���W�̃J���[�ύX
HBRUSH CCursor32Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
	if(nChangeColor)
	{
		if(nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_MSGBOX)
		{
			// X,Y�\���̃e�L�X�g�̎��̂ݕ\���F�ύX
			if(pWnd->GetDlgCtrlID()== IDC_X) pDC->SetTextColor(TextColor);
			if(pWnd->GetDlgCtrlID()== IDC_Y) pDC->SetTextColor(TextColor);
		}
	}
	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������
	return hbr;	// �o�b�N�O���E���h�J���[�͕W���̂��̂��g�p
}

// �_�C�A���O�̌�������ύX����
void CCursor32Dlg::UpdateAppearance()
{
	// �ύX���_�C�A���O�ɔ��f������
	SetWindowText(title);
	InvalidateRect(NULL);

	// �E�C���h�E�ʒu�̐ݒ�
	RECT wRect;
	// ��ʊO�̂Ƃ��͌��_�ɕ\�����鏈��
	GetClientRect(&wRect);
	if(GetSystemMetrics(SM_CXFULLSCREEN) < x_init + wRect.right-wRect.left - 20
			|| x_init<0) x_init=0;
	if(GetSystemMetrics(SM_CYFULLSCREEN) < y_init  + wRect.bottom-wRect.top
			|| y_init<0) y_init=0;
	// �E�C���h�E�ʒu�̐ݒ�@���@��Ɏ�O�ɕ\�����ǂ����̐ݒ�
	if(isTopmost)
		SetWindowPos(&wndTopMost, x_init,y_init,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);	
	else
	{
		SetWindowPos(&wndBottom, x_init,y_init,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
		SetWindowPos(&wndTop, x_init,y_init,0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
	}

	// �^�X�N�o�[�ɕ\�����邩�ǂ����̐ݒ�
	LONG ex_style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if(isNotTaskbar) ex_style &= ~WS_EX_APPWINDOW;
	else ex_style |= WS_EX_APPWINDOW;
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style);
	ShowWindow(SW_HIDE);	// ��\���A�\����؂�ւ���
	ShowWindow(SW_SHOW);
}
