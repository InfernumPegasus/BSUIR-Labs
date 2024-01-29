using System;
using System.Windows.Forms;

namespace Distribution
{
    /// <summary>
    /// The main class of the application.
    /// </summary>
    internal static class Program
    {
        [STAThread]
        static void Main()
        {
            ApplicationConfiguration.Initialize();
            Application.Run(new MainForm());
        }
    }
}
