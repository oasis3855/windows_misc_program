#if !defined(AFX_INSTDLG_H__2DA7246E_5DAA_4E72_97CD_740D3216DACC__INCLUDED_)
#define AFX_INSTDLG_H__2DA7246E_5DAA_4E72_97CD_740D3216DACC__INCLUDED_

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
	BOOL	m_chk_uninst_start;
	int		m_r_autoexec;
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
	afx_msg void OnBtnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_INSTDLG_H__2DA7246E_5DAA_4E72_97CD_740D3216DACC__INCLUDED_)