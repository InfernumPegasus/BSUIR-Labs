using System;
using System.Collections.Generic;

namespace Lab3.Extensions
{
    public static class StringExtension
    {
        public static IEnumerable<string> SplitIntoGroups(this string text, int size)
        {
            if (text.Length % size != 0)
                throw new ArgumentException("Invalid size!");

            var groups = new string[text.Length / size];
            var index = 0;

            for (var i = 0; i < text.Length; i += size)
                groups[index++] = text.Substring(i, size);

            return groups;
        }
        
        public static IEnumerable<string> SplitByLength(this string str, int maxLength) {
            for (var index = 0; index < str.Length; index += maxLength) {
                yield return str.Substring(index, Math.Min(maxLength, str.Length - index));
            }
        }
    }
}