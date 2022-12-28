// KanCodeDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CKanCodeDlg dialog

class CKanCodeDlg : public CDialog
{
// 構築
public:
//	CEdit EditInputBox;
	CRichEditCtrl EditInputBox;
	CKanCodeDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// Dialog Data
	//{{AFX_DATA(CKanCodeDlg)
	enum { IDD = IDD_KANCODE_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CKanCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CKanCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCnv();
	afx_msg void OnCclp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

