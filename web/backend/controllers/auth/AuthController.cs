using System.Security.Claims;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Options;
using Models.Auth;
using Models.Proxy.Telnet;
using Microsoft.IdentityModel.Tokens;
using System.Text;
using System.IdentityModel.Tokens.Jwt;
using Microsoft.Extensions.Caching.Memory;


namespace Controllers.Auth
{
    [ApiController]
    [Route("api/v1/[controller]")]
    public class AuthController: ControllerBase
    {
        private readonly JWTSettings _jwtSettings;
        private readonly IMemoryCache _memoryCache;

        public AuthController(IOptions<JWTSettings> settings, IMemoryCache cache)
        {
            _jwtSettings = settings.Value;
            _memoryCache = cache;
        }

        [HttpPost]
        public IActionResult Authenticate([FromBody] ApiKey request)
        {
            if (request == null || string.IsNullOrWhiteSpace(request.Key))
            {
                return BadRequest("API key is required.");
            }

            var token = CreateToken(request);
            return Ok(token);
        }

        // refactor this to a repo
        private UserToken CreateToken(ApiKey request)
        {
            // set up auth logic later
            var claims = new[]
            {
                new Claim(ClaimTypes.Role, "Admin"),
                new Claim(JwtRegisteredClaimNames.Jti, Guid.NewGuid().ToString())
            };

            var key = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(_jwtSettings.Secret));
            var creds = new SigningCredentials(key, SecurityAlgorithms.HmacSha256);

            var tokenDescriptor = new SecurityTokenDescriptor
            {
                Subject = new ClaimsIdentity(claims),
                Expires = DateTime.UtcNow.AddHours(_jwtSettings.DurationInHours),
                SigningCredentials = creds
            };

            var tokenHandler = new JwtSecurityTokenHandler();
            var token = tokenHandler.CreateToken(tokenDescriptor);
            var tokenString = tokenHandler.WriteToken(token);
            
            var cacheOptions = new MemoryCacheEntryOptions().SetAbsoluteExpiration(
                TimeSpan.FromHours(_jwtSettings.DurationInHours)
            );
            
            _memoryCache.Set(tokenString, true, cacheOptions);
            return new UserToken { Token = tokenString, ExpiresInHours = _jwtSettings.DurationInHours };
        }
    }
}