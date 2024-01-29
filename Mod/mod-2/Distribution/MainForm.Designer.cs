using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace Distribution
{
    public partial class MainForm
    {
        private GroupBox parameterBox;
        private GroupBox expectedBox;
        private GroupBox computedBox;
        private GroupBox generatorBox;
        private GroupBox distributionBox;

        private Chart chart;
        private ChartArea chartArea;
        private Series chartSeries;

        private Button generateButton;
       

        private Label parameterLabel1;
        private Label parameterLabel2;

        private NumericUpDown parameterUpDown1;
        private NumericUpDown parameterUpDown2;

        private Label expectedLabel1;
        private Label expectedLabel2;

        private TextBox expectedTextBox1;
        private TextBox expectedTextBox2;

        private Label computedLabel1;
        private Label computedLabel2;

        private TextBox computedTextBox1;
        private TextBox computedTextBox2;

        private RadioButton generatorButton1;
        private RadioButton generatorButton2;

        private RadioButton distributionButton1;
        private RadioButton distributionButton2;
        private RadioButton distributionButton3;
        private RadioButton distributionButton4;
        private RadioButton distributionButton5;
        private RadioButton distributionButton6;

        private void InitializeComponent()
        {
            parameterBox = new GroupBox();
            expectedBox = new GroupBox();
            computedBox = new GroupBox();
            generatorBox = new GroupBox();
            distributionBox = new GroupBox();

            chart = new Chart();
            chartArea = new ChartArea();
            chartSeries = new Series();

            generateButton = new Button();
 
            parameterLabel1 = new Label();
            parameterLabel2 = new Label();

            parameterUpDown1 = new NumericUpDown();
            parameterUpDown2 = new NumericUpDown();

            expectedLabel1 = new Label();
            expectedLabel2 = new Label();

            expectedTextBox1 = new TextBox();
            expectedTextBox2 = new TextBox();

            computedLabel1 = new Label();
            computedLabel2 = new Label();

            computedTextBox1 = new TextBox();
            computedTextBox2 = new TextBox();

            generatorButton1 = new RadioButton();
            generatorButton2 = new RadioButton();

            distributionButton1 = new RadioButton();
            distributionButton2 = new RadioButton();
            distributionButton3 = new RadioButton();
            distributionButton4 = new RadioButton();
            distributionButton5 = new RadioButton();
            distributionButton6 = new RadioButton();

            parameterBox.SuspendLayout();
            expectedBox.SuspendLayout();
            computedBox.SuspendLayout();
            generatorBox.SuspendLayout();
            distributionBox.SuspendLayout();

            SuspendLayout();

            //
            // parameterBox.
            //
            parameterBox.Controls.AddRange(new Control[]
            {
                parameterLabel1,
                parameterLabel2,
                parameterUpDown1,
                parameterUpDown2
            });

            parameterBox.Location = new Point(5, 5);
            parameterBox.Name = "parameterBox";
            parameterBox.Size = new Size(300, 95);
            parameterBox.Text = "Параметры";

            //
            // expectedBox.
            //
            expectedBox.Controls.AddRange(new Control[]
            {
                expectedLabel1,
                expectedLabel2,
                expectedTextBox1,
                expectedTextBox2
            });

            expectedBox.Location = new Point(5, 105);
            expectedBox.Name = "expectedBox";
            expectedBox.Size = new Size(300, 95);
            expectedBox.Text = "Ожид.";

            //
            // computedBox.
            //
            computedBox.Controls.AddRange(new Control[]
            {
                computedLabel1,
                computedLabel2,
                computedTextBox1,
                computedTextBox2
            });

            computedBox.Location = new Point(5, 205);
            computedBox.Name = "computedBox";
            computedBox.Size = new Size(300, 95);
            computedBox.Text = "Реал.";

            //
            // generatorBox.
            //
            generatorBox.Controls.AddRange(new Control[]
            {
                generatorButton1,
                generatorButton2
            });

            generatorBox.Location = new Point(5, 305);
            generatorBox.Name = "generatorBox";
            generatorBox.Size = new Size(300, 95);
            generatorBox.Text = "Генераторы";

            //
            // distributionBox.
            //
            distributionBox.Controls.AddRange(new Control[]
            {
                distributionButton1,
                distributionButton2,
                distributionButton3,
                distributionButton4,
                distributionButton5,
                distributionButton6
            });

            distributionBox.Location = new Point(5, 405);
            distributionBox.Name = "distributionBox";
            distributionBox.Size = new Size(300, 305);
            distributionBox.Text = "Распределение";

            //
            // chart.
            //
            chart.ChartAreas.Add(chartArea);
            chart.Location = new Point(310, 15);
            chart.Name = "chart";
            chart.Series.Add(chartSeries);
            chart.Size = new Size(985, 695);
            chart.TabStop = false;

            //
            // chartArea.
            //
            chartArea.AxisX.MajorGrid.Enabled = false;
            chartArea.AxisY.MajorGrid.Enabled = false;
            chartArea.Name = "chartArea";

            //
            // chartSeries.
            //
            chartSeries.ChartArea = "chartArea";
            chartSeries.Color = Color.Blue;
            chartSeries.IsXValueIndexed = true;
            chartSeries.Name = "chartSeries";
            chartSeries.XValueType = ChartValueType.UInt64;
            chartSeries.YValueType = ChartValueType.UInt64;

            //
            // generateButton.
            //
            generateButton.Click += GenerateButtonClick;
            generateButton.Location = new Point(510, 715);
            generateButton.Name = "generateButton";
            generateButton.Size = new Size(490, 30);
            generateButton.Text = "Сгенерировать";

            //
            // parameterLabel1.
            //
            parameterLabel1.Font = new Font(parameterLabel1.Font, FontStyle.Italic);
            parameterLabel1.Location = new Point(5, 25);
            parameterLabel1.Name = "parameterLabel1";
            parameterLabel1.Size = new Size(60, 30);
            parameterLabel1.Text = "Чисел";
            parameterLabel1.TextAlign = ContentAlignment.MiddleLeft;

            //
            // parameterLabel2.
            //
            parameterLabel2.Font = new Font(parameterLabel2.Font, FontStyle.Italic);
            parameterLabel2.Location = new Point(5, 60);
            parameterLabel2.Name = "parameterLabel2";
            parameterLabel2.Size = new Size(60, 30);
            parameterLabel2.Text = "Разб.";
            parameterLabel2.TextAlign = ContentAlignment.MiddleLeft;

            //
            // parameterUpDown1.
            //
            parameterUpDown1.Location = new Point(100, 25);
            parameterUpDown1.Maximum = 10_000_000;
            parameterUpDown1.Minimum = 1_000;
            parameterUpDown1.Name = "parameterUpDown1";
            parameterUpDown1.Size = new Size(195, 30);
            parameterUpDown1.TextAlign = HorizontalAlignment.Right;
            parameterUpDown1.UpDownAlign = LeftRightAlignment.Left;

            parameterUpDown1.Value = 10_000;

            //
            // parameterUpDown2.
            //
            parameterUpDown2.Location = new Point(100, 60);
            parameterUpDown2.Maximum = 1_000;
            parameterUpDown2.Minimum = 10;
            parameterUpDown2.Name = "parameterUpDown2";
            parameterUpDown2.Size = new Size(195, 30);
            parameterUpDown2.TextAlign = HorizontalAlignment.Right;
            parameterUpDown2.UpDownAlign = LeftRightAlignment.Left;

            parameterUpDown2.Value = 256;

            //
            // expectedLabel1.
            //
            expectedLabel1.Font = new Font(expectedLabel1.Font, FontStyle.Italic);
            expectedLabel1.Location = new Point(5, 25);
            expectedLabel1.Name = "expectedLabel1";
            expectedLabel1.Size = new Size(60, 30);
            expectedLabel1.Text = "Мат. ож.";
            expectedLabel1.TextAlign = ContentAlignment.MiddleLeft;

            //
            // expectedLabel2.
            //
            expectedLabel2.Font = new Font(expectedLabel2.Font, FontStyle.Italic);
            expectedLabel2.Location = new Point(5, 60);
            expectedLabel2.Name = "expectedLabel2";
            expectedLabel2.Size = new Size(60, 30);
            expectedLabel2.Text = "Дисп.";
            expectedLabel2.TextAlign = ContentAlignment.MiddleLeft;

            //
            // expectedTextBox1.
            //
            expectedTextBox1.Location = new Point(100, 25);
            expectedTextBox1.Name = "expectedTextBox1";
            expectedTextBox1.ReadOnly = true;
            expectedTextBox1.Size = new Size(195, 30);
            expectedTextBox1.TabStop = false;
            expectedTextBox1.TextAlign = HorizontalAlignment.Right;

            //
            // expectedTextBox2.
            //
            expectedTextBox2.Location = new Point(100, 60);
            expectedTextBox2.Name = "expectedTextBox2";
            expectedTextBox2.ReadOnly = true;
            expectedTextBox2.Size = new Size(195, 30);
            expectedTextBox2.TabStop = false;
            expectedTextBox2.TextAlign = HorizontalAlignment.Right;

            //
            // computedLabel1.
            //
            computedLabel1.Font = new Font(computedLabel1.Font, FontStyle.Italic);
            computedLabel1.Location = new Point(5, 25);
            computedLabel1.Name = "computedLabel1";
            computedLabel1.Size = new Size(60, 30);
            computedLabel1.Text = "Мат. ож.";
            computedLabel1.TextAlign = ContentAlignment.MiddleLeft;

            //
            // computedLabel2.
            //
            computedLabel2.Font = new Font(computedLabel2.Font, FontStyle.Italic);
            computedLabel2.Location = new Point(5, 60);
            computedLabel2.Name = "computedLabel2";
            computedLabel2.Size = new Size(60, 30);
            computedLabel2.Text = "Дисп.";
            computedLabel2.TextAlign = ContentAlignment.MiddleLeft;

            //
            // computedTextBox1.
            //
            computedTextBox1.Location = new Point(100, 25);
            computedTextBox1.Name = "computedTextBox1";
            computedTextBox1.ReadOnly = true;
            computedTextBox1.Size = new Size(195, 30);
            computedTextBox1.TabStop = false;
            computedTextBox1.TextAlign = HorizontalAlignment.Right;

            //
            // computedTextBox2.
            //
            computedTextBox2.Location = new Point(100, 60);
            computedTextBox2.Name = "computedTextBox2";
            computedTextBox2.ReadOnly = true;
            computedTextBox2.Size = new Size(195, 30);
            computedTextBox2.TabStop = false;
            computedTextBox2.TextAlign = HorizontalAlignment.Right;

            //
            // generatorButton1.
            //
            generatorButton1.Checked = true;
            generatorButton1.Font = new Font("Courier New",
                9,
                FontStyle.Regular,
                GraphicsUnit.Point);
            generatorButton1.Location = new Point(5, 25);
            generatorButton1.Name = "generatorButton1";
            generatorButton1.Size = new Size(200, 30);
            generatorButton1.Text = "Лемер";

            //
            // generatorButton2.
            //
            generatorButton2.Font = new Font("Courier New",
                9,
                FontStyle.Regular,
                GraphicsUnit.Point);
            generatorButton2.Location = new Point(5, 60);
            generatorButton2.Name = "generatorButton2";
            generatorButton2.Size = new Size(200, 30);
            generatorButton2.Text = "МСП";

            //
            // distributionButton1.
            //
            distributionButton1.Checked = true;
            distributionButton1.Font = new Font("Courier New",
                9,
                FontStyle.Regular,
                GraphicsUnit.Point);
            distributionButton1.Location = new Point(5, 25);
            distributionButton1.Name = "distributionButton1";
            distributionButton1.Size = new Size(200, 30);
            distributionButton1.Text = "Равномерное";

            //
            // distributionButton2.
            //
            distributionButton2.Font = new Font("Courier New",
                9,
                FontStyle.Regular,
                GraphicsUnit.Point);
            distributionButton2.Location = new Point(5, 60);
            distributionButton2.Name = "distributionButton2";
            distributionButton2.Size = new Size(200, 30);
            distributionButton2.Text = "Гаусса";

            //
            // distributionButton3.
            //
            distributionButton3.Font = new Font("Courier New",
                9,
                FontStyle.Regular,
                GraphicsUnit.Point);
            distributionButton3.Location = new Point(5, 95);
            distributionButton3.Name = "distributionButton3";
            distributionButton3.Size = new Size(200, 30);
            distributionButton3.Text = "Экспоненциальное";

            //
            // distributionButton4.
            //
            distributionButton4.Font = new Font("Courier New",
                9,
                FontStyle.Regular,
                GraphicsUnit.Point);
            distributionButton4.Location = new Point(5, 130);
            distributionButton4.Name = "distributionButton4";
            distributionButton4.Size = new Size(200, 30);
            distributionButton4.Text = "Гамма";

            //
            // distributionButton5.
            //
            distributionButton5.Font = new Font("Courier New",
                9,
                FontStyle.Regular,
                GraphicsUnit.Point);
            distributionButton5.Location = new Point(5, 165);
            distributionButton5.Name = "distributionButton5";
            distributionButton5.Size = new Size(200, 30);
            distributionButton5.Text = "Треугольное";

            //
            // distributionButton6.
            //
            distributionButton6.Font = new Font("Courier New",
                9,
                FontStyle.Regular,
                GraphicsUnit.Point);
            distributionButton6.Location = new Point(5, 200);
            distributionButton6.Name = "distributionButton6";
            distributionButton6.Size = new Size(200, 30);
            distributionButton6.Text = "Симпсона";

            //
            // MainForm.
            //
            BackColor = Color.Bisque;
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1300, 750);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            MaximizeBox = false;
            MinimizeBox = false;
            Text = "Lab2";

            Controls.AddRange(new Control[]
            {
                parameterBox,
                expectedBox,
                computedBox,
                generatorBox,
                distributionBox
            });

            Controls.Add(chart);

            Controls.AddRange(new Control[]
            {
                generateButton
            });

            parameterBox.ResumeLayout(true);
            expectedBox.ResumeLayout(true);
            computedBox.ResumeLayout(true);
            generatorBox.ResumeLayout(true);
            distributionBox.ResumeLayout(true);

            ResumeLayout(true);
        }
    }
}
