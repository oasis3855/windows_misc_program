#if !defined(AFX_INSTDLG_H__56D0E476_4F9A_493C_84AF_DB6520ACD0BF__INCLUDED_)
#define AFX_INSTDLG_H__56D0E476_4F9A_493C_84AF_DB6520ACD0BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InstDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CInstDlg �_�C�A���O

class CInstDlg : public CDialog
{
// �R���X�g���N�V����
public:
	BOOL m_IsAdminInst;
	CInstDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CInstDlg)
	enum { IDD = IDD_DLG_INST };
	CButton	m_r_autoexec_ctrl1;
	CButton	m_r_autoexec_ctrl2;
	CButton	m_r_autoexec_ctrl3;
	CButton	m_r_autoexec_ctrl4;
	int		m_r_autoexec;
	BOOL	m_chk_uninst_cpl;
	BOOL	m_chk_uninst_start;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CInstDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CInstDlg)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_INSTDLG_H__56D0E476_4F9A_493C_84AF_DB6520ACD0BF__INCLUDED_)
