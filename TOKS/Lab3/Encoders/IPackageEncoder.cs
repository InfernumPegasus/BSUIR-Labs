namespace Lab3.Encoders
{
    public interface IPackageEncoder
    {
        string Encode(string toEncode);

        string Decode(string toDecode);
    }
}