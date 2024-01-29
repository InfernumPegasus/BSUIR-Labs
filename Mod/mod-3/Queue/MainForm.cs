using System;
using System.Windows.Forms;

namespace Queue
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void ButtonClick(Object sender, EventArgs args)
        {
            Double first = (Double) upDown1.Value;
            Double second = (Double) upDown2.Value;

            Queue queue = new Queue(first, second);

            Int32 ticks = (Int32) upDown3.Value;

            for (Int32 i = 0; i < ticks; i++)
            {
                queue.NextState();
            }
            
            textBox1.Text = $"{(Double) queue.QueueCount / (queue.RequestCount * queue.RequestFrequency) / 0.35:0.000}";
            textBox2.Text = $"{(Double) queue.QueueCount / (queue.RequestCount * queue.RequestFrequency):0.000}";

            textBox3.Text = $"{queue.RequestCount}";
            textBox4.Text = $"{queue.FailureCount}";
            textBox5.Text = $"{queue.QueueCount}";
        }
    }
}