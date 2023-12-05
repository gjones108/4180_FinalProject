namespace _4180Project_COMConnector
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.mbedCOM = new System.IO.Ports.SerialPort(this.components);
            this.lightlabel = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.humidlabel = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.templabel = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.moistlabel = new System.Windows.Forms.Label();
            this.thresholdtxt = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // mbedCOM
            // 
            this.mbedCOM.PortName = "COM4";
            this.mbedCOM.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.mbedCOM_DataReceived);
            // 
            // lightlabel
            // 
            this.lightlabel.AutoSize = true;
            this.lightlabel.BackColor = System.Drawing.Color.Transparent;
            this.lightlabel.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lightlabel.ForeColor = System.Drawing.SystemColors.Window;
            this.lightlabel.Location = new System.Drawing.Point(95, 133);
            this.lightlabel.Name = "lightlabel";
            this.lightlabel.Size = new System.Drawing.Size(82, 25);
            this.lightlabel.TabIndex = 1;
            this.lightlabel.Text = "[empty]";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.SystemColors.Window;
            this.label3.Location = new System.Drawing.Point(219, 81);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(95, 25);
            this.label3.TabIndex = 2;
            this.label3.Text = "Humidity";
            // 
            // humidlabel
            // 
            this.humidlabel.AutoSize = true;
            this.humidlabel.BackColor = System.Drawing.Color.Transparent;
            this.humidlabel.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.humidlabel.ForeColor = System.Drawing.SystemColors.Window;
            this.humidlabel.Location = new System.Drawing.Point(272, 133);
            this.humidlabel.Name = "humidlabel";
            this.humidlabel.Size = new System.Drawing.Size(82, 25);
            this.humidlabel.TabIndex = 3;
            this.humidlabel.Text = "[empty]";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.BackColor = System.Drawing.Color.Transparent;
            this.label5.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.SystemColors.Window;
            this.label5.Location = new System.Drawing.Point(203, 213);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(125, 25);
            this.label5.TabIndex = 4;
            this.label5.Text = "Temperature";
            // 
            // templabel
            // 
            this.templabel.AutoSize = true;
            this.templabel.BackColor = System.Drawing.Color.Transparent;
            this.templabel.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.templabel.ForeColor = System.Drawing.SystemColors.Window;
            this.templabel.Location = new System.Drawing.Point(272, 273);
            this.templabel.Name = "templabel";
            this.templabel.Size = new System.Drawing.Size(82, 25);
            this.templabel.TabIndex = 5;
            this.templabel.Text = "[empty]";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.Color.Transparent;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.SystemColors.Window;
            this.label4.Location = new System.Drawing.Point(64, 220);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(90, 24);
            this.label4.TabIndex = 6;
            this.label4.Text = "Moisture";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // moistlabel
            // 
            this.moistlabel.AutoSize = true;
            this.moistlabel.BackColor = System.Drawing.Color.Transparent;
            this.moistlabel.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.moistlabel.ForeColor = System.Drawing.SystemColors.Window;
            this.moistlabel.Location = new System.Drawing.Point(95, 273);
            this.moistlabel.Name = "moistlabel";
            this.moistlabel.Size = new System.Drawing.Size(82, 25);
            this.moistlabel.TabIndex = 7;
            this.moistlabel.Text = "[empty]";
            // 
            // thresholdtxt
            // 
            this.thresholdtxt.BackColor = System.Drawing.Color.DarkGreen;
            this.thresholdtxt.ForeColor = System.Drawing.SystemColors.Window;
            this.thresholdtxt.Location = new System.Drawing.Point(138, 357);
            this.thresholdtxt.Name = "thresholdtxt";
            this.thresholdtxt.Size = new System.Drawing.Size(43, 20);
            this.thresholdtxt.TabIndex = 8;
            this.thresholdtxt.Text = "30.0";
            this.thresholdtxt.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.OliveDrab;
            this.button1.Location = new System.Drawing.Point(190, 354);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(60, 23);
            this.button1.TabIndex = 9;
            this.button1.Text = "Set";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(372, 321);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(15, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "%";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.BackColor = System.Drawing.Color.Transparent;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.ForeColor = System.Drawing.Color.LightGreen;
            this.label6.Location = new System.Drawing.Point(31, 19);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(309, 37);
            this.label6.TabIndex = 11;
            this.label6.Text = "Plant Health Tracker";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.BackColor = System.Drawing.Color.Transparent;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.ForeColor = System.Drawing.SystemColors.Window;
            this.label7.Location = new System.Drawing.Point(89, 333);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(202, 18);
            this.label7.TabIndex = 12;
            this.label7.Text = "Moisture Alarm Threshold";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox1.Image = global::_4180Project_COMConnector.Properties.Resources.bulb;
            this.pictureBox1.Location = new System.Drawing.Point(14, 109);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(75, 76);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 13;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox2.Image = global::_4180Project_COMConnector.Properties.Resources.humidity;
            this.pictureBox2.Location = new System.Drawing.Point(190, 109);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(73, 76);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 14;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox3.Image = global::_4180Project_COMConnector.Properties.Resources.temp;
            this.pictureBox3.Location = new System.Drawing.Point(188, 241);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(75, 72);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 15;
            this.pictureBox3.TabStop = false;
            this.pictureBox3.Click += new System.EventHandler(this.pictureBox3_Click);
            // 
            // pictureBox4
            // 
            this.pictureBox4.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox4.Image = global::_4180Project_COMConnector.Properties.Resources._3937304;
            this.pictureBox4.Location = new System.Drawing.Point(27, 247);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(62, 66);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox4.TabIndex = 16;
            this.pictureBox4.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.SystemColors.Window;
            this.label1.Location = new System.Drawing.Point(49, 81);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(107, 25);
            this.label1.TabIndex = 17;
            this.label1.Text = "Light Level";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.Info;
            this.panel1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("panel1.BackgroundImage")));
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.pictureBox4);
            this.panel1.Controls.Add(this.pictureBox3);
            this.panel1.Controls.Add(this.pictureBox2);
            this.panel1.Controls.Add(this.pictureBox1);
            this.panel1.Controls.Add(this.label7);
            this.panel1.Controls.Add(this.label6);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.thresholdtxt);
            this.panel1.Controls.Add(this.moistlabel);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.templabel);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.humidlabel);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.lightlabel);
            this.panel1.Location = new System.Drawing.Point(-2, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(366, 390);
            this.panel1.TabIndex = 0;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ClientSize = new System.Drawing.Size(360, 386);
            this.Controls.Add(this.panel1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.IO.Ports.SerialPort mbedCOM;
        private System.Windows.Forms.Label lightlabel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label humidlabel;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label templabel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label moistlabel;
        private System.Windows.Forms.TextBox thresholdtxt;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel1;
    }
}

