// InfoBar00.h : INFOBAR00 アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_INFOBAR00_H__525BBCAD_48E3_4E79_A054_992BBD7B7892__INCLUDED_)
#define AFX_INFOBAR00_H__525BBCAD_48E3_4E79_A054_992BBD7B7892__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル
#include "InfoBar00Dlg.h"

/////////////////////////////////////////////////////////////////////////////
// CInfoBar00App:
// このクラスの動作の定義に関しては InfoBar00.cpp ファイルを参照してください。
//

class CInfoBar00App : public CWinApp
{
public:
	BOOL m_bMutexOwner;
	HANDLE m_hMutex;
	BOOL bInstalled;
	BOOL UninstallSeq(void);
	BOOL InstallSeq(void);
	void RegConfigWrite(CInfoBar00Dlg *dlg);
	void RegConfigRead(CInfoBar00Dlg *dlg);
	CInfoBar00App();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CInfoBar00App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CInfoBar00App)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_INFOBAR00_H__525BBCAD_48E3_4E79_A054_992BBD7B7892__INCLUDED_)
