#if !defined(AFX_DLGSYSCONF_H__0D33F4EA_9101_4F9F_B2AA_91AC267403C3__INCLUDED_)
#define AFX_DLGSYSCONF_H__0D33F4EA_9101_4F9F_B2AA_91AC267403C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysConf.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSysConf �_�C�A���O

class CDlgSysConf : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgSysConf)

// �R���X�g���N�V����
public:
	COLORREF m_cForeColor, m_cBackColor;
	CDlgSysConf();
	~CDlgSysConf();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgSysConf)
	enum { IDD = IDD_DLG_SYSCONF };
	CStatic	m_ctrl_area_fore_color;
	CStatic	m_ctrl_area_back_color;
	UINT	m_nSpeed;
	UINT	m_nTimer;
	UINT	m_tInterval;
	BOOL	m_bTopmost;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDlgSysConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgSysConf)
	afx_msg void OnBtnBackColor();
	afx_msg void OnBtnForeColor();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGSYSCONF_H__0D33F4EA_9101_4F9F_B2AA_91AC267403C3__INCLUDED_)
