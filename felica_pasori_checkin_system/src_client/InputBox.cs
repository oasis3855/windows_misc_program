using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace Pasoli_checkin
{
	/// <summary>
	/// InputBox の概要の説明です。
	/// </summary>
	public class InputBox : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox textBox_uri;
		private System.Windows.Forms.Button btn_cansel;
		private System.Windows.Forms.Button btn_ok;
		private System.Windows.Forms.Label label1;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		/****
		* コンストラクタ
		*
		*（テキストボックスの初期値を決めるためのパラメータを追加）
		****/
		public InputBox(string strInitTextBox)
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			/****
			* テキストボックスの初期値を設定
			****/
			this.textBox_uri.Text = strInitTextBox;

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
				if(components != null)
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
			this.textBox_uri = new System.Windows.Forms.TextBox();
			this.btn_cansel = new System.Windows.Forms.Button();
			this.btn_ok = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// textBox_uri
			// 
			this.textBox_uri.Location = new System.Drawing.Point(16, 16);
			this.textBox_uri.Name = "textBox_uri";
			this.textBox_uri.Size = new System.Drawing.Size(480, 20);
			this.textBox_uri.TabIndex = 0;
			this.textBox_uri.Text = "textBox1";
			// 
			// btn_cansel
			// 
			this.btn_cansel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btn_cansel.Location = new System.Drawing.Point(248, 64);
			this.btn_cansel.Name = "btn_cansel";
			this.btn_cansel.Size = new System.Drawing.Size(120, 24);
			this.btn_cansel.TabIndex = 1;
			this.btn_cansel.Text = "キャンセル";
			this.btn_cansel.Click += new System.EventHandler(this.btn_cansel_Click);
			// 
			// btn_ok
			// 
			this.btn_ok.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btn_ok.Location = new System.Drawing.Point(376, 64);
			this.btn_ok.Name = "btn_ok";
			this.btn_ok.Size = new System.Drawing.Size(120, 24);
			this.btn_ok.TabIndex = 1;
			this.btn_ok.Text = "OK";
			this.btn_ok.Click += new System.EventHandler(this.btn_ok_Click);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(24, 40);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(472, 16);
			this.label1.TabIndex = 2;
			this.label1.Text = "（起動時に引数としてURLを指定することも出来ます）";
			// 
			// InputBox
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(512, 91);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.btn_cansel);
			this.Controls.Add(this.textBox_uri);
			this.Controls.Add(this.btn_ok);
			this.Name = "InputBox";
			this.Text = "サーバURLの設定";
			this.ResumeLayout(false);

		}
		#endregion

		/**********
		 * ダイアログ操作の結果を返すためのパブリック変数の定義
		 *********/
		public string strReturn;	// OKもしくはキャンセルボタンの別を格納

		/**********
		 * OK ボタンが押された時
		 *********/
		private void btn_ok_Click(object sender, System.EventArgs e)
		{
			this.strReturn = this.textBox_uri.Text;
			this.Close();
		}

		/**********
		 * キャンセル ボタンが押された時
		 *********/
		private void btn_cansel_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}
	}
}
