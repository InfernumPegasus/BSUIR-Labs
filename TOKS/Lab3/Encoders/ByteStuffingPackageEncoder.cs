using System.Linq;

namespace Lab3.Encoders
{
    public static class ByteStuffingPackageEncoder
    {
        private const char InsertChar = '#';

        public static string Encode(string toEncode)
        {
            if (toEncode.Length == 0)
                return string.Empty;

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
            
            return string.Empty;
        }
    }
}