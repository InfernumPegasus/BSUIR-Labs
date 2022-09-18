using System;
using System.IO.Ports;
using System.Threading;

namespace Lab1
{
    public class SerialPortWrapper
    {
        private readonly SerialPort _serialPort;

        public bool Synchronized { get; set; }

        public SerialPortWrapper(string portName, int rate)
        {
            _serialPort = new SerialPort(portName, rate, Parity.None, 8, StopBits.One);
            Synchronized = true;
        }

        public void InitializePort()
        {
            _serialPort.Open();

            if (!_serialPort.IsOpen)
            {
                throw new NotSupportedException("Cannot initialize port!");
            }

            _serialPort.DataReceived += ReceiveData;

            _serialPort.ErrorReceived += ReceiveError;
        }

        private void ReceiveData(object sender, SerialDataReceivedEventArgs serialDataReceivedEventArgs)
        {
            var data = new char[_serialPort.BytesToRead];
            _serialPort.Read(data, 0, data.Length);

            Console.Write($"{_serialPort.PortName} read: ");
            Console.WriteLine(data);
        }

        public void SendData(string str)
        {
            if (str.Length == 0)
            {
                Console.WriteLine("No data to send!");
                return;
            }

            Console.WriteLine($"{_serialPort.PortName} write: " + str);
            Console.WriteLine();

            // Request To Send - enable
            _serialPort.RtsEnable = true;

            _serialPort.Write(str.ToCharArray(), 0, str.Length);

            if (Synchronized)
            {
                Thread.Sleep(100);
            }

            _serialPort.RtsEnable = false;
        }

        private void SetRate(int rate)
        {
            try
            {
                _serialPort.BaudRate = rate;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }
        
        private static void ReceiveError(object sender, SerialErrorReceivedEventArgs e)
        {
            Console.WriteLine("Receive error!");
        }

        public static void ChangeRate(ref SerialPortWrapper port, int rate)
        {
            var currentRate = port._serialPort.BaudRate;
            port.SetRate(rate);
            Console.WriteLine($"Baud rate changed from {currentRate} to {rate}");
        }
    }

    internal static class Program
    {
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

        private static void GetAllPorts()
        {
            var ports = SerialPort.GetPortNames();
            if (ports.Length < 2)
            {
                Console.WriteLine("No available ports!");
                Environment.Exit(1);
            }

            Console.WriteLine("Available ports:");
            foreach (var port in ports)
            {
                Console.WriteLine(port);
            }

            Console.WriteLine();
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

        public static void Main()
        {
            var port1 = new SerialPortWrapper("COM1", 9600);
            var port2 = new SerialPortWrapper("COM2", 9600);

            port1.InitializePort();
            port2.InitializePort();

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
                        GetAllPorts();
                        break;
                    case 2:
                        Console.WriteLine("Enter data to send:");
                        port1.SendData(Console.ReadLine());
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

                Console.WriteLine("Press any key to continue...");
                Console.ReadKey();
                Console.Clear();
            }
        }
    }
}