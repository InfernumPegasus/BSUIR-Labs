using System;

namespace Lab4
{
    public static class CollisionMaker
    {
        private static readonly Random Random = new Random();
        
        public static string MakeCollision(string data) =>
            Random.Next(0, 2) != 0 ? data : data.Insert(0, "*");
    }
}