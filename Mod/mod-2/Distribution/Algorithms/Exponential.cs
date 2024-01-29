using System;

using Distribution.Generators;

namespace Distribution.Algorithms
{
    /// <summary>
    /// Represents a pseudorandom number generator based on
    /// the exponential distribution.
    /// </summary>
    public class Exponential : IDistributable
    {
        private IRandomable R { get; }

        private double Lambda { get; }

        public Exponential(IRandomable r, double lambda)
        {
            R = r;
            Lambda = lambda;
        }

        public double Next()
        {
            var r = (double)R.Next() / ulong.MaxValue;
            return -1 / Lambda * Math.Log(r);
        }
    }
}
