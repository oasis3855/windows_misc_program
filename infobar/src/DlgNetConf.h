#if !defined(AFX_DLGNETCONF_H__77914E7B_ADCD_4144_BE44_E191F0AD8F39__INCLUDED_)
#define AFX_DLGNETCONF_H__77914E7B_ADCD_4144_BE44_E191F0AD8F39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNetConf.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNetConf �_�C�A���O

class CDlgNetConf : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgNetConf)

// �R���X�g���N�V����
public:
	int m_nMode;
	CDlgNetConf();
	~CDlgNetConf();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgNetConf)
	enum { IDD = IDD_DLG_NETCONF };
	CEdit	m_sURL_ctrl;
	CButton	m_btn_detail_ctrl;
	CEdit	m_sPhHeader_ctrl;
	CEdit	m_nPhGetcount_ctrl;
	CEdit	m_nPhSkip_ctrl;
	CComboBox	m_ctrl_cmb_qlist;
	CString	m_sURL;
	UINT	m_nPort;
	CString	m_sPhHeader;
	UINT	m_nPhSkip;
	UINT	m_nPhGetcount;
	CString	m_sProxy;
	CString	m_sTitle;
	BOOL	m_chkDelSpace;
	BOOL	m_chkCr2Spc;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDlgNetConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgNetConf)
	afx_msg void OnSelchangeComboQlist();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnDetail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGNETCONF_H__77914E7B_ADCD_4144_BE44_E191F0AD8F39__INCLUDED_)
