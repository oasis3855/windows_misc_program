#if !defined(AFX_INSTDLG_H__90FB7E2C_9CF7_4B05_B81E_CFE2CE556CDF__INCLUDED_)
#define AFX_INSTDLG_H__90FB7E2C_9CF7_4B05_B81E_CFE2CE556CDF__INCLUDED_

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
	CInstDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CInstDlg)
	enum { IDD = IDD_DLG_INST };
	int		m_r_autoexec;
	BOOL	m_chk_uninst_start;
	BOOL	m_chk_uninst_cpl;
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
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_INSTDLG_H__90FB7E2C_9CF7_4B05_B81E_CFE2CE556CDF__INCLUDED_)
