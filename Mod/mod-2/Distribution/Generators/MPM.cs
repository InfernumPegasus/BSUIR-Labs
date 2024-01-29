using System;
using System.Numerics;

namespace Distribution.Generators
{
    /// <summary>
    /// Represents a pseudorandom number generator based on
    /// the middle-product method.
    /// </summary>
    public class MPM : IRandomable
    {
        private BigInteger R0 { get; set; }
        private BigInteger R1 { get; set; }

        public MPM(UInt64 r0, UInt64 r1)
        {
            R0 = r0;
            R1 = r1;
        }

        public UInt64 Next()
        {
            (R0, R1) = (R1, (R0 * R1 >> 32) & 0xFFFF_FFFF_FFFF_FFFF);
            return (UInt64)R1;
        }
    }
}
