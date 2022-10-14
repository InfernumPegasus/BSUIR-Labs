using System;
using System.IO.Ports;
using System.Threading;
using Lab3.Encoders;

namespace Lab3
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
                throw new NotSupportedException("Cannot initialize port!");

            _serialPort.DataReceived += ReceiveData;
            _serialPort.ErrorReceived += ReceiveError;
        }

        public void SendData(string data)
        {
            try
            {
                var bytes = Crc8Calculator.ConvertStringToBytes(data);
                var checksum = Crc8Calculator.ComputeChecksum(bytes);
                data += Crc8Calculator.ChecksumFieldName + checksum;
                Console.WriteLine($"{_serialPort.PortName} write: " + data);

                _serialPort.Write(data.ToCharArray(), 0, data.Length);

                if (Synchronized)
                    Thread.Sleep(100);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }
        
        private void ReceiveData(object sender, SerialDataReceivedEventArgs serialDataReceivedEventArgs)
        {
            var receivedData = new char[_serialPort.BytesToRead];
            _serialPort.Read(receivedData, 0, receivedData.Length);
            
            var receiveDataStr = new string(receivedData);
            Console.WriteLine($"{_serialPort.PortName} read: {receiveDataStr}");
            
            var checksumIndex = receiveDataStr.LastIndexOfAny(Crc8Calculator.ChecksumFieldName.ToCharArray());

            if (checksumIndex != -1)
            {
                var checksum = int.Parse(receiveDataStr.Substring(checksumIndex + 1));
                var strBeforeChecksum = receiveDataStr.Substring(0, receiveDataStr.LastIndexOf(Crc8Calculator.ChecksumFieldName[0]));

                Console.WriteLine($"Checksum: {checksum}");
                Console.WriteLine($"Message: {strBeforeChecksum}");
            }
            else
            {
                Console.WriteLine("Cannot resolve checksum!");
            }
        }

        private static void ReceiveError(object sender, SerialErrorReceivedEventArgs e) =>
            Console.WriteLine("Receive error!");
        
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

        public static void ChangeRate(ref SerialPortWrapper port, int rate)
        {
            var currentRate = port._serialPort.BaudRate;
            port.SetRate(rate);
            Console.WriteLine($"Baud rate changed from {currentRate} to {rate}");
        }
    }
}