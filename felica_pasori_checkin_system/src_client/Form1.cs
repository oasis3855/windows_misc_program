/***************************************************

Form1.cs : アプリケーション用クラスの定義を行います。

Pasoli_checkin ソフトウエア
  Copyright (C) 2008, INOUE. Hirokazu
  All rights reserved.


 http://www.opensource.jp/gpl/gpl.ja.html

 このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフトウェ
 ア財団によって発行された GNU 一般公衆利用許諾契約書(バージョン2か、希
 望によってはそれ以降のバージョンのうちどれか)の定める条件の下で再頒布
 または改変することができます。
 
 このプログラムは有用であることを願って頒布されますが、*全くの無保証* 
 です。商業可能性の保証や特定の目的への適合性は、言外に示されたものも含
 め全く存在しません。詳しくはGNU 一般公衆利用許諾契約書をご覧ください。
 
 あなたはこのプログラムと共に、GNU 一般公衆利用許諾契約書の複製物を一部
 受け取ったはずです。もし受け取っていなければ、フリーソフトウェア財団ま
 で請求してください(宛先は the Free Software Foundation, Inc., 59
 Temple Place, Suite 330, Boston, MA 02111-1307 USA)。


***************************************************/

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

using System.Text;
using System.Runtime.InteropServices;



namespace Pasoli_checkin
{
	/// <summary>
	/// Form1 の概要の説明です。
	/// </summary>
	public class Form_Main : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button btn_close;
		private System.Windows.Forms.TextBox textBox_result;
		private System.Windows.Forms.Button btn_read_felica_once;
		private System.Windows.Forms.Button btn_read_felica_loop;
		private System.Windows.Forms.Button btn_thread_kill;
		private System.Windows.Forms.Button btn_network_test;
		private System.Windows.Forms.CheckBox checkBox_network_enable;
		private System.Windows.Forms.Button btn_set_uri;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form_Main()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			/****
			 * コマンドライン引数があれば、strUri の初期値とする
			 ****/
			string [] strArgv = System.Environment.GetCommandLineArgs();
			if(strArgv.Length >= 2)	strUri = strArgv[1];

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows フォーム デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this.btn_network_test = new System.Windows.Forms.Button();
			this.btn_close = new System.Windows.Forms.Button();
			this.textBox_result = new System.Windows.Forms.TextBox();
			this.btn_read_felica_once = new System.Windows.Forms.Button();
			this.btn_read_felica_loop = new System.Windows.Forms.Button();
			this.btn_thread_kill = new System.Windows.Forms.Button();
			this.checkBox_network_enable = new System.Windows.Forms.CheckBox();
			this.btn_set_uri = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// btn_network_test
			// 
			this.btn_network_test.Location = new System.Drawing.Point(16, 96);
			this.btn_network_test.Name = "btn_network_test";
			this.btn_network_test.Size = new System.Drawing.Size(144, 24);
			this.btn_network_test.TabIndex = 2;
			this.btn_network_test.Text = "ネットワーク試験";
			this.btn_network_test.Click += new System.EventHandler(this.btn_network_test_Click);
			// 
			// btn_close
			// 
			this.btn_close.Location = new System.Drawing.Point(336, 128);
			this.btn_close.Name = "btn_close";
			this.btn_close.Size = new System.Drawing.Size(144, 24);
			this.btn_close.TabIndex = 3;
			this.btn_close.Text = "プログラム終了";
			this.btn_close.Click += new System.EventHandler(this.btn_close_Click);
			// 
			// textBox_result
			// 
			this.textBox_result.Font = new System.Drawing.Font("ＭＳ ゴシック", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.textBox_result.Location = new System.Drawing.Point(16, 8);
			this.textBox_result.Name = "textBox_result";
			this.textBox_result.Size = new System.Drawing.Size(472, 26);
			this.textBox_result.TabIndex = 1;
			this.textBox_result.Text = "";
			// 
			// btn_read_felica_once
			// 
			this.btn_read_felica_once.Location = new System.Drawing.Point(176, 96);
			this.btn_read_felica_once.Name = "btn_read_felica_once";
			this.btn_read_felica_once.Size = new System.Drawing.Size(144, 24);
			this.btn_read_felica_once.TabIndex = 4;
			this.btn_read_felica_once.Text = "ICカードテスト読み込み";
			this.btn_read_felica_once.Click += new System.EventHandler(this.btn_read_felica_once_Click);
			// 
			// btn_read_felica_loop
			// 
			this.btn_read_felica_loop.Location = new System.Drawing.Point(64, 48);
			this.btn_read_felica_loop.Name = "btn_read_felica_loop";
			this.btn_read_felica_loop.Size = new System.Drawing.Size(168, 32);
			this.btn_read_felica_loop.TabIndex = 4;
			this.btn_read_felica_loop.Text = "カードリーダ起動";
			this.btn_read_felica_loop.Click += new System.EventHandler(this.btn_read_felica_loop_Click);
			// 
			// btn_thread_kill
			// 
			this.btn_thread_kill.Location = new System.Drawing.Point(248, 48);
			this.btn_thread_kill.Name = "btn_thread_kill";
			this.btn_thread_kill.Size = new System.Drawing.Size(168, 32);
			this.btn_thread_kill.TabIndex = 4;
			this.btn_thread_kill.Text = "カードリーダ停止";
			this.btn_thread_kill.Click += new System.EventHandler(this.btn_thread_kill_Click);
			// 
			// checkBox_network_enable
			// 
			this.checkBox_network_enable.Checked = true;
			this.checkBox_network_enable.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBox_network_enable.Location = new System.Drawing.Point(16, 128);
			this.checkBox_network_enable.Name = "checkBox_network_enable";
			this.checkBox_network_enable.Size = new System.Drawing.Size(304, 16);
			this.checkBox_network_enable.TabIndex = 5;
			this.checkBox_network_enable.Text = "カードリーダ読み込み時にネットワーク機能を使う";
			// 
			// btn_set_uri
			// 
			this.btn_set_uri.Location = new System.Drawing.Point(336, 96);
			this.btn_set_uri.Name = "btn_set_uri";
			this.btn_set_uri.Size = new System.Drawing.Size(144, 24);
			this.btn_set_uri.TabIndex = 6;
			this.btn_set_uri.Text = "サーバURL設定";
			this.btn_set_uri.Click += new System.EventHandler(this.btn_set_uri_Click);
			// 
			// Form_Main
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(504, 155);
			this.Controls.Add(this.btn_set_uri);
			this.Controls.Add(this.checkBox_network_enable);
			this.Controls.Add(this.textBox_result);
			this.Controls.Add(this.btn_close);
			this.Controls.Add(this.btn_network_test);
			this.Controls.Add(this.btn_read_felica_once);
			this.Controls.Add(this.btn_read_felica_loop);
			this.Controls.Add(this.btn_thread_kill);
			this.Name = "Form_Main";
			this.Text = "FeliCaを用いたチェックイン記録システム";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.Form_Main_Closing);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// アプリケーションのメイン エントリ ポイントです。
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form_Main());
		}

		/**********
		 * サーバのPerlスクリプトアドレス（環境に合わせて、必ず変更すること！）
		 *********/
		string strUri = "http://192.168.1.1/pasoli_checkin.cgi";


		/**********
		 * FeliCaサポートDLLの関数を使うための宣言
		 *********/
		[DllImport("DllFeliCaLib.dll")]
		private extern static int ReadFelicaIdOnce(System.Text.StringBuilder lpStrOutput);


		/**********
		 * 閉じるボタンを押した時の処理
		 *********/
		private void btn_close_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}

		/**********
		 * サーバのURI設定ボタンを押した時
		 *********/
		private void btn_set_uri_Click(object sender, System.EventArgs e)
		{
			// ボタンを無効化（グレー表示）に切り替える
			Change_Button_State(false);

			// InputBox ダイアログ生成
			InputBox dlg = new InputBox(this.strUri);
			// モーダルダイアログを表示
			if(dlg.ShowDialog() == DialogResult.OK)
			{
				// OKボタンが押された場合、URI文字列を書き換える
				this.strUri = dlg.strReturn;
			}
			// InputBox ダイアログを破棄
			dlg.Dispose();
		
			// ボタンを再有効化
			Change_Button_State(true);
		}

		/**********
		 * ネットワーク試験ボタンを押した時の処理
		 *********/
		private void btn_network_test_Click(object sender, System.EventArgs e)
		{

			// サーバに接続して、応答を得る
			string resText = Network_Sync("net_test", "ABC 0123 試験文字列 xyz", "shift_jis");

			this.textBox_result.Text = resText;

			// ボタンを再有効化
			Change_Button_State(true);
		}



		/**********
		 * ICカードテスト読み込みボタンを押した時の処理
		 *********/
		private void btn_read_felica_once_Click(object sender, System.EventArgs e)
		{
			System.Text.StringBuilder lpStr = new System.Text.StringBuilder(64);

			// ボタンを無効化（グレー表示）に切り替える
			Change_Button_State(false);

			// テキストボックスをクリア
			this.textBox_result.Text = "";

			// FeliCaデータ読み出し（DLL）
			int nResult = ReadFelicaIdOnce(lpStr);

			if(nResult == 1)	// 読み出し成功
			{
				string strTemp = lpStr.ToString();
				if(checkBox_network_enable.Checked == true)
				{
					// サーバに接続して、応答を得る
					string resText = Network_Sync("id_disp", strTemp, "shift_jis");

					this.textBox_result.Text = resText;
				}
				else
				{
					this.textBox_result.Text = strTemp;
				}
			}
			else
			{
				this.textBox_result.Text = "ICカードデータ読み込み失敗";
			}

			// ボタンを再有効化
			Change_Button_State(true);
		
		}

		/**********
		 * カードリーダ連続読み込みスレッドとの相互通信用宣言
		 *********/
		private delegate void DelegateSetTextbox(string strBuf);
		System.Threading.Thread threadMain;
		volatile int nThreadInit = 0;

		/**********
		 * カードリーダ起動ボタンを押した時の処理
		 *********/
		private void btn_read_felica_loop_Click(object sender, System.EventArgs e)
		{
			threadMain = new System.Threading.Thread(new System.Threading.ThreadStart(thread_felica_read));
			threadMain.Start();
			nThreadInit = 1;

			// 他のボタンを無効化（グレー表示）に切り替える
			Change_Button_State(false);
			btn_thread_kill.Enabled = true;		// 停止ボタンだけ有効

		}

		/**********
		 * カードリーダ停止ボタンを押した時の処理
		 *********/
		private void btn_thread_kill_Click(object sender, System.EventArgs e)
		{
			if(nThreadInit == 0)
			{
				this.textBox_result.Text = "カードリーダ停止（起動されていません）";
				return;
			}
			if(threadMain.ThreadState == System.Threading.ThreadState.Unstarted)
			{
				this.textBox_result.Text = "カードリーダ停止";
				return;
			}
			if(threadMain.IsAlive != true)
			{
				this.textBox_result.Text = "カードリーダ停止";
				return;
			}
			threadMain.Abort();
			threadMain.Join();
			textBox_result.Text = "カードリーダ停止";

			// ボタンを有効化
			Change_Button_State(true);
		}


		/**********
		 * カードリーダを連続して読み込むスレッド
		 *********/
		private void thread_felica_read()
		{
			DelegateSetTextbox dlgt = new DelegateSetTextbox(DelegateSetTextboxFunction);

			try
			{
				int nResult;
				string strTemp;
				string strPrevID = "";		// 1回前に読み込んだID
				System.Text.StringBuilder lpStr = new System.Text.StringBuilder(64);

				// スレッド メインループ
				for(;;)
				{
					// FeliCaデータ読み出し（DLL）
					nResult = ReadFelicaIdOnce(lpStr);

					if(nResult == 1)
					{	// FeliCa読み込み成功時

						strTemp = lpStr.ToString();		//IDm_PMmを文字列に格納

						if(strPrevID != strTemp)
						{	// 前回と違うIDm_PMmの時

							strPrevID = strTemp;
							if(checkBox_network_enable.Checked == true)
							{
								// サーバに接続して、応答を得る
								strTemp = Network_Sync("checkin", strTemp, "shift_jis");
							}

						}
						else strTemp = "";
					}
					else
					{	// FeliCa読み込み失敗時
						strTemp = "ICカードを読み取り機にタッチしてください";
						strPrevID = "";
					}

					// テキストボックスに表示（空の文字列の時は変更しない）
					if(strTemp != "") this.Invoke(dlgt, new object[] {strTemp});

					// 1秒休止
					System.Threading.Thread.Sleep(1000);

				}
			}
			catch(System.Threading.ThreadAbortException)
			{
				// スレッド終了時の処理をする
			}
		}

		/**********
		 * スレッド内より、フォームのテキストボックスを変更するためのデリゲート
		 *********/
		private void DelegateSetTextboxFunction(string strBuf)
		{
			this.textBox_result.Text = strBuf;
		}


		/**********
		 * フォームが閉じられる時に実行する（スレッド停止処理を行う）
		 * 
		 * （フォームのプロパティタブで、イベント追加した）
		 *********/
		private void Form_Main_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			if(nThreadInit == 0) return;
			if(threadMain.ThreadState == System.Threading.ThreadState.Unstarted) return;
			if(threadMain.IsAlive != true) return;

			threadMain.Abort();
			threadMain.Join();

		}

		/**********
		// ネットワーク接続されているかの状況を得るAPI
		 *********/
		[System.Runtime.InteropServices.DllImport("wininet.dll")]
		extern static bool InternetGetConnectedState(out int lpdwFlags, int dwReserved);

		/**********
		 * ネットワーク機能用 共通関数 （サーバが任意のエンコードの場合）
		 * 
		 * strCmd, strData :	POSTメッセージとして送信するデータ（cmd=strCmd&data=strData を送信）
		 *						strDataはURLエンコードして送信される
		 * strEncode :	サーバの文字エンコード形式（"shift_jis" や "UTF-8" などを想定）
		 *********/
		private string Network_Sync(string strCmd, string strData, string strEncode)
		{
			int nFlag;
			if (InternetGetConnectedState(out nFlag, 0) == false)
			{	// ネットワークに接続されていない
				return("");
			}

			// SJIS用のテキストバッファ
			System.Text.Encoding enc = 	System.Text.Encoding.GetEncoding(strEncode);
			// テキスト（C#はデフォルトでUTF-8）
			string resText;

			// POSTするテキストを作成する（strDataは指定されたエンコード形式でurlencode）
			string strPostData = string.Format("cmd={0}&data={1}&dummy=dummy",
					strCmd,	
					System.Web.HttpUtility.UrlEncode(strData, enc));
					// 「VisualStudioのプロジェクトで、System.Web.dll への参照を追加する」こと
			byte[] resPostBytes = enc.GetBytes(strPostData);


			try
			{
				// ネットワークコネクションの定義
				System.Net.WebClient wc = new System.Net.WebClient();
				// UTF-8以外はヘッダ情報が必要（URLエンコードして送信するため）
				wc.Headers.Add("Content-Type","application/x-www-form-urlencoded");

				// データの送信・応答を受信
				byte[] resData = wc.UploadData(strUri, resPostBytes);
				// コネクションの破棄
				wc.Dispose();

				// 応答バイト列を、文字列に変換して格納
				resText = enc.GetString(resData);

			}
			catch
			{	// ネットワークエラー
				resText = "";
			}

			return(resText);
		}

		/**********
		 * ネットワーク機能用 共通関数 （サーバがUTF-8エンコードの場合）
		 * 
		 * strCmd, strData :	POSTメッセージとして送信するデータ（cmd=strCmd&data=strData を送信）
		 *						strDataはURLエンコードして送信される
		 *********/
		private string Network_Sync_UTF8(string strCmd, string strData)
		{
			int nFlag;
			if (InternetGetConnectedState(out nFlag, 0) == false)
			{	// ネットワークに接続されていない
				return("");
			}

			string resText;

			try
			{
				// ネットワークコネクションの定義
				System.Net.WebClient wc = new System.Net.WebClient();
				System.Collections.Specialized.NameValueCollection ps = new System.Collections.Specialized.NameValueCollection();

				// POSTで送信するデータセットを定義する
				ps.Add("cmd", strCmd);
				ps.Add("data", strData);
				ps.Add("dummy", "dummy");

				// データの送信・応答を受信
				byte[] resData = wc.UploadValues(strUri, ps);
				// コネクションの破棄
				wc.Dispose();

				// 応答バイト列を、文字列に変換して格納
				//UTF-8の場合
				resText = System.Text.Encoding.UTF8.GetString(resData);
				// SJISの場合
				// System.Text.Encoding enc = 	System.Text.Encoding.GetEncoding("shift_jis");
				// resText = enc.GetString(resData);

			}
			catch
			{	// ネットワークエラー
				resText = "";
			}

			return(resText);
		}


		/**********
		 * ボタン有効化・無効化状態変更用 共通関数
		 *********/
		private void Change_Button_State(bool bEnabled)
		{
			this.btn_read_felica_loop.Enabled = bEnabled;
			this.btn_thread_kill.Enabled = bEnabled;
			this.btn_network_test.Enabled = bEnabled;
			this.btn_read_felica_once.Enabled = bEnabled;
			this.btn_close.Enabled = bEnabled;
			this.btn_set_uri.Enabled = bEnabled;
		}

	}
}

/**********
* EOF
*********/
