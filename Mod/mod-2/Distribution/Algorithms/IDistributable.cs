using System;

namespace Distribution.Algorithms
{
    /// <summary>
    /// Defines a method to generate next <see cref="Double"/>
    /// by distributions.
    /// </summary>
    public interface IDistributable
    {
        Double Next();
    }
}
