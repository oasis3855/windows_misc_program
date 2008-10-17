using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace Pasoli_checkin
{
	/// <summary>
	/// InputBox �̊T�v�̐����ł��B
	/// </summary>
	public class InputBox : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox textBox_uri;
		private System.Windows.Forms.Button btn_cansel;
		private System.Windows.Forms.Button btn_ok;
		private System.Windows.Forms.Label label1;
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		private System.ComponentModel.Container components = null;

		/****
		* �R���X�g���N�^
		*
		*�i�e�L�X�g�{�b�N�X�̏����l�����߂邽�߂̃p�����[�^��ǉ��j
		****/
		public InputBox(string strInitTextBox)
		{
			//
			// Windows �t�H�[�� �f�U�C�i �T�|�[�g�ɕK�v�ł��B
			//
			InitializeComponent();

			/****
			* �e�L�X�g�{�b�N�X�̏����l��ݒ�
			****/
			this.textBox_uri.Text = strInitTextBox;

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
				if(components != null)
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
			this.btn_cansel.Text = "�L�����Z��";
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
			this.label1.Text = "�i�N�����Ɉ����Ƃ���URL���w�肷�邱�Ƃ��o���܂��j";
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
			this.Text = "�T�[�oURL�̐ݒ�";
			this.ResumeLayout(false);

		}
		#endregion

		/**********
		 * �_�C�A���O����̌��ʂ�Ԃ����߂̃p�u���b�N�ϐ��̒�`
		 *********/
		public string strReturn;	// OK�������̓L�����Z���{�^���̕ʂ��i�[

		/**********
		 * OK �{�^���������ꂽ��
		 *********/
		private void btn_ok_Click(object sender, System.EventArgs e)
		{
			this.strReturn = this.textBox_uri.Text;
			this.Close();
		}

		/**********
		 * �L�����Z�� �{�^���������ꂽ��
		 *********/
		private void btn_cansel_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}
	}
}
