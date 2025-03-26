namespace ImageResizingWinForms
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            btnUpload = new Button();
            txtPath = new TextBox();
            btnConvert = new Button();
            btnSave = new Button();
            openFileDialog1 = new OpenFileDialog();
            saveFileDialog1 = new SaveFileDialog();
            lblTime = new Label();
            lWidth = new Label();
            lHeight = new Label();
            nWidth = new NumericUpDown();
            nHeight = new NumericUpDown();
            radioButton1 = new RadioButton();
            radioButton2 = new RadioButton();
            FunnyButton = new Button();
            funnyBar = new ProgressBar();
            label1 = new Label();
            pixelBox = new PixelBox();
            ((System.ComponentModel.ISupportInitialize)nWidth).BeginInit();
            ((System.ComponentModel.ISupportInitialize)nHeight).BeginInit();
            ((System.ComponentModel.ISupportInitialize)pixelBox).BeginInit();
            SuspendLayout();
            // 
            // btnUpload
            // 
            btnUpload.Font = new Font("Segoe UI", 14F);
            btnUpload.Location = new Point(25, 24);
            btnUpload.Name = "btnUpload";
            btnUpload.Size = new Size(195, 54);
            btnUpload.TabIndex = 0;
            btnUpload.Text = "Upload Image";
            btnUpload.UseVisualStyleBackColor = true;
            btnUpload.Click += btnUpload_Click;
            // 
            // txtPath
            // 
            txtPath.Location = new Point(25, 84);
            txtPath.Name = "txtPath";
            txtPath.Size = new Size(359, 23);
            txtPath.TabIndex = 1;
            // 
            // btnConvert
            // 
            btnConvert.Font = new Font("Segoe UI", 14F);
            btnConvert.Location = new Point(25, 449);
            btnConvert.Name = "btnConvert";
            btnConvert.Size = new Size(359, 54);
            btnConvert.TabIndex = 2;
            btnConvert.Text = "Convert";
            btnConvert.UseVisualStyleBackColor = true;
            btnConvert.Click += btnConvert_Click;
            // 
            // btnSave
            // 
            btnSave.Font = new Font("Segoe UI", 14F);
            btnSave.Location = new Point(25, 509);
            btnSave.Name = "btnSave";
            btnSave.Size = new Size(359, 54);
            btnSave.TabIndex = 3;
            btnSave.Text = "Save";
            btnSave.UseVisualStyleBackColor = true;
            btnSave.Click += btnSave_Click;
            // 
            // openFileDialog1
            // 
            openFileDialog1.FileName = "openFileDialog1";
            // 
            // lblTime
            // 
            lblTime.AutoSize = true;
            lblTime.Font = new Font("Segoe UI", 12F);
            lblTime.Location = new Point(25, 413);
            lblTime.Name = "lblTime";
            lblTime.Size = new Size(228, 21);
            lblTime.TabIndex = 5;
            lblTime.Text = "Assembler code execution time:";
            // 
            // lWidth
            // 
            lWidth.AutoSize = true;
            lWidth.Font = new Font("Segoe UI", 12F);
            lWidth.Location = new Point(25, 136);
            lWidth.Name = "lWidth";
            lWidth.Size = new Size(98, 21);
            lWidth.TabIndex = 8;
            lWidth.Text = "Target Width";
            // 
            // lHeight
            // 
            lHeight.AutoSize = true;
            lHeight.Font = new Font("Segoe UI", 12F);
            lHeight.Location = new Point(209, 136);
            lHeight.Name = "lHeight";
            lHeight.Size = new Size(102, 21);
            lHeight.TabIndex = 9;
            lHeight.Text = "Target Height";
            // 
            // nWidth
            // 
            nWidth.Location = new Point(25, 160);
            nWidth.Maximum = new decimal(new int[] { int.MaxValue, 0, 0, 0 });
            nWidth.Minimum = new decimal(new int[] { 2, 0, 0, 0 });
            nWidth.Name = "nWidth";
            nWidth.Size = new Size(120, 23);
            nWidth.TabIndex = 10;
            nWidth.Value = new decimal(new int[] { 2, 0, 0, 0 });
            // 
            // nHeight
            // 
            nHeight.Location = new Point(209, 160);
            nHeight.Maximum = new decimal(new int[] { int.MaxValue, 0, 0, 0 });
            nHeight.Minimum = new decimal(new int[] { 2, 0, 0, 0 });
            nHeight.Name = "nHeight";
            nHeight.Size = new Size(120, 23);
            nHeight.TabIndex = 11;
            nHeight.Value = new decimal(new int[] { 2, 0, 0, 0 });
            // 
            // radioButton1
            // 
            radioButton1.AutoSize = true;
            radioButton1.Checked = true;
            radioButton1.Location = new Point(25, 205);
            radioButton1.Name = "radioButton1";
            radioButton1.Size = new Size(40, 19);
            radioButton1.TabIndex = 12;
            radioButton1.TabStop = true;
            radioButton1.Text = "C#";
            radioButton1.UseVisualStyleBackColor = true;
            // 
            // radioButton2
            // 
            radioButton2.AutoSize = true;
            radioButton2.Location = new Point(25, 230);
            radioButton2.Name = "radioButton2";
            radioButton2.Size = new Size(49, 19);
            radioButton2.TabIndex = 13;
            radioButton2.Text = "Asm";
            radioButton2.UseVisualStyleBackColor = true;
            // 
            // FunnyButton
            // 
            FunnyButton.Font = new Font("Segoe UI", 14F);
            FunnyButton.Location = new Point(25, 278);
            FunnyButton.Name = "FunnyButton";
            FunnyButton.Size = new Size(359, 54);
            FunnyButton.TabIndex = 14;
            FunnyButton.Text = "Sweep";
            FunnyButton.UseVisualStyleBackColor = true;
            FunnyButton.Click += FunnyButton_Click;
            // 
            // funnyBar
            // 
            funnyBar.Location = new Point(25, 357);
            funnyBar.Margin = new Padding(3, 2, 3, 2);
            funnyBar.Maximum = 2000;
            funnyBar.Name = "funnyBar";
            funnyBar.Size = new Size(359, 27);
            funnyBar.TabIndex = 15;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new Font("Segoe UI", 12F);
            label1.Location = new Point(25, 334);
            label1.Name = "label1";
            label1.Size = new Size(124, 21);
            label1.TabIndex = 16;
            label1.Text = "Sweep Progress:";
            // 
            // pixelBox
            // 
            pixelBox.Location = new Point(416, 24);
            pixelBox.Name = "pixelBox";
            pixelBox.Size = new Size(690, 541);
            pixelBox.SizeMode = PictureBoxSizeMode.StretchImage;
            pixelBox.TabIndex = 17;
            pixelBox.TabStop = false;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1124, 577);
            Controls.Add(pixelBox);
            Controls.Add(label1);
            Controls.Add(funnyBar);
            Controls.Add(FunnyButton);
            Controls.Add(radioButton2);
            Controls.Add(radioButton1);
            Controls.Add(nHeight);
            Controls.Add(nWidth);
            Controls.Add(lHeight);
            Controls.Add(lWidth);
            Controls.Add(lblTime);
            Controls.Add(btnSave);
            Controls.Add(btnConvert);
            Controls.Add(txtPath);
            Controls.Add(btnUpload);
            Name = "Form1";
            Text = "BilinearInterpolator";
            ((System.ComponentModel.ISupportInitialize)nWidth).EndInit();
            ((System.ComponentModel.ISupportInitialize)nHeight).EndInit();
            ((System.ComponentModel.ISupportInitialize)pixelBox).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button btnUpload;
        private TextBox txtPath;
        private Button btnConvert;
        private Button btnSave;
        private OpenFileDialog openFileDialog1;
        private SaveFileDialog saveFileDialog1;
        private Label lblTime;
        private Label lWidth;
        private Label lHeight;
        private NumericUpDown nWidth;
        private NumericUpDown nHeight;
        private RadioButton radioButton1;
        private RadioButton radioButton2;
        private Button FunnyButton;
        private ProgressBar funnyBar;
        private Label label1;
        private PixelBox pixelBox;
    }
}
