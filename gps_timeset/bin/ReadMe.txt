
____________________________________________________________________________

              GPS Time Set （GPSデータによるシステム時間設定）
                 version 1.0.1 for Windows Mobile / Windows

                                                Copyright (C) 2008 井上 博計
                                                   http://oasis.halfmoon.jp/

____________________________________________________________________________












もくじ

      はじめに
      １．動作環境
      ２．使用法
      ３．アンインストール方法
      ４．バージョンアップ情報
      ５．使用許諾条件





はじめに
      このプログラムでは、GPS受信で得られる時刻（UTC時刻）を用いて、システム
      時刻を修正することができます。
          ●Windows Mobile 版 （GpsTimeSetCE.exe）
          ●Windows (Windows XP/Vista) 版 （GpsTimeSet.exe）

１．動作環境
      ● Windows Mobile 版

      Windows CE 4.2以降
      例：Pocket PC 2003, Windows Mobie 5.0, 6

      MFCCE300.DLL および COREDLL.DLL がシステムフォルダに存在すること

      ● Windows (XP/Vista) 版

      Windows 95/98/98SE/Me/2000/XP/Vista 等

      MFC71.DLL および MSVCR71.DLL がシステムフォルダに存在すること

２．使用法
      インストーラは存在しないため、プログラム（*.exe ファイルのみ）を適当な
      フォルダにコピーして利用。

      「COMポート」および「速度」をコンボボックスで設定した後、「受信（受信開
      始）」ボタンを押すと、GPSからデータを受信する。

      受信状態が安定したら、「時刻設定」ボタンを押すと、システム時刻がGPSデー
      タを用いて設定される。（Windows XP系の場合は、時刻設定権限があるユーザ
      で実行する）

      「停止（受信中止）」ボタンを押すと、GPSデータの受信を終了する。（COMポ
      ートを閉じる）

      「閉じる」ボタンでプログラムを終了する。（受信停止していない場合、受信
      停止も自動的に行われる）


３．アンインストール方法
      プログラム（*.exe ファイル）を削除する。

      レジストリやINIファイルなど、設定は保存されていないため、これらの消去を
      考える必要は無い


４．バージョンアップ情報
      バージョン 1.0 (2008/04/19)
        当初

      バージョン 1.0.1 (2008/05/31)
        「時刻設定」で年月日も設定対象にする（ZDA受信している場合）
        「時刻設定」の時差修正で、日をまたぐ場合の計算間違いを修正


５．使用許諾条件
      GNU GPL フリーソフトウエア

      http://www.opensource.jp/gpl/gpl.ja.html

      このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフトウ
      ェア財団によって発行された GNU 一般公衆利用許諾契約書(バージョン2か、希
      望によってはそれ以降のバージョンのうちどれか)の定める条件の下で再頒布ま
      たは改変することができます。

      このプログラムは有用であることを願って頒布されますが、*全くの無保証* で
      す。商業可能性の保証や特定の目的への適合性は、言外に示されたものも含め
      全く存在しません。詳しくはGNU 一般公衆利用許諾契約書をご覧ください。

      あなたはこのプログラムと共に、GNU 一般公衆利用許諾契約書の複製物を一部
      受け取ったはずです。もし受け取っていなければ、フリーソフトウェア財団ま
      で請求してください(宛先は the Free Software Foundation, Inc., 59 Templ
      e Place, Suite 330, Boston, MA 02111-1307 USA)。


                                                              2008 / 05 / 31
____________________________________________________________________________
