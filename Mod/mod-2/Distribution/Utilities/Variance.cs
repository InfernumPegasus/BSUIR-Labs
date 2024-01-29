using System.Collections.Generic;
using System.Linq;

namespace Distribution.Utilities
{
    /// <summary>
    /// Represents a class of computing variance.
    /// </summary>
    public static class Variance
    {
        public static double Compute(List<double> values)
        {
            var mean = Mean.Compute(values);

            var result = values.Sum(value => (value - mean) * (value - mean));

            return result / values.Count;
        }
    }
}
