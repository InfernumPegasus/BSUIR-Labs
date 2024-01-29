using System;

using Distribution.Generators;

namespace Distribution.Algorithms
{
    /// <summary>
    /// Represents a pseudorandom number generator based on
    /// the Simpson distribution.
    /// </summary>
    public class Simpson : IDistributable
    {
        private Uniform R { get; set; }

        public Simpson(IRandomable r, Double a, Double b)
        {
            R = new Uniform(r, a / 2, b / 2);
        }

        public Double Next()
        {
            return R.Next() + R.Next();
        }
    }
}
