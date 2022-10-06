using System;
using System.IO.Ports;
using System.Threading;
using Lab3.Encoders;

namespace Lab3
{
    public class SerialPortWrapper
    {
        private readonly SerialPort _serialPort;

        private readonly IPackageEncoder _packageEncoder;

        public bool Synchronized { get; set; }

        public SerialPortWrapper(string portName, int rate, IPackageEncoder packageEncoder)
        {
            _serialPort = new SerialPort(portName, rate, Parity.None, 8, StopBits.One);
            _packageEncoder = packageEncoder;

            Synchronized = true;
        }

        public void InitializePort()
        {
            _serialPort.Open();

            if (!_serialPort.IsOpen)
                throw new NotSupportedException("Cannot initialize port!");

            _serialPort.DataReceived += ReceiveData;
            _serialPort.ErrorReceived += ReceiveError;
        }

        public void SendData(string str)
        {
            var encoded = _packageEncoder.Encode(str);
            
            if (encoded.Length == 0)
            {
                Console.WriteLine("No data to send!");
                return;
            }

            Console.WriteLine($"{_serialPort.PortName} write: " + encoded);

            _serialPort.Write(encoded.ToCharArray(), 0, encoded.Length);

            if (Synchronized)
                Thread.Sleep(100);
        }
        
        private void ReceiveData(object sender, SerialDataReceivedEventArgs serialDataReceivedEventArgs)
        {
            var data = new char[_serialPort.BytesToRead];
            _serialPort.Read(data, 0, data.Length);

            Console.Write($"{_serialPort.PortName} read: ");
            Console.WriteLine(_packageEncoder.Decode(new string(data)));
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
        
        private static void ReceiveError(object sender, SerialErrorReceivedEventArgs e) =>
            Console.WriteLine("Receive error!");

        public static void ChangeRate(ref SerialPortWrapper port, int rate)
        {
            var currentRate = port._serialPort.BaudRate;
            port.SetRate(rate);
            Console.WriteLine($"Baud rate changed from {currentRate} to {rate}");
        }
    }
}