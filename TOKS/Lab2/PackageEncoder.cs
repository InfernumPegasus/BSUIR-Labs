using System;
using System.Linq;

namespace Lab2
{
    public static class PackageEncoder
    {
        private const char InsertChar = '#';

        public static string Encode(string toEncode)
        {
            if (toEncode.Length == 0)
            {
                Console.WriteLine("No data to encode! Returning empty string.");
                return string.Empty;
            }
            
            var encoded = string.Empty;
            var first = toEncode[0];
            encoded += first;
            for (var i = 1; i < toEncode.Length; i++)
            {
                if (toEncode[i] == first)
                    encoded += InsertChar;

                encoded += toEncode[i];
            }

            return encoded;
        }

        public static string Decode(string encoded)
        {
            if (encoded.Length != 0)
                return encoded
                    .Where(t => t != InsertChar)
                    .Aggregate(string.Empty, (current, t) => current + t);
            
            Console.WriteLine("No data to decode! Returning empty string.");
            return string.Empty;
        }

        public static bool IsCorrectlyEncoded(string data)
        {
            var encoded = Encode(data);
            var decoded = Decode(encoded);

            Console.WriteLine($"Input string: {data}");
            Console.WriteLine($"Encoded string: {encoded}");
            Console.WriteLine($"Decoded string: {decoded}");

            return data == decoded;
        }
    }
}