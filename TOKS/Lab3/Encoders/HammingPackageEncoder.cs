using System;
using System.Linq;
using Lab3.Extensions;

namespace Lab3.Encoders
{
    public class HammingPackageEncoder : IPackageEncoder
    {
        public string Encode(string toEncode)
        {
            return toEncode
                .Select(ascii => Convert.ToString(ascii, 2)
                    .PadLeft(8, '0'))
                .Select(binary => binary
                    .Select(bit => $"{bit}{bit}{bit}"))
                .Aggregate(string.Empty, (current, tripled) => 
                    current + string.Join("", tripled));
        }
        
        public string Decode(string toDecode)
        {
            var bitGroups = toDecode.SplitIntoGroups(3);
            var corrected = bitGroups.Select(trio => 
                trio.Count(bit => bit == '0') > 1 ? 0 : 1);
            var bytes = string.Join("", corrected).SplitIntoGroups(8);

            return bytes
                .Select(b => Convert.ToInt32(b, 2))
                .Select(Convert.ToChar)
                .Aggregate(string.Empty, (current, c) => current + c);
        }
    }
}