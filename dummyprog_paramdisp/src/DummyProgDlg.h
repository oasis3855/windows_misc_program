// DummyProgDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_DUMMYPROGDLG_H__BC6E13D2_A988_47F2_B911_87312C6D37F8__INCLUDED_)
#define AFX_DUMMYPROGDLG_H__BC6E13D2_A988_47F2_B911_87312C6D37F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDummyProgDlg �_�C�A���O

class CDummyProgDlg : public CDialog
{
// �\�z
public:
	CDummyProgDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDummyProgDlg)
	enum { IDD = IDD_DUMMYPROG_DIALOG };
	CString	m_edit_param;
	CString	m_edit_thisprog;
	CString	m_edit_curdir;
	CString	m_edit_winver;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDummyProgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDummyProgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DUMMYPROGDLG_H__BC6E13D2_A988_47F2_B911_87312C6D37F8__INCLUDED_)
