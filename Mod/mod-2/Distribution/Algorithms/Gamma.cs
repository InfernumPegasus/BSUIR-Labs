using System;

using Distribution.Generators;

namespace Distribution.Algorithms
{
    /// <summary>
    /// Represents a pseudorandom number generator based on
    /// the gamma distribution.
    /// </summary>
    public class Gamma : IDistributable
    {
        private IRandomable R { get; }
        
        private double Lambda { get; }
        
        private ulong Eta { get; }

        public Gamma(IRandomable r, double lambda, ulong eta)
        {
            R = r;
            Lambda = lambda;
            Eta = eta;
        }

        public double Next()
        {
            var temp = 1.0;

            for (uint i = 0; i < Eta; i++)
            {
                temp *= (double)R.Next() / ulong.MaxValue;
            }

            return -1.0 / Lambda * Math.Log(temp);
        }
    }
}
