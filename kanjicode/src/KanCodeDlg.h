// KanCodeDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CKanCodeDlg dialog

class CKanCodeDlg : public CDialog
{
// �\�z
public:
//	CEdit EditInputBox;
	CRichEditCtrl EditInputBox;
	CKanCodeDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// Dialog Data
	//{{AFX_DATA(CKanCodeDlg)
	enum { IDD = IDD_KANCODE_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CKanCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CKanCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCnv();
	afx_msg void OnCclp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

