using System;

namespace Distribution.Generators
{
    /// <summary>
    /// Defines a method to generate next <see cref="UInt64"/>
    /// by pseudorandom number generators.
    /// </summary>
    public interface IRandomable
    {
        UInt64 Next();
    }
}
