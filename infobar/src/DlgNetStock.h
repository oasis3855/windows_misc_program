#if !defined(AFX_DLGNETSTOCK_H__DCAB6613_BD04_46D4_9BA3_FED0BF2832EA__INCLUDED_)
#define AFX_DLGNETSTOCK_H__DCAB6613_BD04_46D4_9BA3_FED0BF2832EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNetStock.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNetStock �_�C�A���O

class CDlgNetStock : public CDialog
{
// �R���X�g���N�V����
public:
	CDlgNetStock(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgNetStock)
	enum { IDD = IDD_DLG_NET_STOCK };
	CComboBox	m_cmb_preset_ctrl;
	CString	m_sURL;
	CString	m_sItems;
	CString	m_sPhHeader;
	CString	m_sPhHeaderB;
	CString	m_sPhHeaderC;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgNetStock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgNetStock)
	afx_msg void OnSelchangeCmbPreset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGNETSTOCK_H__DCAB6613_BD04_46D4_9BA3_FED0BF2832EA__INCLUDED_)
