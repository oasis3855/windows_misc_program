// Cursor32.h : CURSOR32 アプリケーションのメイン ヘッダー ファイルです。
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル
#include "Cursor32Dlg.h"

/////////////////////////////////////////////////////////////////////////////
// CCursor32App:
// このクラスの動作の定義に関しては Cursor32.cpp ファイルを参照してください。
//

class CCursor32App : public CWinApp
{
public:
	void UninstallSeq();
	void InstallSeq();
	void RegConfigRead(CCursor32Dlg *dlg);
	void RegConfigWrite(CCursor32Dlg *dlg);
	CCursor32App();

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CCursor32App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CCursor32App)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

