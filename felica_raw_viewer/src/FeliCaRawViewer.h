/***************************************************

FeliCaRawViewer.h : FELICARAWVIEWER アプリケーションのメイン ヘッダー ファイルです。

FeliCa Raw Viewer ソフトウエア
  Copyright (C) 2007, INOUE. Hirokazu
  All rights reserved.

このソフトウエアでは、felicalib ライブラリを利用しています
  Copyright (C) 2007, Takuya Murakami
  All rights reserved.

The BSD License (http://opensource.org/licenses/bsd-license.php)
---------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer. 
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution. 
* Neither the name of the this project nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

以下の日本語訳は参考のために添付したものです。英語版だけが法律的に有効である
ことに注意してください。

BSDライセンス条項 （日本語訳）
    (http://sourceforge.jp/projects/opensource/wiki/licenses%2Fnew_BSD_license)
-----------------------------
ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を
満たす場合に限り、再頒布および使用が許可されます。

* ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記
  免責条項を含めること。 
* バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の
  著作権表示、本条件一覧、および下記免責条項を含めること。 
* 書面による特別の許可なしに、本ソフトウェアから派生した製品の宣伝または販売
  促進に、このプロジェクトの名前またはコントリビューターの名前を使用しては
  ならない。

本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供
されており、明示黙示を問わず、商業的な使用可能性、および特定の目的に対する
適合性に関する暗黙の保証も含め、またそれに限定されない、いかなる保証も
ありません。著作権者もコントリビューターも、事由のいかんを問わず、損害発生の
原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任であるか（過失
その他の）不法行為であるかを問わず、仮にそのような損害が発生する可能性を
知らされていたとしても、本ソフトウェアの使用によって発生した（代替品または
代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、
またそれに限定されない）直接損害、間接損害、偶発的な損害、特別損害、懲罰的
損害、または結果損害について、一切責任を負わないものとします。


***************************************************/

#if !defined(AFX_FELICARAWVIEWER_H__875DD426_2822_4D94_A691_EF256379B290__INCLUDED_)
#define AFX_FELICARAWVIEWER_H__875DD426_2822_4D94_A691_EF256379B290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CFeliCaRawViewerApp:
// このクラスの動作の定義に関しては FeliCaRawViewer.cpp ファイルを参照してください。
//

class CFeliCaRawViewerApp : public CWinApp
{
public:
	CFeliCaRawViewerApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFeliCaRawViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CFeliCaRawViewerApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FELICARAWVIEWER_H__875DD426_2822_4D94_A691_EF256379B290__INCLUDED_)
