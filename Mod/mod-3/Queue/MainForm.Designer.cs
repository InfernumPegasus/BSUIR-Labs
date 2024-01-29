using System;
using System.Drawing;
using System.Windows.Forms;

namespace Queue
{
    partial class MainForm
    {
        private Button button;

        private Label divider1;
        private Label divider2;
        private Label divider3;
        private Label divider4;

        private NumericUpDown upDown1;
        private NumericUpDown upDown2;
        private NumericUpDown upDown3;

        private TextBox textBox1;
        private TextBox textBox2;
        private TextBox textBox3;
        private TextBox textBox4;
        private TextBox textBox5;

        private Label label1;
        private Label label2;
        private Label label3;
        private Label label4;
        private Label label5;
        private Label label6;
        private Label label7;
        private Label label8;

        private void InitializeComponent()
        {
            button = new Button();

            divider1 = new Label();
            divider2 = new Label();
            divider3 = new Label();
            divider4 = new Label();

            upDown1 = new NumericUpDown();
            upDown2 = new NumericUpDown();
            upDown3 = new NumericUpDown();

            textBox1 = new TextBox();
            textBox2 = new TextBox();
            textBox3 = new TextBox();
            textBox4 = new TextBox();
            textBox5 = new TextBox();

            label1 = new Label();
            label2 = new Label();
            label3 = new Label();
            label4 = new Label();
            label5 = new Label();
            label6 = new Label();
            label7 = new Label();
            label8 = new Label();

            SuspendLayout();

            #region button

            button.Click += ButtonClick;
            button.Location = new Point(5, 315);
            button.Name = "button";
            button.Size = new Size(390, 30);
            button.Text = "Сгенерировать";

            #endregion

            #region divider1

            divider1.AutoSize = false;
            divider1.BorderStyle = BorderStyle.Fixed3D;
            divider1.Location = new Point(5, 75);
            divider1.Name = "divider1";
            divider1.Size = new Size(390, 2);

            #endregion

            #region divider2

            divider2.AutoSize = false;
            divider2.BorderStyle = BorderStyle.Fixed3D;
            divider2.Location = new Point(5, 152);
            divider2.Name = "divider2";
            divider2.Size = new Size(390, 2);

            #endregion

            #region divider3

            divider3.AutoSize = false;
            divider3.BorderStyle = BorderStyle.Fixed3D;
            divider3.Location = new Point(5, 266);
            divider3.Name = "divider3";
            divider3.Size = new Size(390, 2);

            #endregion

            #region divider4

            divider4.AutoSize = false;
            divider4.BorderStyle = BorderStyle.Fixed3D;
            divider4.Location = new Point(5, 308);
            divider4.Name = "divider4";
            divider4.Size = new Size(390, 2);

            #endregion

            #region upDown1

            upDown1.DecimalPlaces = 1;
            upDown1.Increment = (Decimal)0.1;
            upDown1.Location = new Point(200, 5);
            upDown1.Maximum = (Decimal)1.0;
            upDown1.Minimum = (Decimal)0.0;
            upDown1.Name = "upDown1";
            upDown1.Size = new Size(195, 30);
            upDown1.TextAlign = HorizontalAlignment.Right;
            upDown1.UpDownAlign = LeftRightAlignment.Right;

            upDown1.Value = (Decimal)0.5;

            #endregion

            #region upDown2

            upDown2.DecimalPlaces = 1;
            upDown2.Increment = (Decimal)0.1;
            upDown2.Location = new Point(200, 40);
            upDown2.Maximum = (Decimal)1.0;
            upDown2.Minimum = (Decimal)0.0;
            upDown2.Name = "upDown2";
            upDown2.Size = new Size(195, 30);
            upDown2.TextAlign = HorizontalAlignment.Right;
            upDown2.UpDownAlign = LeftRightAlignment.Right;

            upDown2.Value = (Decimal)0.5;

            #endregion

            #region upDown3

            upDown3.Location = new Point(200, 274);
            upDown3.Maximum = 10000000;
            upDown3.Minimum = 1;
            upDown3.Name = "upDown3";
            upDown3.Size = new Size(195, 30);
            upDown3.TextAlign = HorizontalAlignment.Right;
            upDown3.UpDownAlign = LeftRightAlignment.Right;

            upDown3.Value = 10000;

            #endregion

            #region textBox1

            textBox1.Location = new Point(200, 82);
            textBox1.Name = "textBox1";
            textBox1.ReadOnly = true;
            textBox1.Size = new Size(195, 30);
            textBox1.TabStop = false;
            textBox1.TextAlign = HorizontalAlignment.Right;

            #endregion

            #region textBox2

            textBox2.Location = new Point(200, 117);
            textBox2.Name = "textBox2";
            textBox2.ReadOnly = true;
            textBox2.Size = new Size(195, 30);
            textBox2.TabStop = false;
            textBox2.TextAlign = HorizontalAlignment.Right;

            #endregion

            #region textBox3

            textBox3.Location = new Point(200, 159);
            textBox3.Name = "textBox3";
            textBox3.ReadOnly = true;
            textBox3.Size = new Size(195, 30);
            textBox3.TabStop = false;
            textBox3.TextAlign = HorizontalAlignment.Right;

            #endregion

            #region textBox4

            textBox4.Location = new Point(200, 194);
            textBox4.Name = "textBox4";
            textBox4.ReadOnly = true;
            textBox4.Size = new Size(195, 30);
            textBox4.TabStop = false;
            textBox4.TextAlign = HorizontalAlignment.Right;

            #endregion

            #region textBox5

            textBox5.Location = new Point(200, 229);
            textBox5.Name = "textBox5";
            textBox5.ReadOnly = true;
            textBox5.Size = new Size(195, 30);
            textBox5.TabStop = false;
            textBox5.TextAlign = HorizontalAlignment.Right;

            #endregion

            #region label1

            label1.Location = new Point(5, 5);
            label1.Name = "label1";
            label1.Size = new Size(190, 30);
            label1.Text = "Отказ 1 канала";
            label1.TextAlign = ContentAlignment.MiddleLeft;

            #endregion

            #region label2

            label2.Location = new Point(5, 40);
            label2.Name = "label2";
            label2.Size = new Size(190, 30);
            label2.Text = "Отказ 2 канала";
            label2.TextAlign = ContentAlignment.MiddleLeft;

            #endregion

            #region label3

            label3.Location = new Point(5, 82);
            label3.Name = "label3";
            label3.Size = new Size(190, 30);
            label3.Text = "Ср. время заявки в оч.";
            label3.TextAlign = ContentAlignment.MiddleLeft;

            #endregion

            #region label4

            label4.Location = new Point(5, 117);
            label4.Name = "label4";
            label4.Size = new Size(190, 30);
            label4.Text = "Средняя длина очереди";
            label4.TextAlign = ContentAlignment.MiddleLeft;

            #endregion

            #region label5

            label5.Location = new Point(5, 159);
            label5.Name = "label5";
            label5.Size = new Size(190, 30);
            label5.Text = "Количество запросов";
            label5.TextAlign = ContentAlignment.MiddleLeft;

            #endregion

            #region label6

            label6.Location = new Point(5, 194);
            label6.Name = "label6";
            label6.Size = new Size(190, 30);
            label6.Text = "Количество ошибок";
            label6.TextAlign = ContentAlignment.MiddleLeft;

            #endregion

            #region label7

            label7.Location = new Point(5, 229);
            label7.Name = "label7";
            label7.Size = new Size(190, 30);
            label7.Text = "Счетчик очереди";
            label7.TextAlign = ContentAlignment.MiddleLeft;

            #endregion

            #region label8

            label8.Location = new Point(5, 274);
            label8.Name = "label8";
            label8.Size = new Size(190, 30);
            label8.Text = "Такты";
            label8.TextAlign = ContentAlignment.MiddleLeft;

            #endregion

            #region this

            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(400, 350);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            MaximizeBox = false;
            MinimizeBox = false;
            Text = "ЛР3. Вариант 19";

            Controls.AddRange(new Control[]
            {
                button,
                divider1,
                divider2,
                divider3,
                divider4,
                upDown1,
                upDown2,
                upDown3,
                textBox1,
                textBox2,
                textBox3,
                textBox4,
                textBox5,
                label1,
                label2,
                label3,
                label4,
                label5,
                label6,
                label7,
                label8,
            });

            #endregion

            ResumeLayout(true);
        }
    }
}
