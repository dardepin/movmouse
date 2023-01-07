
namespace movmouse
{
    partial class Form1
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
            this.WindowsComboBox = new System.Windows.Forms.ComboBox();
            this.StartButton = new System.Windows.Forms.Button();
            this.ReloadButton = new System.Windows.Forms.Button();
            this.NotifyLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // WindowsComboBox
            // 
            this.WindowsComboBox.FormattingEnabled = true;
            this.WindowsComboBox.Location = new System.Drawing.Point(12, 12);
            this.WindowsComboBox.Name = "WindowsComboBox";
            this.WindowsComboBox.Size = new System.Drawing.Size(240, 21);
            this.WindowsComboBox.TabIndex = 0;
            // 
            // StartButton
            // 
            this.StartButton.Location = new System.Drawing.Point(339, 12);
            this.StartButton.Name = "StartButton";
            this.StartButton.Size = new System.Drawing.Size(75, 23);
            this.StartButton.TabIndex = 1;
            this.StartButton.Text = "START";
            this.StartButton.UseVisualStyleBackColor = true;
            this.StartButton.Click += new System.EventHandler(this.StartButton_Click);
            // 
            // ReloadButton
            // 
            this.ReloadButton.Location = new System.Drawing.Point(258, 12);
            this.ReloadButton.Name = "ReloadButton";
            this.ReloadButton.Size = new System.Drawing.Size(75, 23);
            this.ReloadButton.TabIndex = 2;
            this.ReloadButton.Text = "RELOAD";
            this.ReloadButton.UseVisualStyleBackColor = true;
            this.ReloadButton.Click += new System.EventHandler(this.ReloadButton_Click);
            // 
            // NotifyLabel
            // 
            this.NotifyLabel.AutoSize = true;
            this.NotifyLabel.Location = new System.Drawing.Point(12, 50);
            this.NotifyLabel.Name = "NotifyLabel";
            this.NotifyLabel.Size = new System.Drawing.Size(0, 13);
            this.NotifyLabel.TabIndex = 3;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(424, 72);
            this.Controls.Add(this.NotifyLabel);
            this.Controls.Add(this.ReloadButton);
            this.Controls.Add(this.StartButton);
            this.Controls.Add(this.WindowsComboBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Form1";
            this.Text = "Mouse mover v0.2";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox WindowsComboBox;
        private System.Windows.Forms.Button StartButton;
        private System.Windows.Forms.Button ReloadButton;
        private System.Windows.Forms.Label NotifyLabel;
    }
}

