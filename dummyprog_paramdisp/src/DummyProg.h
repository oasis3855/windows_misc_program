// DummyProg.h : DUMMYPROG アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_DUMMYPROG_H__FA4FCF41_1B47_4A15_8767_9B8425A461F4__INCLUDED_)
#define AFX_DUMMYPROG_H__FA4FCF41_1B47_4A15_8767_9B8425A461F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CDummyProgApp:
// このクラスの動作の定義に関しては DummyProg.cpp ファイルを参照してください。
//

class CDummyProgApp : public CWinApp
{
public:
	CDummyProgApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDummyProgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CDummyProgApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DUMMYPROG_H__FA4FCF41_1B47_4A15_8767_9B8425A461F4__INCLUDED_)
