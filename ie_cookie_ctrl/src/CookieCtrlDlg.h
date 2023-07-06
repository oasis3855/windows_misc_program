// CookieCtrlDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_COOKIECTRLDLG_H__01212946_0903_11D5_AD9A_0090CCA1066A__INCLUDED_)
#define AFX_COOKIECTRLDLG_H__01212946_0903_11D5_AD9A_0090CCA1066A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCookieCtrlDlg �_�C�A���O

class CCookieCtrlDlg : public CDialog
{
// �\�z
public:
	BOOL ReInstallSeq(void);
	CCookieCtrlDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CCookieCtrlDlg)
	enum { IDD = IDD_COOKIECTRL_DIALOG };
	CButton	m_sw_days_ctrl;
	CButton	m_btn_folder_brw;
	CButton	m_btn_folder_auto;
	CEdit	m_str_folder_ctrl;
	CStatic	m_str_days_ctrl;
	CScrollBar	m_scroll_days_ctrl;
	CEdit	m_int_days_ctrl;
	CEdit	m_str_exclude_ctrl;
	CString	m_str_folder;
	CString	m_str_ext;
	CString	m_str_exclude;
	int		m_int_days;
	BOOL	m_sw_days;
	BOOL	m_sw_exclude;
	BOOL	m_chk_auto_folder;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCookieCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CCookieCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnFolderBrw();
	afx_msg void OnBtnFolderAuto();
	afx_msg void OnBtnExt();
	afx_msg void OnCheckExclude();
	afx_msg void OnCheckDays();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnRun();
	afx_msg void OnBtnStartup();
	afx_msg void OnBtnVersion();
	afx_msg void OnBtnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnCheckAutoFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_COOKIECTRLDLG_H__01212946_0903_11D5_AD9A_0090CCA1066A__INCLUDED_)
