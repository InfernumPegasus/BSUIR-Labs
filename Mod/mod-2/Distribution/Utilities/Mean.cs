using System;
using System.Collections.Generic;
using System.Linq;

namespace Distribution.Utilities
{
    /// <summary>
    /// Represents a class of computing mean.
    /// </summary>
    public static class Mean
    {
        public static double Compute(List<double> values)
        {
            return values.Sum() / values.Count;
        }
    }
}
