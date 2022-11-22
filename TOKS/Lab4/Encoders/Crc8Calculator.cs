using System;
using System.Linq;

namespace Lab4.Encoders
{
    public static class Crc8Calculator
    {
        private static readonly byte[] Table = new byte[256];

        private const byte Poly = 0x07;

        public const string ChecksumFieldName = "|checksum";
        
        public static byte ComputeChecksum(params byte[] bytes)
        {
            byte crc = 0;
            if (bytes != null && bytes.Length > 0)
                crc = bytes.Aggregate(crc, (current, b) => Table[current ^ b]);

            return crc;
        }

        static Crc8Calculator()
        {
            for (var i = 0; i < 256; ++i)
            {
                var temp = i;
                for (var j = 0; j < 8; ++j)
                {
                    if ((temp & 0x80) != 0)
                        temp = (temp << 1) ^ Poly;
                    else
                        temp <<= 1;
                }

                Table[i] = (byte) temp;
            }
        }

        public static byte[] ConvertStringToBytes(string str)
        {
            var bytes = new byte[str.Length];
            for (var i = 0; i < str.Length; i++)
                bytes[i] = Convert.ToByte(str[i]);

            return bytes;
        }

    }
}