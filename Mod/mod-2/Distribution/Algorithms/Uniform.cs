using System;

using Distribution.Generators;

namespace Distribution.Algorithms
{
    /// <summary>
    /// Represents a pseudorandom number generator based on
    /// the uniform distribution.
    /// </summary>
    public class Uniform : IDistributable
    {
        private IRandomable R { get; set; }

        private Double A { get; set; }
        private Double B { get; set; }

        public Uniform(IRandomable r, Double a, Double b)
        {
            R = r;
            A = a;
            B = b;
        }

        public Double Next()
        {
            Double r = (Double)R.Next() / UInt64.MaxValue;
            return A + (B - A) * r;
        }
    }
}
