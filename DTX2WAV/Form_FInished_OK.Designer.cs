﻿namespace DTX2WAV
{
	partial class Form_Finished_OK
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form_Finished_OK));
			this.button_OK = new System.Windows.Forms.Button();
			this.label_finished = new System.Windows.Forms.Label();
			this.pictureBox_OKIcon = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox_OKIcon)).BeginInit();
			this.SuspendLayout();
			// 
			// button_OK
			// 
			resources.ApplyResources(this.button_OK, "button_OK");
			this.button_OK.Name = "button_OK";
			this.button_OK.UseVisualStyleBackColor = true;
			this.button_OK.Click += new System.EventHandler(this.button_OK_Click);
			// 
			// label_finished
			// 
			resources.ApplyResources(this.label_finished, "label_finished");
			this.label_finished.Name = "label_finished";
			// 
			// pictureBox_OKIcon
			// 
			resources.ApplyResources(this.pictureBox_OKIcon, "pictureBox_OKIcon");
			this.pictureBox_OKIcon.Name = "pictureBox_OKIcon";
			this.pictureBox_OKIcon.TabStop = false;
			// 
			// Form_Finished_OK
			// 
			resources.ApplyResources(this, "$this");
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ControlBox = false;
			this.Controls.Add(this.pictureBox_OKIcon);
			this.Controls.Add(this.label_finished);
			this.Controls.Add(this.button_OK);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "Form_Finished_OK";
			this.Shown += new System.EventHandler(this.Form_Finished_OK_Shown);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox_OKIcon)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button button_OK;
		private System.Windows.Forms.Label label_finished;
		private System.Windows.Forms.PictureBox pictureBox_OKIcon;
	}
}