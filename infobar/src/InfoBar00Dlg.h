// InfoBar00Dlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_INFOBAR00DLG_H__9F77ABE2_E057_470E_9F77_BB82A73B81E6__INCLUDED_)
#define AFX_INFOBAR00DLG_H__9F77ABE2_E057_470E_9F77_BB82A73B81E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInfoBar00Dlg �_�C�A���O

class CInfoBar00Dlg : public CDialog
{
// �\�z
public:
	BOOL bExitSeq;
	UINT nTimerID;
	void EndDialogSeq(void);
	UINT nPosX, nPosY;
	BOOL bDispTopmost;
	void MakeLCD(void);
	time_t tPrev, tInterval;
	COLORREF cBackColor, cForeColor;
	UINT nMoveSpeed;
	UINT nTimer;
	int nInfoWndWidth;
	UINT MemStrLength;
	void CalcMemStrLength(void);
	int _nTimCurArticle;
	BOOL IsMemDC_active;
	BOOL IsInfoWndDC_active;
	int ScrCurPos;
	CBrush BrushInfoBack;
	RECT InfoWndRect;
	CBitmap MemBmp;
	CDC MemDC;
	CDC *InfoWndDC;
	CInfoBar00Dlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CInfoBar00Dlg)
	enum { IDD = IDD_INFOBAR00_DIALOG };
	CStatic	m_ctrl_info_area;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CInfoBar00Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CInfoBar00Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuConfig();
	afx_msg void OnMenuExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_INFOBAR00DLG_H__9F77ABE2_E057_470E_9F77_BB82A73B81E6__INCLUDED_)
