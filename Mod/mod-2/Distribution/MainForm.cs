using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

using Distribution.Algorithms;
using Distribution.Generators;
using Distribution.Utilities;

namespace Distribution
{
    /// <summary>
    /// Represents the main window of the application.
    /// </summary>
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void GenerateButtonClick(Object sender, EventArgs args)
        {
            // Text = "Distribution � Generating�";

            List<Double> values = new List<Double>();

            IRandomable generator = new Lehmer(a: 16_769_023,
                m: ulong.MaxValue - 1_678,
                x: 5_542_985_019_387);

            if (generatorButton2.Checked)
            {
                generator = new MPM(r0: 20_214_752,
                    r1: 9_842_349_174_754);
            }

            IDistributable distribution = new Uniform(generator,
                a: 120.0,
                b: 185.0);

            Double expectM = (120.0 + 185.0) / 2.0;
            Double expectV = (185.0 - 120.0) * (185.0 - 120.0) / 12.0;

            if (distributionButton2.Checked)
            {
                distribution = new Gauss(generator,
                    m: 132.0,
                    sigma: 8.0,
                    n: 12);

                expectM = 132.0;
                expectV = 8.0 * 8.0;
            }

            if (distributionButton3.Checked)
            {
                distribution = new Exponential(generator,
                    lambda: 4.5);

                expectM = 1.0 / 4.5;
                expectV = 1.0 / (4.5 * 4.5);
            }

            if (distributionButton4.Checked)
            {
                distribution = new Gamma(generator,
                    lambda: 14.0,
                    eta: 8);

                expectM = 8.0 / 14.0;
                expectV = 8.0 / (14.0 * 14.0);
            }

            if (distributionButton5.Checked)
            {
                distribution = new Triangular(generator,
                    a: 75.0,
                    b: 200.0);

                expectM = (75.0 + 75.0 + 200.0) / 3.0;
                expectV = (75.0 * 75.0 + 200.0 * 200.0 - 2 * 75.0 * 200.0) / 18.0;
            }

            if (distributionButton6.Checked)
            {
                distribution = new Simpson(generator,
                    a: 35.0,
                    b: 90.0);

                expectM = (35.0 + 90.0) / 2.0;
                expectV = (90.0 - 35.0) * (90.0 - 35.0) / 24.0;
            }

            UInt64 n = (UInt64)parameterUpDown1.Value;

            for (UInt64 i = 0; i < n; i++)
            {
                values.Add(distribution.Next());
            }

            Double computeE = Mean.Compute(values);
            Double computeV = Variance.Compute(values);

            expectedTextBox1.Text = String.Format("{0:0.000000}", expectM);
            expectedTextBox2.Text = String.Format("{0:0.000000}", expectV);

            computedTextBox1.Text = String.Format("{0:0.000000}", computeE);
            computedTextBox2.Text = String.Format("{0:0.000000}", computeV);

            UInt64 k = (UInt64)parameterUpDown2.Value;

            UInt64[] counts = new UInt64[k];

            Double min = values.Min();
            Double max = values.Max();

            foreach (Double value in values)
            {
                UInt64 index = (UInt64)Math.Floor(k * (value - min) / (max - min));

                if (index >= k)
                {
                    index = k - 1;
                }

                counts[index] += 1;
            }

            chartSeries.Points.Clear();

            for (UInt64 i = 0; i < k; i++)
            {
                chartSeries.Points.AddXY(i, counts[i]);
            }

            // Text = "Distribution � Done!";
        }

        private void ClearButtonClick(Object sender, EventArgs args)
        {
            // Text = "Distribution";

            chartSeries.Points.Clear();

            expectedTextBox1.Text = "";
            expectedTextBox2.Text = "";

            computedTextBox1.Text = "";
            computedTextBox2.Text = "";
        }
    }
}
