// Cursor32Dlg.h : ヘッダー ファイル
//

#define	USER_TIMER_ID	0x0314	/// timer def

/////////////////////////////////////////////////////////////////////////////
// CCursor32Dlg dialog

class CCursor32Dlg : public CDialog
{
// 構築
public:
	void UpdateAppearance(void);
	int timertick;
	BOOL isNotTaskbar;
	BOOL isTopmost;
	int y_init;
	int x_init;
	CString title;
	BOOL nChangeColor;
	COLORREF TextColor;
	BOOL nChangeDlgColor;
	COLORREF DlgColor;
	COLORREF DlgBackColor;
	int IsAssist, IsRelmode;
	UINT hotkey1,hotkey2,hotkey_rel;
	CCursor32Dlg(CWnd* pParent=NULL);

// Dialog Data
	//{{AFX_DATA(CCursor32Dlg)
	enum { IDD = IDD_CURSOR32_DIALOG };
	//}}AFX_DATA

	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CCursor32Dlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void CloseDialog(void);

	int TimerOK;
	unsigned long Counter;
	long rel_x, rel_y;	// 相対座標の原点



	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCursor32Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAppAbout();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

