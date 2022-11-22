using System;
using System.IO.Ports;
using System.Threading;
using Lab4.Encoders;

namespace Lab4
{
    public class SerialPortWrapper
    {
        private const int MaxSendAttempts = 10;

        private int CurrentAttempt;
        
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
                Console.WriteLine($"Current send attempt: {++CurrentAttempt}");
                if (CurrentAttempt >= MaxSendAttempts)
                {
                    Console.WriteLine("Max send attempts reached! Stop sending data!");
                    return;
                }
                
                var bytes = Crc8Calculator.ConvertStringToBytes(data);
                var checksum = Crc8Calculator.ComputeChecksum(bytes);
                
                var collisionData = CollisionMaker.MakeCollision(data);
                collisionData += Crc8Calculator.ChecksumFieldName + checksum;

                Console.WriteLine($"Entered data: {data}");
                Console.WriteLine($"{_serialPort.PortName} write: " + collisionData);

                _serialPort.Write(collisionData.ToCharArray(), 0, collisionData.Length);

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

                if (checksum !=
                    Crc8Calculator.ComputeChecksum(Crc8Calculator.ConvertStringToBytes(strBeforeChecksum)))
                {
                    Console.WriteLine("Collision detected! Try to send message again.");
                    Thread.Sleep(300);
                    SendData(Program.SavedData);
                    return;
                }
                
                Console.WriteLine($"Checksum: {checksum}");
                Console.WriteLine($"Message: {strBeforeChecksum}");
            }
            else
            {
                Console.WriteLine("Checksum field not found!");
                Thread.Sleep(100);
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