


namespace Models.Auth
{
    public class UserToken
    {
        public string Token { get; set; } = string.Empty;
        public int ExpiresInHours { get; set; }
    }
}