// InfoBar00Dlg.h : ヘッダー ファイル
//

#if !defined(AFX_INFOBAR00DLG_H__9F77ABE2_E057_470E_9F77_BB82A73B81E6__INCLUDED_)
#define AFX_INFOBAR00DLG_H__9F77ABE2_E057_470E_9F77_BB82A73B81E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include"NetAccessThread.h"


/////////////////////////////////////////////////////////////////////////////
// CInfoBar00Dlg ダイアログ

class CInfoBar00Dlg : public CDialog
{
// 構築
public:
	void CopyGlobalParam();
//	CThreadParam ThreadParam;
	void DrawStringOnBmp(CString *sBuf);
	CString sFontName;
	int nFontPoint;
	CFont m_Font;
	BOOL bWndSizeChanged;
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
	int nInfoWndWidth, nInfoWndHeight;
	UINT MemStrLength;
	void CalcMemStrLength(CString *sBuf);
	int _nTimCurArticle;
	BOOL IsMemDC_active;
	BOOL IsInfoWndDC_active;
	int ScrCurPos;
	CBrush BrushInfoBack;
	RECT InfoWndRect;
	CBitmap MemBmp;
	CDC MemDC;
	CDC *InfoWndDC;
	CInfoBar00Dlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

//	CNetAccessThread* pThread;


// ダイアログ データ
	//{{AFX_DATA(CInfoBar00Dlg)
	enum { IDD = IDD_INFOBAR00_DIALOG };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CInfoBar00Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuHelp();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_INFOBAR00DLG_H__9F77ABE2_E057_470E_9F77_BB82A73B81E6__INCLUDED_)
