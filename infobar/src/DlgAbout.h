#if !defined(AFX_DLGABOUT_H__3D816F1E_4671_43F6_BAA0_4664CAACA02B__INCLUDED_)
#define AFX_DLGABOUT_H__3D816F1E_4671_43F6_BAA0_4664CAACA02B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAbout.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout �_�C�A���O

class CDlgAbout : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgAbout)

// �R���X�g���N�V����
public:
	CDlgAbout();
	~CDlgAbout();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgAbout)
	enum { IDD = IDD_DLG_ABOUT };
		// ���� - ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
		//    ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDlgAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgAbout)
	afx_msg void OnBtnMakeunin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGABOUT_H__3D816F1E_4671_43F6_BAA0_4664CAACA02B__INCLUDED_)