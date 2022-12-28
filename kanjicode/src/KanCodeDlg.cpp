// KanCodeDlg.cpp : �C���v�������e�[�V���� �t�@�C��
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
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂�
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
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
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKanCodeDlg �_�C�A���O

CKanCodeDlg::CKanCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKanCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKanCodeDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKanCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKanCodeDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
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
// CKanCodeDlg ���b�Z�[�W �n���h��

BOOL CKanCodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
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
	
	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	VERIFY(EditInputBox.SubclassDlgItem(IDC_EDIT_INPUT, this));

	CHARFORMAT ch;
	EditInputBox.GetDefaultCharFormat(ch);
	ch.dwMask |= CFM_BOLD;
	ch.dwEffects |= CFE_BOLD;
	EditInputBox.SetDefaultCharFormat(ch);

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
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

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CKanCodeDlg::OnPaint() 
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
HCURSOR CKanCodeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CKanCodeDlg::OnCnv() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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
	// SJIS�R�[�h��\��
	sprintf((LPSTR)str, "%04X", sjis);
	SetDlgItemText(IDC_EDIT_SJIS, str);
	
	// Shift JIS ���� JIS �ւ̕ϊ�
	if(sjis >= 0xfa40)
	{	// �g���̈�(FA40H �` FC4BH) �̏���
		// SJIS �� xx7F,xxFD �` xx3F �����݂��Ȃ����̏���
		(sjis-0xfa00)%0x100 > 0x7f ? c = sjis-1 : c = sjis;
		c = c - 0xfa40 - (sjis-0xfa00)/0x100*(1+3+0x40);
		// JIS �� xx7F �` xx20 �̃X�L�b�v����
		jis = 0x9321 + (c/(0x7e - 0x21 + 1))*0x100 + c%(0x7e - 0x21 + 1);
	}
	else jis = _mbcjmstojis(sjis);

	sprintf((LPSTR)str, "%04X", jis);
	SetDlgItemText(IDC_EDIT_JIS, str);
	
	// ��_�R�[�h�̕\��
	if(sjis >= 0xfa40)
	{	// �g���̈�(FA40H �` FC4BH)�̏���
		kt = 11501 + (c/94)*100 + c%94;
	}
	else
	{
		// JIS �R�[�h�́u�X�y�[�X���� 0x2121�v����̑��Έʒu�����߂�
		// JIS �R�[�h�� 0x7F�`0x20(0xa2��)���g���Ă��Ȃ������l������
		c = jis - 0x2121 - ((jis>>8) -0x21)*0xa2;
		// ��_�R�[�h�� 95�`00���g�p����Ă��Ȃ�
		// 100�܂łŁA94�����g���Ă��Ȃ������l������
		kt = 101 + (c/94)*100 + c%94;
	}
	if(jis == 0) kt = 0;

	sprintf((LPSTR)str, "%05u", kt);
	SetDlgItemText(IDC_EDIT_KU, str);

}

void CKanCodeDlg::OnCclp() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	EditInputBox.Paste();
	OnCnv();
}
