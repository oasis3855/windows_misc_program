// PropertySheet.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CPropAbout �_�C�A���O

class CPropAbout : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropAbout)

// �R���X�g���N�V����
public:
	BOOL m_uninstall;
	CPropAbout();
	~CPropAbout();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPropAbout)
	enum { IDD = IDD_CNF_ABOUT };
	CString	m_ddx_odometer;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CPropAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPropAbout)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CPropConf �_�C�A���O

class CPropConf : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropConf)

// �R���X�g���N�V����
public:
	COLORREF m_TextColor;
	COLORREF m_DlgColor;
	COLORREF m_DlgBackColor;
	CPropConf();
	~CPropConf();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPropConf)
	enum { IDD = IDD_CNF_CONFIG };
	CButton	m_pickdlgcol;
	CButton	m_pickdlgbackcol;
	CButton	m_pickcolor;
	CListBox	m_hotkey2;
	CListBox	m_hotkey1;
	BOOL	m_ddx_assist;
	CString	m_ddx_title;
	int		m_ddx_x;
	int		m_ddx_y;
	int		m_ddx_timertick;
	int		m_ddx_hotkey1;
	int		m_ddx_hotkey2;
	BOOL	m_ddx_changecolor;
	BOOL	m_ddx_isNotTaskbar;
	BOOL	m_ddx_isTopmost;
	BOOL	m_ddx_nChangeDlgColor;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CPropConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPropConf)
	virtual BOOL OnInitDialog();
	afx_msg void OnAssist();
	afx_msg void OnPickcolor();
	afx_msg void OnChangecolor();
	afx_msg void OnPickdlgbackcol();
	afx_msg void OnPickdlgcol();
	afx_msg void OnChangedlgcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
