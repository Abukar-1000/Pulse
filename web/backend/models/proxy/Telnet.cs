
namespace Models.Proxy.Telnet
{
    public class JWTSettings
    {
        public string Secret { get; set; } = string.Empty;
        public int DurationInHours { get; set; }
    }
}