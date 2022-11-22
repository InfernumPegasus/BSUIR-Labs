using System;
using System.IO.Ports;

namespace Lab4
{
    internal static class Program
    {
        public static string SavedData;
        
        private static int GetNumber()
        {
            bool parsed;
            int choice;

            do
            {
                parsed = int.TryParse(Console.ReadLine(), out choice);
            } while (!parsed);

            return choice;
        }

        private static void ToggleSynchronization(ref SerialPortWrapper port1, ref SerialPortWrapper port2)
        {
            Console.WriteLine("Current ports synchronization state:\n" +
                              $"Port1: {port1.Synchronized}\n" +
                              $"Port2: {port2.Synchronized}");

            Console.WriteLine("Toggle?\n1 - yes");
            var choice = GetNumber();

            var synchronized = port1.Synchronized == port2.Synchronized;
            switch (choice)
            {
                case 1 when synchronized:
                    port1.Synchronized = port2.Synchronized = !port1.Synchronized;
                    break;
                case 1:
                    Console.WriteLine("Ports are not synchronized! Setting to sync condition");
                    port1.Synchronized = port2.Synchronized = true;
                    break;
            }
        }

        private static void Menu(ref SerialPortWrapper port1, ref SerialPortWrapper port2)
        {
            while (true)
            {
                Console.WriteLine("Choose option:");
                Console.WriteLine("1 - Get all port names");
                Console.WriteLine("2 - Send");
                Console.WriteLine("3 - Change 1 port rate");
                Console.WriteLine("4 - Change 2 port rate");
                Console.WriteLine("5 - Toggle synchronization");
                Console.WriteLine("6 - Exit");
                Console.Write("> ");

                var choice = GetNumber();
                Console.WriteLine();

                switch (choice)
                {
                    case 1:
                        foreach (var port in SerialPort.GetPortNames())
                            Console.WriteLine(port);
                        break;
                    case 2:
                        Console.WriteLine("Enter data to send:");
                        SavedData = Console.ReadLine();
                        port1.SendData(SavedData);
                        break;
                    case 3:
                        Console.WriteLine("Enter new rate:");
                        choice = GetNumber();
                        SerialPortWrapper.ChangeRate(ref port1, choice);
                        break;
                    case 4:
                        Console.WriteLine("Enter new rate:");
                        choice = GetNumber();
                        SerialPortWrapper.ChangeRate(ref port2, choice);
                        break;
                    case 5:
                        ToggleSynchronization(ref port1, ref port2);
                        break;
                    case 6:
                        return;
                    default:
                        Console.WriteLine("Unknown option");
                        break;
                }

                // Console.WriteLine("Press any key to continue...");
                Console.ReadKey();
                Console.Clear();
            }
        }
        
        public static void Main()
        {
            var port1 = new SerialPortWrapper("COM1", 9600);
            var port2 = new SerialPortWrapper("COM2", 9600);

            port1.InitializePort();
            port2.InitializePort();

            if (SerialPort.GetPortNames().Length < 2)
            {
                Console.WriteLine("No available ports! Check your ports settings!");
                Environment.Exit(1);
            }

            Menu(ref port1, ref port2);
        }
    }
}
