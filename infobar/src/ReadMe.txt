========================================================================
       MICROSOFT FOUNDATION CLASS ライブラリ : InfoBar00
========================================================================


AppWizard が生成した InfoBar00 には、Microsoft Foundation Class の基本的な
使い方が示されています。アプリケーション作成のひな型としてお使いください。

このファイルには InfoBar00 を構成している各ファイルの概要説明が含まれています。

InfoBar00.dsp
    このファイル (プロジェクト ファイル) はプロジェクト レベルの情報を含み、
    シングル プロジェクトまたはサブ プロジェクトのビルドに使用されます。他の
    ユーザーとプロジェクト ファイル (.dsp) を共有できますが、メイク ファイル
    はローカルにエクスポートしてください。

InfoBar00.h
    このファイルはアプリケーションの中心となるインクルードファイルです。この
    ファイルは他のプロジェクトの固有のインクルードファイル（Resource.h  も含
    みます） をインクルードし、また CInfoBar00App アプリケーションクラスを宣
    言します。

InfoBar00.cpp
    このファイルは CInfoBar00App アプリケーションクラスを含むアプリケーション
    の中心となるソースファイルです。

InfoBar00.rc
    このファイルはプログラムが使用する Microsoft Windows のリソースを列挙しま
    す。このファイルは RES サブディレクトリに保存されているアイコン、ビットマ
    ップ、カーソルを含みます。このファイルは Microsoft Visual C++ で直接
    編集可能です。

InfoBar00.clw
    このファイルは ClassWizard  が既存のクラスの編集やクラスを新規作成すると
    きに使う情報を持っています。またこのファイルは、ClassWizard によるメッセ
    ージ マップやダイアログ データ マップの編集や生成、プロトタイプ メンバ関
    数の生成に必要な情報も持っています。

res\InfoBar00.ico
    このファイルは、アプリケーションのアイコンとして使用されるアイコンファイ
    ルです。このアイコンはリソース ファイル InfoBar00.rc によってインクルード
    されます。

res\InfoBar00.rc2
    このファイルは Microsoft Visual C++ によって編集されないリソースを含んでいま
    す。このファイルにリソース エディタで編集されないすべてのリソースを記述
    してください。




/////////////////////////////////////////////////////////////////////////////

AppWizard は 1 つのダイアログ クラスを作成します:

InfoBar00Dlg.h, InfoBar00Dlg.cpp - ダイアログ
    これらのファイルは CInfoBar00Dlg クラスを含みます。このクラスは 
    アプリケーションのメイン ダイアログの動作を定義します。ダイアログ
    テンプレートは Microsoft Visual C++ で編集可能な InfoBar00.rc に含
    まれます。


/////////////////////////////////////////////////////////////////////////////
その他の標準ファイル:

StdAfx.h, StdAfx.cpp
    これらのファイルはプリコンパイル済ヘッダー ファイル (PCH) InfoBar00.pch 
    やプリコンパイル済型ファイル StdAfx.obj を構築するために使われるファイルです。

Resource.h
    このファイルは新規リソース ID を定義する標準ヘッダー ファイルです。
    Microsoft Visual C++ はこのファイルを読み込み更新します。

/////////////////////////////////////////////////////////////////////////////
その他の注意:

"TODO:" で始まるコメントは、ソースコードの追加やカスタマイズの必要な場所を示
します。

アプリケーションで共有 DLL 内で MFC を使用する場合や、アプリケーションが
オペレーティング システムで現在使用している言語以外の言語を使用している場合は、
Microsoft Visual C++ CD-ROM から、ローカライズされたリソースに対応する
MFC42XXX.DLL を system または system32 ディレクトリにコピーして、ファイル名を 
MFC42LOC.DLL にする必要があります。("XXX" は言語の省略形を現します。例えば、
MFC42JPN.DLL は日本語に翻訳されたリソースを含みます。)
このようにしなかった場合には、アプリケーション内のいくつかの UI 要素はオペ
レーティング システムの言語のまま残ります。

/////////////////////////////////////////////////////////////////////////////
