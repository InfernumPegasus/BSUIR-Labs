using System;
using System.IO;

namespace Queue
{
    public class Queue
    {
        // private readonly StreamWriter _streamWriter = new StreamWriter("numbers");
        
        private readonly Random _random = new();

        private Byte _requestCounter;

        private Byte Channel1 { get; set; }

        private Byte _buffer;

        private Byte Channel2 { get; set; }

        private bool _blocked;

        public readonly byte RequestFrequency = 2;

        private const int BufferSize = 2;

        public Double First { get; }
        public Double Second { get; }

        public Int32 RequestCount { get; private set; }
        public Int32 FailureCount { get; private set; }
        public Int32 QueueCount { get; private set; }

        public Queue(Double first, Double second)
        {
            _requestCounter = RequestFrequency;
            Channel1 = 0;
            _buffer = 0;
            Channel2 = 0;

            First = first;
            Second = second;

            RequestCount = 0;
            FailureCount = 0;
            QueueCount = 0;
        }

        public void NextState()
        {
            if (Channel2 == 1)
            {
                if (_random.NextDouble() > Second)
                {
                    Channel2 = 0;
                }
            }

            if (_buffer > 0)
            {
                if (Channel2 == 0)
                {
                    _buffer--;
                    Channel2 = 1;
                }
            }

            if (Channel1 == 1)
            {
                if (_blocked)
                {
                    if (_buffer < BufferSize)
                    {
                        _buffer++;
                        _blocked = false;
                        Channel1 = 0;
                    }
                }
                else if (_random.NextDouble() > First)
                {
                    if (_buffer == BufferSize)
                    {
                        _blocked = true;
                    }
                    else
                    {
                        Channel1 = 0;
                        _buffer++;
                    }
                }
            }

            if (_buffer > 0)
            {
                if (Channel2 == 0)
                {
                    _buffer--;
                    Channel2 = 1;
                }
            }

            if (_requestCounter != 1)
            {
                _requestCounter--;
            }
            else
            {
                _requestCounter = RequestFrequency;
                RequestCount++;

                if (Channel1 == 0)
                {
                    Channel1 = 1;
                }
                else
                {
                    FailureCount++;
                }
            }

            if (_buffer != 0)
            {
                QueueCount++;
            }

            /*Console.WriteLine($"{_requestCounter} {Channel1} {_buffer} {Channel2}");*/
            // _streamWriter.WriteLine($"{_requestCounter} {Channel1} {_buffer} {Channel2}");
        }
    }
}