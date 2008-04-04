// BacklightControlDlg.h : header file
//

#if !defined(AFX_BACKLIGHTCONTROLDLG_H__10446306_C7BC_4377_8CD0_16683DF1D326__INCLUDED_)
#define AFX_BACKLIGHTCONTROLDLG_H__10446306_C7BC_4377_8CD0_16683DF1D326__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CBacklightControlDlg dialog

class CBacklightControlDlg : public CDialog
{
// Construction
public:
	CBacklightControlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBacklightControlDlg)
	enum { IDD = IDD_BACKLIGHTCONTROL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBacklightControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBacklightControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonD4();
	afx_msg void OnButtonD0();
	afx_msg void OnButtonAuto();
	afx_msg void OnButtonGet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BACKLIGHTCONTROLDLG_H__10446306_C7BC_4377_8CD0_16683DF1D326__INCLUDED_)
