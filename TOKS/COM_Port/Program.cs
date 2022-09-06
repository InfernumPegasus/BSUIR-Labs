using System;
using System.IO.Ports;
using System.Threading;

namespace COM_Port
{
    public class SerialPortWrapper
    {
        private readonly SerialPort _serialPort;
        
        private const int MaxRate = 268435456;

        private bool Synchronized { get; }

        public SerialPortWrapper(string portName, int rate)
        {
            _serialPort = new SerialPort(portName, rate, Parity.None, 8, StopBits.One);
            Synchronized = true;
        }

        private void SetRate(int rate)
        {
            if (rate > MaxRate || rate <= 0)
            {
                throw new ArgumentOutOfRangeException(nameof(rate));
            }
            _serialPort.BaudRate = rate;
        }

        public void InitializePort()
        {
            _serialPort.Open();

            if (!_serialPort.IsOpen)
            {
                throw new ArgumentException("Cannot initialize port!");
            }

            _serialPort.DataReceived += ReceiveData;

            _serialPort.ErrorReceived += ReceiveError;
        }

        private static void ReceiveError(object sender, SerialErrorReceivedEventArgs e)
        {
            Console.WriteLine("Receive error!");
        }

        private void ReceiveData(object sender, SerialDataReceivedEventArgs serialDataReceivedEventArgs)
        {
            var data = new char[_serialPort.BytesToRead];
            _serialPort.Read(data, 0, data.Length);

            Console.WriteLine($"{_serialPort.PortName} read:");
            Console.WriteLine(data);
            
            // _serialPort.DiscardInBuffer();
        }

        public void Send(string str)
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
            
            // _serialPort.DiscardOutBuffer();
        }

        public static void ChangeRate(ref SerialPortWrapper port, int rate)
        {
            try
            {
                port.SetRate(rate);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
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
                Console.WriteLine("5 - Exit");
                Console.Write("> ");

                var choice = GetNumber();

                switch (choice)
                {
                    case 1:
                        GetAllPorts();
                        break;
                    case 2:
                        Console.WriteLine("Enter data to send:");
                        port1.Send(Console.ReadLine());
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
                        return;
                    default:
                        Console.WriteLine("Unknown option"); 
                        break;
                }

                Console.ReadKey();
                Console.Clear();
            }
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
    }
}
