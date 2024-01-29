using System;

using Distribution.Generators;

namespace Distribution.Algorithms
{
    /// <summary>
    /// Represents a pseudorandom number generator based on
    /// the Gauss distribution.
    /// </summary>
    public class Gauss : IDistributable
    {
        private IRandomable R { get; }

        private double M { get; }
        private double Sigma { get; }

        private ulong N { get; }

        public Gauss(IRandomable r, double m, double sigma, ulong n)
        {
            R = r;
            M = m;
            Sigma = sigma;
            N = n;
        }

        public double Next()
        {
            var temp = 0.0;

            for (ulong i = 0; i < N; i++)
            {
                temp += (double)R.Next() / ulong.MaxValue;
            }

            temp -= N / 2;
            temp *= Sigma * Math.Sqrt(12.0 / N);

            return M + temp;
        }
    }
}
