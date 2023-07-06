## sf2mrtg : SpeedFanのデータをMRTGで取り込む補助ツール for Windows<!-- omit in toc -->

---
[Home](https://oasis3855.github.io/webpage/) > [Software](https://oasis3855.github.io/webpage/software/index.html) > [Software Download](https://oasis3855.github.io/webpage/software/software-download.html) > [windows_misc_program](../README.md) > ***speedfan_to_mrtg*** (this page)

<br />
<br />

Last Updated : Jul. 2008

- [ソフトウエアのダウンロード](#ソフトウエアのダウンロード)
- [機能の概要](#機能の概要)
  - [制限事項](#制限事項)
- [動作環境](#動作環境)
- [バージョンアップ履歴](#バージョンアップ履歴)
- [ライセンス](#ライセンス)

<br />
<br />

## ソフトウエアのダウンロード

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [このGitHubリポジトリを参照する](../speedfan_to_mrtg/download) 

- ![download icon](../readme_pics/soft-ico-download-darkmode.gif)   [GoogleDriveを参照する](https://drive.google.com/drive/folders/0B7BSijZJ2TAHMjZjZDNjNGYtYzEwMC00OTc4LWExMDYtZmZjZmVjZGQ4MzI3?resourcekey=0-GrtqSQgImQN13d-TF1AVMw) 

<br />
<br />

## 機能の概要

SpeedFanのログ データファイル（CSV形式または共有メモリー）から温度データ等をMRTGで読み込むための中間処理プログラムです。

ユーザ権限による制限でSpeedFan SNMP Extension経由で値が取得できない場合の代替手段です。 

コマンドラインでの実行結果表示は次のようになります。これはMRTGでのデータ収集フォーマットに対応しています。

<pre>
C:\Program Files\sf2mrtg> sf2mrtg
3700
4500
uptime 0 days 02 hours 25 minutes
Temperature
</pre>

### 制限事項

共有メモリーから取り込む場合で、Windows Vista以降のユーザアクセス制御（UAC)がONの場合には、MRTGのプロセス所有者とSpeedFanのプロセス所有者が一致していないとエラーとなります。 

<br />
<br />

## 動作環境

- Windows 95/98/Me/NT/2000/XP/Vista/7 

<br />
<br />

## バージョンアップ履歴

- Version 1.0 (2007/07/17)

    当初

<br />
<br />

## ライセンス

このソフトウエアは [GNU General Public License v3ライセンスで公開する](https://gpl.mhatta.org/gpl.ja.html) フリーソフトウエア

