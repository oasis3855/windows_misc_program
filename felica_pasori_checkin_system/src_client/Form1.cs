/***************************************************

Form1.cs : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B

Pasoli_checkin �\�t�g�E�G�A
  Copyright (C) 2008, INOUE. Hirokazu
  All rights reserved.


 http://www.opensource.jp/gpl/gpl.ja.html

 ���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t�g�E�F
 �A���c�ɂ���Ĕ��s���ꂽ GNU ��ʌ��O���p�����_��(�o�[�W����2���A��
 �]�ɂ���Ă͂���ȍ~�̃o�[�W�����̂����ǂꂩ)�̒�߂�����̉��ōĔЕz
 �܂��͉��ς��邱�Ƃ��ł��܂��B
 
 ���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖��ۏ�* 
 �ł��B���Ɖ\���̕ۏ؂����̖ړI�ւ̓K�����́A���O�Ɏ����ꂽ���̂���
 �ߑS�����݂��܂���B�ڂ�����GNU ��ʌ��O���p�����_�񏑂��������������B
 
 ���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU ��ʌ��O���p�����_�񏑂̕��������ꕔ
 �󂯎�����͂��ł��B�����󂯎���Ă��Ȃ���΁A�t���[�\�t�g�E�F�A���c��
 �Ő������Ă�������(����� the Free Software Foundation, Inc., 59
 Temple Place, Suite 330, Boston, MA 02111-1307 USA)�B


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
	/// Form1 �̊T�v�̐����ł��B
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
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form_Main()
		{
			//
			// Windows �t�H�[�� �f�U�C�i �T�|�[�g�ɕK�v�ł��B
			//
			InitializeComponent();

			/****
			 * �R�}���h���C������������΁AstrUri �̏����l�Ƃ���
			 ****/
			string [] strArgv = System.Environment.GetCommandLineArgs();
			if(strArgv.Length >= 2)	strUri = strArgv[1];

			//
			// TODO: InitializeComponent �Ăяo���̌�ɁA�R���X�g���N�^ �R�[�h��ǉ����Ă��������B
			//
		}

		/// <summary>
		/// �g�p����Ă��郊�\�[�X�Ɍ㏈�������s���܂��B
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

		#region Windows �t�H�[�� �f�U�C�i�Ő������ꂽ�R�[�h 
		/// <summary>
		/// �f�U�C�i �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�ŕύX���Ȃ��ł��������B
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
			this.btn_network_test.Text = "�l�b�g���[�N����";
			this.btn_network_test.Click += new System.EventHandler(this.btn_network_test_Click);
			// 
			// btn_close
			// 
			this.btn_close.Location = new System.Drawing.Point(336, 128);
			this.btn_close.Name = "btn_close";
			this.btn_close.Size = new System.Drawing.Size(144, 24);
			this.btn_close.TabIndex = 3;
			this.btn_close.Text = "�v���O�����I��";
			this.btn_close.Click += new System.EventHandler(this.btn_close_Click);
			// 
			// textBox_result
			// 
			this.textBox_result.Font = new System.Drawing.Font("�l�r �S�V�b�N", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
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
			this.btn_read_felica_once.Text = "IC�J�[�h�e�X�g�ǂݍ���";
			this.btn_read_felica_once.Click += new System.EventHandler(this.btn_read_felica_once_Click);
			// 
			// btn_read_felica_loop
			// 
			this.btn_read_felica_loop.Location = new System.Drawing.Point(64, 48);
			this.btn_read_felica_loop.Name = "btn_read_felica_loop";
			this.btn_read_felica_loop.Size = new System.Drawing.Size(168, 32);
			this.btn_read_felica_loop.TabIndex = 4;
			this.btn_read_felica_loop.Text = "�J�[�h���[�_�N��";
			this.btn_read_felica_loop.Click += new System.EventHandler(this.btn_read_felica_loop_Click);
			// 
			// btn_thread_kill
			// 
			this.btn_thread_kill.Location = new System.Drawing.Point(248, 48);
			this.btn_thread_kill.Name = "btn_thread_kill";
			this.btn_thread_kill.Size = new System.Drawing.Size(168, 32);
			this.btn_thread_kill.TabIndex = 4;
			this.btn_thread_kill.Text = "�J�[�h���[�_��~";
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
			this.checkBox_network_enable.Text = "�J�[�h���[�_�ǂݍ��ݎ��Ƀl�b�g���[�N�@�\���g��";
			// 
			// btn_set_uri
			// 
			this.btn_set_uri.Location = new System.Drawing.Point(336, 96);
			this.btn_set_uri.Name = "btn_set_uri";
			this.btn_set_uri.Size = new System.Drawing.Size(144, 24);
			this.btn_set_uri.TabIndex = 6;
			this.btn_set_uri.Text = "�T�[�oURL�ݒ�";
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
			this.Text = "FeliCa��p�����`�F�b�N�C���L�^�V�X�e��";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.Form_Main_Closing);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// �A�v���P�[�V�����̃��C�� �G���g�� �|�C���g�ł��B
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form_Main());
		}

		/**********
		 * �T�[�o��Perl�X�N���v�g�A�h���X�i���ɍ��킹�āA�K���ύX���邱�ƁI�j
		 *********/
		string strUri = "http://192.168.1.1/pasoli_checkin.cgi";


		/**********
		 * FeliCa�T�|�[�gDLL�̊֐����g�����߂̐錾
		 *********/
		[DllImport("DllFeliCaLib.dll")]
		private extern static int ReadFelicaIdOnce(System.Text.StringBuilder lpStrOutput);


		/**********
		 * ����{�^�������������̏���
		 *********/
		private void btn_close_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}

		/**********
		 * �T�[�o��URI�ݒ�{�^������������
		 *********/
		private void btn_set_uri_Click(object sender, System.EventArgs e)
		{
			// �{�^���𖳌����i�O���[�\���j�ɐ؂�ւ���
			Change_Button_State(false);

			// InputBox �_�C�A���O����
			InputBox dlg = new InputBox(this.strUri);
			// ���[�_���_�C�A���O��\��
			if(dlg.ShowDialog() == DialogResult.OK)
			{
				// OK�{�^���������ꂽ�ꍇ�AURI�����������������
				this.strUri = dlg.strReturn;
			}
			// InputBox �_�C�A���O��j��
			dlg.Dispose();
		
			// �{�^�����ėL����
			Change_Button_State(true);
		}

		/**********
		 * �l�b�g���[�N�����{�^�������������̏���
		 *********/
		private void btn_network_test_Click(object sender, System.EventArgs e)
		{

			// �T�[�o�ɐڑ����āA�����𓾂�
			string resText = Network_Sync("net_test", "ABC 0123 ���������� xyz", "shift_jis");

			this.textBox_result.Text = resText;

			// �{�^�����ėL����
			Change_Button_State(true);
		}



		/**********
		 * IC�J�[�h�e�X�g�ǂݍ��݃{�^�������������̏���
		 *********/
		private void btn_read_felica_once_Click(object sender, System.EventArgs e)
		{
			System.Text.StringBuilder lpStr = new System.Text.StringBuilder(64);

			// �{�^���𖳌����i�O���[�\���j�ɐ؂�ւ���
			Change_Button_State(false);

			// �e�L�X�g�{�b�N�X���N���A
			this.textBox_result.Text = "";

			// FeliCa�f�[�^�ǂݏo���iDLL�j
			int nResult = ReadFelicaIdOnce(lpStr);

			if(nResult == 1)	// �ǂݏo������
			{
				string strTemp = lpStr.ToString();
				if(checkBox_network_enable.Checked == true)
				{
					// �T�[�o�ɐڑ����āA�����𓾂�
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
				this.textBox_result.Text = "IC�J�[�h�f�[�^�ǂݍ��ݎ��s";
			}

			// �{�^�����ėL����
			Change_Button_State(true);
		
		}

		/**********
		 * �J�[�h���[�_�A���ǂݍ��݃X���b�h�Ƃ̑��ݒʐM�p�錾
		 *********/
		private delegate void DelegateSetTextbox(string strBuf);
		System.Threading.Thread threadMain;
		volatile int nThreadInit = 0;

		/**********
		 * �J�[�h���[�_�N���{�^�������������̏���
		 *********/
		private void btn_read_felica_loop_Click(object sender, System.EventArgs e)
		{
			threadMain = new System.Threading.Thread(new System.Threading.ThreadStart(thread_felica_read));
			threadMain.Start();
			nThreadInit = 1;

			// ���̃{�^���𖳌����i�O���[�\���j�ɐ؂�ւ���
			Change_Button_State(false);
			btn_thread_kill.Enabled = true;		// ��~�{�^�������L��

		}

		/**********
		 * �J�[�h���[�_��~�{�^�������������̏���
		 *********/
		private void btn_thread_kill_Click(object sender, System.EventArgs e)
		{
			if(nThreadInit == 0)
			{
				this.textBox_result.Text = "�J�[�h���[�_��~�i�N������Ă��܂���j";
				return;
			}
			if(threadMain.ThreadState == System.Threading.ThreadState.Unstarted)
			{
				this.textBox_result.Text = "�J�[�h���[�_��~";
				return;
			}
			if(threadMain.IsAlive != true)
			{
				this.textBox_result.Text = "�J�[�h���[�_��~";
				return;
			}
			threadMain.Abort();
			threadMain.Join();
			textBox_result.Text = "�J�[�h���[�_��~";

			// �{�^����L����
			Change_Button_State(true);
		}


		/**********
		 * �J�[�h���[�_��A�����ēǂݍ��ރX���b�h
		 *********/
		private void thread_felica_read()
		{
			DelegateSetTextbox dlgt = new DelegateSetTextbox(DelegateSetTextboxFunction);

			try
			{
				int nResult;
				string strTemp;
				string strPrevID = "";		// 1��O�ɓǂݍ���ID
				System.Text.StringBuilder lpStr = new System.Text.StringBuilder(64);

				// �X���b�h ���C�����[�v
				for(;;)
				{
					// FeliCa�f�[�^�ǂݏo���iDLL�j
					nResult = ReadFelicaIdOnce(lpStr);

					if(nResult == 1)
					{	// FeliCa�ǂݍ��ݐ�����

						strTemp = lpStr.ToString();		//IDm_PMm�𕶎���Ɋi�[

						if(strPrevID != strTemp)
						{	// �O��ƈႤIDm_PMm�̎�

							strPrevID = strTemp;
							if(checkBox_network_enable.Checked == true)
							{
								// �T�[�o�ɐڑ����āA�����𓾂�
								strTemp = Network_Sync("checkin", strTemp, "shift_jis");
							}

						}
						else strTemp = "";
					}
					else
					{	// FeliCa�ǂݍ��ݎ��s��
						strTemp = "IC�J�[�h��ǂݎ��@�Ƀ^�b�`���Ă�������";
						strPrevID = "";
					}

					// �e�L�X�g�{�b�N�X�ɕ\���i��̕�����̎��͕ύX���Ȃ��j
					if(strTemp != "") this.Invoke(dlgt, new object[] {strTemp});

					// 1�b�x�~
					System.Threading.Thread.Sleep(1000);

				}
			}
			catch(System.Threading.ThreadAbortException)
			{
				// �X���b�h�I�����̏���������
			}
		}

		/**********
		 * �X���b�h�����A�t�H�[���̃e�L�X�g�{�b�N�X��ύX���邽�߂̃f���Q�[�g
		 *********/
		private void DelegateSetTextboxFunction(string strBuf)
		{
			this.textBox_result.Text = strBuf;
		}


		/**********
		 * �t�H�[���������鎞�Ɏ��s����i�X���b�h��~�������s���j
		 * 
		 * �i�t�H�[���̃v���p�e�B�^�u�ŁA�C�x���g�ǉ������j
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
		// �l�b�g���[�N�ڑ�����Ă��邩�̏󋵂𓾂�API
		 *********/
		[System.Runtime.InteropServices.DllImport("wininet.dll")]
		extern static bool InternetGetConnectedState(out int lpdwFlags, int dwReserved);

		/**********
		 * �l�b�g���[�N�@�\�p ���ʊ֐� �i�T�[�o���C�ӂ̃G���R�[�h�̏ꍇ�j
		 * 
		 * strCmd, strData :	POST���b�Z�[�W�Ƃ��đ��M����f�[�^�icmd=strCmd&data=strData �𑗐M�j
		 *						strData��URL�G���R�[�h���đ��M�����
		 * strEncode :	�T�[�o�̕����G���R�[�h�`���i"shift_jis" �� "UTF-8" �Ȃǂ�z��j
		 *********/
		private string Network_Sync(string strCmd, string strData, string strEncode)
		{
			int nFlag;
			if (InternetGetConnectedState(out nFlag, 0) == false)
			{	// �l�b�g���[�N�ɐڑ�����Ă��Ȃ�
				return("");
			}

			// SJIS�p�̃e�L�X�g�o�b�t�@
			System.Text.Encoding enc = 	System.Text.Encoding.GetEncoding(strEncode);
			// �e�L�X�g�iC#�̓f�t�H���g��UTF-8�j
			string resText;

			// POST����e�L�X�g���쐬����istrData�͎w�肳�ꂽ�G���R�[�h�`����urlencode�j
			string strPostData = string.Format("cmd={0}&data={1}&dummy=dummy",
					strCmd,	
					System.Web.HttpUtility.UrlEncode(strData, enc));
					// �uVisualStudio�̃v���W�F�N�g�ŁASystem.Web.dll �ւ̎Q�Ƃ�ǉ�����v����
			byte[] resPostBytes = enc.GetBytes(strPostData);


			try
			{
				// �l�b�g���[�N�R�l�N�V�����̒�`
				System.Net.WebClient wc = new System.Net.WebClient();
				// UTF-8�ȊO�̓w�b�_��񂪕K�v�iURL�G���R�[�h���đ��M���邽�߁j
				wc.Headers.Add("Content-Type","application/x-www-form-urlencoded");

				// �f�[�^�̑��M�E��������M
				byte[] resData = wc.UploadData(strUri, resPostBytes);
				// �R�l�N�V�����̔j��
				wc.Dispose();

				// �����o�C�g����A������ɕϊ����Ċi�[
				resText = enc.GetString(resData);

			}
			catch
			{	// �l�b�g���[�N�G���[
				resText = "";
			}

			return(resText);
		}

		/**********
		 * �l�b�g���[�N�@�\�p ���ʊ֐� �i�T�[�o��UTF-8�G���R�[�h�̏ꍇ�j
		 * 
		 * strCmd, strData :	POST���b�Z�[�W�Ƃ��đ��M����f�[�^�icmd=strCmd&data=strData �𑗐M�j
		 *						strData��URL�G���R�[�h���đ��M�����
		 *********/
		private string Network_Sync_UTF8(string strCmd, string strData)
		{
			int nFlag;
			if (InternetGetConnectedState(out nFlag, 0) == false)
			{	// �l�b�g���[�N�ɐڑ�����Ă��Ȃ�
				return("");
			}

			string resText;

			try
			{
				// �l�b�g���[�N�R�l�N�V�����̒�`
				System.Net.WebClient wc = new System.Net.WebClient();
				System.Collections.Specialized.NameValueCollection ps = new System.Collections.Specialized.NameValueCollection();

				// POST�ő��M����f�[�^�Z�b�g���`����
				ps.Add("cmd", strCmd);
				ps.Add("data", strData);
				ps.Add("dummy", "dummy");

				// �f�[�^�̑��M�E��������M
				byte[] resData = wc.UploadValues(strUri, ps);
				// �R�l�N�V�����̔j��
				wc.Dispose();

				// �����o�C�g����A������ɕϊ����Ċi�[
				//UTF-8�̏ꍇ
				resText = System.Text.Encoding.UTF8.GetString(resData);
				// SJIS�̏ꍇ
				// System.Text.Encoding enc = 	System.Text.Encoding.GetEncoding("shift_jis");
				// resText = enc.GetString(resData);

			}
			catch
			{	// �l�b�g���[�N�G���[
				resText = "";
			}

			return(resText);
		}


		/**********
		 * �{�^���L�����E��������ԕύX�p ���ʊ֐�
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
