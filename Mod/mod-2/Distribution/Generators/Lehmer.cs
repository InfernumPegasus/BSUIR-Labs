using System;
using System.Numerics;

namespace Distribution.Generators
{
    /// <summary>
    /// Represents a pseudorandom number generator based on
    /// the Lehmer’s algorithm.
    /// </summary>
    public class Lehmer : IRandomable
    {
        private BigInteger A { get; set; }
        private BigInteger M { get; set; }
        private BigInteger X { get; set; }

        public Lehmer(UInt64 a, UInt64 m, UInt64 x)
        {
            A = a;
            M = m;
            X = x;
        }

        public UInt64 Next()
        {
            X = A * X % M;
            return (UInt64)(X & 0xFFFF_FFFF_FFFF_FFFF);
        }
    }
}
