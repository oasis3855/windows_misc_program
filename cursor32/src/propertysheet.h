// PropertySheet.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPropAbout ダイアログ

class CPropAbout : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropAbout)

// コンストラクション
public:
	BOOL m_uninstall;
	CPropAbout();
	~CPropAbout();

// ダイアログ データ
	//{{AFX_DATA(CPropAbout)
	enum { IDD = IDD_CNF_ABOUT };
	CString	m_ddx_odometer;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CPropAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropAbout)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CPropConf ダイアログ

class CPropConf : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropConf)

// コンストラクション
public:
	COLORREF m_TextColor;
	COLORREF m_DlgColor;
	COLORREF m_DlgBackColor;
	CPropConf();
	~CPropConf();

// ダイアログ データ
	//{{AFX_DATA(CPropConf)
	enum { IDD = IDD_CNF_CONFIG };
	CButton	m_pickdlgcol;
	CButton	m_pickdlgbackcol;
	CButton	m_pickcolor;
	CListBox	m_hotkey2;
	CListBox	m_hotkey1;
	BOOL	m_ddx_assist;
	CString	m_ddx_title;
	int		m_ddx_x;
	int		m_ddx_y;
	int		m_ddx_timertick;
	int		m_ddx_hotkey1;
	int		m_ddx_hotkey2;
	BOOL	m_ddx_changecolor;
	BOOL	m_ddx_isNotTaskbar;
	BOOL	m_ddx_isTopmost;
	BOOL	m_ddx_nChangeDlgColor;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CPropConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPropConf)
	virtual BOOL OnInitDialog();
	afx_msg void OnAssist();
	afx_msg void OnPickcolor();
	afx_msg void OnChangecolor();
	afx_msg void OnPickdlgbackcol();
	afx_msg void OnPickdlgcol();
	afx_msg void OnChangedlgcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
